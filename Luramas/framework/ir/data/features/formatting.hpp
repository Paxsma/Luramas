#pragma once
#include "refrence_string.hpp"
#include <cstdint>
#include <string>

namespace luramas {

      namespace ir {

            namespace data {

                  namespace format {

                        struct format {

                              /* 
                  
                    * Amount of indent for each (Added too multiplier when hit). 
                    * 1 for normal 0 for nothing. 
                    
                  */
                              struct indent {

                                    bool disabled = false; /* All indents are disabled. */

                                    /* Generic indent */
                                    std::string indent = "   ";    /* Amount of indent per line e.g. a1 += 1; */
                                    std::size_t indent_start = 0u; /* Start on indent amount. */

                                    /* if */
                                    std::size_t indent_if_pre = 0u;  /* Indent on if statement. */
                                    std::size_t indent_if_post = 1u; /* Indent after if statement. */

                                    /* else if  */
                                    std::size_t indent_elseif_post = 1u; /* Indent after else if statement. */

                                    /* else */
                                    std::size_t indent_else_post = 1u; /* Indent after else statement. */

                                    /* loop */
                                    std::size_t indent_loop_pre = 0u;  /* Indent on loop statement. */
                                    std::size_t indent_loop_post = 1u; /* Indent after loop statement. */

                                    /* for */
                                    std::size_t indent_numeric_for_loop_pre = 0u;  /* Indent on numeric for loop statement. */
                                    std::size_t indent_numeric_for_loop_post = 1u; /* Indent after numeric for loop statement. */
                                    std::size_t indent_generic_for_loop_pre = 0u;  /* Indent on generic for loop statement. */
                                    std::size_t indent_generic_for_loop_post = 1u; /* Indent after generic for loop statement. */

                                    /* function */
                                    std::size_t indent_global_function_pre = 0u;     /* Indent on global function statement. */
                                    std::size_t indent_global_function_post = 1u;    /* Indent after global function statement. */
                                    std::size_t indent_scope_function_pre = 0u;      /* Indent on scope function statement. */
                                    std::size_t indent_scope_function_post = 1u;     /* Indent after scope function statement. */
                                    std::size_t indent_anonymous_function_pre = 0u;  /* Indent on anonymous function statement. */
                                    std::size_t indent_anonymous_function_post = 1u; /* Indent after anonymous function statement. */

                                    /* Comment */
                                    std::size_t indent_comment_expandable = 1u; /* Indent for comment. [Expandable] */
                                    std::size_t indent_comment = 1u;            /* Indent for comment. [NOT Expandable, Independent from preceeding indent] */

                                    /* 
                        
                             Indent (LUA):

                                (0u)
                             1:  foo = foo + bar;
                             2:  bar = 4;
                             3:  end
                             4: bar = foo;
                             5: bar_1 = foo_1;
                            
                                (1u)
                             1:  foo = foo + bar;
                             2:  bar = 4;
                             3: end
                             4: bar = foo;
                             5: bar_1 = foo_1;

                                (2u)
                             1:  foo = foo + bar;
                             2:  bar += 4;
                             3:  end
                             4:  bar += foo;
                             5:  bar_1 += foo_1;

                                (3u) **follows indent_end**
                             1:  foo = foo + bar;
                             2:  bar = 4;
                             3:    end
                             4:    bar = foo;
                             5:    bar_1 = foo_1;

                                (4u) **follows indent_end**
                             1: foo = foo + bar;
                             2: bar = 4;
                             3: end
                             4:    bar = foo;
                             5:    bar_1 = foo_1;

                             Indent (PYTHON):

                                (0u)
                             1:  foo += bar;
                             2:  bar = 4;
                             3: bar = foo;
                             4: bar_1 = foo_1;
                            
                                (1u)
                             1:  foo += bar;
                             2:  bar = 4;
                             3: bar = foo;
                             4: bar_1 = foo_1;

                                (2u)
                             1:  foo += bar;
                             2:  bar = 4;
                             3:  bar = foo;
                             4:  bar_1 = foo_1;

                                (3u) **follows indent_end**
                             1:  foo += bar;
                             2:  bar = 4;
                             3:    bar = foo;
                             4:    bar = foo_1;

                                (4u) **follows indent_end**
                             1:  foo += bar;
                             2:  bar = 4;
                             3:    bar = foo;
                             4:    bar = foo_1;
                        */
                                    std::uint8_t indent_format_scope_end = 1u; /* Scope end LUA:"end, until" | PYTHON:(SCOPE END) */

