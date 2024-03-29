#pragma once
#include <array>
#include <cstdint>
#include <iostream>

namespace luramas {

      namespace ast {

            /* Types of closures. */
            enum class closure_kinds : std::uint8_t {
                  nothing,
                  main,
                  scoped,
                  global,
                  anonymous
            };

            /* 
      
            * Expr/Stat(important ones, Descriptor exists) type. {desc, expr, stat or descriptor?, intended usage/set for mostly}
            * Both AST exprs, stat, discriptors are used in the same enum.
            * Both get treated differently but relatively the same so the same enum makes it more efficient for searching etc.
            * Stat and Exprs, etc arent identified by enum name but what it comment it. (Does not cause a huge problem because of how it is setup but it does make it look cleaner),
        
      */
            enum class element_kinds : std::uint8_t {

                  lex, /* Specific type refer to lexer. (DESCRIPTOR) [PLACEHOLDER] */

#pragma region exit
                  desc_exit_pre,  /* Exits decompilation before instruction. [LIFTER] */
                  desc_exit_post, /* Exits decompilation after instruction. [LIFTER] */
                  desc_exit_dead, /* Instruction exits because of a return. [AST] */
#pragma endregion Exit Descriptors

#pragma region arith
                  expr_arith,  /* r1 += r1 + r1 [AST] */
                  expr_arithk, /* r1 += r1 + 1  [AST] */
#pragma endregion Arith Expressions

#pragma region locvar
                  stat_locvar,         /* Dest is locvar. [ALL] */
                  stat_locvar_upvalue, /* Dest locvar turns into a upvalue. [ALL] */
                  stat_locvar_multret, /* Dest is locvar(Multret). [ALL] */
#pragma endregion Local Variable Statements

#pragma region returns
                  desc_return,   /* Return instruction [AST] */
                  desc_noreturn, /* Instruction that does not return. [AST] */
#pragma endregion Return Descriptors

#pragma region expression_descriptors
                  desc_expression_begin,  /* Begin of possible expression. [AST] */
                  desc_expression_member, /* Member of possible expression. [AST] */
                  desc_expression_end,    /* End of possible expression. (Guaranteeds destination register) [AST] */
#pragma endregion Expression Descriptors

                  condition_logical_end,
                  condition_logical_start,
                  condition_logical,

                  /* Multret concat **Applicable too move, set(table/global/upv)** */
                  call_multret_start,  /* Call multret concat start (DESCRIPTOR) [LIFTER] */
                  call_multret_member, /* Call multret concat member (DESCRIPTOR) [LIFTER] */
                  call_multret_end,    /* Call multret concat end (DESCRIPTOR) [LIFTER] */

#pragma region operand_register_scope
                  desc_source_outside_scope,    /* Source operand register declared outside of scope that isn't an argument. [ALL] */
                  desc_reg_outside_scope,       /* Reg operand register declared outside of scope that isn't an argument. [ALL] */
                  desc_value_outside_scope,     /* Value operand register declared outside of scope that isn't an argument. [ALL] */
                  desc_compare_outside_scope,   /* Compare operand register declared outside of scope that isn't an argument. [ALL] */
                  desc_table_reg_outside_scope, /* Table Register operand register declared outside of scope that isn't an argument. [ALL] */
#pragma endregion Operand Register Scope

#pragma region operand_register_values
                  desc_left_value_begin,   /* Begin of left value in expression. [AST] */
                  desc_left_value_member,  /* Member of left value in expression. [AST] */
                  desc_left_value_end,     /* End of left value in expression. [AST] */
                  desc_right_value_begin,  /* Begin of right value in expression. [AST] */
                  desc_right_value_member, /* Member of right value in expression. [AST] */
                  desc_right_value_end,    /* End of right value in expression. [AST] */
                  desc_lr_value,           /* Uses left/right values from predecessor instruction, preceeding instructions must have right/left value element kinds. [AST] */
#pragma endregion Operand Register Values, Guaranteeds Left and Right Values when Applicable

#pragma region constants
                  stat_while,         /* while () follows condition(not jumpback). [ALL] */
                  stat_repeat,        /* repeat [ALL] */
                  stat_loop_break,    /* break [ALL] */
                  stat_if,            /* if () [ALL] */
                  stat_elseif,        /* elseif () [ALL] */
                  stat_else,          /* else [ALL] */
                  stat_for_iv_start,  /* for i,v in pairs ({ 1 }) do [ALL] */
                  stat_for_start,     /* for generic start, serves as jump too OP_FORLOOPG (follows format). [ALL] */
                  stat_for_n_start,   /* for numeric start, serves as jump too OP_FORLOOPN (follows format). (numeric) [ALL] */
                  stat_for_prep,      /* for preperation instruction [AST] */
                  stat_jump_break,    /* Jump/condition breaks out of loop. [LIFTER] */
                  stat_goto,          /* Goto (label). [LIFTER] */
                  stat_jump_continue, /* Jump/condition continues loop. [LIFTER] */

                  stat_while_end,  /* while () end(jumpback) [AST] */
                  stat_repeat_end, /* until () follows condition(typically jumpback). [ALL] */
                  stat_scope_end,  /* scope end (generic) [ALL] */
                  stat_for_iv_end, /* for(i,v) end(jumpback) [AST] */
                  stat_for_end,    /* for generic end(jumpback) [AST] */
                  stat_for_n_end,  /* for numeric end(jumpback) [AST] */
#pragma endregion Constant Statements

#pragma region call
                  desc_call_routine_start, /* Call routine start. [AST] */
                  desc_call_routine_end,   /* Call routine end. [AST] */
#pragma endregion Call Descriptors

#pragma region concat
                  desc_concat_routine_start, /* Concat routine start. [AST] */
                  desc_concat_routine_end,   /* Concat routine end. [AST] */
#pragma endregion Concat Descriptors

                  conditional_expression_predicted, /* Indication that this may be the end of a conditional_expression. (Does not garunteed one) (DESCRIPTOR) [AST] */
                  conditional_expression_start,     /* Conditional expression routine start. (DESCRIPTOR) [AST] */
                  conditional_expression_end,       /* Conditional expression routine end. (Compare flag gets sets in dest(Hard coded else for OP_LOADBOOL/OP_GETTABUPVALUE(no source register))) (DESCRIPTOR) [AST] */
                  conditional_expression_end_emit,  /* Same as conditonal_expression_end but it emits data too the end of compare flag (EXPR) [LIFTER]*/

                  desc_conditional_jumpout, /* Condition jumps out of current scope (Ignores break). [AST] */

                  conditional_break, /* Condition breaks out of loop but doesn't garunteed a break. (DESCRIPTOR) [AST] */

                  condition_nonmutable,    /* Condition that cannot be converted into while/if/elseif etc. (DESCRIPTOR) [AST] */
                  condition_concat_start,  /* Concat a condition(universal) (start). (DESCRIPTOR) [AST] */
                  condition_concat_member, /* Concat a condition(universal) (member). (DESCRIPTOR) [AST] */
                  condition_concat_end,    /* Concat a condition(universal) (end will get written too compare flag). (DESCRIPTOR) [AST] */
                  condition_true,          /* Sets compare flag too true garunteing that while true expressions get set as true (expr type). (EXPR) [ALL] */
                  condition_flag,          /* Writes result to flag. (DESCRIPTOR) [LIFTER] */
                  condition_emit_next,     /* Emits compare data too dest register in next instruction. (DESCRIPTOR) [LIFTER] */
                  condition_append_source, /* Appends source too flag compare flag. (DESCRIPTOR) [LIFTER] */

