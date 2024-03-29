#pragma once
#include "../emit.hpp"
#include "../expression/logical.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace branch {

                        /* Emits if routine too refrence. */
                        __inline void emit_if(luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(luau_langkeyword_scope_if);

                              luramas::ir::data::refrence_string ref("");
                              luramas::emitter_ir::luau::logical::emit_logical_operation(ref, op, lvalue, rvalue, format);

                              buffer += format->spacing.format_parenthesis_ref(luau_langkeyword_parenthesis_open, ref, luau_langkeyword_parenthesis_close);
                              buffer += format->spacing.format_scope_begin(luau_langkeyword_scope_if_begin);

                              return;
                        }

                        /* Emits if routine too refrence (pre compiled compare). */
                        __inline void emit_if_pre(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(luau_langkeyword_scope_if);
                              buffer += compiled;
                              buffer += format->spacing.format_scope_begin(luau_langkeyword_scope_if_begin);

                              return;
                        }

                        /* Emits else if routine too refrence. */
                        __inline void emit_elseif(luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(luau_langkeyword_scope_elseif);

                              luramas::ir::data::refrence_string ref("");
                              luramas::emitter_ir::luau::logical::emit_logical_operation(ref, op, lvalue, rvalue, format);

                              buffer += format->spacing.format_parenthesis_ref(luau_langkeyword_parenthesis_open, ref, luau_langkeyword_parenthesis_close);
                              buffer += format->spacing.format_scope_begin(luau_langkeyword_scope_elseif_begin);

                              return;
                        }

                        /* Emits else if routine too refrence (pre compiled compare). */
                        __inline void emit_elseif_pre(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(luau_langkeyword_scope_elseif);
                              buffer += compiled;
                              buffer += format->spacing.format_scope_begin(luau_langkeyword_scope_elseif_begin);

                              return;
                        }

                        /* Emits else too refrence. */
                        __inline void emit_else(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(luau_langkeyword_scope_else);
                              buffer += format->spacing.format_scope_begin(luau_langkeyword_scope_else_begin);

                              return;
                        }

                        /* Emits scope end too refrence. */
                        __inline void emit_scope_end(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += luau_langkeyword_scope_end;
                              return;
                        }

                  } // namespace branch

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas