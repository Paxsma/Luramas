#pragma once
#include "../../debug.hpp"
#include "../../il/lexer/lexer.hpp"
#include "../../ir/ir.hpp"
#include "../../ir/lifter/lifter/lifter_data.hpp"
#include "../debug.hpp"
#include "ast.hpp"
#include "kinds.hpp"
#include "transformers/transformers_macros.hpp"
#include <algorithm>
#include <inttypes.h>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

namespace luramas {

      namespace ast {

            enum class element {
                  front,
                  back
            };

            enum class str_type : std::uint8_t {
                  disassembly,
                  all /* Debug functions macro must be true for AST. */
            };

            struct node {

                  std::uintptr_t address = 0u; /* Address */

                  std::vector<std::pair<element_kinds, std::size_t /* Count usally used for ends, repeat, etc. */>> expr = {{element_kinds::lex, 0u}}; /* Expression types. (Follows order) *All will get emmited(str). */

                  /* Convert destination to scoped? */
                  struct variables {
                        bool set_prefix = false;                /* Used in lifter to set suffix to variable name. */
                        std::string name = "";                  /* Variable name (Suffix, actual variable name if is_upvalue is true, closure names wont get set here)  */
                        std::vector<std::string> multret_names; /* Variable names for multret follows name. (All variable names will be here including first, name is for ones without multret  (Garunteed)) */
                        std::size_t multret_amount = 0u;        /* Variable multret count for regs. */
                  } variables;

                  /* Extra information for branch. */
                  struct branches {
                        bool opposite = false; /* Aka truth, opposite compare from opcode. */
                  } branches;

                  /* Extra information for tables. */
                  struct tables {
                        std::uintptr_t end_table = 0u; /* End table node address(not scopped). */
                  } tables;

                  /* Extra information for loops. */
                  struct loops {

                        std::shared_ptr<luramas::ast::node> start_node = nullptr;                            /* Start of loop. */
                        std::shared_ptr<luramas::ast::node> end_node = nullptr;                              /* Used for prologue and epilogue of loop. */
                        std::unordered_map<std::uint16_t /* Reg */, std::string /* Name */> iteration_names; /* Override iteration variable names. */

                        std::uint16_t start_reg = 0u; /* Start register (Format, End of loop) */
                        std::uint16_t end_reg = 0u;   /* End register (Format, End of loop) */

                  } loops;

                  /* Extra information for closure. */
                  struct closures {
                        std::size_t closure_idx = 0u; /* Index of relating closure too ast->closure. */
                        std::shared_ptr<node> setglobal_node = nullptr;
                        std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */> idx_nodes; /* Set index nodes for closure. */
                  } closures;

                  /* Extra information for flag instructions. */
                  struct flags {

                        struct poparg_flag {

                              /* Pops args from stack for OP_CCALL. */
                              std::vector<std::uint16_t> call_pop;

                        } poparg_flag;

                  } flags;

                  /* Binary operation kind. */
                  luramas::ast::bin_kinds bin_kind = luramas::ast::bin_kinds::nothing;

                  std::shared_ptr<luramas::il::lexer::lexeme> lex; /* Node lexeme data. Has all the detailed information. */

                  /* Special */
                  std::shared_ptr<node> sub_node = nullptr;             /* When a move instruction is hit this will be the source node(mutable by for loop scopes(preps or jumptoo if no prep)). */
                  std::vector<std::shared_ptr<node>> dest_nodes_init;   /* Where dests where intialy initialized(mutable by for loop scopes). (MAY BE UNSORTED) */
                  std::vector<std::shared_ptr<node>> source_nodes_init; /* Where sources where intialy initialized(mutable by for loop scopes). (MAY BE UNSORTED)  */
                  std::vector<std::shared_ptr<node>> source_nodes;      /* If a instruction has source/reg operands with regs it will get when those regs where last set(Can be init, not arg). (MAY BE UNSORTED) */

                  /* Node functions */

                  /* Appends expr */
                  template <element_kinds type>
                  void add_expr(const std::size_t count = 1u, const element ele = element::back, const std::size_t pos = 0u /* Optional positon overrides ele. */) {

                        if (!count) {
                              return;
                        }

                        /* Replace only lex with type. */
                        if (!this->expr.empty() && this->expr.front().first == luramas::ast::element_kinds::lex /* Used as place holder. */) {
                              this->expr.front().first = type;
                              this->expr.front().second = count;
                        } else {

                              const auto pair = std::make_pair(type, count);

                              if (pos) {
                                    this->expr.insert(this->expr.begin() + pos, pair);
                              } else {

                                    if (ele == element::back) {
                                          this->expr.emplace_back(pair);
                                    } else {
                                          this->expr.insert(this->expr.begin(), pair);
                                    }
                              }
                        }

                        return;
                  }

                  /* Appends with nonconsant type. */
                  void add_expr_tt(const element_kinds type, const std::size_t count = 1u, const element ele = element::back, const std::size_t pos = 0u /* Optional positon overrides elee. */) {

                        if (!count) {
                              return;
                        }

                        /* Replace only lex with type. */
                        if (!this->expr.empty() && this->expr.front().first == luramas::ast::element_kinds::lex /* Used as place holder. */) {
                              this->expr.front().first = type;
                              this->expr.front().second = count;
                        } else {

                              const auto pair = std::make_pair(type, count);

                              if (pos) {
                                    this->expr.insert(this->expr.begin() + pos, pair);
                              } else {

                                    if (ele == element::back) {
                                          this->expr.emplace_back(pair);
                                    } else {
                                          this->expr.insert(this->expr.begin(), pair);
                                    }
                              }
                        }

                        return;
                  }

                  /* Remove expr */
                  template <element_kinds type>
                  void remove_expr() {

                        /* Replace only lex with type. */
                        if (this->expr.size() == 1u && this->expr.front().first == type) {
                              this->expr.front().first = luramas::ast::element_kinds::lex;
                              this->expr.front().second = 1u;
                        } else {

                              for (auto i = 0u; i < this->expr.size(); i++) {

                                    const auto expr = this->expr[i];

                                    if (expr.first == type) {
                                          this->expr.erase(this->expr.begin() + i);
                                          break;
                                    }
                              }
                        }

                        return;
                  }

                  /* Remove expr */
                  template <element_kinds type>
                  std::pair<element_kinds, std::size_t> get_expr() {

                        for (const auto &expr : this->expr)
                              if (expr.first == type) {
                                    return expr;
                              }

#if DEBUG_AST_DISPLAY_WARNINGS
                        std::printf("[WARNING] Returning dead expr for get_expr.\n");
#endif

                        return std::make_pair(element_kinds::lex, 1u);
                  }

                  /* Adds expression if type isn't a expr. **Used when their should be one garunteed expr of one type.**  */
                  template <element_kinds type>
                  void add_existance(const std::size_t count = 1u, const element ele = element::back) {

                        if (!this->has_expr(type)) {
                              this->add_expr<type>(count, ele);
                        }

                        return;
                  }

                  /* Replaces next target from 0 with type and count. */
                  template <element_kinds target, element_kinds type>
                  void replace_next(const std::size_t count = 1u) {

                        for (auto &expr : this->expr)
                              if (expr.first == target) {
                                    expr.first = type;
                                    expr.second = count;
                                    break;
                              }

                        return;
                  }

