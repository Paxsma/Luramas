#pragma once

/* Unary */
#define lua_langkeyword_not "not"
#define lua_langkeyword_minus "-"
#define lua_langkeyword_lenght "#"
#define lua_langkeyword_bitnot "~"

/* Arith */
#define lua_langkeyword_arith_add "+"
#define lua_langkeyword_arith_sub "-"
#define lua_langkeyword_arith_pow "^"
#define lua_langkeyword_arith_mul "*"
#define lua_langkeyword_arith_div "/"
#define lua_langkeyword_arith_and "&"
#define lua_langkeyword_arith_or "|"
#define lua_langkeyword_arith_mod "%"
#define lua_langkeyword_arith_shl "<<"
#define lua_langkeyword_arith_shr ">>"
#define lua_langkeyword_arith_idiv "//"
#define lua_langkeyword_arith_xor "~"

/* Compare */
#define lua_langkeyword_compare_lessthan "<"
#define lua_langkeyword_compare_lessthanequal "<="
#define lua_langkeyword_compare_equal "=="
#define lua_langkeyword_compare_notequal "~="
#define lua_langkeyword_compare_greaterthan ">"
#define lua_langkeyword_compare_greaterthanequal ">="
#define lua_langkeyword_compare_and "and"
#define lua_langkeyword_compare_or "or"

/* Parenthesis */
#define lua_langkeyword_parenthesis_open "("
#define lua_langkeyword_parenthesis_close ")"

/* Index */
#define lua_langkeyword_index_open "["
#define lua_langkeyword_index_close "]"
#define lua_langkeyword_index_generic "."

/* Keyword */
#define lua_langkeyword_nil "nil"
#define lua_langkeyword_true "true"
#define lua_langkeyword_false "false"
#define lua_langkeyword_vararg "..."
#define lua_langkeyword_nothing ""

/* Selfcall */
#define lua_langkeyword_selfcall ":"

/* Call */
#define lua_langkeyword_call_parenthesis_open "("
#define lua_langkeyword_call_parenthesis_close ")"
#define lua_langkeyword_call_seperator ","

/* Return */
#define lua_langkeyword_return "return"
#define lua_langkeyword_return_seprator ","

/* Concat */
#define lua_langkeyword_concat ".."

/* Loop */

/* do */
#define lua_langkeyword_loop_do "do"
#define lua_langkeyword_loop_do_begin ""

/* while */
#define lua_langkeyword_loop_while "while"
#define lua_langkeyword_loop_while_begin ""

/* Repeat */
#define lua_langkeyword_loop_repeat "repeat"
#define lua_langkeyword_loop_repeat_begin ""

/* until, break, continue */
#define lua_langkeyword_loop_until "until"
#define lua_langkeyword_loop_break "break"
#define lua_langkeyword_loop_continue "continue"

/* Scope */

/* if */
#define lua_langkeyword_scope_if "if"
#define lua_langkeyword_scope_if_begin "then"

/* elseif */
#define lua_langkeyword_scope_elseif "elseif"
#define lua_langkeyword_scope_elseif_begin "then"

/* else */
#define lua_langkeyword_scope_else "else"
#define lua_langkeyword_scope_else_begin ""

/* end */
#define lua_langkeyword_scope_end "end"

/* Table */
#define lua_langkeyword_table_start "{"
#define lua_langkeyword_table_end "}"
#define lua_langkeyword_table_seperator ","
#define lua_langkeyword_table_index_start "["
#define lua_langkeyword_table_index_end "]"
#define lua_langkeyword_table_index_assignment "="

/* Comment */
#define lua_langkeyword_comment "--"
#define lua_langkeyword_comment_expandable_start "--[["
#define lua_langkeyword_comment_expandable_end "]]--"

/* for */
#define lua_langkeyword_for "for"
#define lua_langkeyword_for_generic "in"
#define lua_langkeyword_for_numeric "="
#define lua_langkeyword_for_scope_start "do"
#define lua_langkeyword_for_seperator ","

/* String */
#define lua_langkeyword_string_start '\"'
#define lua_langkeyword_string_end '\"'

/* Line */
#define lua_langkeyword_line_break '\n'
#define lua_langkeyword_semicolon ";"

/* Assignment */
#define lua_langkeyword_assignment "="
#define lua_langkeyword_list_rvalue ","
#define lua_langkeyword_list_lvalue ","

/* function */
#define lua_langkeyword_function_global "function"
#define lua_langkeyword_function_global_scope_begin ""
#define lua_langkeyword_function_global_scope_end "end"

#define lua_langkeyword_function_scope_variable "local"
#define lua_langkeyword_function_scope_datatype "function"
#define lua_langkeyword_function_scope_begin ""
#define lua_langkeyword_function_scope_end "end"

#define lua_langkeyword_function_anonymous "(function"
#define lua_langkeyword_function_args_parenthesis_open "("
#define lua_langkeyword_function_args_parenthesis_close ")"

/* Variable datatype */
#define lua_langkeyword_dynamic_datatype "local"
