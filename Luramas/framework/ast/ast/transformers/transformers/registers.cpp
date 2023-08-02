#include "../transformers.hpp"

/* Sees if destination is logical in scope. */
bool luramas::ast::transformers::registers::logical_dest_register(std::shared_ptr<luramas::ast::ast> &ast, std::shared_ptr<luramas::ast::node> start, const std::int16_t target) {

      /* Check for loop too see if they override any of the regs. */
      const auto next_scope = ast->body->visit_rest_scope_addr(start->address);
      for (const auto &i : next_scope) {

            /* See if register falls within loop vars. */
            if (i->has_expr(luramas::ast::element_kinds::for_start) || i->has_expr(luramas::ast::element_kinds::for_iv_start) || i->has_expr(luramas::ast::element_kinds::for_n_start)) {
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
            scope += (s_node->count_expr<luramas::ast::element_kinds::repeat_>() +
                      s_node->count_expr<luramas::ast::element_kinds::while_>() +
                      s_node->count_expr<luramas::ast::element_kinds::for_start>() +
                      s_node->count_expr<luramas::ast::element_kinds::for_iv_start>() +
                      s_node->count_expr<luramas::ast::element_kinds::for_n_start>() +
                      s_node->count_expr<luramas::ast::element_kinds::if_>());
            scope -= s_node->count_expr<luramas::ast::element_kinds::scope_end>();

            /* Set but now used outside of scope. */
            if (dest_scope > scope && scope >= 0) {
                  debug_success("Used outside of scope valid register on %s", s_node->str().c_str());
                  no_locvar = false;
                  break;
            }

            /* Out of scope or last scope and hit return. */
            if (scope < 0 || (!scope && s_node->lex->kind == luramas::il::lexer::inst_kinds::return_)) {

                  debug_line("Register use out of scope or hit return without scoped on %s", s_node->str().c_str());

                  const auto source = luramas::ast::transformers::registers::get_source_list(ast, s_node);
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
            if (s_node->lex->kind == luramas::il::lexer::inst_kinds::capture && s_node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::source>() && s_node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg == target_1) {
                  debug_line("Register used on capture for %s", s_node->str().c_str());
                  no_locvar = (dest_node != start);
                  break;
            }

            /* Inc for concat start, call start, and table start. Dec for concat end, call end, and start end. */
            routine_inc_lv(s_node, routine);
            routine_dec_lv(s_node, routine);

            /* Check reg operands for usage. */
            s_node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::source>(check_usage);
            s_node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::compare>(check_usage);
            s_node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::reg>(check_usage);
            s_node->lex->operand_expr_callback<luramas::il::lexer::operand_kinds::table_reg>(check_usage);

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

            if (s_node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                  const auto dest = s_node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

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

/* Gets list of all dests registers being set.  **Dependent on for loop data** */
std::vector<std::uint16_t> luramas::ast::transformers::registers::get_dest_list(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &node) {

      std::vector<std::uint16_t> retn;

      /* Loops */
      const auto for_node = node->loops.end_node;
      if (for_node != nullptr && node->loops.start_node == node) {

            for (auto i = node->loops.end_node->loops.start_reg; i <= node->loops.end_node->loops.end_reg; ++i)
                  retn.emplace_back(i);
      }

      switch (node->lex->disassembly->op) {

            case luramas::il::arch::opcodes::OP_RETURN: {

                  const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;
                  auto amt = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

                  if (amt) {

                        for (auto a = dest; a < (dest + amt); ++a)
                              retn.emplace_back(a);
                  }

                  break;
            }

            case luramas::il::arch::opcodes::OP_GETVARARGS: {

                  const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                  auto amt = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

                  if (amt == LUA_MULTRET) {
                        amt = (*(&node - 1u))->lex->disassembly->operands.front()->dis.reg;
                  }

                  if (amt == 0) {
                        amt = 1u;
                  }

                  for (auto a = dest; a < (dest + amt); ++a)
                        retn.emplace_back(a);

                  break;
            }

            case luramas::il::arch::opcodes::OP_CCALL: {

                  auto call_retn = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().back()->dis.val;
                  const auto call = node->lex->disassembly->operands.front()->dis.reg;

                  /* Skip this */
                  if (std::find(node->flags.poparg_flag.call_pop.begin(), node->flags.poparg_flag.call_pop.end(), call) != node->flags.poparg_flag.call_pop.end()) {
                        retn.emplace_back(call + 1u);
                  }

                  /* Append to dest. (Fill for multiple retn) */
                  for (auto i = 0; i < call_retn; ++i) {

                        const auto reg = call + i;

                        if (std::find(retn.begin(), retn.end(), reg) == retn.end()) {
                              retn.emplace_back(reg);
                        }
                  }

                  break;
            }

            default: {

                  /* Append dest */
                  if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                        retn.emplace_back(node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg);
                  }

                  break;
            }
      }

      return retn;
}

/* Gets list of all source registers being set. */
std::vector<std::uint16_t> luramas::ast::transformers::registers::get_source_list(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &node) {

      bool ignore = false;
      std::vector<std::uint16_t> retn;

      /* Skip loop */
      if (node->lex->kind == luramas::il::lexer::inst_kinds::for_) {
            return retn;
      }

      switch (node->lex->disassembly->op) {

            case luramas::il::arch::opcodes::OP_RETURN: {

                  const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;
                  auto amt = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

                  if (amt) {

                        if (amt == -1)
                              amt = node->lex->disassembly->operands.front()->dis.reg;

                        for (auto a = dest; a < (dest + amt); ++a)
                              retn.emplace_back(a);
                  }

                  ignore = true;

                  break;
            }

            case luramas::il::arch::opcodes::OP_CCALL: {

                  auto args = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;
                  const auto start = node->lex->disassembly->operands.front()->dis.reg;

                  for (auto i = 0; i < args; ++i) {

                        /* Skip pop */
                        if (std::find(node->flags.poparg_flag.call_pop.begin(), node->flags.poparg_flag.call_pop.end(), i) != node->flags.poparg_flag.call_pop.end()) {
                              continue;
                        }

                        retn.emplace_back(i + start + 1u);
                  }

                  ignore = true;

                  break;
            }

            default: {
                  break;
            }
      }

      /* Done */
      if (ignore) {
            return retn;
      }

      /* Append source */
      if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::source>()) {

            const auto regz = node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>();

            for (const auto &operand : regz)
                  retn.emplace_back(operand->dis.reg);
      }

      /* Append reg */
      if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::reg>()) {

            const auto regz = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>();

            for (const auto &operand : regz)
                  retn.emplace_back(operand->dis.reg);
      }

      /* Append compare */
      if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::compare>()) {

            const auto regz = node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>();

            for (const auto &operand : regz)
                  retn.emplace_back(operand->dis.reg);
      }

      return retn;
}

/* Sets register_scope_dest expr. */
void luramas::ast::transformers::registers::set_source_scope_expr(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();

      luramas::ast::transformers::scopes::scope_data::scope scope(ast, true);
      scope = all;

      luramas::ast::transformers::scopes::scope_data::scope_vector<std::uint16_t> regs(scope);

      for (const auto &i : all) {

            /* Change table */
            regs[i];

            /* Append dests */
            for (const auto d : luramas::ast::transformers::registers::get_dest_list(ast, i))
                  regs.push_back(d);

            /* Check source */
            for (const auto s : luramas::ast::transformers::registers::get_source_list(ast, i))
                  if (std::find_if(ast->arg_regs.begin(), ast->arg_regs.end(), [&](const std::pair<std::int16_t, std::string> &pair) { return pair.first == s; }) == ast->arg_regs.end() /* Not arg? */ && regs.find<true>(s) /* Out of scope */ && !regs.find(s) /* In scope */) {
                        i->add_expr<luramas::ast::element_kinds::source_outside_scope>();
                  } else {
                        i->add_expr<luramas::ast::element_kinds::source_inside_scope>();
                  }
      }

      return;
}

/* Register is used twice by either source or dest without being reset by either vice versa. */
bool luramas::ast::transformers::registers::reg_used_twice(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &start, const std::uint16_t target, const bool same_start /* Dest hit and start are the same? */) {

      bool source = false;
      bool dest = false;

      std::shared_ptr<luramas::ast::node> last_dest = nullptr;
      const auto rest = ast->body->visit_rest_current(start->address);

      for (const auto &i : rest) {

            for (const auto reg : luramas::ast::transformers::registers::get_dest_list(ast, i)) {

                  if (reg == target) {

                        if (dest) {

                              if (same_start) {
                                    return (last_dest == start) ? true : false;
                              } else {
                                    return true;
                              }
                        }

                        dest = true;
                        source = false;
                        last_dest = start;
                  }
            }

            for (const auto reg : luramas::ast::transformers::registers::get_dest_list(ast, i)) {

                  if (reg == target) {

                        if (source) {

                              if (same_start) {
                                    return (last_dest == start) ? true : false;
                              } else {
                                    return true;
                              }
                        }

                        dest = false;
                        source = true;
                  }
            }
      }

      return false;
}

/* Sets linked exprs (Dependent on set arguements functions). */
void luramas::ast::transformers::registers::set_statement_expr(std::shared_ptr<luramas::ast::ast> &ast) {

      std::intptr_t routine = 0;
      std::vector<std::uintptr_t> ignore;
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
                        if (!curr_node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {
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
                        if (!i->has_expr(luramas::ast::element_kinds::statement_member)) {
                              ast->body->visit_addr(pre_analyzed_addresses.front())->add_existance<luramas::ast::element_kinds::statement_begin>();
                        }

                        /* Fill */
                        const auto range = ast->body->visit_range(pre_analyzed_addresses.front(), pre_analyzed_addresses.back());
                        for (const auto &i : range)
                              if (!i->has_expr(luramas::ast::element_kinds::statement_begin) && !i->has_expr(luramas::ast::element_kinds::statement_end)) {
                                    i->add_existance<luramas::ast::element_kinds::statement_member>();
                              }

                        /* Fix end sometimes gets missed. */
                        if (!i->has_expr(luramas::ast::element_kinds::statement_begin) && !i->has_expr(luramas::ast::element_kinds::statement_end)) {
                              ast->body->visit_addr(pre_analyzed_addresses.back())->add_existance<luramas::ast::element_kinds::statement_member>();
                        }

                        /* End */
                        if (!i->has_expr(luramas::ast::element_kinds::statement_member)) {
                              i->add_existance<luramas::ast::element_kinds::statement_end>();
                        }

                        /* Append */
                        analyzed_addresses.insert(analyzed_addresses.end(), pre_analyzed_addresses.begin(), pre_analyzed_addresses.end());
                  }

            skip:
                  /* Clear */
                  vect.clear();
                  pre_analyzed_addresses.clear();

            } else if (i->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                  /* Sigular */
                  i->add_expr<luramas::ast::element_kinds::statement_single>();
            }
      }

      return;
}