                  /* Removal all target (Expr/Stat/Descriptor). */
                  template <element_kinds target>
                  void remove_all_expr() {

                        while (this->has_expr(target)) {
                              this->remove_expr<target>();
                        }

                        return;
                  }

                  /* Expr/Stat/Descriptor kind exists? */
                  bool has_expr(const element_kinds type) {
                        for (const auto &i : this->expr)
                              if (i.first == type) {
                                    return true;
                              }
                        return false;
                  }

                  /* Counts (Expr/Stat/Descriptor) count total. */
                  template <element_kinds type>
                  std::uintptr_t count_expr() {
                        std::uintptr_t count = 0u;
                        for (const auto &i : this->expr)
                              if (i.first == type)
                                    count += i.second;
                        return count;
                  }

                  /* Counts (Expr/Stat/Descriptor) count total with non static type. */
                  std::uintptr_t count_expr(const element_kinds type) {
                        std::uintptr_t count = 0u;
                        for (const auto &i : this->expr)
                              if (i.first == type)
                                    count += i.second;
                        return count;
                  }

                  /* Counts (Expr/Stat/Descriptor) member total. */
                  template <element_kinds type>
                  std::uintptr_t count_expr_member() {
                        std::uintptr_t count = 0u;
                        for (const auto &i : this->expr)
                              if (i.first == type)
                                    count++;
                        return count;
                  }

                  /* Collapses (Expr/Stat/Descriptor) by count. */
                  void collapse_expr() {

                        for (auto i = 0u; i < this->expr.size(); ++i) {

                              auto expr = this->expr[i];

                              while (expr.second > 1u) {

                                    this->add_expr_tt(expr.first, 1u, luramas::ast::element::back, i);
                                    --expr.second;
                              }
                        }

                        return;
                  }

                  /* Gets final sub node. */
                  std::shared_ptr<node> get_sub_node() {

                        auto sub = this->sub_node;

                        while (sub != nullptr)
                              if (sub->sub_node != nullptr && sub->sub_node != sub) {
                                    sub = sub->sub_node;
                              } else {
                                    break;
                              }

                        return sub;
                  }

#if DEBUG_AST_NODE

                  /* Turns expr pair into a string. */
                  std::string expr_str(const std::pair<element_kinds, std::size_t> &p) {

                        std::string retn = "";

                        switch (p.first) {

                              case element_kinds::lex: {
                                    retn += "lex";
                                    break;
                              }

                              case element_kinds::exit_post: {
                                    retn += "exit_post";
                                    break;
                              }
                              case element_kinds::exit_pre: {
                                    retn += "exit_pre";
                                    break;
                              }
                              case element_kinds::exit_dead: {
                                    retn += "exit_dead";
                                    break;
                              }

                              case element_kinds::arith: {
                                    retn += "arith";
                                    break;
                              }
                              case element_kinds::arithK: {
                                    retn += "arithK";
                                    break;
                              }

                              case element_kinds::locvar: {
                                    retn += "locvar";
                                    break;
                              }
                              case element_kinds::locvar_upvalue: {
                                    retn += "locvar_upvalue";
                                    break;
                              }
                              case element_kinds::locvar_multret: {
                                    retn += "locvar_multret";
                                    break;
                              }

                              case element_kinds::statement_begin: {
                                    retn += "statement_begin";
                                    break;
                              }
                              case element_kinds::statement_member: {
                                    retn += "statement_member";
                                    break;
                              }
                              case element_kinds::statement_end: {
                                    retn += "statement_end";
                                    break;
                              }
                              case element_kinds::statement_single: {
                                    retn += "statement_single";
                                    break;
                              }

                              case element_kinds::table: {
                                    retn += "table";
                                    break;
                              }

                              case element_kinds::call_multret_start: {
                                    retn += "call_multret_start";
                                    break;
                              }
                              case element_kinds::call_multret_member: {
                                    retn += "call_multret_member";
                                    break;
                              }
                              case element_kinds::call_multret_end: {
                                    retn += "call_multret_end";
                                    break;
                              }

                              case element_kinds::for_iv_start: {
                                    retn += "for_iv_start";
                                    break;
                              }
                              case element_kinds::for_iv_end: {
                                    retn += "for_iv_end";
                                    break;
                              }
                              case element_kinds::for_start: {
                                    retn += "for_start";
                                    break;
                              }
                              case element_kinds::for_end: {
                                    retn += "for_end";
                                    break;
                              }
                              case element_kinds::for_n_start: {
                                    retn += "for_n_start";
                                    break;
                              }
                              case element_kinds::for_n_end: {
                                    retn += "for_n_end";
                                    break;
                              }
                              case element_kinds::for_prep: {
                                    retn += "for_prep";
                                    break;
                              }

                              case element_kinds::source_inside_scope: {
                                    retn += "source_inside_scope";
                                    break;
                              }
                              case element_kinds::source_outside_scope: {
                                    retn += "source_outside_scope";
                                    break;
                              }

                              case element_kinds::repeat_: {
                                    retn += "repeat";
                                    break;
                              }
                              case element_kinds::while_: {
                                    retn += "while";
                                    break;
                              }
                              case element_kinds::while_end: {
                                    retn += "while_end";
                                    break;
                              }
                              case element_kinds::until_: {
                                    retn += "until";
                                    break;
                              }
                              case element_kinds::break_: {
                                    retn += "break";
                                    break;
                              }
                              case element_kinds::scope_end: {
                                    retn += "scope_end";
                                    break;
                              }

                              case element_kinds::call_routine_start: {
                                    retn += "call_routine_start";
                                    break;
                              }
                              case element_kinds::call_routine_end: {
                                    retn += "call_routine_end";
                                    break;
                              }

                              case element_kinds::concat_routine_start: {
                                    retn += "concat_routine_start";
                                    break;
                              }
                              case element_kinds::concat_routine_end: {
                                    retn += "concat_routine_end";
                                    break;
                              }

                              case element_kinds::conditional_expression_predicted: {
                                    retn += "conditional_expression_predicted";
                                    break;
                              }
                              case element_kinds::conditional_expression_start: {
                                    retn += "conditional_expression_start";
                                    break;
                              }
                              case element_kinds::conditional_expression_end: {
                                    retn += "conditional_expression_end";
                                    break;
                              }
                              case element_kinds::conditional_expression_end_emit: {
                                    retn += "conditional_expression_end_emit";
                                    break;
                              }

                              case element_kinds::conditonal_jumps_out: {
                                    retn += "conditonal_jumps_out";
                                    break;
                              }
                              case element_kinds::conditional_break: {
                                    retn += "conditional_break";
                                    break;
                              }

                              case element_kinds::if_: {
                                    retn += "if";
                                    break;
                              }
                              case element_kinds::elseif_: {
                                    retn += "elseif";
                                    break;
                              }
                              case element_kinds::else_: {
                                    retn += "else";
                                    break;
                              }
                              case element_kinds::condition_and: {
                                    retn += "and";
                                    break;
                              }
                              case element_kinds::condition_or: {
                                    retn += "or";
                                    break;
                              }
                              case element_kinds::condition_nonmutable: {
                                    retn += "condition_nonmutable";
                                    break;
                              }
                              case element_kinds::condition_concat_start: {
                                    retn += "condition_concat_start";
                                    break;
                              }
                              case element_kinds::condition_concat_member: {
                                    retn += "condition_concat_member";
                                    break;
                              }
                              case element_kinds::condition_concat_end: {
                                    retn += "condition_concat_end";
                                    break;
                              }
                              case element_kinds::condition_true: {
                                    retn += "condition_true";
                                    break;
                              }
                              case element_kinds::condition_routine: {
                                    retn += "condition_routine";
                                    break;
                              }
                              case element_kinds::condition_routine_start: {
                                    retn += "condition_routine_start";
                                    break;
                              }
                              case element_kinds::condition_routine_end: {
                                    retn += "condition_routine_end";
                                    break;
                              }
                              case element_kinds::condition_flag: {
                                    retn += "condition_flag";
                                    break;
                              }
                              case element_kinds::condition_break: {
                                    retn += "condition_break";
                                    break;
                              }
                              case element_kinds::condition_emit_next: {
                                    retn += "condition_emit_next";
                                    break;
                              }
                              case element_kinds::condition_logical_start: {
                                    retn += "condition_logical_start";
                                    break;
                              }
                              case element_kinds::condition_logical: {
                                    retn += "condition_logical";
                                    break;
                              }
                              case element_kinds::condition_logical_end: {
                                    retn += "condition_logical_end";
                                    break;
                              }
                              case element_kinds::condition_append_source: {
                                    retn += "condition_append_source";
                                    break;
                              }

                              case element_kinds::condition_end_scope: {
                                    retn += "condition_end_scope";
                                    break;
                              }
                              case element_kinds::condition_start_scope: {
                                    retn += "condition_start_scope";
                                    break;
                              }
                              case element_kinds::condition_start_scope_post: {
                                    retn += "condition_start_scope_post";
                                    break;
                              }

                              case element_kinds::jump_elseif: {
                                    retn += "jump_elseif";
                                    break;
                              }
                              case element_kinds::jump_else: {
                                    retn += "jump_else";
                                    break;
                              }

                              case element_kinds::table_start: {
                                    retn += "table_start";
                                    break;
                              }
                              case element_kinds::table_element: {
                                    retn += "table_element";
                                    break;
                              }
                              case element_kinds::table_end: {
                                    retn += "table_end";
                                    break;
                              }
                              case element_kinds::table_index: {
                                    retn += "table_index";
                                    break;
                              }

                              case element_kinds::closure_scoped: {
                                    retn += "closure_scoped";
                                    break;
                              }
                              case element_kinds::closure_global: {
                                    retn += "closure_global";
                                    break;
                              }
                              case element_kinds::closure_anonymous: {
                                    retn += "closure_anonymous";
                                    break;
                              }

                              case element_kinds::bad_instruction: {
                                    retn += "bad_instruction";
                                    break;
                              }
                              case element_kinds::dead_instruction: {
                                    retn += "dead_instruction";
                                    break;
                              }
                              case element_kinds::conditional: {
                                    retn += "conditional";
                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unkown expr for expr string.");
                              }
                        }

                        retn = '[' + retn + "]: " + std::to_string(p.second);

                        return retn;
                  }

#endif