                                    std::uint8_t indent_end = 0u; /* this->"indent_format_end" must be set too 3 or 4. */

                                    /* Collapse and alligns.  */
                                    struct collapse {

                                          /* Collapse table and allign. */
                                          struct table {

                                                bool disabled = false; /* All collapses are disabled for table. */

                                                std::size_t collapse_table = 4u; /* If table size is greater than or equal too this value it will collapse and allign all table members. */

                                          } table;

                                          /* Collapse args and allign. */
                                          struct args {

                                                bool disabled = true; /* All collapses are disabled for args. */

                                                std::size_t collapse_arg = 4u; /* If arguement count is greater than or equal too this value it will collapse and allign all arguements. */

                                          } args;

                                          /* Collapse parameters and allign. */
                                          struct parameters {

                                                bool disabled = true; /* All collapses are disabled for parameters. */

                                                std::size_t collapse_params = 4u; /* If parameter count is greater than or equal too this value it will collapse and allign all arguements. */

                                          } parameters;

                                          /* Collapse loop iterations variables  and allign. */
                                          struct for_loop {

                                                bool disabled = true; /* All collapses are disabled for parameters. */

                                                std::size_t collapse_for_loop = 4u; /* If for loop variable count is greater than or equal too this value it will collapse and allign all variables. */

                                          } for_loop;

                                          struct comment {

                                                struct concat {

                                                      bool disabled = false; /* Disabled comment concat. */

                                                } concat;

                                                bool disabled = false; /* Disable comments from collapsing. */

                                          } comment;

                                    } collapse;

                              } indent;

                              struct spacing {

                                    bool disabled = false; /* All spacings are disabled. */

                                    /* Args */
                                    std::string spacing_arg_post = " "; /* Amount of arguement spacing post comma e.g. a1,??a2 */
                                    std::string spacing_arg_pre = "";   /* Amount of arguement spacing pre comma e.g. a1??,a2 */

                                    /* Params */
                                    std::string spacing_param_post = " ";            /* Amount of parameter spacing post comma e.g. (a1,??a2) */
                                    std::string spacing_param_pre = "";              /* Amount of parameter spacing pre comma e.g. (a1??,a2) */
                                    std::string spacing_param_parenthesis_post = ""; /* Amount of parameter parenthesis spacing post comma e.g. (a1,a2??) */
                                    std::string spacing_param_parenthesis_pre = "";  /* Amount of parameter parenthesis spacing pre comma e.g. (??a1,a2) */

                                    /* Table */
                                    std::string spacing_table_seperator_post = " "; /* Amount of table spacing post comma e.g. { a1,??a2 } */
                                    std::string spacing_table_seperator_pre = "";   /* Amount of table spacing pre comma e.g. { a1??,a2 } */
                                    std::string spacing_table_open = "";            /* Amount of table spacing post open e.g. {??a1 */
                                    std::string spacing_table_close = "";           /* Amount of table spacing pre close e.g. a2??} */
                                    std::string spacing_table_index_pre = "";       /* Amount of index spacing post index e.g. [??a1 */
                                    std::string spacing_table_index_post = "";      /* Amount of index spacing pre index e.g. a2??] */

                                    /* Index */
                                    std::string spacing_index_open_post = "";        /* Amount of index spacing post bracket e.g. a1[??1] */
                                    std::string spacing_index_open_pre = "";         /* Amount of index spacing pre bracket e.g. a1??[1] */
                                    std::string spacing_index_close_post = "";       /* Amount of index spacing post bracket e.g. a1[1??] */
                                    std::string spacing_index_close_pre = "";        /* Amount of index spacing pre bracket e.g. a1[1]?? */
                                    std::string spacing_index_assignment_post = " "; /* Amount of index equal spacing post equal e.g. a1=?? */
                                    std::string spacing_index_assignment_pre = " ";  /* Amount of index equal spacing pre equal e.g. a1??= */

