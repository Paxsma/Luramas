#include "../transformers.hpp"

void luramas::ast::transformers::locvars::set_lv(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uint16_t> registers = {std::uint16_t(ast->arg_regs.size())}; /* Registers for scope. (Based on target register) */
      std::uintptr_t routine = 0u;                                                  /* Inside concat, call, table routine, inc for start, dec for end. */

      for (const auto &node : ast->body->visit_all()) {

            /* Turns node into variable. */
            auto node_var = [&](const std::shared_ptr<luramas::il::arch::operand::operand> &operand) -> void {
                  /* Not set yet. */

                  node->add_safe<luramas::ast::element_kinds::stat_locvar>();

                  /* Inc for call if. */
                  if (node->lex->kind == luramas::il::lexer::inst_kinds::ccall) {

                        auto retn = node->lex->operand_kind<luramas::il::lexer::operand_kinds::value>().back()->dis.val;

                        if (retn > 1) {
                              node->variables.multret_amount = static_cast<std::size_t>(retn);
                              node->add_safe<luramas::ast::element_kinds::stat_locvar_multret>();
                        }

                        registers.back() += static_cast<std::uint16_t>(retn);

                  } else {
                        ++registers.back();
                  }

                  /* Set names */
                  if (node->variables.multret_amount) {

                        /* Append to dest. (Fill for multiple retn) */
                        for (auto i = operand->dis.reg; i < (operand->dis.reg + node->variables.multret_amount); ++i) {

                              node->variables.multret_names.emplace_back(luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(ast->lifter_config->variable_prefix, i, ast->lifter_config->var_suffix_char));
                        }

                  } else {
                        node->variables.name = luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(ast->lifter_config->variable_prefix, operand->dis.reg, ast->lifter_config->var_suffix_char);
                  }

                  return;
            };

            /* End of scope. */
            for (auto i = 0u; i < node->count_elem<luramas::ast::element_kinds::stat_scope_end>(); ++i) {
                  registers.pop_back();
            }

            /* Log reg scope start. */
            if (node->has_elem(luramas::ast::element_kinds::stat_for_start) || node->has_elem(luramas::ast::element_kinds::stat_for_n_start) || node->has_elem(luramas::ast::element_kinds::stat_for_iv_start)) {

                  registers.emplace_back(node->loops.end_node->loops.end_reg + 1u);

            } else if (node->has_elem(luramas::ast::element_kinds::stat_if) || node->has_elem(luramas::ast::element_kinds::stat_repeat) || node->has_elem(luramas::ast::element_kinds::stat_while)) {

                  registers.emplace_back(registers.back());

            } else if (node->has_elem(luramas::ast::element_kinds::stat_elseif) || node->has_elem(luramas::ast::element_kinds::stat_else)) {

                  registers.back() = (*(registers.end() - 1));
            }

            /* Inc for concat start, call start, and table start. Dec for concat end, call end, and start end. */
            routine_inc(node, routine);
            routine_dec(node, routine);

            /* scoped closure is apart of a routine mutate it to an anonymous closure. */
            if (node->has_elem(luramas::ast::element_kinds::stat_closure_scoped) && routine) {
                  node->replace_next<luramas::ast::element_kinds::stat_closure_scoped, luramas::ast::element_kinds::stat_closure_anonymous>();
                  ast->closures[node->closures.closure_idx]->closure_kind = luramas::ast::closure_kinds::anonymous;
            }

            /* Not inside routine and node does not have a dest operand. */
            if (!routine && node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::dest>()) {

                  debug_line("Node with dest and no routine on %s", node->str().c_str());

                  auto bad = false; /* Failed any checks (can also be used if node is already set). */
                  const auto dest = node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front();

                  /* Node has a scoped closure so emit a pre-set name too it. */
                  if (node->has_elem(luramas::ast::element_kinds::stat_closure_scoped)) {

                        /* Create closure name. */
                        std::string buffer = "";
                        luramas::emitter_ir::common::locvar::emit_locvar_alphabetical_name(buffer, ast->lifter_config->function_prefix, registers.back(), ast->lifter_config->function_suffix_char);

                        /* It is an scoped closure so emit is as a variable datatype. */
                        ast->closures[node->closures.closure_idx]->closure_name->emitter_datatype_var<true>(buffer);
                  }

                  /* Capture with source garunteeds locvar so check there. */
                  const auto captures = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst_scope<luramas::il::arch::opcodes::OP_SETLIST>(node->address, true));
                  for (const auto &capture : captures)
                        if (capture->lex->has_operand_kind<luramas::il::lexer::operand_kinds::source>() && capture->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.upvalue_reg == dest->dis.reg && luramas::ast::transformers::registers::logical_dest_register(ast, node, dest->dis.reg)) {
#if DEBUG_AST_LOCVAR_REGISTERS
                              if (dest->reg == registers.back()) {
#endif
                                    debug_success("Set capture locvar for %s", node->str().c_str());
                                    node_var(dest);

#if DEBUG_AST_LOCVAR_REGISTERS
                              }
#endif
                              bad = true;
                              break;
                        }
                  if (bad) {
                        continue;
                  }

                  /* Check concat and call routines if the dest is used as a dest in them no locvar. */
                  const auto calls = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_next_expr_scope<luramas::ast::element_kinds::desc_call_routine_start>(node->address, true));
                  for (const auto &call : calls) {

                        const auto node_end = ast->body->visit_relative_next_expr_scope_current<luramas::ast::element_kinds::desc_call_routine_end>(call->address, {luramas::ast::element_kinds::desc_call_routine_start});

                        /* Target dest reg used in call routine dest. */
                        for (const auto &call_node : ast->body->visit_range(call->address, node_end->address))
                              if (call_node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::dest>() && call_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == registers.back()) {
                                    bad = true;
                              }
                  }
                  if (bad) {
                        continue;
                  }

                  /* Concat */
                  const auto concats = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_next_expr_scope<luramas::ast::element_kinds::desc_concat_routine_start>(node->address, true));
                  for (const auto &concat : concats) {

                        const auto node_end = ast->body->visit_relative_next_expr_scope_current<luramas::ast::element_kinds::desc_concat_routine_end>(concat->address, {luramas::ast::element_kinds::desc_concat_routine_start});

                        /* Target dest reg used in call routine dest. */
                        for (const auto &concat_node : ast->body->visit_range(concat->address, node_end->address))
                              if (concat_node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::dest>() && concat_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == registers.back()) {
                                    bad = true;
                              }
                  }
                  if (bad) {
                        continue;
                  }

                  /*

					After checking concat and call routines which can garunteed a variable this is where we mostly look at hueristics and decide if
					a register will become a variable or not. Either one it's set too will effect the preformance of the decompiled code and accuracy.

				*/

                  /* Mutate scoped closure to anonymous. */
                  if (node->has_elem(luramas::ast::element_kinds::stat_closure_scoped)) {

                        if (dest->dis.reg != registers.back()) {
                              node->replace_next<luramas::ast::element_kinds::stat_closure_scoped, luramas::ast::element_kinds::stat_closure_anonymous>();
                              ast->closures[node->closures.closure_idx]->closure_kind = luramas::ast::closure_kinds::anonymous;
                        } else {

                              std::string buffer = "";
                              luramas::emitter_ir::common::locvar::emit_locvar_alphabetical_name(buffer, ast->lifter_config->function_prefix, registers.back()++, ast->lifter_config->function_suffix_char);

                              /* Scoped so variable */
                              ast->closures[node->closures.closure_idx]->closure_name->emitter_datatype_var<true>(buffer);
                        }

                  } else {

#if DEBUG_AST_LOCVAR_REGISTERS
                        if (dest->reg == registers.back()) {
#endif

                              /* Check multret */
                              bool logical_mult = false;
                              if (node->lex->kind == luramas::il::lexer::inst_kinds::ccall) {

                                    auto retn = node->lex->operand_kind<luramas::il::lexer::operand_kinds::value>().back()->dis.val;

                                    if (retn > 1) {

                                          /* Check all multrets */
                                          for (auto i = dest->dis.reg; i < (dest->dis.reg + retn); ++i)
                                                if (luramas::ast::transformers::registers::logical_dest_register(ast, node, i)) {
                                                      logical_mult = true;
                                                }
                                    }
                              }

                              /* See if dest register is logical. */
                              if (logical_mult || luramas::ast::transformers::registers::logical_dest_register(ast, node, dest->dis.reg)) {

                                    /* Mutate global */
                                    if (node->has_elem(luramas::ast::element_kinds::stat_closure_global)) {

                                          node->replace_next<luramas::ast::element_kinds::stat_closure_global, luramas::ast::element_kinds::stat_closure_scoped>();
                                          node->remove_all_elem<luramas::ast::element_kinds::stat_closure_global>();

                                          ast->closures[node->closures.closure_idx]->closure_kind = luramas::ast::closure_kinds::scoped;

                                          if (node->closures.setglobal_node != nullptr) {
                                                node->closures.setglobal_node->remove_all_elem<luramas::ast::element_kinds::desc_dead_instruction>();
                                          }

                                          std::string buffer = "";
                                          luramas::emitter_ir::common::locvar::emit_locvar_alphabetical_name(buffer, ast->lifter_config->function_prefix, registers.back()++, ast->lifter_config->function_suffix_char);

                                          /* Global */
                                          ast->closures[node->closures.closure_idx]->closure_name->emitter_datatype_global<true>(buffer);

                                    } else {
                                          debug_success("Set normal locvar for %s", node->str().c_str());
                                          node_var(node->lex->disassembly->operands.front());
                                    }

                                    /* Has indexes, remove dead instructions. */
                                    if (node->closures.idx_nodes.first != nullptr) {

                                          const auto range = ast->body->visit_range_current(node->closures.idx_nodes.first->address, node->closures.idx_nodes.second->address);
                                          for (const auto &i : range) {
                                                i->remove_all_elem<luramas::ast::element_kinds::desc_dead_instruction>();
                                          }
                                    }
                              }

                              continue;

#if DEBUG_AST_LOCVAR_REGISTERS
                        }
#endif
                  }

            } else if (!routine && node->has_elem(luramas::ast::element_kinds::table_end)) { /* No routine and end of table garunteed locvar. */
                  debug_success("Set table locvar for %s", node->str().c_str());
                  node_var(node->lex->disassembly->operands.front());
            }
      }

      return;
}

