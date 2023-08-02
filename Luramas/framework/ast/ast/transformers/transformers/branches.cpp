#include "../transformers.hpp"

void luramas::ast::transformers::branches::set(std::shared_ptr<luramas::ast::ast> &ast, const std::uintptr_t begin, const std::uintptr_t end, const std::vector<std::uintptr_t> dead /* Always opposite and when hit. */, const std::int16_t logical_operation_target /* Used for ignoring ands/ors. */, const bool check_loops /* Checks jumps too see if they lead too loop by expr and preform opposite. */, const bool last_include /* Includes last compare as or. */, const bool logical_operation /* ??? */) {

      const auto conditions = ast->body->visit_range_type_current<luramas::il::lexer::inst_kinds::branch_condition>(begin, end);
      const auto over_target = *std::max_element(dead.begin(), dead.end());
      const auto range = ast->body->visit_range_current(begin, end);

      std::vector<std::pair<std::uintptr_t /* Jump */, std::size_t /* Scopes */>> scopes;
      std::unordered_map<std::uintptr_t /* Jmp loc */, std::size_t /* Count */> jmp_hit;

      const auto has_val = [&scopes](const std::uintptr_t addr) {
            return std::find_if(scopes.begin(), scopes.end(), [&addr](const std::pair<std::uintptr_t, std::size_t> &ele) { return ele.first == addr; }) != scopes.end();
      };

      debug_line("End on %s", ast->body->visit_addr(end)->str().c_str());

      for (const auto &node : range) {

            if (node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition || node->lex->kind == luramas::il::lexer::inst_kinds::branch) {

                  debug_line("Condition at %s", node->str().c_str());

                  const auto jmp = node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;

                  /* Add hit */
                  if (jmp_hit.find(jmp) == jmp_hit.end()) {
                        jmp_hit.insert(std::make_pair(jmp, 0u));
                  } else {
                        ++jmp_hit[jmp];
                  }

                  /* Hit dead? */
                  if (std::find(dead.begin(), dead.end(), jmp) != dead.end()) {

                        debug_line("Jump location is end.");

                        /* Close any open scopes. */
                        if (!scopes.empty() && scopes.back().second) {
                              debug_result("Adding condition close expr.");
                              node->add_expr<luramas::ast::element_kinds::condition_end_scope>(scopes.back().second);
                        }

                        /* Not the last branch. */
                        if (conditions.back() != node) {
                              debug_success("Condition isnt last adding and expr.");
                              node->add_expr<luramas::ast::element_kinds::condition_or>();
                        }

                        /* Always opposite */
                        if (!logical_operation) {

                              node->branches.opposite = true;

                              /* Change "or" too "and" because opposite. */
                              if (node->has_expr(luramas::ast::element_kinds::condition_or)) {
                                    node->replace_next<luramas::ast::element_kinds::condition_or, luramas::ast::element_kinds::condition_and>(node->get_expr<luramas::ast::element_kinds::condition_or>().second);
                              }

                              debug_line("Node condition has been set too oposite.");
                        }

                        /* Include last */
                        if (conditions.back() == node && last_include) {
                              debug_success("Condition is last with included last, adding or expr.");
                              node->add_expr<luramas::ast::element_kinds::condition_or>();
                              node->branches.opposite = false;
                        }

                        /* Fix for loop/while/repeat/generic */
                        const auto prev = ast->body->visit_previous_addr(jmp);
                        if (check_loops && prev != nullptr && condition_break_out(prev)) {

                              debug_line("Node condition has been set too original for loop.");
                              node->branches.opposite = false;
                        }

                  } else {

                        /* Add some scope if there is none just too garunteed a scope. */
                        if (scopes.empty()) {
                              scopes.push_back(std::make_pair(jmp, 0u));
                        }

                        if (node->has_expr(luramas::ast::element_kinds::condition_emit_next)) {

                              debug_warning("Conditon emits to next.");

                              /* Condition emits compare to loadb. If there is no logical condition for variable exit thought if there is check writing register with dest register of loadb. */
                              if (logical_operation_target == -1) {
                                    continue;
                              } else if (logical_operation_target != -1 && ast->body->visit_next(node)->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg != logical_operation_target) {
                                    continue;
                              }
                        }

                        /* Jmp exceeds dead values. */
                        if (jmp > over_target) {

                              debug_line("Jump exceeds target.");

                              /* Last condition isnt current so or. */
                              if (conditions.back() != node) {
                                    debug_success("Condition isnt current adding or.");
                                    node->add_expr<luramas::ast::element_kinds::condition_or>();
                              } else {

                                    /* Close any open scopes. */
                                    if (scopes.back().second) {
                                          debug_result("Closing open scope(s).");
                                          node->add_expr<luramas::ast::element_kinds::condition_end_scope>(scopes.back().second);
                                    }
                              }

                        } else {

                              /* Next hit end scope. */
                              if (scopes.back().first == (node->address + node->lex->disassembly->len)) {

                                    if (scopes.back().second) {
                                          debug_result("Closing open scope(s).");
                                          node->add_expr<luramas::ast::element_kinds::condition_end_scope>(scopes.back().second);
                                    }

                                    scopes.pop_back();

                                    /* and/or */
                                    if (!scopes.empty()) {

                                          if (jmp_hit[jmp]) {
                                                debug_success("Adding or expr.");
                                                node->add_expr<luramas::ast::element_kinds::condition_or>();
                                          } else {
                                                debug_success("Adding and expr.");
                                                node->add_expr<luramas::ast::element_kinds::condition_and>();
                                          }
                                    }

                              } else {

                                    /* New and sub scope */
                                    if (scopes.back().first != jmp && scopes.back().first > jmp) {

                                          debug_success("New sub scope adding, and open exprs.");

                                          /* Change "or" too "and" because opposite. */
                                          if (node->has_expr(luramas::ast::element_kinds::condition_or)) {
                                                node->replace_next<luramas::ast::element_kinds::condition_or, luramas::ast::element_kinds::condition_and>(node->get_expr<luramas::ast::element_kinds::condition_or>().second);
                                          } else {
                                                node->add_expr<luramas::ast::element_kinds::condition_and>();
                                          }

                                          node->add_expr<luramas::ast::element_kinds::condition_start_scope>();
                                          scopes.push_back(std::make_pair(jmp, 1u /* Starts off with one could inc. */));
                                          node->branches.opposite = true;

                                    } else {

                                          /* Jmps too end. */
                                          if (jmp == end || jmp == (end + ast->body->visit_addr(end)->lex->disassembly->len)) {

                                                if (logical_operation) {
                                                      debug_success("Jumps too end adding and expr.");
                                                      node->add_expr<luramas::ast::element_kinds::condition_and>();
                                                      node->branches.opposite = true;
                                                } else {
                                                      debug_success("Jumps too end adding or expr.");
                                                      node->add_expr<luramas::ast::element_kinds::condition_or>();
                                                }

                                          } else {

                                                /* Jmp doesnt = current and its found in scopes. */
                                                if (scopes.back().first != jmp && has_val(jmp)) {
                                                      debug_success("Jump isnt current and found in scopes adding or expr.");
                                                      node->add_expr<luramas::ast::element_kinds::condition_or>();
                                                } else {
                                                      debug_success("Adding and expr.");
                                                      node->add_expr<luramas::ast::element_kinds::condition_and>();
                                                      node->branches.opposite = true;
                                                }
                                          }
                                    }
                              }
                        }
                  }
            }
      }

      return;
}

