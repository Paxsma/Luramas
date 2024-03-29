#include "../transformers.hpp"

void luramas::ast::transformers::registers::set_sources_scope_element(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();

      luramas::ast::transformers::scopes::scope_data::scope scope(ast, true);
      scope = all;

      luramas::ast::transformers::scopes::scope_data::scope_vector<std::uint16_t> regs(scope);

      for (const auto &i : all) {

            /* Change node */
            regs[i];

            /* Append dests */
            for (const auto d : i->extract_dest_regs())
                  regs.push_back(d);

            /* Check source */
            for (const auto s : i->extract_source_regs())
                  if (std::find_if(ast->arg_regs.begin(), ast->arg_regs.end(), [&](const std::pair<std::int16_t, std::string> &pair) { return pair.first == s; }) == ast->arg_regs.end() /* Not arg? */ && regs.find<true>(s) /* Out of scope */ && !regs.find(s) /* In scope */) {

                        for (const auto o : i->locate_source_regs(s))
                              switch (o) {
                                    case il::lexer::operand_kinds::source: {
                                          i->add_elem<luramas::ast::element_kinds::desc_source_outside_scope>();
                                          break;
                                    }
                                    case il::lexer::operand_kinds::reg: {
                                          i->add_elem<luramas::ast::element_kinds::desc_reg_outside_scope>();
                                          break;
                                    }
                                    case il::lexer::operand_kinds::value: {
                                          i->add_elem<luramas::ast::element_kinds::desc_value_outside_scope>();
                                          break;
                                    }
                                    case il::lexer::operand_kinds::compare: {
                                          i->add_elem<luramas::ast::element_kinds::desc_compare_outside_scope>();
                                          break;
                                    }
                                    case il::lexer::operand_kinds::table_reg: {
                                          i->add_elem<luramas::ast::element_kinds::desc_table_reg_outside_scope>();
                                          break;
                                    }
                                    default: {
                                          break;
                                    }
                              }
                  }
      }

      return;
}

void luramas::ast::transformers::registers::set_sources_left_right_element(std::shared_ptr<luramas::ast::ast> &ast) {

      std::shared_ptr<luramas::ast::node> left_value = nullptr;
      std::shared_ptr<luramas::ast::node> left_value_begin = nullptr;
      std::shared_ptr<luramas::ast::node> right_value = nullptr;

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            /* Must have begin, end, or member if not then reset. */
            if (i->has_elem(luramas::ast::element_kinds::desc_expression_begin)) {

                  /* Set left value begin. */
                  if (left_value == nullptr) {
                        left_value_begin = i;
                  }

                  /* Already set reset. */
                  if (left_value != nullptr && right_value != nullptr) {
                        left_value = nullptr;
                        left_value_begin = nullptr;
                        right_value = nullptr;
                  }

            } else if (i->has_elem(luramas::ast::element_kinds::desc_expression_end)) {

                  if (left_value == nullptr) {
                        left_value = i;
                  } else {
                        right_value = i;
                  }

            } else if (!i->has_elem(luramas::ast::element_kinds::desc_expression_member)) {

                  /* Right value is next */
                  if (right_value != nullptr && ast->body->visit_next(right_value) == i) {

                        /* Check if left/right value destination registers are in current source. */
                        const auto sources = i->extract_source_regs();
                        const auto l_dest = left_value->extract_dest_regs().front();
                        const auto r_dest = right_value->extract_dest_regs().front();
                        if (sources.size() == 2u &&
                            std::find(sources.begin(), sources.end(), l_dest) != sources.end() &&
                            std::find(sources.begin(), sources.end(), r_dest) != sources.end()) {

                              /* Found, set left/right value element kinds. */

                              /* Left value */
                              left_value_begin->add_elem<luramas::ast::element_kinds::desc_left_value_begin>();

                              for (const auto &i : ast->body->visit_range(left_value_begin->address, left_value->address))
                                    i->add_elem<luramas::ast::element_kinds::desc_left_value_member>();

                              left_value->add_elem<luramas::ast::element_kinds::desc_left_value_end>();

                              /* Right value */
                              ast->body->visit_next(left_value)->add_elem<luramas::ast::element_kinds::desc_right_value_begin>();

                              for (const auto &i : ast->body->visit_range(ast->body->visit_next(left_value)->address, right_value->address))
                                    i->add_elem<luramas::ast::element_kinds::desc_right_value_member>();

                              right_value->add_elem<luramas::ast::element_kinds::desc_right_value_end>();

                              /* End */
                              i->add_elem<luramas::ast::element_kinds::desc_lr_value>();

                        } else {
                              left_value = nullptr;
                              left_value_begin = nullptr;
                              right_value = nullptr;
                        }

                  } else {
                        left_value = nullptr;
                        left_value_begin = nullptr;
                        right_value = nullptr;
                  }
            }
      }

      return;
}