void luramas::ast::transformers::locvars::set_logical_operations(std::shared_ptr<luramas::ast::ast> &ast) {
      return;
      auto all = ast->body->visit_all();
      for (auto &i : all) {

            /* Branch condition or a instruction that writes compare flag too dest. */
            const auto i_next = ast->body->visit_next(i);
            if (i->lex->kind == luramas::il::lexer::inst_kinds::branch_condition || i->lex->kind == luramas::il::lexer::inst_kinds::compare_dest) {

                  bool node_nit_b = false;
                  auto cached_init = i;                                   /* Mutable by logical operations. */
                  std::vector<std::uint16_t> compares;                    /* Singular loadb compare(jmp 1+; loadb r1 +1; loadb r1 0; ???) jumps log registers and see if it gets used in compare first. */
                  std::vector<std::uint16_t> compares_double;             /* Double compare(jmp 3+ loadb r1,+1; loadb r1 0; loadb r2, 0; ???) */
                  std::shared_ptr<luramas::ast::node> node_hit = nullptr; /* Prevents infinite looping if something goes wrong. */

                  /* Make sure it hasnt already been analyzed by another routine. */
                  if (!i->has_elem(luramas::ast::element_kinds::condition_logical_start) &&
                      !i->has_elem(luramas::ast::element_kinds::condition_logical) &&
                      !i->has_elem(luramas::ast::element_kinds::condition_logical_end) &&
                      !i->has_elem(luramas::ast::element_kinds::condition_concat_member) &&
                      !i->has_elem(luramas::ast::element_kinds::condition_concat_start) &&
                      !i->has_elem(luramas::ast::element_kinds::condition_concat_end)) {

                        /* Operation could be logical expression? */
                        bool predicted_logical = false;

                        do {

                              debug_success("Starting with node %s", i->str().c_str());

                              /* Attempts to prevent bugs. */
                              if (node_hit != i) {
                                    node_hit = i;
                                    node_nit_b = false;
                              } else {

                                    /* Looped over 3 on the same node. */
                                    if (node_nit_b) {
                                          debug_warning("Looped infinitly for %s", i->str().c_str());
#if DEBUG_AST_DISPLAY_WARNINGS
                                          std::printf("[WARNING] Looped infinitely on %s for logical conditions.\n", i->lex->disassembly->data.c_str());
#endif
                                          break;
                                    }

                                    node_nit_b = true;
                              }

                              /* No compare routine */
                              if (i->lex->kind != luramas::il::lexer::inst_kinds::compare_dest &&
                                  !i->has_elem(luramas::ast::element_kinds::desc_conditional_routine_start) &&
                                  !i->has_elem(luramas::ast::element_kinds::desc_conditional_routine_end) &&
                                  !i->has_elem(luramas::ast::element_kinds::desc_conditional_routine)) {
                                    debug_warning("No compare routine for %s", i->str().c_str());
                                    break;
                              }

                              /* Get next compare jump if current isnt one. */
                              if (i->lex->kind != luramas::il::lexer::inst_kinds::compare_dest && i->lex->kind != luramas::il::lexer::inst_kinds::branch_condition) {
                                    i = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(i->address, false));
                                    debug_line("Current wasn't compare changed too %s", i->str().c_str());
                              }

                              /* Not a compare branch something went wrong. */
                              if (i == nullptr || (i->lex->kind != luramas::il::lexer::inst_kinds::branch_condition && i->lex->kind != luramas::il::lexer::inst_kinds::compare_dest)) {
                                    debug_warning("Current is not a compare.");
                                    break;
                              }

                              auto current_jmp = (i->lex->kind == luramas::il::lexer::inst_kinds::compare_dest) ? 0u : i->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr; /* Current jump target address. */

                              /* Check range for return if there is a return. It has a return so break, end concatation of branches. */
                              if (current_jmp) {

                                    bool retn = false;
                                    const auto range = ast->body->visit_range(i->address, current_jmp);
                                    for (const auto &i : range) {
                                          if (i->lex->kind == luramas::il::lexer::inst_kinds::return_) {
                                                retn = true;
                                                break;
                                          }
                                    }
                                    if (retn) { /* Not concat */
                                          debug_warning("Has return, not concat.");
                                          break;
                                    }
                              }

                              /* Check next jump and current for different breaks. */
                              if (current_jmp) {

                                    const auto next_jmp = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(i->address, false));
                                    if (next_jmp != nullptr) {

                                          const auto jmp_prev = ast->body->visit_previous_addr(current_jmp);
                                          const auto next_jmp_prev = ast->body->visit_previous_addr(next_jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);

                                          if (jmp_prev != nullptr && next_jmp_prev != nullptr && stat_jump_break(jmp_prev) && stat_jump_break(next_jmp_prev)) {

                                                /* Not same jump */
                                                if (jmp_prev->address != next_jmp_prev->address) {
                                                      debug_warning("Next is break out.");
                                                      break;
                                                }
                                          }
                                    }
                              }

                              /* Get any jumps that jumps out of current jump and isnt a break. */
                              std::shared_ptr<luramas::ast::node> jump_out_n = nullptr;
                              std::shared_ptr<luramas::ast::node> jump_out_temp = nullptr;

                              /* There is a jump? */
                              if (current_jmp) {
                                    do {
                                          jump_out_n = jump_out_temp;
                                          // jump_out_temp = jump_out(
                                          //     (jump_out_n == nullptr) ? i->address : jump_out_n->address,
                                          //     (jump_out_n == nullptr) ? current_jmp : jump_out_n->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                                    } while (jump_out_temp != nullptr);
                              }

                              /* Jump out so pass throught again. */
                              if (jump_out_n != nullptr) {
                                    i = jump_out_n;
                                    debug_success("Jump out found for %s", i->str().c_str());
                              } else {

                                    /* Check for compares with current branch. */
                                    const auto compare_count = i->lex->count_operand_kind<luramas::il::lexer::operand_kinds::compare>();
                                    if (!compares.empty() || !compares_double.empty()) {

                                          const auto compare_operands = i->lex->operand_kind<luramas::il::lexer::operand_kinds::compare>();

                                          if (compare_count == 1u && compares.size() == 1u) {

                                                /* Singular */
                                                if (!compares.empty() && compare_operands.front()->dis.reg == compares.front()) {
                                                      compares.clear(); /* Hit */
                                                }

                                                /* Double */
                                                if (!compare_operands.empty() && compare_operands.front()->dis.reg == compares_double.front()) {
                                                      compares_double.clear(); /* Hit */
                                                }

                                          } else if (compares.size() == 2u) { /* Max compare is 2 */

                                                std::vector<std::uint16_t> t_vect = {compare_operands.front()->dis.reg, compare_operands.back()->dis.reg};

                                                std::sort(t_vect.begin(), t_vect.end());
                                                std::sort(compares.begin(), compares.end());
                                                std::sort(compares_double.begin(), compares_double.end());

                                                /* Check for compares when sorted. */

                                                /* Singular */
                                                if (!compares.empty() && compares.front() == t_vect.front() && compares.back() == t_vect.back()) {
                                                      compares.clear(); /* Hit */
                                                }

                                                /* Double */
                                                if (!compares_double.empty() && compares_double.front() == t_vect.front() && compares_double.back() == t_vect.back()) {
                                                      compares_double.clear(); /* Hit */
                                                }
                                          }
                                    }

                                    /* Make sure branch compare has 2 compares. */
                                    if (compare_count != 2u && i->lex->kind != luramas::il::lexer::inst_kinds::compare_dest) {

                                          debug_line("Compare count isn't 2.");

                                          const auto next = ast->body->visit_next(i);

                                          /* Nothing */
                                          if (next == nullptr) {
                                                debug_warning("Next is nullptr.");
                                                break;
                                          }
                                    }

                                    const auto next = ast->body->visit_next(i);

                                    /* Nothing */
                                    if (next == nullptr) {
                                          debug_warning("Next is nullptr.");
                                          break;
                                    }

                                    /* Next is a compare dest. */
                                    if (next->lex->kind == luramas::il::lexer::inst_kinds::compare_dest) {

                                          debug_line("Next is a compare dest on %s", next->str().c_str());

                                          compares.push_back(next->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg);

                                          /* Exceeded max of 2 routine is something else. */
                                          if (compares.size() > 2u) {
                                                debug_warning("Compares exceeded max of 2.");
                                                break;
                                          }

                                          /* Set next and contiue */
                                          i = next;
                                          continue;
                                    }

                                    /* Previous from jump must be loadb. */
                                    if (current_jmp) {

                                          const auto prev = ast->body->visit_previous_addr(current_jmp);
                                          if (prev->lex->kind == luramas::il::lexer::inst_kinds::compare_dest) {

                                                debug_line("Previous is a compare dest at %s", prev->str().c_str());

                                                compares_double.emplace_back(prev->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg);

                                                /* Exceeded max of 2 routine is something else. */
                                                if (compares_double.size() > 2u) {
                                                      debug_warning("Compares exceeded max of 2.");
                                                      break;
                                                }

                                                /* Visit jump */
                                                i = ast->body->visit_addr(current_jmp);
                                                continue;

                                          } else {

                                                /* Analyze range of start current and jump. */
                                                auto next_inst = ast->body->visit_next(i);
                                                const auto next_jmp = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(i->address, false));

                                                /* No next end. */
                                                if (next_inst == nullptr) {
                                                      debug_warning("Next instruction is nullptr.");
                                                      break;
                                                }
                                                if (next_jmp == nullptr) {

                                                      debug_warning("Next jump is nullptr.");

                                                      /* Still could be logical exprssion. */

                                                      /* Logical operation. */
                                                      if (ast->body->filled(i, prev)) {

                                                            debug_result("Current - previous is filled %s - %s", i->str().c_str(), prev->str().c_str());

                                                            i = ast->body->visit_addr(current_jmp);
                                                            debug_line("Current - previous is filled, current changed too %s", i->str().c_str());

                                                            if (cached_init->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                                                                  const auto prev = ast->body->visit_previous_addr(cached_init->address);

                                                                  if (prev != nullptr) {
                                                                        cached_init = prev;
                                                                  }

                                                                  debug_result("Cached changed too %s", cached_init->str().c_str());
                                                            }

                                                            predicted_logical = true;
                                                            debug_success("Predicted logical is true.");
                                                      }

                                                      break;
                                                }

                                                /* Jump data */
                                                const auto next_jmp_target = next_jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                                                const auto next_jmp_target_node = ast->body->visit_addr(next_jmp_target);

                                                /* Jump too same address possibly or? */
                                                if (current_jmp == next_jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) {

                                                      debug_line("Current jump = next jump on %s", next_jmp->str().c_str());

                                                      /* No compare routine */
                                                      if (!next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine_start) && !next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine_end) && !next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine)) {

                                                            debug_line("No compare routine for %s", next_inst->str().c_str());

                                                            /* Logical operation. */
                                                            if (ast->body->filled(i, ast->body->visit_previous_addr(next_jmp->address))) {

                                                                  debug_success("Next jump and current is filled.");

                                                                  i = next_jmp;

                                                                  if (cached_init->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                                                                        cached_init = ast->body->visit_previous_addr(cached_init->address);
                                                                  }

                                                                  predicted_logical = true;

                                                            } else {
                                                                  debug_warning("Next jump and current is not filled.");
                                                                  break;
                                                            }

                                                      } else {

                                                            /* Has compare routine following next. */
                                                            i = next_jmp;
                                                            debug_line("Current mutated too %s", i->str().c_str());

                                                            /* Maybe logical operation? */
                                                            if (i->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                                                                  debug_line("Current is branch condition.");

                                                                  const auto jmp_target_next = i->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;

                                                                  /* Logical operation */
                                                                  if (ast->body->filled(i, ast->body->visit_previous_addr(jmp_target_next))) {

                                                                        debug_success("Previous jump and current is filled.");

                                                                        i = ast->body->visit_previous_addr(jmp_target_next);

                                                                        if (cached_init->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                                                                              cached_init = ast->body->visit_previous_addr(cached_init->address);
                                                                        }

                                                                        predicted_logical = true;
                                                                  }
                                                            }
                                                      }

                                                      continue;
                                                }

                                                /* Next is loadb check also next. */
                                                if (next_inst->lex->disassembly->op == luramas::il::arch::opcodes::OP_LOADBOOL) {
                                                      next_inst = ast->body->visit_next(next_inst);
                                                      debug_line("Next instuction mutated too %s", next_inst->str().c_str());
                                                }

                                                /* Logical operation. */
                                                if (ast->body->filled(i, prev)) {

                                                      debug_success("Current and previous is filled for %s", prev->str().c_str());

                                                      i = ast->body->visit_addr(current_jmp);

                                                      if (cached_init->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                                                            cached_init = ast->body->visit_previous_addr(cached_init->address);
                                                      }

                                                      predicted_logical = true;
                                                }

                                                /* Logical operation. */
                                                if (ast->body->filled(i, ast->body->visit_previous_addr(next_jmp_target))) {

                                                      /* Check too see if regs gets used. */
                                                      debug_success("Current and previous instruction is filled.");

                                                      i = ast->body->visit_addr(current_jmp);

                                                      if (cached_init->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                                                            cached_init = ast->body->visit_previous_addr(cached_init->address);
                                                      }

                                                      predicted_logical = true;
                                                }

                                                /* Next leads too routine */
                                                if (next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine_start) || next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine_end) || next_inst->has_elem(luramas::ast::element_kinds::desc_conditional_routine)) {

                                                      debug_line("Next leads to condition for %s", next_inst->str().c_str());

                                                      /* Check loadbs */

                                                      const auto next = ast->body->visit_addr(i->address + i->lex->disassembly->len);

                                                      /* Nothing */
                                                      if (next == nullptr) {
                                                            debug_warning("Next is nullptr.");
                                                            break;
                                                      }

                                                      /* Next has loadb with jump. */
                                                      if (next->lex->disassembly->op == luramas::il::arch::opcodes::OP_LOADBOOL && next->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp) {

                                                            /* Singular */
                                                            if (next_jmp_target == (next->address + next->lex->disassembly->len)) {
                                                                  /* Set next and contiue */
                                                                  i = next_jmp;
                                                                  continue;
                                                            }
                                                      }

                                                      /* Take jump and check previouses */
                                                      const auto prev = ast->body->visit_previous_addr(next_jmp_target);
                                                      if (prev->lex->disassembly->op == luramas::il::arch::opcodes::OP_LOADBOOL) {

                                                            /* Doesnt have jump check previous. */
                                                            if (!prev->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp) {

                                                                  const auto p_prev = ast->body->visit_previous_addr(prev->address);

                                                                  /* Has previous loadb jump.  */
                                                                  if (p_prev->lex->disassembly->op == luramas::il::arch::opcodes::OP_LOADBOOL && p_prev->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp) {

                                                                        /* Check logical could be locvar. */
                                                                        if (luramas::ast::transformers::registers::logical_dest_register(ast, p_prev, p_prev->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg)) {
                                                                              break;
                                                                        }

                                                                        i = next_jmp;
                                                                        continue;
                                                                  } else {
                                                                        break;
                                                                  }

                                                            } else {

                                                                  /* Target goes to loadb, same regs and jump. */
                                                                  if (next_jmp_target_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg == prev->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg) {
                                                                        /* Set next and contiue */

                                                                        /* Check logical could be locvar. */
                                                                        if (luramas::ast::transformers::registers::logical_dest_register(ast, next_jmp_target_node, next_jmp_target_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg)) {
                                                                              break;
                                                                        }

                                                                        i = next_jmp;
                                                                        continue;
                                                                  }
                                                            }
                                                      }
                                                }

                                                /* Check all jump conditions within jump and see if any hit jump to current jump target and all conditions are all filled. */
                                                const auto cond_jumps = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_type_next_addr<luramas::il::lexer::inst_kinds::branch_condition>(true, i->address));
                                                for (const auto &jmp : cond_jumps) {

                                                      const auto jmp_target = jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                                                      const auto next = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(jmp->address, false));

                                                      /* No next jump. */
                                                      if (next == nullptr || next->address >= jmp_target) {
                                                            break;
                                                      }
                                                }

                                                /* Check next jump loc previous for jumpout. */
                                                const auto next_loc_prev_jmp = ast->body->visit_previous_addr(next_jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                                                if (next_loc_prev_jmp->has_elem(luramas::ast::element_kinds::desc_conditional_jumpout)) {

                                                      debug_result("Next jump loc previous jumps out for %s", next_jmp->str().c_str());
                                                      i = ast->body->visit_prev_type<luramas::il::lexer::inst_kinds::branch_condition>(next_loc_prev_jmp->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);

                                                      continue;
                                                }

                                                debug_warning("Couldn't find anything, breaking for %s", i->str().c_str());
                                                break;
                                          }
                                    }
                              }

                        } while (true);

                        cached_init->add_elem<luramas::ast::element_kinds::condition_logical_start>();
                        debug_result("Cached init added expr conditon logical start on %s", cached_init->str().c_str());
                        debug_line("Current is currently %s", i->str().c_str());

                        /* Fix i. */
                        if (!predicted_logical && (i->lex->kind != luramas::il::lexer::inst_kinds::compare_dest || i->lex->kind != luramas::il::lexer::inst_kinds::branch_condition)) {

                              auto prev = ast->body->visit_previous_addr(i->address);
                              prev = (prev != nullptr) ? ((cached_init->address < prev->address) ? prev : i) : i;

                              if (prev->lex->kind == luramas::il::lexer::inst_kinds::compare_dest || prev->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                                    i = prev;
                                    debug_result("Mutated current too %s", i->str().c_str());
                              }
                        }

                        if (predicted_logical) {

                              debug_line("Is predicted logical.");

                              i->add_elem<luramas::ast::element_kinds::conditional_expression_predicted>();
                              debug_success("Added conditional expression predicted too current on %s", i->str().c_str());

                              /* Fix i too previous instruction from jump. */
                              const auto prev_cond = ast->body->visit_prev_type_current<luramas::il::lexer::inst_kinds::branch_condition>(i->address);
                              if (prev_cond != nullptr && i->address == prev_cond->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) {

                                    i = ast->body->visit_previous_addr(i->address);
                                    debug_result("Mutated current for predicted logical too %s", i->str().c_str());
                              }
                        }

                        debug_line("Adding condition logical exprs too members range from %s -  %s", cached_init->str().c_str(), i->str().c_str());

                        /* Members */
                        const auto range = ast->body->visit_range(cached_init->address, i->address);
                        for (const auto &i : range)
                              i->add_elem<luramas::ast::element_kinds::condition_logical>();

                        /* End */
                        i->add_elem<luramas::ast::element_kinds::condition_logical_end>();
                        debug_success("Added condition logical end expr too %s", i->str().c_str());
                  }
            }
      }

      return;
}