void luramas::ast::transformers::branches::set_valid_branch_routine(std::shared_ptr<luramas::ast::ast> &ast) {

      std::size_t idx = 0u;
      std::size_t routine = 0u;

      const auto conditions = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst_kinds<luramas::il::lexer::inst_kinds::branch_condition>(true));

      /* Nothing to analyze */
      if (conditions.empty()) {
            return;
      }

      auto condition = conditions[idx++]->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>();
      auto target_1 = condition.front()->dis.reg;
      auto target_2 = (condition.size() > 1u) ? condition.back()->dis.reg : -1;
      bool used_target_1 = false;
      bool used_target_2 = false;

      std::shared_ptr<luramas::ast::node> start_node = nullptr;

      /* Checks operands if targets get used or not but if it does get used just resets target. */
      std::function<void(const std::shared_ptr<luramas::il::arch::operand::operand> &, const luramas::il::lexer::operand_kinds)> check_usage = [&](const std::shared_ptr<luramas::il::arch::operand::operand> &operand, const luramas::il::lexer::operand_kinds tt) mutable {
            const auto val = operand->dis.reg;

            if (val == target_1) {
                  used_target_1 = false;
                  start_node = nullptr;
            }

            if (target_2 != -1 && signed(val) == target_2) {
                  used_target_2 = false;
                  start_node = nullptr;
            }

            return;
      };

      auto all = ast->body->visit_all();
      for (auto &node : all) {

            node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::source>(check_usage);
            node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::reg>(check_usage);

            /* Inc for concat start, call start, and table start. Dec for concat end, call end, and start end. */
            routine_inc(node, routine);
            routine_dec(node, routine);

            if (!routine && node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                  const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                  /* Check target usage. Abrubt end. */
                  if (dest == target_1) {

                        /* Set start node for buffer. */
                        if (start_node == nullptr) {
                              start_node = node;
                        }

                        /* Set twice without used. Abrubt end. */
                        if (used_target_1) {
                              used_target_1 = false;
                              used_target_2 = false;
                              start_node = nullptr;
                              continue;
                        }

                        used_target_1 = true;
                  }

                  if (target_2 != -1 && signed(dest) == target_2) {

                        /* Set start node for buffer. */
                        if (start_node == nullptr) {
                              start_node = node;
                        }

                        /* Set twice without used. Abrubt end. */
                        if (used_target_2) {
                              used_target_1 = false;
                              used_target_2 = false;
                              start_node = nullptr;
                              continue;
                        }

                        used_target_2 = true;
                  }
            }

            if (node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                  /* Start node is null so just use current node. */
                  if (start_node == nullptr) {
                        start_node = node;
                  }

                  /* Append condition routine expr. */
                  start_node->add_expr<luramas::ast::element_kinds::condition_routine_start>();

                  node->add_expr<luramas::ast::element_kinds::condition_routine_end>();

                  const auto range_br = ast->body->visit_range(start_node->address, node->address);
                  for (const auto &i : range_br) {
                        i->add_expr<luramas::ast::element_kinds::condition_routine>();
                  }

                  /* Set next */
                  if (conditions.size() != idx) {
                        condition = conditions[idx++]->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>();
                        target_1 = condition.front()->dis.reg;
                        target_2 = (condition.size() > 1u) ? condition.back()->dis.reg : -1;
                        used_target_1 = false;
                        used_target_2 = false;
                        start_node = nullptr;
                  }
            }
      }

      return;
}

