#include "../transformers.hpp"

void luramas::ast::transformers::loops::sort_loops(std::shared_ptr<luramas::ast::ast> &ast) {

      auto labels = ast->body->visit_all_goto();
      std::sort(labels.begin(), labels.end(), [](std::pair<std::uintptr_t, std::vector<std::shared_ptr<luramas::ast::node>>> &a, std::pair<std::uintptr_t, std::vector<std::shared_ptr<luramas::ast::node>>> &b) { return a.first < b.first; });

      const auto all = ast->body->visit_all();
      for (const auto &node : all) {

            auto node_found = std::find_if(labels.begin(), labels.end(), [&](const std::pair<std::uintptr_t, std::vector<std::shared_ptr<luramas::ast::node>>> &pair) { return pair.first == node->address; });

            /* Has goto */
            if (!labels.empty() && node_found != labels.end()) {

                  debug_success("Label at %s", node->str().c_str());

                  std::vector<std::pair<luramas::ast::element_kinds, std::size_t>> exprs;

                  if (labels.size() == 1u) {
                        continue;
                  }

                  /* Collapse exprs */
                  node->collapse_expr();

                  /* Reverse */
                  std::reverse(node_found->second.begin(), node_found->second.end());

                  for (const auto &label : node_found->second) {

                        if (label->has_expr(luramas::ast::element_kinds::while_end)) {

                              exprs.emplace_back(node->get_expr<luramas::ast::element_kinds::while_>());
                              node->remove_expr<luramas::ast::element_kinds::while_>();

                        } else if (label->has_expr(luramas::ast::element_kinds::until_)) {

                              exprs.emplace_back(node->get_expr<luramas::ast::element_kinds::repeat_>());
                              node->remove_expr<luramas::ast::element_kinds::repeat_>();

                        } else if (label->has_expr(luramas::ast::element_kinds::for_end)) {

                              exprs.emplace_back(node->get_expr<luramas::ast::element_kinds::for_start>());
                              node->remove_expr<luramas::ast::element_kinds::for_start>();

                        } else if (label->has_expr(luramas::ast::element_kinds::for_n_end)) {

                              exprs.emplace_back(node->get_expr<luramas::ast::element_kinds::for_n_start>());
                              node->remove_expr<luramas::ast::element_kinds::for_n_start>();

                        } else if (label->has_expr(luramas::ast::element_kinds::for_iv_end)) {

                              exprs.emplace_back(node->get_expr<luramas::ast::element_kinds::for_iv_start>());
                              node->remove_expr<luramas::ast::element_kinds::for_iv_start>();
                        }
                  }

                  /* Append sorted exprs */
                  for (const auto &expr : exprs)
                        node->add_expr_tt(expr.first, expr.second);
            }
      }

      return;
}

/* Sets forprep instructions exprs. ** Can be called by parent in favor for children** */
void luramas::ast::transformers::loops::set_for_prep_exprs(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            switch (i->lex->disassembly->op) {

                  case luramas::il::arch::opcodes::OP_INITFORLOOPG:
                  case luramas::il::arch::opcodes::OP_INITFORLOOPN:
                  case luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL: {
                        if (!i->has_expr(luramas::ast::element_kinds::for_prep)) {
                              debug_success("Setting forprep expr on %s", i->str().c_str());
                              i->add_expr<luramas::ast::element_kinds::for_prep>();
                        }
                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return;
}

void luramas::ast::transformers::loops::set_for_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto forgloops = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_FORLOOPG>(true));
      const auto fornloops = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_FORLOOPN>(true));

      /* Forgloops. */
      for (const auto &forloop : forgloops) {

            const auto jump_node = ast->body->visit_addr(forloop->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);

            /* Already analyzed */
            if (jump_node->loops.end_node != nullptr && forloop->loops.end_node != nullptr) {
                  continue;
            }

            /* for i,v in ipairs/pairs */
            if (jump_node->lex->disassembly->op == luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL) {
                  debug_success("Setting for iv routine on %s", forloop->str().c_str());
                  jump_node->add_expr<luramas::ast::element_kinds::for_iv_start>();
                  forloop->add_expr<luramas::ast::element_kinds::for_iv_end>();
            } else {
                  debug_success("Setting generic for routine on %s", forloop->str().c_str());
                  jump_node->add_expr<luramas::ast::element_kinds::for_start>();
                  forloop->add_expr<luramas::ast::element_kinds::for_end>();
            }

            forloop->add_expr<luramas::ast::element_kinds::scope_end>();

            jump_node->loops.start_node = jump_node;
            forloop->loops.start_node = jump_node;
            jump_node->loops.end_node = forloop;
            forloop->loops.end_node = forloop;

            /* Set start/end registers */
            forloop->loops.start_reg = forloop->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
            forloop->loops.end_reg = forloop->loops.start_reg + 4u;
      }

      /* Fornloops. */
      for (const auto &forloop : fornloops) {

            const auto loop = ast->body->visit_addr(forloop->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);

            /* Already analyzed */
            if (loop->loops.end_node != nullptr && forloop->loops.end_node != nullptr) {
                  continue;
            }

            debug_success("Setting for n routine on %s", forloop->str().c_str());
            loop->add_expr<luramas::ast::element_kinds::for_n_start>();
            forloop->add_expr<luramas::ast::element_kinds::scope_end>();
            forloop->add_expr<luramas::ast::element_kinds::for_n_end>();

            loop->loops.end_node = forloop;
            forloop->loops.end_node = forloop;

            /* Set start/end registers */
            forloop->loops.start_reg = forloop->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
            forloop->loops.end_reg = forloop->loops.start_reg + forloop->lex->count_operand_expr<luramas::il::lexer::operand_kinds::value>();
      }

      return;
}