                  /* Debug */

#if debug_functions

                  /* Prints disassembly */
                  void debug_print_disassembly(const char *const state = " ") {
                        std::printf("[Node-Debug(%s)] %" PRIuPTR " %s\n", state, this->lex->disassembly->addr, this->lex->disassembly->data->data.c_str());
                        return;
                  }

                  /* Prints everything */
                  void debug_print_all(const char *const state = " ") {
                        std::printf("[Node-Debug(%s)] %" PRIuPTR " %s", state, this->lex->disassembly->addr, this->lex->disassembly->data->data.c_str());
                        for (const auto &i : this->expr)
                              std::printf("%s", this->expr_str(i).c_str());
                        std::printf("\n");
                        return;
                  }

#endif

                  template <str_type type = str_type::disassembly>
                  std::string str() {

                        switch (type) {

#if debug_functions
                              case str_type::all: {

                                    auto retn = std::to_string(this->lex->disassembly->addr) + " " + this->lex->disassembly->disassemble();

                                    for (const auto &i : this->expr)
                                          retn += this->expr_str(i);

                                    return retn;
                              }
#endif
                              case str_type::disassembly: {
                                    return std::string(std::to_string(this->lex->disassembly->addr) + " " + this->lex->disassembly->disassemble());
                              }

                              default: {
                                    return "";
                              }
                        }
                  }
            };

            struct block {

                  std::uintptr_t node_start = 0u; /* PC start. */
                  std::uintptr_t node_end = 0u;   /* PC final instruction. */

                  std::vector<std::shared_ptr<node>> nodes;     /* Nodes in block. (Body) */
                  std::vector<std::shared_ptr<block>> branches; /* 2 elements; first is branch taken second is not, 1 there is only a jump (calls\for don't count, jumpbacks will refer to other nodes that have been skipped else wont have anything(may get fragmented), may be extras if dead instruction is next), 0 no jumps.  */

#pragma region is

                  /* Sees wether if a range start, sources(end) is filled with instructions dest all eventually lead too sources(arg). Start is ignored for first iteration. */
                  bool filled(const std::shared_ptr<node> &start /* Start of anlyzation. */, const std::shared_ptr<node> &sources, const bool ignore_routines = false /* Ignores routine exprs until start is hit with -1 = 0. */) {

                        std::intptr_t routine = 0;

                        std::vector<std::uintptr_t> analyzed_addresses;
                        std::vector<std::shared_ptr<luramas::ast::node>> vect;

                        /* Only one address check for dest. */
                        if ((start->address + start->lex->disassembly->len) == sources->address) {
                              return sources->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>();
                        }

                        /* Same for source */
                        if (start->address == sources->address) {
                              return true;
                        }

                        /* No dest */
                        if (!start->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {
                              return false;
                        }

                        /* Go through each source too see if they fill wil single instruction. */
                        if (!sources->source_nodes.empty()) {

                              vect.insert(vect.begin(), sources->source_nodes.begin(), sources->source_nodes.end());

                              do {

                                    auto curr_node = vect.front();

                                    if (!ignore_routines) {

                                          routine_inc_safe(curr_node, routine);
                                          routine_dec_safe(curr_node, routine);

                                          if (routine) {
                                                goto skip;
                                          }
                                    }

                                    /* No dest */
                                    if (!curr_node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {
                                          return false;
                                    }

                                    if (curr_node->address >= start->address && !curr_node->source_nodes.empty()) {
                                          vect.insert(vect.end(), curr_node->source_nodes.begin(), curr_node->source_nodes.end());
                                    }

                                    if (curr_node->address >= start->address && std::find(analyzed_addresses.begin(), analyzed_addresses.end(), curr_node->address) == analyzed_addresses.end()) {
                                          analyzed_addresses.emplace_back(curr_node->address);
                                    }

                              skip:

                                    /* Remove current. */
                                    vect.erase(std::remove(vect.begin(), vect.end(), curr_node), vect.end());

                              } while (!vect.empty());
                        }

                        /* Check all anlyzed addresses make sure they fill. */
                        bool fill = false;
                        auto on = start;
                        std::sort(analyzed_addresses.begin(), analyzed_addresses.end());
                        for (const auto i : analyzed_addresses) {

                              on = this->visit_addr(on->address + on->lex->disassembly->len);

                              if (!(fill = (i == on->address))) {
                                    break;
                              }
                        }

                        return fill;
                  }

                  /* Visits all nodes to see if their opcodes match with target. */
                  template <luramas::il::arch::opcodes op>
                  bool has_inst() {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->disassembly->op == op) {
                                    return true;
                              }
                        }

                        return false;
                  }