                  /* Will get emmited to condition flag post compare. */
                  condition_and,              /* if/elseif/nested(and) appends to if_statements (Can be applied to until or while) (EXPR) [ALL] */
                  condition_or,               /* if/elseif/nested(or)  appends to if_statements (Can be applied to until or while) (EXPR) [ALL] */
                  condition_end_scope,        /* Conditon flag end scope:  %s ) [ALL] */
                  condition_start_scope,      /*  Conditon flag start scope(PRE): ( %s   [ALL] */
                  condition_start_scope_post, /*  Conditon flag start scope(POST): ( %s   [ALL] */

#pragma region conditional_routines
                  /* Describes all instructions in a conditional routine. It can encapsulate vars, args, anything. */
                  desc_conditional_routine,       /* Conditional routine [AST] */
                  desc_conditional_routine_start, /* Conditional routine start [AST] */
                  desc_conditional_routine_end,   /* Conditional routine [AST] */
#pragma endregion Conditional Routines

                  /* Jump */
                  jump_elseif, /* Jump leads too elseif (DESCRIPTOR) [AST] */
                  jump_else,   /* Jump leads too else (DESCRIPTOR) [AST] */

                  table_start,   /* Table { (EXPR) [ALL] */
                  table_element, /* Element in table. (Not usable for setlist cause of concatation) (EXPR) [ALL] */
                  table_end,     /* Table } (Will get ignored and use SETLIST instruction integral operand amt if it hits SETLIST.) (EXPR) [ALL] */
                  table_index,   /* Extra expr used for certain things (Will be appended when everything is done). (EXPR) [ALL] */

#pragma region closure
                  stat_closure_scoped,    /* Scoped function **Can be mutated by set locvar set if it is a locvar will be scoped else anonymous** [ALL] */
                  stat_closure_global,    /* Global function **Can be mutated by set locvar if it is a locvar instead of a global will turn into scoped** [ALL] */
                  stat_closure_anonymous, /* Anonymous function [ALL] */
#pragma endregion Closure Statements

#pragma region instruction
                  desc_bad_instruction,  /* Instruction will never get executed no matter watch branch is taken or not. [AST] */
                  desc_dead_instruction, /* Instruction gets ignored by lifter but exprs dont. [LIFTER] */
#pragma endregion Instruction Descriptors

                  desc_dest_mutate_condition_flag /* Condition flag will get written by dest. [ALL] */

            };

            /* Binary operation kinds. */
            enum class bin_kinds : std::uint8_t {
                  nothing,

                  /* Arith */
                  add_,  /* (+) Arithmetic Addition */
                  sub_,  /* (-) Arithmetic Subtraction */
                  mul_,  /* (*) Arithmetic Multiplication */
                  div_,  /* (/) Arithmetic Division */
                  idiv_, /* (//) Arithmetic Floor Division */
                  mod_,  /* (*) Arithmetic Multiply */
                  pow_,  /* (^) Arithmetic Power */
                  and_,  /* (&) Unary And */
                  xor_,  /* (~) Xor */
                  shl_,  /* (<<) Shift Left */
                  shr_,  /* (>>) Shift Right */
                  or_,   /* (|) Unary Or */

                  /* Unary */
                  len_,    /* Lenght of array/vector/table. */
                  minus_,  /* Unary minus */
                  not_,    /* Unary not */
                  bitnot_, /* Unary bitnot */
                  plus_,   /* Unary plus */

                  /* Compares, for both set cmp flags and jmps with compare. */
                  eq_,  /* (==) */
                  ne_,  /* (!=) */
                  lt_,  /* (<) */
                  lte_, /* (<=) */
                  gt_,  /* (>) */
                  gte_  /* (>=) */
            };