void luramas::ast::transformers::loops::set_loop_startend_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::shared_ptr<luramas::ast::node>> nodes;

      const auto jump_backs = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_JUMPBACK>(true));
      const auto jump_conds = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_next_type<luramas::il::lexer::inst_kinds::branch_condition>(true));

      /* Check typical jumpbacks *Previous inst is condition its until else end for while. */
      for (const auto &jmp_back : jump_backs) {

            if (ast->body->visit_previous_addr(jmp_back->address)->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                  jmp_back->add_expr<luramas::ast::element_kinds::until_>(); /* Until end. */
                  nodes.emplace_back(jmp_back);
            } else {
                  jmp_back->add_expr<luramas::ast::element_kinds::scope_end>(); /* While loop end. */
                  jmp_back->add_expr<luramas::ast::element_kinds::while_end>(); /* While loop end. */
                  nodes.emplace_back(jmp_back);
            }
      }

      /* See if jump memaddrs are negatives usally means there until. */
      for (const auto &jmp_back : jump_conds)
            if (jmp_back->lex->disassembly->operands[std::find(jmp_back->lex->operands.begin(), jmp_back->lex->operands.end(), luramas::il::lexer::operand_kinds::jmpaddr) - jmp_back->lex->operands.begin()]->dis.jmp < 0) { /* See if mem address of jump is negative (We need to get idx of memaddr operand). */
                  jmp_back->add_expr<luramas::ast::element_kinds::until_>();                                                                                                                                                  /* Until end. */
                  nodes.emplace_back(jmp_back);
            }

      /* We need to get range of until/while routines conditions. */
      for (const auto &jumpback : nodes) {

            std::shared_ptr<luramas::ast::node> marked_node = nullptr;

            /* See if jumpback is a branch. */
            if (jumpback->lex->kind != luramas::il::lexer::inst_kinds::branch_condition && jumpback->lex->kind != luramas::il::lexer::inst_kinds::branch) {
                  throw std::runtime_error("until/while branch jumpback isn't a branch.");
            }

            const auto ref_addr = jumpback->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
            const auto jmp_node = ast->body->visit_addr(ref_addr);
            const auto nodes_routine = ast->body->visit_range_current(ref_addr, jumpback->address);
            auto condition = ast->body->visit_expr_routine_range_touching<luramas::ast::element_kinds::condition_routine_start, luramas::ast::element_kinds::condition_routine_end>(ref_addr, jumpback->address);

            if (jumpback->has_expr(luramas::ast::element_kinds::until_)) {

                  jmp_node->add_expr<luramas::ast::element_kinds::repeat_>();

                  if (condition.empty()) {
                        /* No conditions in it (Garunteed repeat (true) do) */
                        jumpback->add_expr<luramas::ast::element_kinds::condition_true>(1u, luramas::ast::element::front);
                  } else {

                        const auto cond = condition.back();

                        if (cond.second->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr > jumpback->address) {

                              /* Not analyzed */
                              if (!cond.first->has_expr(luramas::ast::element_kinds::condition_concat_start)) {

                                    cond.first->add_expr<luramas::ast::element_kinds::condition_concat_start>();
                                    cond.second->add_expr<luramas::ast::element_kinds::condition_concat_end>();

                                    branches::set(ast, cond.first->address, cond.second->address, {jumpback->address});

                                    /* Propagate conditional concat members. */
                                    const auto range = ast->body->visit_range(cond.first->address, cond.second->address);
                                    for (const auto &cm_node : range) {
                                          cm_node->add_expr<luramas::ast::element_kinds::condition_concat_member>();
                                    }
                              }

                        } else {
                              /* Last conditon doesn't jump out possible repeat until(true)*/
                              jumpback->add_expr<luramas::ast::element_kinds::condition_true>(1u, luramas::ast::element::front);
                        }
                  }

            } else { /* While loop */

                  /* No conditions in it (Garunteed repeat (true) do) */
                  if (condition.empty()) {
                        jmp_node->add_expr<luramas::ast::element_kinds::repeat_>();
                        jumpback->add_expr<luramas::ast::element_kinds::condition_true>(1u, luramas::ast::element::front);
                  } else {

                        const auto cond = condition.front();

                        if (cond.second->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr > jumpback->address) {

                              bool dont_set = false;
                              std::shared_ptr<luramas::ast::node> begin_node = cond.first;

                              /* Go through second and first range and see regs logicals. */
                              const auto range = ast->body->visit_range_current(cond.first->address, cond.second->address);
                              for (const auto &node : range) {

                                    auto nearest_compare = std::get<std::shared_ptr<luramas::ast::node>>(ast->body->visit_next_type_addr<luramas::il::lexer::inst_kinds::branch_condition>(node->address, false));

                                    /* Check dest for compare value. */
                                    if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                                          const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                                          const auto compare = nearest_compare->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>();

                                          /* Hit reg set of compare. */
                                          if ((compare.size() == 1u && compare.front()->dis.reg == dest) || (compare.size() == 2u && (compare.front()->dis.reg == dest || compare.back()->dis.reg == dest))) {

                                                const auto logical = luramas::ast::transformers::registers::logical_dest_register(ast, node, dest);

                                                if (!logical && begin_node == nullptr) {
                                                      begin_node = node;
                                                } else if (logical) {

                                                      /* Last codition and last is nearest with it being logical so while (true) */
                                                      if (cond.second == nearest_compare) {
                                                            jmp_node->add_expr<luramas::ast::element_kinds::while_>();
                                                            jmp_node->add_expr<luramas::ast::element_kinds::condition_true>(1u, luramas::ast::element::front);
                                                            dont_set = true;
                                                            break;
                                                      }

                                                      begin_node = nullptr;
                                                }
                                          }
                                    }
                              }

                              /* Write condition routine. */
                              if (!dont_set) {

                                    begin_node->add_expr<luramas::ast::element_kinds::condition_concat_start>();
                                    cond.second->add_expr<luramas::ast::element_kinds::condition_concat_end>();
                                    cond.second->add_expr<luramas::ast::element_kinds::while_>();
                                    branches::set(ast, begin_node->address, cond.second->address, {jumpback->address, (jumpback->address + jumpback->lex->disassembly->len)});

                                    /* Propagate conditional concat members. */
                                    const auto range = ast->body->visit_range(begin_node->address, cond.second->address);
                                    for (const auto &cm_node : range) {
                                          cm_node->add_expr<luramas::ast::element_kinds::condition_concat_member>();
                                    }
                              }

                        } else {
                              /* Last conditon doesn't jump out possible repeat until(true)*/
                              jmp_node->add_expr<luramas::ast::element_kinds::repeat_>();
                              jumpback->add_expr<luramas::ast::element_kinds::condition_true>(1u, luramas::ast::element::front);
                        }
                  }
            }

            /* End of routine is given with until. Find begging. */
            for (const auto &node : nodes_routine) {

                  /* Break **Not definite jump to end of until routine or end of while can mean break of any conditional** */
                  if (node->lex->kind == luramas::il::lexer::inst_kinds::branch) {

                        if (!node->has_expr(luramas::ast::element_kinds::break_) /* No break */ && node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr > jumpback->address) {
                              node->add_expr<luramas::ast::element_kinds::break_>();
                        }
                  }

                  if (node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                        if (node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr > jumpback->address) {
                              node->add_expr<luramas::ast::element_kinds::conditional_break>();
                        }
                  }
            }
      }

      return;
}