                  /* See if next opcode from addr exists. (Ignores current) */
                  template <luramas::il::arch::opcodes op>
                  bool has_next_inst(const std::uintptr_t addr) {

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > addr && i->lex->disassembly->op == op) {
                                    return true;
                              }
                        }

                        return false;
                  }

                  /* See if address is inside rotuine. (Doesnt count close). */
                  template <element_kinds target, element_kinds close>
                  bool inside_routine(const std::uintptr_t addr) {

                        std::int32_t count = 0;

                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* Inc */
                              if (i->has_expr(target)) {
                                    ++count;
                              }

                              /* Dec */
                              if (count != 0 && i->has_expr(close)) {
                                    --count;
                              }

                              /* Inside */
                              if (count && i->address == addr) {
                                    return true;
                              }
                        }

                        return false;
                  }

#pragma endregion

                  /* All visits gets sorted automatically by address. */

#pragma region visitors

                  /* Visits all blocks in ast.  */
                  std::vector<std::shared_ptr<node>> visit_all() {

                        /* Return cached */
                        if (!cached.all_nodes.empty()) {
                              return cached.all_nodes;
                        }

                        std::vector<std::shared_ptr<node>> retn;
                        std::vector<block *> scopes = {this};

                        do {

                              auto current_block = scopes.front();

                              retn.insert(retn.end(), current_block->nodes.begin(), current_block->nodes.end());

                              /* Add nested blocks. */
                              for (const auto &i : current_block->branches)
                                    scopes.emplace_back(i.get());

                              /* Remove current. */
                              scopes.erase(std::remove(scopes.begin(), scopes.end(), current_block), scopes.end());

                              this->remove_dupes(scopes); /* Remove duplicates. */

                        } while (!scopes.empty());

                        if (retn.empty()) {
                              throw std::runtime_error("Returning no data for visit_all.");
                        }

                        this->remove_dupes(retn);
                        this->sort_addr(retn);

                        /* Set cache */
                        cached.all_nodes = retn;

                        return retn;
                  }

                  /* Visits all instructions but fliped vector. */
                  std::vector<std::shared_ptr<node>> visit_all_flip() {
                        auto all = this->visit_all();
                        std::reverse(all.begin(), all.end());
                        return all;
                  }

                  /* Visits first/all opcode value block. */
                  template <luramas::il::arch::opcodes op>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_inst(const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->disassembly->op == op) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visits first/all opcode value in scope. */
                  template <luramas::il::arch::opcodes op>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_inst_scope(const std::uintptr_t addr, const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_rest_scope_addr(addr);
                        for (const auto &i : all_nodes) {

                              if (i->lex->disassembly->op == op) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visits first/all types in a block. */
                  template <luramas::il::lexer::inst_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_inst_kinds(const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through all nodes and find given type. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->kind == type) {

                                    if (all) { /* Passed all so emplace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        if (retn.empty()) {
                              std::printf("[WARNING] visit_inst_kinds returned empty for all.\n");
                        }
#endif

                        return retn;
                  }

                  /* Visits first/all types in a scope. */
                  template <luramas::il::lexer::inst_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_type_scope(const bool all /* All nodes with instruction. */, const bool ignore_branch = true /* Ignores branches not compare */) {

                        std::uintptr_t ignore_till = 0u;
                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through all nodes and find given type. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address >= ignore_till) {

                                    ignore_till = 0u;

                                    if (i->lex->kind == type) {

                                          if (all) { /* Passed all so emplace node. */
                                                retn.emplace_back(i);
                                          } else { /* Not all so return node. */
                                                return i;
                                          }
                                    }
                              }

                              if (!ignore_till && i->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {
                                    ignore_till = i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                              }

                              if (!ignore_till && i->lex->kind == luramas::il::lexer::inst_kinds::branch && !ignore_branch) {
                                    ignore_till = i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                              }
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        if (retn.empty()) {
                              std::printf("[WARNING] Visit_type returned empty for all.\n");
                        }
#endif

                        return retn;
                  }

                  /* Visits next/all types in a block. (Ignores current) */
                  template <luramas::il::lexer::inst_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_type_next_addr(const bool all /* All nodes with instruction. */, const std::uintptr_t addr) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through all nodes and find given type. */
                        const auto all_nodes = this->visit_rest(addr);
                        for (const auto &i : all_nodes) {

                              if (i->lex->kind == type) {

                                    if (all) { /* Passed all so emplace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        if (retn.empty()) {
                              std::printf("[WARNING] Visit_type_next_addr returned empty for all.\n");
                        }
#endif

                        return retn;
                  }

                  /* Visits previous type from an address. (Includes current) */
                  template <luramas::il::lexer::inst_kinds type>
                  std::shared_ptr<node> visit_prev_type_current(const std::uintptr_t target) {

                        std::shared_ptr<node> retn = nullptr;

                        /* Iterate through all nodes and find given type. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->kind == type && i->address <= target) {
                                    retn = i;
                              }
                        }

                        return retn;
                  }

                  /* Visits previous type from an address. (Ignores current) */
                  template <luramas::il::lexer::inst_kinds type>
                  std::shared_ptr<node> visit_prev_type(const std::uintptr_t target) {

                        std::shared_ptr<node> retn = nullptr;

                        /* Iterate through all nodes and find given type. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->kind == type && i->address < target) {
                                    retn = i;
                              }
                        }

                        return retn;
                  }

                  /* Visits next/all opcode from addr. (Ignores current.) */
                  template <luramas::il::arch::opcodes op>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_inst(const std::uintptr_t addr, const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through all nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > addr && i->lex->disassembly->op == op) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

                        /* Nothing. */
                        if (retn.empty()) {
                              throw std::runtime_error("Returning no data for visit_next_inst.");
                        }

                        return retn;
                  }

                  /* Visits next. */
                  std::shared_ptr<node> visit_next(const std::shared_ptr<node> node) {
                        return this->visit_addr(node->address + node->lex->disassembly->len);
                  }

                  /* Visit node with address. */
                  std::shared_ptr<node> visit_addr(const std::uintptr_t addr) {

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address == addr)
                                    return i;
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        std::printf("[WARNING] Returning no data for visit_addr.\n");
#endif

                        return nullptr;
                  }

                  /* Visit node with expression. */
                  template <element_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_expr(const bool all) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->has_expr(type)) {

                                    if (all) {
                                          retn.emplace_back(i);
                                    } else {
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visit next node with expression. (Ignores current address) */
                  template <element_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_expr(const std::uintptr_t address, const bool all) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > address && i->has_expr(type)) {

                                    if (all) {
                                          retn.emplace_back(i);
                                    } else {
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visit next node with expression. (Includes current address) */
                  template <element_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_expr_current(const std::uintptr_t address, const bool all) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address >= address && i->has_expr(type)) {

                                    if (all) {
                                          retn.emplace_back(i);
                                    } else {
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visit next node with expression. (Ignores current address) */
                  template <element_kinds type>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_expr_scope(const std::uintptr_t address, const bool all) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_rest_scope_addr(address);
                        for (const auto &i : all_nodes) {

                              if (i->has_expr(type)) {

                                    if (all) {
                                          retn.emplace_back(i);
                                    } else {
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visit node with previous address. */
                  std::shared_ptr<node> visit_previous_addr(const std::uintptr_t addr) {

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if ((i->address + i->lex->disassembly->len) == addr)
                                    return i;
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        std::printf("[WARNING] Nullptr returning for previous addr.");
#endif

                        return nullptr;
                  }

                  /* Visits next/all inst type. */
                  template <luramas::il::lexer::inst_kinds inst>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_type(const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->lex->kind == inst) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

                        return retn;
                  }

                  /* Visits next/all inst type from address. Doesn't include current. */
                  template <luramas::il::lexer::inst_kinds inst>
                  std::variant<std::vector<std::shared_ptr<node>>, std::shared_ptr<node>> visit_next_type_addr(const std::uintptr_t addr, const bool all /* All nodes with instruction. */) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > addr && i->lex->kind == inst) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(i);
                                    } else { /* Not all so return node. */
                                          return i;
                                    }
                              }
                        }

                        if (all) {
                              return retn;
                        } else {
                              return nullptr;
                        }
                  }

                  /* Visits all inst type in range. (Includes being, end) */
                  template <luramas::il::lexer::inst_kinds inst>
                  std::vector<std::shared_ptr<node>> visit_range_type_current(const std::uintptr_t begin, const std::uintptr_t end) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address >= begin && i->address <= end && i->lex->kind == inst) {
                                    retn.emplace_back(i);
                              }
                        }

                        return retn;
                  }

                  /* Visits all inst type in range. (Ignores being, end) */
                  template <luramas::il::lexer::inst_kinds inst>
                  std::vector<std::shared_ptr<node>> visit_range_type(const std::uintptr_t begin, const std::uintptr_t end) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > begin && i->address < end && i->lex->kind == inst) {
                                    retn.emplace_back(i);
                              }
                        }

                        return retn;
                  }

                  /* Visits next inst type in range. (Includes being, end) */
                  template <luramas::il::lexer::inst_kinds inst>
                  std::shared_ptr<node> visit_range_type_next(const std::uintptr_t begin, const std::uintptr_t end) {

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address >= begin && i->address <= end && i->lex->kind == inst) {
                                    return i;
                              }
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        std::printf("[WARNING] Nothing will be returned for visit_range_type_next.\n");
#endif

                        return nullptr;
                  }

                  /* Visits node with previous node with given register as dest. */
                  std::shared_ptr<node> visit_previous_dest_register(const std::uintptr_t on_address, const std::uint16_t target_reg) {

                        std::shared_ptr<luramas::ast::node> retn = nullptr;

                        /* Iterate through all nodes and find given dest register thats before on_address. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* yeah */
                              if (i->address < on_address && !i->lex->operands.empty() && i->lex->operands.front() == luramas::il::lexer::operand_kinds::dest && i->lex->disassembly->operands.front()->dis.reg == target_reg) {

                                    if (retn == nullptr /* First */) {
                                          retn = i;
                                    } else if (retn->address < i->address /* Nearest */) {
                                          retn = i;
                                    }
                              }
                        }

                        /* Node is null. */
                        if (retn == nullptr)
                              throw std::runtime_error("Couldn't find previous dest based on register.");

                        return retn;
                  }

                  /* Visits rest of nodes for all blocks. (Ignores current) */
                  std::vector<std::shared_ptr<node>> visit_rest(const std::uintptr_t on_address) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address > on_address)
                                    retn.emplace_back(i);
                        }

                        if (retn.empty()) {
                              throw std::runtime_error("Returning no data for visit_addr.");
                        }

                        return retn;
                  }

                  /* Visits rest of nodes for all blocks. (Includes current) */
                  std::vector<std::shared_ptr<node>> visit_rest_current(const std::uintptr_t on_address) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->address >= on_address)
                                    retn.emplace_back(i);
                        }

                        if (retn.empty()) {
                              throw std::runtime_error("Returning no data for visit_addr.");
                        }

                        return retn;
                  }

                  /* Visits rest of nodes for all blocks backwards with start being start passed. (Includes current) */
                  std::vector<std::shared_ptr<node>> visit_rest_current_flip(const std::uintptr_t start) {

                        std::vector<std::shared_ptr<node>> vect = {this->visit_addr(start)};

                        auto prev = this->visit_previous_addr(start);
                        while (prev != nullptr) {
                              vect.emplace_back(prev);
                              prev = this->visit_previous_addr(prev->address);
                        }

                        return vect;
                  }

                  /* Visits relative node to op being target and args being addatives till op hits = dec and args = inc(singular) and its 0.  */
                  template <luramas::il::arch::opcodes op>
                  std::shared_ptr<node> visit_relative_inst(const std::vector<luramas::il::arch::opcodes> rel) {

                        auto count = 0u;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* Inc for relative. */
                              if (std::find(rel.begin(), rel, i->lex->disassembly->op) != rel.end())
                                    ++count;

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (i->lex->disassembly->op == op)
                                    if (!count) {
                                          return i;
                                    } else {
                                          --count;
                                    }
                        }

                        throw std::runtime_error("Returning no data for visit_relative_inst.");
                  }

                  /* Visit alls nodes inside scope with passed addr. (Ignores current) */
                  std::vector<std::shared_ptr<node>> visit_rest_scope_addr(const std::uintptr_t on_address) {

                        std::vector<std::shared_ptr<node>> nodes;

                        std::intptr_t scope_ = 0;
                        const auto next = this->visit_rest(on_address);
                        for (const auto &i : next) {

                              /* Scope */
                              scope_ += (i->count_expr<luramas::ast::element_kinds::repeat_>() +
                                         i->count_expr<luramas::ast::element_kinds::while_>() +
                                         i->count_expr<luramas::ast::element_kinds::for_start>() +
                                         i->count_expr<luramas::ast::element_kinds::for_iv_start>() +
                                         i->count_expr<luramas::ast::element_kinds::for_n_start>() +
                                         i->count_expr<luramas::ast::element_kinds::if_>());
                              scope_ -= i->count_expr<luramas::ast::element_kinds::scope_end>();

                              /* Out of scope */
                              if (scope_ < 0) {
                                    break;
                              }

                              nodes.emplace_back(i);
                        }

                        return nodes;
                  }

                  /* Visit alls nodes inside scope with passed addr. (Includes current) */
                  std::vector<std::shared_ptr<node>> visit_rest_scope_addr_current(const std::uintptr_t on_address) {

                        std::vector<std::shared_ptr<node>> nodes;

                        std::intptr_t scope_ = 0;
                        const auto next = this->visit_rest_current(on_address);
                        for (const auto &i : next) {

                              /* Scope */
                              scope_ += (i->count_expr<luramas::ast::element_kinds::repeat_>() +
                                         i->count_expr<luramas::ast::element_kinds::while_>() +
                                         i->count_expr<luramas::ast::element_kinds::for_start>() +
                                         i->count_expr<luramas::ast::element_kinds::for_iv_start>() +
                                         i->count_expr<luramas::ast::element_kinds::for_n_start>() +
                                         i->count_expr<luramas::ast::element_kinds::if_>());
                              scope_ -= i->count_expr<luramas::ast::element_kinds::scope_end>();

                              /* Out of scope */
                              if (scope_ < 0) {
                                    break;
                              }

                              nodes.emplace_back(i);
                        }

                        return nodes;
                  }

                  /* Visits next relative node to op being target and args being addatives till op hits = dec and args = inc(singular) and its 0.  (Ignores current) */
                  template <luramas::il::arch::opcodes op>
                  std::shared_ptr<node> visit_relative_next_inst(const std::uintptr_t on_address, const std::vector<luramas::il::arch::opcodes> rel) {

                        auto count = 0u;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* If current node address isnt bigger repeat till it is.*/
                              if (i->address <= on_address) {
                                    continue;
                              }

                              /* Inc for relative. */
                              if (std::find(rel.begin(), rel.end(), i->lex->disassembly->op) != rel.end()) {
                                    ++count;
                              }

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (i->lex->disassembly->op == op) {
                                    if (!count) {
                                          return i;
                                    } else {
                                          --count;
                                    }
                              }
                        }

                        throw std::runtime_error("Returning no data for visit_relative_inst.");
                  }

                  /* Visits next relative node to expr being target and args being addatives till exprs(rel arg) hits = dec and exper_target(template target) = inc(singular) and its 0.  (Ignores current) */
                  template <element_kinds target>
                  std::shared_ptr<node> visit_relative_next_expr(const std::uintptr_t on_address, const std::vector<element_kinds> rel) {

                        auto count = 0;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_rest(on_address);
                        for (const auto &i : all_nodes) {

                              /* Inc for relative. */
                              for (const auto r : rel)
                                    if (i->has_expr(r)) {
                                          count += i->count_expr(r);
                                    }

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (i->has_expr(target)) {

                                    count -= i->count_expr<target>();

                                    if (count <= 0) {
                                          return i;
                                    }
                              }
                        }

                        throw std::runtime_error("Returning no data for visit_relative_next_expr_scope_current.");
                  }

                  /* Visits next relative node to expr being target and args being addatives till exprs(rel arg) hits = dec and exper_target(template target) = inc(singular) and its 0.  (Includes current) */
                  template <element_kinds target>
                  std::shared_ptr<node> visit_relative_next_expr_scope_current(const std::uintptr_t on_address, const std::vector<element_kinds> rel) {

                        std::intptr_t count = 0;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_rest_current(on_address);
                        for (const auto &i : all_nodes) {

                              /* Inc for relative. */
                              for (const auto r : rel)
                                    if (i->has_expr(r)) {
                                          count += i->count_expr(r);
                                    }

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (i->has_expr(target)) {

                                    count -= i->count_expr<target>();

                                    if (count <= 0) {
                                          return i;
                                    }
                              }
                        }

                        throw std::runtime_error("Returning no data for visit_relative_next_expr.");
                  }

                  /* Visits next relative node in scope to expr being target and args being addatives till exprs(rel arg) hits = dec and exper_target(template target) = inc(singular) and its 0.  (Ignores current) */
                  template <element_kinds target>
                  std::shared_ptr<node> visit_relative_next_expr_scope(const std::uintptr_t on_address, const std::vector<element_kinds> rel) {

                        auto count = 0u;

                        /* Iterate through block nodes and find given instruction. */
                        const auto all_nodes = this->visit_rest_scope_addr(on_address);
                        for (const auto &i : all_nodes) {

                              /* Inc for relative. */
                              for (const auto r : rel)
                                    if (i->has_expr(r)) {
                                          ++count;
                                          break;
                                    }

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (i->has_expr(target)) {

                                    if (!count) {
                                          return i;
                                    } else {
                                          --count;
                                    }
                              }
                        }

                        throw std::runtime_error("Returning no data for visit_relative_next_expr.");
                  }

                  /* Visits expr routines (doesn't count for target).  */
                  template <element_kinds target, element_kinds close>
                  std::variant<std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>>, std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>> visit_expr_routine(const bool all /* All nodes with instruction. */) {

                        bool inside = false;
                        std::shared_ptr<node> begin = nullptr;
                        std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>> retn;

                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              if (i->has_expr(target)) {
                                    inside = true;
                                    begin = i;
                              }

                              /* If found op dec if count isnt 0. If it is 0 then return node. */
                              if (inside && i->has_expr(close)) {

                                    if (all) { /* Has all so emblace node. */
                                          retn.emplace_back(std::make_pair(begin, i));
                                    } else { /* Not all so return node. */
                                          return std::make_pair(begin, i);
                                    }

                                    inside = false;
                              }
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        if (retn.empty()) {
                              std::printf("[WARNING] Returning no data for visit_expr_routine.\n");
                        }
#endif

                        return retn;
                  }

                  /* Visits expr routines in range (includes start and end). */
                  template <element_kinds target, element_kinds close>
                  std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>> visit_expr_routine_range(const std::uintptr_t start, const std::uintptr_t end) {

                        std::vector<std::pair<std::shared_ptr<node>, std::shared_ptr<node>>> retn;

                        const auto routines = std::get<std::vector<std::pair<std::shared_ptr<node>, std::shared_ptr<node>>>>(this->visit_expr_routine<target, close>(true));
                        for (const auto &i : routines) {

                              if (i.first->address >= start && i.first->address <= end && i.second->address >= start && i.second->address <= end)
                                    retn.emplace_back(i);
                        }

                        this->remove_dupes(retn);
                        this->sort_addr(retn);

                        return retn;
                  }

                  /* Visits expr routines in range that are touching and concats them to one object. (includes start and end).*/
                  template <element_kinds target, element_kinds close>
                  std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End*/>> visit_expr_routine_range_touching(const std::uintptr_t start, const std::uintptr_t end) {

                        std::shared_ptr<node> begin = nullptr;
                        std::shared_ptr<node> last = nullptr;
                        std::vector<std::pair<std::shared_ptr<node>, std::shared_ptr<node>>> retn;

                        const auto routines = this->visit_expr_routine_range<target, close>(start, end);
                        for (auto idx = 0u; idx < routines.size(); idx++) {

                              const auto curr = routines[idx];

                              if (begin == nullptr) {
                                    begin = curr.first;
                              }
                              last = curr.second;

                              /* End of idx */
                              if (idx == routines.size() - 1u) {
                                    retn.emplace_back(std::make_pair(begin, curr.second));
                                    begin = nullptr;
                                    break;
                              }

                              /* Check next */
                              if (routines[idx + 1u].first->address != (curr.second->address + curr.second->lex->disassembly->len)) {
                                    retn.emplace_back(std::make_pair(begin, curr.second));
                                    begin = nullptr;
                              }
                        }

                        if (begin != nullptr) {
                              retn.emplace_back(std::make_pair(begin, last));
                        }

                        this->sort_addr(retn);

                        return retn;
                  }

                  /* Visits all nodes between addresses (Ignores start, end) */
                  std::vector<std::shared_ptr<node>> visit_range(const std::uintptr_t start, const std::uintptr_t end) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* Between addresses. */
                              if (i->address > start && i->address < end)
                                    retn.emplace_back(i);
                        }