void luramas::ast::transformers::registers::set_statement_expr(std::shared_ptr<luramas::ast::ast> &ast) {

      std::intptr_t routine = 0;
      std::vector<std::uintptr_t> ignore; /* Instructions to ignore, routine instructions: call, concat,  */
      std::vector<std::uintptr_t> pre_analyzed_addresses;
      std::vector<std::uintptr_t> analyzed_addresses;
      std::vector<std::shared_ptr<luramas::ast::node>> vect;

      /* Skip all routines. */
      const auto all_f = ast->body->visit_all();
      for (const auto &i : all_f) {

            routine_inc_safe(i, routine);

            if (routine) {
                  ignore.emplace_back(i->address);
            }

            routine_dec_safe(i, routine);
      }

      const auto all = ast->body->visit_all_flip();
      for (const auto &i : all) {

            /* Skip any already analyzed. */
            if ((!analyzed_addresses.empty() && std::find(analyzed_addresses.begin(), analyzed_addresses.end(), i->address) != analyzed_addresses.end()) || std::find(ignore.begin(), ignore.end(), i->address) != ignore.end()) {
                  continue;
            }

            /* Go through each source too see if they fill wil single instruction. */
            if (!i->source_nodes.empty()) {

                  vect.insert(vect.begin(), i->source_nodes.begin(), i->source_nodes.end());

                  do {

                        auto curr_node = vect.front();

                        /* No dest */
                        if (!curr_node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::dest>()) {
                              break;
                        }

                        /* All sources */
                        if (!curr_node->source_nodes.empty()) {

                              /* Check for bounds. */
                              for (const auto &on : curr_node->source_nodes)
                                    if (on->address <= i->address) {
                                          vect.emplace_back(on);
                                    }
                        }

                        /* Add analyzed addresses. */
                        if (curr_node->address <= i->address && std::find(pre_analyzed_addresses.begin(), pre_analyzed_addresses.end(), curr_node->address) == pre_analyzed_addresses.end()) {
                              pre_analyzed_addresses.emplace_back(curr_node->address);
                        }

                        /* Remove current. */
                        vect.erase(std::remove(vect.begin(), vect.end(), curr_node), vect.end());

                  } while (!vect.empty());

                  /* Sort and first. */
                  std::sort(pre_analyzed_addresses.begin(), pre_analyzed_addresses.end());
                  auto on = pre_analyzed_addresses.front();

                  /* Fix */
                  bool on_remove = false;
                  std::uintptr_t remove = 0u;
                  for (const auto i : pre_analyzed_addresses) {

                        if (i != on) {
                              remove = on;
                              on = i;
                              on_remove = true;
                        }

                        on = ast->body->visit_next(ast->body->visit_addr(on))->address;
                  }

                  /* Clean */
                  if (on_remove) {

                        /* Use another vector too prevent bugs. */
                        std::vector<std::uintptr_t> remove_addr;

                        for (const auto i : pre_analyzed_addresses)
                              if (i <= remove) {
                                    remove_addr.emplace_back(i);
                              }

                        for (const auto i : remove_addr)
                              pre_analyzed_addresses.erase(std::remove(pre_analyzed_addresses.begin(), pre_analyzed_addresses.end(), i), pre_analyzed_addresses.end());
                  }

                  /* Anything? */
                  if (!pre_analyzed_addresses.empty() && pre_analyzed_addresses.size() != 1u) {

                        /* Make sure its consitent. */
                        auto on = pre_analyzed_addresses.front();
                        for (const auto i : pre_analyzed_addresses) {

                              if (on == i || ast->body->visit_next(ast->body->visit_addr(on))->address == i) {
                                    on = i;
                              } else {
                                    goto skip;
                              }
                        }

                        /* Place first */
                        if (!i->has_elem(luramas::ast::element_kinds::desc_expression_member)) {
                              ast->body->visit_addr(pre_analyzed_addresses.front())->add_safe<luramas::ast::element_kinds::desc_expression_begin>();
                        }

                        /* Fill */
                        const auto range = ast->body->visit_range(pre_analyzed_addresses.front(), pre_analyzed_addresses.back());
                        for (const auto &i : range)
                              if (!i->has_elem(luramas::ast::element_kinds::desc_expression_begin) && !i->has_elem(luramas::ast::element_kinds::desc_expression_end)) {
                                    i->add_safe<luramas::ast::element_kinds::desc_expression_member>();
                              }

                        /* Fix end sometimes gets missed. */
                        if (!i->has_elem(luramas::ast::element_kinds::desc_expression_begin) && !i->has_elem(luramas::ast::element_kinds::desc_expression_end)) {
                              ast->body->visit_addr(pre_analyzed_addresses.back())->add_safe<luramas::ast::element_kinds::desc_expression_member>();
                        }

                        /* End */
                        if (!i->has_elem(luramas::ast::element_kinds::desc_expression_member)) {

                              /* Adjust for cmp if next is set. */
                              if (i->lex->kind == luramas::il::lexer::inst_kinds::compare && ast->body->visit_next(i) != nullptr && ast->body->visit_next(i)->lex->kind == luramas::il::lexer::inst_kinds::compare_dest) {
                                    i->add_safe<luramas::ast::element_kinds::desc_expression_member>();
                                    ast->body->visit_next(i)->add_safe<luramas::ast::element_kinds::desc_expression_end>();
                              } else {
                                    i->add_safe<luramas::ast::element_kinds::desc_expression_end>();
                              }
                        }

                        /* Append */
                        analyzed_addresses.insert(analyzed_addresses.end(), pre_analyzed_addresses.begin(), pre_analyzed_addresses.end());
                  }

            skip:
                  /* Clear */
                  vect.clear();
                  pre_analyzed_addresses.clear();
            }
      }

      return;
}

