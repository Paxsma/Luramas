#include "../transformers.hpp"

void luramas::ast::transformers::logicals::set_logical_expression(std::shared_ptr<luramas::ast::ast> &ast) {

      std::int32_t count = 0;
      std::shared_ptr<luramas::ast::node> start_node = nullptr;
      std::shared_ptr<luramas::ast::node> target_cond = nullptr;

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            auto set = [&]() mutable -> void {
                  debug_success("Setting logical expression start %s, end %s", start_node->str().c_str(), i->str().c_str());

                  /* Set start */
                  start_node->add_existance<luramas::ast::element_kinds::conditional_expression_start>();
                  start_node->add_existance<luramas::ast::element_kinds::condition_concat_start>();

                  /* Set ends */
                  i->add_existance<luramas::ast::element_kinds::condition_concat_end>();
                  i->add_existance<luramas::ast::element_kinds::conditional_expression_end>();
                  i->add_existance<luramas::ast::element_kinds::condition_append_source>();

                  /* Remove OP_LOADBSKIP dead expr. */
                  i->remove_expr<luramas::ast::element_kinds::dead_instruction>();

                  /* Set logical operations. */
                  luramas::ast::transformers::branches::set(ast, start_node->address, i->address, {target_cond->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr}, -1, true, true, true);

                  /* Remove emitted next */
                  target_cond->remove_expr<luramas::ast::element_kinds::condition_emit_next>();
            };

            if (i->has_expr(luramas::ast::element_kinds::condition_logical_start)) {

                  debug_line("Current node has logical start %s", i->str().c_str());

                  /* Set start */
                  if (!count) {
                        start_node = i;
                  }

                  ++count;
                  debug_result("Increased count too %d", count);
            }

            if (count != 1u && i->has_expr(luramas::ast::element_kinds::condition_logical_end)) {

                  debug_line("Decreasing count because current node has logical end expr on %s", i->str().c_str());
                  --count;
                  debug_result("Decreased count too %d", count);

            } else if (i->has_expr(luramas::ast::element_kinds::condition_logical_end)) {

                  debug_line("Current node has condition logical end expr on %s", i->str().c_str());

                  auto target = i;
                  const auto loadb = i->lex->disassembly->op == luramas::il::arch::opcodes::OP_LOADBOOL || i->lex->kind == luramas::il::lexer::inst_kinds::compare;

                  if (!i->has_expr(luramas::ast::element_kinds::conditional_expression_predicted)) {

                        debug_line("Current node doesn't have conditional expression predicted on %s", i->str().c_str());

                        /* See if loadb with jump exists if so get it. */
                        if (!target->lex->has_operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>() || !target->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().back()->dis.val) {
                              debug_line("Target doesnt have jump operand on %s", target->str().c_str());
                              target = ast->body->visit_previous_addr(target->address);
                        }

                        /* Not expression */
                        if (target == nullptr || (target->lex->kind != luramas::il::lexer::inst_kinds::compare_dest && target->lex->kind != luramas::il::lexer::inst_kinds::branch_condition)) {

                              debug_line("Target is nullptr, not loadb, not set compare flag, or branch condition on %s", target->str().c_str());
                              --count;
                              debug_result("Decreased count too %d", count);

                              continue;
                        }

                        if (!target->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().back()->dis.val) {

                              debug_line("Target doesn't jump on %s", target->str().c_str());
                              --count;
                              debug_result("Decreased count too %d", count);

                              continue;
                        }

                        target_cond = (target->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) ? target : ast->body->visit_previous_addr(target->address);

                        /* Not expression */
                        if (target_cond == nullptr || target_cond->lex->kind != luramas::il::lexer::inst_kinds::branch_condition) {
                              debug_result("Target condition is nullptr or isnt a branch condition.");
                              --count;
                              debug_result("Decreased count too %d", count);
                        } else if ((loadb && target_cond->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr == i->address) || (!loadb && target_cond->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr == (i->address + i->lex->disassembly->len))) {

                              debug_line("Target condition is expected loadb with jump or expected branch compare on %s", target->str().c_str());

                              if (!(--count)) {
                                    set();
                              }

                              debug_result("Decreased count too %d", count);
                        } else {

                              debug_warning("Nothing hit decreasing count.");
                              --count;
                              debug_result("Decreased count too %d", count);
                        }

                  } else {

                        debug_line("Current node does have conditional expression predicted on %s", i->str().c_str());

                        /* Get previous from address with type of branch compare. */
                        target_cond = ast->body->visit_prev_type_current<luramas::il::lexer::inst_kinds::branch_condition>(i->address);

                        /* Shouldn't happen but incase it does. */
                        if (target_cond == nullptr) {
                              throw std::runtime_error("target_cond returned nullptr.");
                        }

                        --count;
                        set();
                  }
            }
      }

      return;
}