#if DEBUG_AST_DISPLAY_WARNINGS
                        if (retn.empty()) {
                              std::printf("[WARNING] No will be returned for visit_range.\n");
                        }
#endif

                        return retn;
                  }

                  /* Visits all nodes between addresses (Includes current, end) */
                  std::vector<std::shared_ptr<node>> visit_range_current(const std::uintptr_t start, const std::uintptr_t end) {

                        std::vector<std::shared_ptr<node>> retn;

                        /* Iterate through block nodes. */
                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              /* Between addresses. */
                              if (i->address >= start && i->address <= end) {
                                    retn.emplace_back(i);
                              }
                        }

                        /* Nothing. */
                        if (retn.empty()) {
                              throw std::runtime_error("Returning no data for visit_range_current.");
                        }

                        return retn;
                  }

                  /* Visits rest of nodes that have either element in expr vector passed from start address (includes current). */
                  std::vector<std::shared_ptr<node>> visit_either_expr_vector_addr(const std::uintptr_t start, std::vector<luramas::ast::element_kinds> exprs) {

                        std::vector<std::shared_ptr<node>> retn;

                        const auto all = this->visit_rest_current(start);
                        for (const auto &i : all)
                              for (const auto expr : exprs)
                                    if (i->has_expr(expr)) {
                                          retn.emplace_back(i);
                                          break;
                                    }

                        return retn;
                  }

                  /* Visits nodes that jump too address given with type (Must have memaddr operand) */
                  template <luramas::il::lexer::inst_kinds type>
                  std::vector<std::shared_ptr<node>> visit_inst_kinds_goto(const std::uintptr_t addr) {

                        std::vector<std::shared_ptr<node>> retn;
                        const auto all = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(this->visit_inst_kinds<type>(true));

                        for (const std::shared_ptr<luramas::ast::node> &i : all) {

                              const auto mems = i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>();
                              for (const auto &m : mems)
                                    if (m->ref_addr == addr) {
                                          retn.emplace_back(i);
                                          break;
                                    }
                        }

                        return retn;
                  }

                  /* Visits nodes that jump too address given (Must have memaddr operand) */
                  std::vector<std::shared_ptr<node>> visit_goto(const std::uintptr_t addr) {

                        std::vector<std::shared_ptr<node>> retn;

                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              const auto mems = i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>();
                              for (const auto &m : mems)
                                    if (m->ref_addr == addr) {
                                          retn.emplace_back(i);
                                          break;
                                    }
                        }

                        return retn;
                  }

                  /* Visits nodes that jump too address. */
                  std::vector<std::pair<std::uintptr_t /* Labels address. */, std::vector<std::shared_ptr<node>>> /* Goto addresses */> visit_all_goto() {

                        std::vector<std::pair<std::uintptr_t /* Labels address. */, std::vector<std::shared_ptr<node>>> /* Goto addresses */> retn;

                        const auto all_nodes = this->visit_all();
                        for (const auto &i : all_nodes) {

                              const auto mems = i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>();
                              for (const auto &m : mems) {

                                    const auto key = m->ref_addr;

                                    if (std::find_if(retn.begin(), retn.end(), [&](const auto &pair) { return pair.first == key; }) == retn.end()) {
                                          retn.emplace_back(std::make_pair(key, std::vector<std::shared_ptr<node>>({i})));
                                    } else {
                                          std::find_if(retn.begin(), retn.end(), [&](const auto &pair) { return pair.first == key; })->second.emplace_back(i);
                                    }
                              }
                        }

                        return retn;
                  }