bool luramas::ast::transformers::registers::detect_double_usage_without_reset(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &start, const std::uint16_t target) {

      bool source = false;
      bool dest = false;

      const auto rest = ast->body->visit_rest_current(start->address);
      for (const auto &i : rest) {

            /* Check if target is used in dest operand. */
            for (const auto reg : i->extract_dest_regs()) {

                  if (reg == target) {

                        if (dest) {
                              return true;
                        }

                        dest = true;
                        source = false;
                  }
            }

            /* Check if target is used in source operand. */
            for (const auto reg : i->extract_source_regs()) {

                  if (reg == target) {

                        if (source) {
                              return true;
                        }

                        dest = false;
                        source = true;
                  }
            }
      }

      return false;
}

bool luramas::ast::transformers::registers::logical_dest_register(std::shared_ptr<luramas::ast::ast> &ast, std::shared_ptr<luramas::ast::node> start, const std::int16_t target) {

      /* Check for loop too see if they override any of the regs. */
      const auto next_scope = ast->body->visit_rest_scope_addr(start->address);
      for (const auto &i : next_scope) {

            /* See if register falls within loop vars. */
            if (i->has_elem(luramas::ast::element_kinds::stat_for_start) || i->has_elem(luramas::ast::element_kinds::stat_for_iv_start) || i->has_elem(luramas::ast::element_kinds::stat_for_n_start)) {
                  const auto for_target = i->loops.end_node;
                  if (for_target->loops.start_reg <= target && for_target->loops.end_reg >= target) {
                        return false;
                  }
            }
      }

      /* See if register gets used twice by dest or source with repecting scopes and either or reseting it. */
      std::intptr_t routine = 0;
      std::intptr_t scope = 0;
      std::size_t iter_count = 0u; /* Counts times it was iterated. */
      const auto target_1 = target;
      bool used_target_1_dest = true;
      bool used_target_1_source = false;
      bool no_locvar = true;
      bool return_ = false;       /* Analysys ended on return? */
      std::int8_t used_next = -1; /* Set dest next used source repeat always. */
      std::shared_ptr<luramas::ast::node> dest_node = start;
      std::shared_ptr<luramas::ast::node> dest_node_nm = start; /* Dest node non mutable by sources. */
      std::int32_t dest_scope = 0;                              /* Scope where dest was set. (Can be reset by source) */
      std::int32_t set_scope = 0;                               /* Scope where dest was set. (Cannot be reset by source) */

      debug_success("Starting with: %s", start->str().c_str());

      /* Really just visiting future instructions too see if table source, dest, or idx gets set twice indicating end. */
      const auto rest_nodes = ast->body->visit_rest(start->address);
      for (const auto &s_node : rest_nodes) {

            ++iter_count;

            /* Checks operands if targets get used or not but if it does get used just resets target. */
            bool used_source_twice = false;   /* Used by source twice. */
            bool used_source_routine = false; /* Used by source in routine. */

            std::function<void(const std::shared_ptr<luramas::il::arch::operand::operand> &, const luramas::il::lexer::operand_kinds)> check_usage = [&](const std::shared_ptr<luramas::il::arch::operand::operand> &operand, const luramas::il::lexer::operand_kinds tt) mutable {
                  const auto val = operand->dis.reg;

                  if (val == target_1) {

                        debug_line("Source was hit on %s", s_node->str().c_str());

                        /* Used twice and last dest node is current. */
                        if (used_target_1_source && dest_node_nm == start) {
                              used_source_twice = true;
                              return;
                        }

                        /* Used in routine and last dest node is start. */
                        if (routine && dest_node_nm == start) {
                              used_source_routine = true;
                              return;
                        }

                        /* Reset data */
                        debug_line("Reset data.");

                        used_target_1_source = true;
                        used_target_1_dest = false;

                        /* See if used next constantly. */
                        used_next = (used_next == -1) ? ((dest_node->address + dest_node->lex->disassembly->len) == s_node->address) : -2;

                        dest_scope = 0;
                        dest_node = nullptr;
                  }

                  return;
            };

            /* Scope */
            scope += (s_node->count_elem<luramas::ast::element_kinds::stat_repeat>() +
                      s_node->count_elem<luramas::ast::element_kinds::stat_while>() +
                      s_node->count_elem<luramas::ast::element_kinds::stat_for_start>() +
                      s_node->count_elem<luramas::ast::element_kinds::stat_for_iv_start>() +
                      s_node->count_elem<luramas::ast::element_kinds::stat_for_n_start>() +
                      s_node->count_elem<luramas::ast::element_kinds::stat_if>());
            scope -= s_node->count_elem<luramas::ast::element_kinds::stat_scope_end>();

            /* Set but now used outside of scope. */
            if (dest_scope > scope && scope >= 0) {
                  debug_success("Used outside of scope valid register on %s", s_node->str().c_str());
                  no_locvar = false;
                  break;
            }

            /* Out of scope or last scope and hit return. */
            if (scope < 0 || (!scope && s_node->lex->kind == luramas::il::lexer::inst_kinds::return_)) {

                  debug_line("Register use out of scope or hit return without scoped on %s", s_node->str().c_str());

                  const auto source = s_node->extract_source_regs();
                  return_ = std::find(source.begin(), source.end(), target) != source.end();

                  break;
            }

            /* End of scope */
            if (!scope && s_node->lex->kind == luramas::il::lexer::inst_kinds::return_) {

                  /* Just ended with it just using it as dest. */
                  if (used_target_1_dest && !used_target_1_source) {
                        debug_success("Return used register as source on %s", s_node->str().c_str());
                        no_locvar = false;
                        break;
                  }
            }

            /* Capture */
            if (s_node->lex->kind == luramas::il::lexer::inst_kinds::capture && s_node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::source>() && s_node->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg == target_1) {
                  debug_line("Register used on capture for %s", s_node->str().c_str());
                  no_locvar = (dest_node != start);
                  break;
            }

            /* Inc for concat start, call start, and table start. Dec for concat end, call end, and start end. */
            routine_inc_lv(s_node, routine);
            routine_dec_lv(s_node, routine);

            /* Check reg operands for usage. */
            s_node->lex->operand_kind_callback<luramas::il::lexer::operand_kinds::source>(check_usage);
            s_node->lex->operand_kind_callback<luramas::il::lexer::operand_kinds::compare>(check_usage);
            s_node->lex->operand_kind_callback<luramas::il::lexer::operand_kinds::reg>(check_usage);
            s_node->lex->operand_kind_callback<luramas::il::lexer::operand_kinds::table_reg>(check_usage);

            /* Call */
            if (luramas::ast::transformers::calls::reg_arg(ast, s_node, target)) {

                  debug_line("Source argument was hit on %s", s_node->str().c_str());

                  used_target_1_source = true;
                  used_target_1_dest = false;
                  dest_scope = 0;
                  dest_node = nullptr;
            }

            /* Used by source twice. */
            if (used_source_twice) {
                  debug_success("Used register twice as a source on %s", s_node->str().c_str());
                  no_locvar = false;
                  break;
            }

            /* Used in source in routine. */
            if (used_source_routine) {
                  debug_success("Used register as source in routine on %s", s_node->str().c_str());
                  no_locvar = false;
                  break;
            }

            if (s_node->lex->has_operand_kind<luramas::il::lexer::operand_kinds::dest>()) {

                  const auto dest = s_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                  /* Check target usage. Abrubt end. */
                  if (dest == target_1) {

                        debug_line("Dest was hit on %s", s_node->str().c_str());

                        /* Used in routine not locvar. */
                        if (routine) {
                              debug_warning("Register used as dest inside routine on %s", s_node->str().c_str());
                              no_locvar = true;
                              break;
                        }

                        /* Set twice without used. Locvar */
                        if (used_target_1_dest && set_scope == scope) {
                              debug_warning("Register was set twice without being used on %s", s_node->str().c_str());
                              no_locvar = (dest_node != start && dest_scope > scope && scope >= 0);
                              break;
                        }

                        used_target_1_dest = true;
                        used_target_1_source = false;
                        dest_node = s_node;
                        dest_node_nm = s_node;
                        set_scope = std::int32_t(scope);
                        dest_scope = std::int32_t(scope);
                  }
            }
      }

      /* Ended used on return. */
      if (return_) {
            debug_warning("Not locvar used in return for end.");
            return false;
      }

      /* Not used at all. */
      if (!used_target_1_source && dest_node == start) {
            debug_success("Not used at all.");
            return true;
      }

      /* Not locvar by routine. */
      if (no_locvar || (no_locvar && used_next == 1)) {
            debug_warning("Not a locvar.");
            return false;
      }

      /* Ignore var next to return. */
      const auto next = ast->body->visit_next(start);
      if (next != nullptr && next->lex->kind == luramas::il::lexer::inst_kinds::return_ && iter_count == 1u) {
            debug_warning("Register created right before a return instruction not a locvar.");
            return false;
      }

      debug_success("It is a locvar.");
      return true;
}