                                    /* Arith */
                                    std::string spacing_arith_post = " "; /* Amount of arith operation spacing post operation e.g. a3 = a1+??a2; */
                                    std::string spacing_arith_pre = " ";  /* Amount of arith operation spacing pre operation e.g. a3 = a1??+a1 */

                                    /* Concat */
                                    std::string spacing_concat_post = " "; /* Amount of concat spacing post operation e.g. a1..??; */
                                    std::string spacing_concat_pre = " ";  /* Amount of concat spacing pre operation e.g. a2??.. */

                                    /* Logical operators */
                                    std::string spacing_logical_operator_post = " "; /* Amount of arith operation spacing post operation e.g. if(a1==??a2) */
                                    std::string spacing_logical_operator_pre = " ";  /* Amount of arith operation spacing pre operation e.g. if(a1??==a2) */

                                    /* Parenthesis */
                                    std::string spacing_parenthesis_open_post = "";  /* Amount of parenthesis spacing post open e.g. (??a1+a2) */
                                    std::string spacing_parenthesis_open_pre = "";   /* Amount of parenthesis spacing pre open e.g. ??(a1+a2) */
                                    std::string spacing_parenthesis_close_post = ""; /* Amount of parenthesis spacing post close e.g. (a1+a2)?? */
                                    std::string spacing_parenthesis_close_pre = "";  /* Amount of parenthesis spacing pre close e.g. (a1+a2??) */

                                    /* Assignment */
                                    std::string spacing_assign_post = " "; /* Amount of assign spacing post equal e.g. a1=??a2 */
                                    std::string spacing_assign_pre = " ";  /* Amount of assign spacing pre equal e.g. a1??=a2 */

                                    /* Loop */
                                    std::string spacing_forloop_generic_post = " "; /* Amount of for loop generic(LUA:"in") spacing post operation e.g. a1, a2 in??foo(bar) */
                                    std::string spacing_forloop_generic_pre = " ";  /* Amount of for loop generic(LUA:"in") spacing pre operation e.g. a1, a2??in foo(bar) */
                                    std::string spacing_forloop_numeric_post = " "; /* Amount of for loop numeric(LUA:"=") spacing post operation e.g. a1 =??1,2 */
                                    std::string spacing_forloop_numeric_pre = " ";  /* Amount of for loop numeric(LUA:"=") spacing pre operation e.g. a1??= 1,2 */
                                    std::string spacing_forloop_scope_start = " ";  /* Amount of for loop in scope start(LUA:"do") e.g. foo(bar)??do */
                                    std::string spacing_forloop = " ";              /* Amount of post loop statement spacing e.g. for??() */

                                    /* Constant */
                                    std::string spacing_compare = " "; /* Amount of pre compare statement spacing e.g. if??() */
                                    std::string spacing_then = " ";    /* Amount of pre then statement spacing e.g. if()??then */
                                    std::string spacing_call = "";     /* Amount of call spacing e.g. call??(); */

                                    /* Lua loop constants */
                                    std::string spacing_loop_until = " ";             /* Amount of post until statement spacing e.g. until??()/while??(); */
                                    std::string spacing_loop_while = " ";             /* Amount of post while statement spacing e.g. while??() */
                                    std::string spacing_loop_while_scope_start = " "; /* Amount of while scope start spacing e.g. ??do */

                                    /* Unary */
                                    std::string spacing_not = " ";   /* Amount of not spacing e.g. not??a1; */
                                    std::string spacing_length = ""; /* Amount of length spacing e.g. #??a1; */
                                    std::string spacing_minus = "";  /* Amount of minus spacing e.g. -??a1; */
                                    std::string spacing_bitnot = ""; /* Amount of minus spacing e.g. ~??a1; */

                                    /* ";" */
                                    std::string spacing_semicolon = ""; /* Amount of semicolon spacing e.g. a1 = a2??; */

                                    /* Assignment */
                                    std::string spacing_assignment_post = " "; /* Amount of assignment equal spacing post equal e.g. a1=?? */
                                    std::string spacing_assignment_pre = " ";  /* Amount of assignment equal spacing pre equal e.g. a1??= */