#pragma endregion

                private:
#pragma region clean
                  /* Removes scope dupes. */
                  void remove_dupes(std::vector<block *> &scopes) {
                        std::sort(scopes.begin(), scopes.end());
                        scopes.erase(std::unique(scopes.begin(), scopes.end()), scopes.end());
                        return;
                  }

                  /* Removes node dupes from pair. (removes by address) */
                  void remove_dupes(std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>> &nodes) {
                        std::sort(nodes.begin(), nodes.end());
                        nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
                        return;
                  }

                  /* Removes node dupes. (removes by address) */
                  void remove_dupes(std::vector<std::shared_ptr<node>> &nodes) {
                        std::sort(nodes.begin(), nodes.end());
                        nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
                        return;
                  }

                  /* Sorts nodes by addr. */
                  void sort_addr(std::vector<std::shared_ptr<node>> &nodes) {
                        if (!nodes.empty()) {
                              std::sort(nodes.begin(), nodes.end(), [](const std::shared_ptr<node> &a, const std::shared_ptr<node> &b) -> bool { return a->address < b->address; });
                        }
                        return;
                  }

                  /* Sorts nodes by addr. */
                  void sort_addr(std::vector<std::pair<std::shared_ptr<node> /* Begin */, std::shared_ptr<node> /* End */>> &nodes) {
                        if (!nodes.empty()) {
                              std::sort(nodes.begin(), nodes.end(), [](const std::pair<std::shared_ptr<node>, std::shared_ptr<node>> &a, const std::pair<std::shared_ptr<node>, std::shared_ptr<node>> &b) -> bool { return a.first->address < b.first->address; });
                        }
                        return;
                  }

