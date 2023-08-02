#include "../transformers.hpp"

void luramas::ast::transformers::upvalues::set(std::shared_ptr<luramas::ast::ast> &ast) {

      /* Captures follow newclosure or dupclosure(RARLEY). */
      auto closures = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_NEWCLOSURE>(true));
      const auto dupclosures = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_REFCLOSURE>(true));

      /* All closures. */
      closures.insert(closures.end(), dupclosures.begin(), dupclosures.end());

      for (const auto &node : closures) {

            debug_line("Closure at: %s", node->str().c_str());

            /* Incase it bugs out. */
            node->closures.closure_idx = node->lex->disassembly->operands.back()->dis.closure_id /* Will work for closure. */;

            auto idx = 0u;
            auto next = ast->body->visit_next(node);

            while (next != nullptr && next->lex->disassembly->op == luramas::il::arch::opcodes::OP_SETLIST) {

                  debug_line("Iterating through capture at: %s", next->str().c_str());

                  if (next->lex->has_operand_expr<luramas::il::lexer::operand_kinds::upvalue>()) {
                        /* Pass upvalue */
                        debug_success("Next is upvalue.");
                        ast->closures[node->closures.closure_idx]->upvalues.insert(std::make_pair(idx++, std::make_pair(ast->upvalues[next->lex->operand_expr<luramas::il::lexer::operand_kinds::upvalue>().front()->dis.upvalue_reg].first, -1)));
                  } else {

                        /* Pass variable */

                        const auto reg = next->lex->disassembly->operands.back()->dis.reg;

                        std::string name = "";
                        luramas::emitter_ir::common::locvar::emit_locvar_alphabetical_name(name, ast->lifter_config->upvalue_prefix, std::stoi(std::to_string(ast->ast_id) + std::to_string(reg)) /* Str -> int for formatting */, ast->lifter_config->upvalue_suffix_char);
                        ast->closures[node->closures.closure_idx]->upvalues.insert(std::make_pair(idx++, std::make_pair(name, reg)));

                        /* Change arg name if reg. */
                        for (auto &arg : ast->arg_regs)
                              if (arg.first == reg) {
                                    debug_success("Upvalue is a arguement.");
                                    arg.second = name;
                                    goto next_L;
                              }

                        /* See if function */
                        if (node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == reg) {
                              debug_success("Upvalue is a function.");
                              ast->closures[node->closures.closure_idx]->closure_name->emitter_datatype_upvalue(name);
                              goto next_L;
                        }

                        /* See if var/sub or iterator/sub */
                        const auto back = ast->body->visit_rest_current_flip(node->address);
                        for (const auto &node_ : back) {

                              /* Variable/scoped function */
                              if (node_->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>() && (node_->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == reg || ((node_->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg + node_->variables.multret_amount) >= reg && node_->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg <= reg))) {

                                    if (node_->has_expr(luramas::ast::element_kinds::closure_scoped)) {
                                          /* Scoped function ?? */
                                          debug_success("Upvalue is a scoped function.");

                                          /* Set name of upvalue closure. */
                                          ast->closures[(node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::kvalue>()) ? node_->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx : node_->lex->operand_expr<luramas::il::lexer::operand_kinds::closure>().front()->dis.closure_idx]->closure_name->emitter_datatype_upvalue(name);
                                          break;
                                    } else if (node_->has_expr(luramas::ast::element_kinds::locvar)) {
                                          /* Variable */
                                          debug_success("Upvalue is a scoped variable.");

                                          /* Set multret names if is. */
                                          if (!node_->variables.multret_amount) {
                                                node_->variables.name = name;
                                          } else {
                                                node_->variables.multret_names[reg - node_->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg] = name;
                                          }

                                          node_->add_existance<luramas::ast::element_kinds::locvar_upvalue>();
                                          break;
                                    }

                              } else if (node_->has_expr(luramas::ast::element_kinds::for_iv_start) || node_->has_expr(luramas::ast::element_kinds::for_start) || node_->has_expr(luramas::ast::element_kinds::for_n_start)) {

                                    /* Check for loops */
                                    const auto for_target = node_->loops.end_node;

                                    /* Not a jumpback */
                                    if (for_target->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp > 0)
                                          continue;

                                    /* Iterator */
                                    if (for_target->loops.start_reg <= reg && for_target->loops.end_reg >= reg) {
                                          debug_success("Upvalue is a for loop variable.");
                                          node_->loops.iteration_names.insert(std::make_pair(reg, name));
                                          goto next_L;
                                    }

                                    /* Sub iterator movs for == of capture reg */
                                    if (node_->sub_node != nullptr && node_->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == reg) {
                                          debug_success("Upvalue is a loop variable in relation with subnode.");
                                          node_->loops.iteration_names.insert(std::make_pair(node_->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg, name));
                                          goto next_L;
                                    }

                              } else if (node_->sub_node != nullptr &&
                                         node_->get_sub_node()->has_expr(luramas::ast::element_kinds::locvar) &&
                                         node_->get_sub_node()->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>() &&
                                         (node_->get_sub_node()->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == reg ||
                                             ((node_->get_sub_node()->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg + node_->variables.multret_amount) >= reg && node_->get_sub_node()->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg <= reg))) {

                                    debug_success("Upvalue is apart of a call multret.");

                                    /* Set multret names if is. */
                                    if (!node_->variables.multret_amount) {
                                          node_->get_sub_node()->variables.name = name;
                                    } else {
                                          node_->get_sub_node()->variables.multret_names[reg - node_->get_sub_node()->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg] = name;
                                    }

                                    node_->get_sub_node()->add_existance<luramas::ast::element_kinds::locvar_upvalue>();
                                    goto next_L;
                              }
                        }
                  }

            next_L:
                  next = ast->body->visit_next(next);
            }
      }

      return;
}