                                    std::string spacing_assignment_lvalue_post = " "; /* Amount of assignment lvalue spacing post equal e.g. a1,??a2 */
                                    std::string spacing_assignment_lvalue_pre = " ";  /* Amount of assignment lvalue spacing pre equal e.g. a1??,a2 */
                                    std::string spacing_assignment_rvalue_post = " "; /* Amount of assignment rvalue spacing post equal e.g. a1,??a2 */
                                    std::string spacing_assignment_rvalue_pre = " ";  /* Amount of assignment rvalue spacing pre equal e.g. a1??,a2 */

                                    /* Initial Assignment */
                                    std::string spacing_initial_assignment_post = " ";     /* Amount of initial assignment spacing post equal e.g. a1=?? */
                                    std::string spacing_initial_assignment_pre = " ";      /* Amount of initial assignment spacing pre equal e.g. a1??= */
                                    std::string spacing_initial_datatype_assignment = " "; /* Amount of initial datatype assignment equal spacing equal e.g. var_type ??a1 */

                                    std::string spacing_initial_assignment_lvalue_post = " "; /* Amount of initial assignment lvalue spacing post equal e.g. a1,??a2 */
                                    std::string spacing_initial_assignment_lvalue_pre = " ";  /* Amount of initial assignment lvalue spacing pre equal e.g. a1??,a2 */
                                    std::string spacing_initial_assignment_rvalue_post = " "; /* Amount of initial assignment rvalue spacing post equal e.g. a1,??a2 */
                                    std::string spacing_initial_assignment_rvalue_pre = " ";  /* Amount of initial assignment rvalue spacing pre equal e.g. a1??,a2 */

                                    /* Return */
                                    std::string spacing_return = " "; /* Amount of return statement spacing e.g. return??a1; (WILL ONLY FORMAT IF THERE IS OUTPUT) */

#pragma region spacing_keywords

#pragma region spacing_unary

                                    /* Formats not too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_unary_not(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_not);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats minus too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_unary_minus(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_minus);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats bit not too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_unary_bitnot(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_bitnot);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats length too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_unary_length(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_length);
                                          }

                                          return std::string(keyword);
                                    }

#pragma endregion

#pragma region spacing_parenthesis

                                    /* Formats open parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_parenthesis_open(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_parenthesis_open_pre + keyword + this->spacing_parenthesis_open_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats close parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_parenthesis_close(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_parenthesis_close_pre + keyword + this->spacing_parenthesis_close_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Turns string into parenthesis. **STRING NOT REFRENCE** */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_parenthesis_str(const char *const open, const std::string &str, const char *const close) {
                                          return this->format_parenthesis_open<igore>(open) + str + this->format_parenthesis_close<ignore>(close);
                                    }

                                    /* Turns string into parenthesis. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    luramas::ir::data::refrence_string format_parenthesis_ref(const char *const open, luramas::ir::data::refrence_string &str, const char *const close) {
                                          luramas::ir::data::refrence_string retn("");
                                          retn += this->format_parenthesis_open<ignore>(open);
                                          retn += str;
                                          retn += this->format_parenthesis_close<ignore>(close);
                                          return retn;
                                    }

#pragma endregion

#pragma region spacing_index

                                    /* Formats open parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_index_open(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_index_open_pre + keyword + this->spacing_index_open_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats close parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_index_close(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_index_close_pre + keyword + this->spacing_index_close_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Turns string into parenthesis. **STRING NOT REFRENCE** */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_index_str(const char *const open, const std::string &str, const char *const close) {
                                          return this->format_index_open<ignore>(open) + str + this->format_index_close<ignore>(close);
                                    }