#pragma endregion

                  struct cached {

                        std::vector<std::shared_ptr<node>> all_nodes;

                  } cached;
            };

            class ast : public std::enable_shared_from_this<ast> {

                public:
                  /* Ast transformer callback function follows format (ast&) returns void, used for passthroughs. */
                  using transformer_callback = void (*)(std::shared_ptr<luramas::ast::ast> &ast);

                  bool analyzed = false;                                                                     /* AST has been analyzed? */
                  float total = 0.0f;                                                                        /* Used for total only avialable for main ast. */
                  std::size_t ast_id = 0u;                                                                   /* ID */
                                                                                                             /* Proto for ast. */
                  std::unordered_map<std::uintptr_t, std::shared_ptr<luramas::il::disassembly>> disassembly; /* disassembly of closure (Useful for some stuff). { PC, disassembly } e.g. disassembly of pc=15 disassembly[15]. */
                  std::uintptr_t pc_end = 0u;                                                                /* Pc end */

                  std::shared_ptr<luramas::il::ilang> il = nullptr; /* IL */

                  luramas::emitter_ir::syntax::emitter_syntax syntax = luramas::emitter_ir::syntax::emitter_syntax::nothing;

                  closure_kinds closure_kind = closure_kinds::nothing;                                                                                 /* Closure type. */
                  std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> closure_name = luramas::ir::ir_expr_data::make::expression_data(); /* Closure name. */

                  luramas::ir::lines closure_ir; /* Handled by lifter not ast used in lifter for parent closures. */

                  std::vector<std::pair<std::int16_t /* Reg */, std::string /* Name */>> arg_regs; /* Register for arguments to be placed in. *-1 means: vararg */
                  /* No node can refrence the same address all nodes are unique but branches can reference the same jump. */
                  std::shared_ptr<block> body; /* Main block. */

                  std::unordered_map<std::uintptr_t /* Idx */, std::pair<std::string /* Value */, std::int16_t /* Reg (-1 for upv) */>> upvalues;
                  std::vector<std::shared_ptr<ast>> closures;               /* Any children closures, relates to closure->p. */
                  std::shared_ptr<ir::lifter::lifter_config> lifter_config; /* Linked lifter config. */
                  std::size_t sizecode = 0u;                                /* Size of code. */

                  /* Ast config */
                  struct ast_config {

                        /* Expands tables members if members exceed a certain amount. (Disabled for nested tables). -1 for disabled */
                        std::int32_t table_members_expand = -1;

                        /* Will be in use if var/arg/upvalue/etc suffix uses integer instead of char. (Makes incrementor scoped) */
                        bool scoped_global_incrementor = true;

                  } ast_config;

#pragma region block

                  /* Finds block by start address. */
                  std::shared_ptr<block> find_block(const std::uintptr_t addr) const {

                        const auto found = block_map.find(addr);
                        if (found != block_map.end()) {
                              return found->second;
                        }

                        return nullptr;
                  }

                  /* Finds block by containing address. */
                  std::shared_ptr<block> find_block_addr(const std::uintptr_t addr) const {

                        for (const auto &current_block : block_map) {

                              if (current_block.second->node_start <= addr && current_block.second->node_end >= addr) {
                                    return current_block.second;
                              }
                        }

                        return nullptr;
                  }

                  /* Add block to cache by start address. */
                  void add(const std::shared_ptr<block> &block) {

                        if (block_map.find(block->node_start) == block_map.end()) {
                              block_map.insert(std::make_pair(block->node_start, block));
                        }

                        return;
                  }

                  /* Add passthrough transformer, gets executed in order of emblaced_back. */
                  void add(const char *const debugname, const transformer_callback &passthrough) {
                        this->passthroughs.emplace_back(std::make_pair(debugname, passthrough));
                        return;
                  }

                  /* Run passthroughs for ast. */
                  template <bool debug = false /* Prints passthrough n and name.*/>
                  void run() {

                        /* Call */
                        for (auto i = 0u; i < this->passthroughs.size(); ++i) {
                              auto passthrough = this->passthroughs[i];
                              if (debug) {
                                    std::printf("[AST-PASSTHROUGHS] On passthrough %d calling transformer: %s.\n", i, passthrough.first);
                              }
                              auto ptr = shared_from_this();
                              passthrough.second(ptr);
                        }

                        /* Has been analyzed successfuly. */
                        this->analyzed = true;

                        return;
                  }

                  /* Returns vector of all transformer debugnames. */
                  std::vector<std::string> transformer_debugnames() const {
                        std::vector<std::string> retn;

                        for (const auto &i : this->passthroughs)
                              retn.emplace_back(i.first);

                        return retn;
                  }
#pragma endregion

                  /* Generate string representation of a control flow graph. */
                  template <bool raw = false /* Print exprs/descriptors/stats? */>
                  std::string gen_cfg() const {

                        std::string retn = "";
                        std::vector<std::uintptr_t> labels;
                        std::vector<std::pair<std::uintptr_t /* PC */, std::size_t /* Amt */>> indent_stack;

                        /* Append jump backs for labels. */
                        const auto branch = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(this->body->visit_next_type<luramas::il::lexer::inst_kinds::branch>(true));
                        for (const auto &node : branch) {
                              labels.emplace_back(node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                        }

                        /* Add conditional jump backs for labels. */
                        const auto conditional = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(this->body->visit_next_type<luramas::il::lexer::inst_kinds::branch_condition>(true));
                        for (const auto &node : conditional) {
                              if (node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp < 0) {
                                    labels.emplace_back(node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                              }
                        }

                        auto nodes = this->body->visit_all();
                        for (const auto &i : nodes) {

                              std::size_t mult = 0u;
                              std::string dism = "";

                              /* Indext mult */
                              while (!indent_stack.empty()) {
                                    if (indent_stack.back().first <= i->address) { /* Pop hit address */
                                          indent_stack.pop_back();
                                    } else { /* Hit */
                                          mult = indent_stack.back().second;
                                          break;
                                    }
                              }

                              /* Compile indent */
                              for (auto i = 0u; i < mult; ++i)
                                    dism += "   ";

                              /* Add disassembly */
                              dism += i->lex->disassembly->disassemble();

                              /* Add label */
                              if (std::find(labels.begin(), labels.end(), i->address) != labels.end()) {
                                    retn += "label_" + std::to_string(i->address) + ":\n";
                              }

                              /* Add goto */
                              if (i->lex->kind == luramas::il::lexer::inst_kinds::branch || (i->lex->kind == luramas::il::lexer::inst_kinds::branch_condition &&
                                                                                                (i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp < 0 || std::find(labels.begin(), labels.end(), i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) != labels.end()))) {
                                    dism += " goto label_" + std::to_string(i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr) + ";";
                              }

                              /* Add exprs if specified. */
                              if (!raw) {
                                    dism += " ( ";
                                    for (const auto &p : i->expr)
                                          dism += i->expr_str(p) + " ";
                                    dism += ")";
                              }

                              retn += '\n' + dism;

                              /* Append branch to the stack if there is one. */
                              if (i->lex->kind == il::lexer::inst_kinds::branch_condition) {

                                    const auto top = (indent_stack.size()) ? indent_stack.back().second : 0u;

                                    indent_stack.emplace_back(std::make_pair(i->lex->disassembly->ref->addr, top + 1u));
                              }
                        }

                        return retn;
                  }

                  std::string closure_information(const luramas::emitter_ir::syntax::emitter_syntax syn, std::shared_ptr<luramas::ir::data::format::format> &format) const {

                        std::string retn = "";

                        retn += "Proto:\n";
                        retn += "	* name: " + this->closure_name->str(syn, format) + "\n";
                        retn += "	* type: ";

                        switch (this->closure_kind) {

                              case luramas::ast::closure_kinds::main: {
                                    retn += "main";
                                    break;
                              }

                              case luramas::ast::closure_kinds::scoped: {
                                    retn += "scoped";
                                    break;
                              }

                              case luramas::ast::closure_kinds::anonymous: {
                                    retn += "anonymous";
                                    break;
                              }

                              case luramas::ast::closure_kinds::global: {
                                    retn += "global";
                                    break;
                              }

                              default: {
                                    retn += "nothing";
                                    break;
                              }
                        }

                        retn += "\n";

                        retn += "	* arg count: " + std::to_string(this->arg_regs.size()) + "\n";
                        retn += "	* args: ";

                        for (const auto &i : this->arg_regs)
                              retn += "r" + std::to_string(i.first) + "(" + i.second + ") ";

                        retn += "\n";
                        retn += "	* upvalue count: " + std::to_string(this->upvalues.size()) + "\n";
                        retn += "	* upvalues: ";

                        for (const auto &i : this->upvalues)
                              retn += "r" + std::to_string(i.second.second) + "(" + i.second.first + ") ";

                        return retn;
                  }

                  std::string str() const {

                        std::string retn = "";

                        const auto all = this->body->visit_all();
                        for (const auto &node : all)
                              retn += node->str<luramas::ast::str_type::all>() + "\n";

                        return retn;
                  }

                private:
                  std::map<std::uintptr_t /* Start */, std::shared_ptr<block>> block_map;
                  std::vector<std::pair<const char *const /* Name */, transformer_callback /* Function */>> passthroughs;
            };

            std::shared_ptr<ast> gen_ast(const luramas::emitter_ir::syntax::emitter_syntax syn, std::shared_ptr<luramas::il::ilang> &il_data, const std::shared_ptr<ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format);

      } // namespace ast

} // namespace luramas