void luramas::ast::transformers::branches::set_branch_statements(std::shared_ptr<luramas::ast::ast> &ast) {

      /* Unsafe too make compares as definite concat if those are really variables will get handled later. */

      const auto routines = std::get<std::vector<std::pair<std::shared_ptr<luramas::ast::node>, std::shared_ptr<luramas::ast::node>>>>(ast->body->visit_expr_routine<luramas::ast::element_kinds::condition_logical_start, luramas::ast::element_kinds::condition_logical_end>(true));
      for (const auto &i : routines) {

            if (i.second->lex->kind == luramas::il::lexer::inst_kinds::branch_condition && !i.first->has_expr(luramas::ast::element_kinds::condition_concat_start) && !i.first->has_expr(luramas::ast::element_kinds::condition_concat_end)) {

                  /* If or elseif/else */
                  const auto ref_addr = i.second->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                  const auto concat_start = i.first;
                  const auto concat_end = i.second;
                  const auto scope_end = luramas::ast::transformers::scopes::end_of_scope(ast, concat_end);

                  auto routine_end = ast->body->visit_addr(ref_addr);

                  /* If statement */
                  concat_start->add_expr<luramas::ast::element_kinds::condition_concat_start>();
                  concat_end->add_expr<luramas::ast::element_kinds::condition_concat_end>();
                  concat_end->add_expr<luramas::ast::element_kinds::if_>();
                  debug_success("If statement set on %s", concat_end->str().c_str());

                  /* See if jump is greater then 1. */
                  luramas::ast::transformers::branches::set(ast, concat_start->address, concat_end->address, {ref_addr}, -1, true);

                  /* Propagate conditional concat members. */
                  const auto range = ast->body->visit_range(concat_start->address, concat_end->address);
                  for (const auto &cm_node : range) {
                        cm_node->add_expr<luramas::ast::element_kinds::condition_concat_member>();
                  }

                  /* Set elseifs/else. */
                  auto ref_addr_node = ast->body->visit_addr(ref_addr);
                  auto prev_jmp = ast->body->visit_previous_addr(ref_addr);
                  const auto prev_jmp_const = prev_jmp;

                  /* Loop for jumps. */
                  while (prev_jmp->lex->disassembly->op == luramas::il::arch::opcodes::OP_JUMP /* Jump forward else */ && !prev_jmp->has_expr(luramas::ast::element_kinds::break_)) {

                        debug_line("Jump from current condition leads too previous jump without it being a break on %s with jump target %s", prev_jmp->str().c_str(), ref_addr_node->str().c_str());

                        /* No jump condition else statement. */
                        if (!ref_addr_node->has_expr(luramas::ast::element_kinds::condition_logical_start) || !ref_addr_node->has_expr(luramas::ast::element_kinds::condition_logical_end)) {

                              debug_line("Jump target isn't a logical condition.");

                              /* Check next if if existing. */
                              auto next_jmp = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(ref_addr_node->address, false));

                              if (next_jmp == nullptr || next_jmp->address > prev_jmp->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr ||
                                  !next_jmp->has_expr(luramas::ast::element_kinds::condition_logical_start) || !next_jmp->has_expr(luramas::ast::element_kinds::condition_logical_end) ||
                                  !ast->body->visit_next(prev_jmp)->has_expr(luramas::ast::element_kinds::condition_logical_start)) {

                                    debug_line("Next jump from jump target doesn't exist, out of scope, doesn't have any conditions, etc.");

                                    routine_end = ast->body->visit_addr(prev_jmp_const->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                                    prev_jmp->add_expr<luramas::ast::element_kinds::jump_else>();
                                    ref_addr_node->add_expr<luramas::ast::element_kinds::else_>();

                                    debug_success("Set else statement on %s, routine end on %s from %s", ref_addr_node->str().c_str(), routine_end->str().c_str(), prev_jmp_const->str().c_str());

                                    break;
                              }
                        }

                        /* Take jump and get final condition logical end node. */
                        const auto ref_addr_cl_routine_end = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_expr_current<luramas::ast::element_kinds::condition_logical_end>(ref_addr_node->address, false));
                        const auto ref_addr_cl_routine_end_jmp = ref_addr_cl_routine_end->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;

                        /* Already analyzed */
                        if (ref_addr_node->has_expr(luramas::ast::element_kinds::condition_concat_start)) {
                              debug_warning("Jump target already has been analyzed on %s", ref_addr_node->str().c_str());
                              break;
                        }

                        /* Condition is different else if */
                        if (prev_jmp->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr != prev_jmp_const->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) {
                              /* Set end */
                              debug_line("Current previous jump instruction from jump target jump doesn't match with constant on %s", prev_jmp->str().c_str());

                              routine_end = ast->body->visit_addr(prev_jmp_const->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                              prev_jmp->add_expr<luramas::ast::element_kinds::jump_else>();
                              ref_addr_node->add_expr<luramas::ast::element_kinds::else_>();

                              debug_success("Set else statement on %s", ref_addr_node->str().c_str());
                              break;
                        }

                        /* eleseif end */
                        routine_end = ast->body->visit_addr(prev_jmp_const->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);

                        /* No elseif */
                        if (!ref_addr_cl_routine_end->has_expr(luramas::ast::element_kinds::elseif_)) {

                              debug_line("Jump location next conditional logical routine doesn't have elseif on %s", ref_addr_cl_routine_end->str().c_str());

                              /* elseif statement */
                              ref_addr_node->add_expr<luramas::ast::element_kinds::condition_concat_start>();
                              ref_addr_cl_routine_end->add_expr<luramas::ast::element_kinds::condition_concat_end>();
                              ref_addr_cl_routine_end->add_expr<luramas::ast::element_kinds::elseif_>();
                              prev_jmp->add_expr<luramas::ast::element_kinds::jump_elseif>();

                              debug_success("Set elseif statement on %s", prev_jmp->str().c_str());

                              /* Set data */
                              luramas::ast::transformers::branches::set(ast, ref_addr_node->address, ref_addr_cl_routine_end->address, {ref_addr_cl_routine_end_jmp}, -1, true);

                              /* Propagate conditional concat members. */
                              const auto range = ast->body->visit_range(ref_addr_node->address, ref_addr_cl_routine_end->address);
                              for (const auto &cm_node : range) {
                                    debug_success("Condition concat member set on %s", cm_node->str().c_str());
                                    cm_node->add_expr<luramas::ast::element_kinds::condition_concat_member>();
                              }
                        }

                        /* Set next */
                        ref_addr_node = ast->body->visit_addr(ref_addr_cl_routine_end->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                        prev_jmp = ast->body->visit_previous_addr(ref_addr_node->address);
                  }

                  /* Fix routine_end based on jump. */
                  if (scope_end != nullptr && scope_end->lex->disassembly->op == luramas::il::arch::opcodes::OP_JUMP && ref_addr == scope_end->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) {
                        debug_line("Mutating routine end with scope end %s", scope_end->str().c_str());
                        routine_end = scope_end;
                        debug_line("Routine end mutated too %s", routine_end->str().c_str());
                  }

                  routine_end->add_expr<luramas::ast::element_kinds::scope_end>(1u, (concat_end->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr > 1u) ? luramas::ast::element::front : luramas::ast::element::back /* Empty expression? */);
                  debug_success("Set end expr on %s", routine_end->str().c_str());

                  /* Loop/while/repeat/generic */
                  const auto prev = ast->body->visit_previous_addr(concat_end->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr /* Take jump */);
                  if (condition_break_out(prev)) {

                        debug_line("Previous breaks out of loop routine on %s", prev->str().c_str());

                        /* Append break */
                        concat_end->add_expr<luramas::ast::element_kinds::condition_break>();

                        /* Remove scope end */
                        routine_end->remove_expr<luramas::ast::element_kinds::scope_end>();

                        debug_success("Set break on %s", concat_end->str().c_str());
                  }
            }
      }

      return;
}

void luramas::ast::transformers::branches::set_jumpout_exprs(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uintptr_t> scopes;

      const auto compares = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst_kinds<luramas::il::lexer::inst_kinds::branch_condition>(true));
      for (const auto &i : compares) {

            const auto current_jump = i->lex->disassembly->ref->addr;

            /* Remove scope if we reached out of its scope. */
            scopes.erase(std::remove_if(scopes.begin(), scopes.end(), [i](const auto &scope) { return scope <= i->address; }), scopes.end());

            if (scopes.empty()) {
                  scopes.emplace_back(current_jump); /* Add scope inside of if statement. */
            } else {
                  if (current_jump > scopes.back() /* Hit?? */ && !i->has_expr(luramas::ast::element_kinds::conditional_break)) {
                        i->add_expr<luramas::ast::element_kinds::conditonal_jumps_out>();
                  } else {
                        scopes.emplace_back(current_jump); /* Log scope */
                  }
            }
      }

      return;
}