                                    /* Formats indent refrence too buffer. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    luramas::ir::data::refrence_string format_index_ref(const char *const open, luramas::ir::data::refrence_string &str, const char *const close) {
                                          luramas::ir::data::refrence_string retn("");
                                          retn += this->format_index_open<ignore>(open);
                                          retn += str;
                                          retn += this->format_index_close<ignore>(close);
                                          return retn;
                                    }

#pragma endregion

#pragma region paramater_index

                                    /* Formats open parameter parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_call_parameter_open(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + spacing_param_parenthesis_pre);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats close parameter parenthesis too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_call_parameter_close(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_param_parenthesis_post + keyword);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Adds spacing too format call for call routine. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    void format_call_spacing(luramas::ir::data::refrence_string &buffer) {

                                          if (!this->disabled && !ignore) {
                                                buffer += this->spacing_call;
                                          }

                                          return;
                                    }

#pragma endregion

#pragma region table

                                    /* Formats table seperator too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_table_seperator(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_table_seperator_pre + keyword + this->spacing_table_seperator_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats table start too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_table_start(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_table_open);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats table start too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_table_end(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_table_close + keyword);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats table start too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    luramas::ir::data::refrence_string format_table_index(const char *const open, luramas::ir::data::refrence_string &str, const char *const close, const char *const equal) {

                                          luramas::ir::data::refrence_string retn = open;

                                          if (!this->disabled && !ignore) {
                                                retn += this->spacing_table_index_pre;
                                          }

                                          retn += str;

                                          if (!this->disabled && !ignore) {
                                                retn += this->spacing_table_index_post;
                                          }

                                          retn += close;

                                          if (!this->disabled && !ignore) {
                                                retn += this->spacing_assignment_pre;
                                          }

                                          retn += equal;

                                          if (!this->disabled && !ignore) {
                                                retn += this->spacing_assignment_post;
                                          }

                                          return retn;
                                    }

                                    /* Formats table close too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_table_close(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_table_close + keyword);
                                          }

                                          return std::string(keyword);
                                    }

#pragma endregion

                                    /* Formats arith too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_arith(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_arith_pre + keyword + this->spacing_arith_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats logical operation too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_logical_operation(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_logical_operator_pre + keyword + this->spacing_logical_operator_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats paramter seperator too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_parameter_seperator(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_param_pre + keyword + this->spacing_param_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats concat too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_concat(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_concat_pre + keyword + this->spacing_concat_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats compare too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_compare(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_compare);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats while too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_while(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_loop_while);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats until too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_until(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_loop_until);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats return too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_return(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_return);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats while scope start LUA:"do" too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_while_scope_start(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_loop_while_scope_start + keyword);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats scope begin too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_scope_begin(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_then + keyword);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats compare too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_loop_begin(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_compare);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats semicolon too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_semicolon(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_semicolon + keyword);
                                          }

                                          return std::string(keyword);
                                    }

#pragma region assignment

                                    /* Formats assignment too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_assignment_pre + keyword + this->spacing_assignment_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats assignment lvalue too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_lvalue(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_assignment_lvalue_pre + keyword + this->spacing_assignment_lvalue_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats assignment rvalue too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_rvalue(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_assignment_rvalue_pre + keyword + this->spacing_assignment_rvalue_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats initial assignment too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_initial(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_initial_assignment_pre + keyword + this->spacing_initial_assignment_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats initial assignment lvalue too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_initial_lvalue(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_initial_assignment_lvalue_pre + keyword + this->spacing_initial_assignment_lvalue_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats initial assignment rvalue too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_initial_rvalue(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(this->spacing_initial_assignment_rvalue_pre + keyword + this->spacing_initial_assignment_rvalue_post);
                                          }

                                          return std::string(keyword);
                                    }

                                    /* Formats initial assignment too new string. */
                                    template <bool ignore = false /* Ignores spacing */>
                                    std::string format_assignment_initial_datatype(const char *const keyword) {

                                          if (!this->disabled && !ignore) {
                                                return std::string(keyword + this->spacing_initial_datatype_assignment);
                                          }

                                          return std::string(keyword);
                                    }

#pragma endregion

#pragma endregion Space keywords

                              } spacing;

                              /* All smart formatting can and will be influenced by other members in struct. */
                              struct smart_formatting {

                                    bool disabled = false; /* All smart formattings are disabled. */

                                    struct settable {

                                          bool disabled = false; /* All smart settable formattings are disabled. */