            __inline const char *const element_kind_str(const element_kinds k) {

                  switch (k) {

                        case element_kinds::lex: {
                              return "lex";
                        }

                        case element_kinds::desc_exit_post: {
                              return "exit_post";
                        }
                        case element_kinds::desc_exit_pre: {
                              return "exit_pre";
                        }
                        case element_kinds::desc_exit_dead: {
                              return "exit_dead";
                        }

                        case element_kinds::expr_arith: {
                              return "arith";
                        }
                        case element_kinds::expr_arithk: {
                              return "arithk";
                        }

                        case element_kinds::desc_return: {
                              return "return";
                        }
                        case element_kinds::desc_noreturn: {
                              return "noreturn";
                        }

                        case element_kinds::stat_locvar: {
                              return "locvar";
                        }
                        case element_kinds::stat_locvar_upvalue: {
                              return "locvar_upvalue";
                        }
                        case element_kinds::stat_locvar_multret: {
                              return "locvar_multret";
                        }

                        case element_kinds::desc_expression_begin: {
                              return "expression_begin";
                        }
                        case element_kinds::desc_expression_member: {
                              return "expression_member";
                        }
                        case element_kinds::desc_expression_end: {
                              return "expression_end";
                        }

                        case element_kinds::desc_left_value_begin: {
                              return "left_value_begin";
                        }
                        case element_kinds::desc_left_value_member: {
                              return "left_value_member";
                        }
                        case element_kinds::desc_left_value_end: {
                              return "left_value_end";
                        }
                        case element_kinds::desc_right_value_begin: {
                              return "right_value_begin";
                        }
                        case element_kinds::desc_right_value_member: {
                              return "right_value_member";
                        }
                        case element_kinds::desc_right_value_end: {
                              return "right_value_end";
                        }
                        case element_kinds::desc_lr_value: {
                              return "lr_value";
                        }

                        case element_kinds::call_multret_start: {
                              return "call_multret_start";
                        }
                        case element_kinds::call_multret_member: {
                              return "call_multret_member";
                        }
                        case element_kinds::call_multret_end: {
                              return "call_multret_end";
                        }

                        case element_kinds::stat_for_iv_start: {
                              return "for_iv_start";
                        }
                        case element_kinds::stat_for_iv_end: {
                              return "for_iv_end";
                        }
                        case element_kinds::stat_for_start: {
                              return "for_start";
                        }
                        case element_kinds::stat_for_end: {
                              return "for_end";
                        }
                        case element_kinds::stat_for_n_start: {
                              return "for_n_start";
                        }
                        case element_kinds::stat_for_n_end: {
                              return "for_n_end";
                        }
                        case element_kinds::stat_for_prep: {
                              return "for_prep";
                        }

                        case element_kinds::desc_source_outside_scope: {
                              return "source_outside_scope";
                        }
                        case element_kinds::desc_value_outside_scope: {
                              return "value_outside_scope";
                        }
                        case element_kinds::desc_reg_outside_scope: {
                              return "reg_outside_scope";
                        }
                        case element_kinds::desc_compare_outside_scope: {
                              return "compare_outside_scope";
                        }
                        case element_kinds::desc_table_reg_outside_scope: {
                              return "table_reg_outside_scope";
                        }

                        case element_kinds::stat_repeat: {
                              return "repeat";
                        }
                        case element_kinds::stat_while: {
                              return "while";
                        }
                        case element_kinds::stat_while_end: {
                              return "while_end";
                        }
                        case element_kinds::stat_repeat_end: {
                              return "repeat_end";
                        }
                        case element_kinds::stat_loop_break: {
                              return "break";
                        }
                        case element_kinds::stat_scope_end: {
                              return "scope_end";
                        }

                        case element_kinds::desc_call_routine_start: {
                              return "call_routine_start";
                        }
                        case element_kinds::desc_call_routine_end: {
                              return "call_routine_end";
                        }

                        case element_kinds::desc_concat_routine_start: {
                              return "concat_routine_start";
                        }
                        case element_kinds::desc_concat_routine_end: {
                              return "concat_routine_end";
                        }

                        case element_kinds::conditional_expression_predicted: {
                              return "conditional_expression_predicted";
                        }
                        case element_kinds::conditional_expression_start: {
                              return "conditional_expression_start";
                        }
                        case element_kinds::conditional_expression_end: {
                              return "conditional_expression_end";
                        }
                        case element_kinds::conditional_expression_end_emit: {
                              return "conditional_expression_end_emit";
                        }

                        case element_kinds::desc_conditional_jumpout: {
                              return "conditional_jumpout";
                        }
                        case element_kinds::conditional_break: {
                              return "conditional_break";
                        }

                        case element_kinds::stat_if: {
                              return "if";
                        }
                        case element_kinds::stat_elseif: {
                              return "elseif";
                        }
                        case element_kinds::stat_else: {
                              return "else";
                        }
                        case element_kinds::condition_and: {
                              return "and";
                        }
                        case element_kinds::condition_or: {
                              return "or";
                        }
                        case element_kinds::condition_nonmutable: {
                              return "condition_nonmutable";
                        }
                        case element_kinds::condition_concat_start: {
                              return "condition_concat_start";
                        }
                        case element_kinds::condition_concat_member: {
                              return "condition_concat_member";
                        }
                        case element_kinds::condition_concat_end: {
                              return "condition_concat_end";
                        }
                        case element_kinds::condition_true: {
                              return "condition_true";
                        }
                        case element_kinds::desc_conditional_routine: {
                              return "conditional_routine";
                        }
                        case element_kinds::desc_conditional_routine_start: {
                              return "conditional_routine_start";
                        }
                        case element_kinds::desc_conditional_routine_end: {
                              return "conditional_routine_end";
                        }
                        case element_kinds::condition_flag: {
                              return "condition_flag";
                        }
                        case element_kinds::stat_jump_break: {
                              return "jump_break";
                        }
                        case element_kinds::stat_jump_continue: {
                              return "continue";
                        }
                        case element_kinds::stat_goto: {
                              return "goto";
                        }
                        case element_kinds::condition_emit_next: {
                              return "condition_emit_next";
                        }
                        case element_kinds::condition_append_source: {
                              return "condition_append_source";
                        }

                        case element_kinds::condition_end_scope: {
                              return "condition_end_scope";
                        }
                        case element_kinds::condition_start_scope: {
                              return "condition_start_scope";
                        }
                        case element_kinds::condition_start_scope_post: {
                              return "condition_start_scope_post";
                        }

                        case element_kinds::jump_elseif: {
                              return "jump_elseif";
                        }
                        case element_kinds::jump_else: {
                              return "jump_else";
                        }

                        case element_kinds::table_start: {
                              return "table_start";
                        }
                        case element_kinds::table_element: {
                              return "table_element";
                        }
                        case element_kinds::table_end: {
                              return "table_end";
                        }
                        case element_kinds::table_index: {
                              return "table_index";
                        }

                        case element_kinds::stat_closure_scoped: {
                              return "closure_scoped";
                        }
                        case element_kinds::stat_closure_global: {
                              return "closure_global";
                        }
                        case element_kinds::stat_closure_anonymous: {
                              return "closure_anonymous";
                        }

                        case element_kinds::desc_bad_instruction: {
                              return "bad_instruction";
                        }
                        case element_kinds::desc_dead_instruction: {
                              return "dead_instruction";
                        }
                        case element_kinds::desc_dest_mutate_condition_flag: {
                              return "dest_mutate_condition_flag";
                        }

                        default: {
                              throw std::runtime_error("Unkown expr for expr string.");
                        }
                  }
            }

            namespace kinds_info {

                  namespace scope {

                        constexpr std::array start = {
                            element_kinds::stat_while,
                            element_kinds::stat_repeat,
                            element_kinds::stat_if,
                            element_kinds::stat_for_iv_start,
                            element_kinds::stat_for_start,
                            element_kinds::stat_for_n_start};

                        constexpr std::array end = {
                            element_kinds::stat_while_end,
                            element_kinds::stat_repeat_end,
                            element_kinds::stat_scope_end,
                            element_kinds::stat_for_iv_end,
                            element_kinds::stat_for_end,
                            element_kinds::stat_for_n_end};

                  } // namespace scope

                  constexpr std::array loop_exit = {
                      element_kinds::stat_loop_break,
                      element_kinds::stat_jump_break,
                      element_kinds::stat_repeat_end,
                      element_kinds::stat_while_end,
                      element_kinds::stat_for_end,
                      element_kinds::stat_for_n_end,
                      element_kinds::stat_for_iv_end,
                      element_kinds::desc_return,
                      element_kinds::desc_noreturn};

            } // namespace kinds_info

      } // namespace ast

} // namespace luramas