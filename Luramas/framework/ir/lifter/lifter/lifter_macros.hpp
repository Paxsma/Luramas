#pragma once
#include "lifter.hpp"

#define debug_name "IR-LIFTER-DEBUG"

#pragma region strings

/* Macros */
#define char_valid(ch) ((ch >= 0x30 /* '0' */ && ch <= 0x39 /* '9' */) || (ch >= 0x41 /* 'A' */ && ch <= 0x5A /* 'Z' */) || (ch >= 0x61 /* 'a' */ && ch <= 0x7A /* 'z' */) || ch == 0x5F /* _ */)

/* Sees if idx needs string type idx. */
#define str_idx(idx) std::isdigit(idx.front()) || std::find_if(idx.begin(), idx.end(), [](const char c) { return !char_valid(c); }) != idx.end()

/* Config has char suffix? */
#define config_char(config) config->upvalue_suffix_char || config->arg_suffix_char || config->function_suffix_char || config->iteration_suffix_char || config->var_suffix_char

#pragma endregion String macros

#pragma region code

/* Appends source too compare flag or sets compare flag for source. */
#define logical_expression_dest(node, regs, data)                                          \
      {                                                                                    \
                                                                                           \
            /* Append source too compare flag. */                                          \
            if (node->has_expr(luramas::ast::element_kinds::condition_append_source)) {    \
                                                                                           \
                  regs.back()[flag_compare]->set_rvalue(data);                             \
            }                                                                              \
                                                                                           \
            /* Source = compare flag */                                                    \
            if (node->has_expr(luramas::ast::element_kinds::conditional_expression_end)) { \
                                                                                           \
                  data = regs.back()[flag_compare]->data;                                  \
                  regs.back()[flag_compare]->clear(true);                                  \
            }                                                                              \
      };

#pragma endregion Code used often by lifter