                                          /*
                                Group types that follow logic:

                                (Follows group_newlines)

                                (0): Disabled
                                (1):
                                    call();
                                                    <<<-- group_newlines
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines
                                    call();
                                    v2[2] = 3;
                                    v2[2] = 3;
                                                    <<<-- group_newlines
                                    v3[2] = 3;
                                    v5[2] = 3;
                                                    <<<-- group_newlines
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                                (2):
                                    call();
                                    local v1 = make_table();
                                                    <<<-- group_newlines
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines
                                    v2[2] = 3;
                                    v2[2] = 3;
                                                    <<<-- group_newlines
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                                    <<<-- group_newlines
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                                (3):
                                    call();
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines
                                    v2[2] = 3;
                                    v2[2] = 3;
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                    call();
                                (4):
                                    call();
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                    v2[2] = 3;
                                    v2[2] = 3;
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                                (5):
                                    call();
                                                    <<<-- group_newlines
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines
                                    v2[2] = 3;
                                    v2[2] = 3;
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                    call();
                                (6):
                                    call();
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                    v2[2] = 3;
                                    v2[2] = 3;
                                    v3[2] = 3;
                                    v5[2] = 3;
                                                    <<<-- group_newlines
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                                (7):
                                    call();
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines                               
                                    v2[2] = 3;
                                    v2[2] = 3;
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                                (8):
                                    call();
                                    local v1 = make_table();
                                    v1[2] = 2;
                                    v1[2] = 2;
                                                    <<<-- group_newlines                               
                                    v2[2] = 3;
                                    v2[2] = 3;
                                                    <<<-- group_newlines      
                                    v3[2] = 3;
                                    v5[2] = 3;
                                    v7 = table;
                                    v7[2] = 3;
                                    v7[3] = 3;
                                                    <<<-- group_newlines
                                    call();
                           */
                                          std::uint8_t group = 0u;
                                          std::uint8_t group_newlines = 1u; /* Amount of new lines for group format. */

                                          /*
                               "?? >= min_threshold"  Minimum threshold too indent. 0 = Nothing
                           */
                                          std::uint8_t min_threshold = 2u;

                                          /*
                               "?? <= max_threshold"  Maximum threshold too indent. 0 = Nothing
                           */
                                          std::uint8_t max_threshold = 0u;

                                    } settable;

                                    struct assignment {

                                          struct selfcall {

                                                /* Add function and fix data in table. */
                                                bool disabled = false; /* All smart selfcall formattings are disabled. */

                                                struct table {
                                                      /*
                           
                                        (Follows group_newlines)

                                        0: Disabled
                                        1:
                                            call();
                                                                                         <<<-- group_newlines
                                            local v1 = make:a().ab:a();
                                            local v2 = make:b().ac:ab();
                                            local v4 = make:b().ac:ab();
                                            local v5 = make:c().ad:abc();
                                            make:a().ac:ab();
                                            make:a().ac:ab();
                                                                                         <<<-- group_newlines
                                            call();
                                                                                         <<<-- group_newlines
                                            make:d();
                                            make:e();   
                                                                                         <<<-- group_newlines
                                            call();
                                            local v6 = 10 + (make:a() + 10) / 20;
                                            local v7 = 10 + (make:a() + 10) / 20;
                                            call();
                                                                                         <<<-- group_newlines
                                            local v8 = a[(70 + (make:a()[2][3][4][5]) % 2]:a();
                                            local v9 = a[(70 + (make:a()[2][3][4][5]) % 2]:a();
                                            a[(70 + (make:a()[2][3][4][5]) % 2]:a();
                                            a[(70 + (make:a()[2][3][4][5]) % 2]:a();
                                                                                         <<<-- group_newlines
                                            print(2);
                                        2:
                                            call();
                                            local v1 = make_table();
                                                            <<<-- group_newlines
                                            v1[2] = 2;
                                            v1[2] = 2;
                                                            <<<-- group_newlines
                                            v2[2] = 3;
                                            v2[2] = 3;
                                                            <<<-- group_newlines
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                                            <<<-- group_newlines
                                            v7[2] = 3;
                                            v7[3] = 3;
                                                            <<<-- group_newlines
                                            call();
                                        3:
                                            call();
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                                            <<<-- group_newlines
                                            v2[2] = 3;
                                            v2[2] = 3;
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                            call();
                                        4:
                                            call();
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                            v2[2] = 3;
                                            v2[2] = 3;
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                                            <<<-- group_newlines
                                            call();
                                        5:
                                            call();
                                                            <<<-- group_newlines
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                                            <<<-- group_newlines
                                            v2[2] = 3;
                                            v2[2] = 3;
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                            call();
                                        6:
                                            call();
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                            v2[2] = 3;
                                            v2[2] = 3;
                                            v3[2] = 3;
                                            v5[2] = 3;
                                                            <<<-- group_newlines
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                                            <<<-- group_newlines
                                            call();
                                        7:
                                            call();
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                                            <<<-- group_newlines                               
                                            v2[2] = 3;
                                            v2[2] = 3;
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                                            <<<-- group_newlines
                                            call();
                                        8:
                                            call();
                                            local v1 = make_table();
                                            v1[2] = 2;
                                            v1[2] = 2;
                                                            <<<-- group_newlines                               
                                            v2[2] = 3;
                                            v2[2] = 3;
                                                            <<<-- group_newlines      
                                            v3[2] = 3;
                                            v5[2] = 3;
                                            v7 = table;
                                            v7[2] = 3;
                                            v7[3] = 3;
                                                            <<<-- group_newlines
                                            call();
                                     */
                                                      std::uint8_t group = 0u;
                                                      std::uint8_t group_newlines = 1u; /* Amount of new lines for group format. */
                                                } table;

                                          } selfcall;
                                    };

                              } smart_format;

                              struct comments {

                                    bool disabled = false;         /* Singular comments get singular comment while others get expandable comment. Does not disable expanable comment. */
                                    bool disable_comments = false; /* Disabled all comments from getting emitted. (Overrides every comment setting) */

                              } comments;

                              struct linebreak {

                                    bool disable = false;                    /* Disables all line breaks. */
                                    std::size_t start_linebreak_amount = 1u; /* Amount of linebreaks start per line. */

                                    /* Scoped expandable comments. */
                                    std::size_t linebreak_expandable_comment_pre = 1u;  /* Amount of linebreaks for expandable comments pre e.g. --[[?? */
                                    std::size_t linebreak_expandable_comment_post = 1u; /* Amount of linebreaks for expandable comments pre e.g. ??]] */

                                    /* if */
                                    std::size_t linebreak_if_pre = 0u;  /* Amount of linebreaks on if statement. */
                                    std::size_t linebreak_if_post = 0u; /* Amount of linebreaks after if statement. */

                                    /* else if  */
                                    std::size_t linebreak_elseif_post = 0u; /* Amount of linebreaks after else if statement. */

                                    /* else */
                                    std::size_t linebreak_else_post = 0u; /* Amount of linebreaks after else statement. */

                                    /* loop */
                                    std::size_t linebreak_loop_pre = 0u;  /* Amount of linebreaks on loop statement. */
                                    std::size_t linebreak_loop_post = 0u; /* Amount of linebreaks after loop statement. */

                                    /* for */
                                    std::size_t linebreak_numeric_for_loop_pre = 0u;  /* Amount of linebreaks on numeric for loop statement. */
                                    std::size_t linebreak_numeric_for_loop_post = 0u; /* Amount of linebreaks after numeric for loop statement. */
                                    std::size_t linebreak_generic_for_loop_pre = 0u;  /* Amount of linebreaks on generic for loop statement. */
                                    std::size_t linebreak_generic_for_loop_post = 0u; /* Amount of linebreaks after generic for loop statement. */

                                    /* function */
                                    std::size_t linebreak_global_function_pre = 0u;     /* Amount of linebreaks on global function statement. */
                                    std::size_t linebreak_global_function_post = 0u;    /* Amount of linebreaks after global function statement. */
                                    std::size_t linebreak_scope_function_pre = 0u;      /* Amount of linebreaks on scope function statement. */
                                    std::size_t linebreak_scope_function_post = 0u;     /* Amount of linebreaks after scope function statement. */
                                    std::size_t linebreak_anonymous_function_pre = 0u;  /* Amount of linebreaks on anonymous function statement. */
                                    std::size_t linebreak_anonymous_function_post = 0u; /* Amount of linebreaks after anonymous function statement. */

                              } linebreak;
                        };

                  } // namespace format

            } // namespace data

      } // namespace ir

} // namespace luramas