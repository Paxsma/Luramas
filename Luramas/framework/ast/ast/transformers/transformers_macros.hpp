#pragma once

#define node_nonmutable(node) node->has_expr(luramas::ast::element_kinds::condition_nonmutable)

#define routine_inc(node, routine) routine += node->count_expr<luramas::ast::element_kinds::concat_routine_start>() + node->count_expr<luramas::ast::element_kinds::call_routine_start>() + node->count_expr<luramas::ast::element_kinds::table_start>() + node->count_expr<luramas::ast::element_kinds::condition_concat_start>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_start>()
#define routine_dec(node, routine) routine -= node->count_expr<luramas::ast::element_kinds::concat_routine_end>() + node->count_expr<luramas::ast::element_kinds::call_routine_end>() + node->count_expr<luramas::ast::element_kinds::table_end>() + node->count_expr<luramas::ast::element_kinds::condition_concat_end>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_end>()

/* Same thing as routines(inc/dec) but conditional concat routines gets ignored because they don't garunteed a locvar. */
#define routine_inc_lv(node, routine) routine += node->count_expr<luramas::ast::element_kinds::concat_routine_start>() + node->count_expr<luramas::ast::element_kinds::call_routine_start>() + node->count_expr<luramas::ast::element_kinds::table_start>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_start>()
#define routine_dec_lv(node, routine) routine -= node->count_expr<luramas::ast::element_kinds::concat_routine_end>() + node->count_expr<luramas::ast::element_kinds::call_routine_end>() + node->count_expr<luramas::ast::element_kinds::table_end>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_end>()

/* Same thing as routines(inc/dec) but it is very basic. */
#define routine_inc_basic(node, routine) routine += node->count_expr<luramas::ast::element_kinds::concat_routine_start>() + node->count_expr<luramas::ast::element_kinds::call_routine_start>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_start>()
#define routine_dec_basic(node, routine) routine -= node->count_expr<luramas::ast::element_kinds::concat_routine_end>() + node->count_expr<luramas::ast::element_kinds::call_routine_end>() + node->count_expr<luramas::ast::element_kinds::conditional_expression_end>()

/* Same thing as routines(inc/dec) but conditional concat/expression routines gets ignored because they don't garunteed routine. */
#define routine_inc_safe(node, routine) routine += node->count_expr<luramas::ast::element_kinds::concat_routine_start>() + node->count_expr<luramas::ast::element_kinds::call_routine_start>() + node->count_expr<luramas::ast::element_kinds::table_start>()
#define routine_dec_safe(node, routine) routine -= node->count_expr<luramas::ast::element_kinds::concat_routine_end>() + node->count_expr<luramas::ast::element_kinds::call_routine_end>() + node->count_expr<luramas::ast::element_kinds::table_end>()

/* See if there is no dest (Includes fastcall) */
#define no_dest(node) !node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()

/* Has generic condition break? */
#define condition_break(node) (node->has_expr(luramas::ast::element_kinds::break_) ||          \
                               node->has_expr(luramas::ast::element_kinds::condition_break) || \
                               node->lex->kind == luramas::il::lexer::inst_kinds::return_ ||   \
                               node->has_expr(luramas::ast::element_kinds::until_) ||          \
                               node->has_expr(luramas::ast::element_kinds::while_end) ||       \
                               node->has_expr(luramas::ast::element_kinds::for_end) ||         \
                               node->has_expr(luramas::ast::element_kinds::for_n_end) ||       \
                               node->has_expr(luramas::ast::element_kinds::for_iv_end))

/* Has condition break out? */
#define condition_break_out(node) (node->has_expr(luramas::ast::element_kinds::until_) ||    \
                                   node->has_expr(luramas::ast::element_kinds::while_end) || \
                                   node->has_expr(luramas::ast::element_kinds::for_end) ||   \
                                   node->has_expr(luramas::ast::element_kinds::for_n_end) || \
                                   node->has_expr(luramas::ast::element_kinds::for_iv_end))
