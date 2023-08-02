#pragma once
#include "../emit.hpp"
#include "../expression/logical.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace loop {

                        /* Emits do routine too refrence. */
                        __inline void emit_do(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += format->spacing.format_while_scope_start(lua_langkeyword_loop_do);
                              buffer += lua_langkeyword_loop_do_begin;
                              return;
                        }

                        /* Emits repeat routine too refrence. */
                        __inline void emit_repeat(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += lua_langkeyword_loop_repeat;
                              buffer += lua_langkeyword_loop_repeat_begin;
                              return;
                        }

                        /* Emits loop break too refrence. */
                        __inline void emit_loop_break(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += lua_langkeyword_loop_break;
                              return;
                        }

                        /* Emits loop continue too refrence. */
                        __inline void emit_loop_continue(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += lua_langkeyword_loop_continue;
                              return;
                        }

                        __inline void emit_while(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_compare(lua_langkeyword_loop_while);

                              luramas::ir::data::refrence_string ref("");
                              luramas::emitter_ir::lua::logical::emit_logical_operation(ref, op, lvalue, rvalue, format);

                              buffer += format->spacing.format_parenthesis_ref(lua_langkeyword_parenthesis_open, ref, lua_langkeyword_parenthesis_close);
                              buffer += format->spacing.format_scope_begin(lua_langkeyword_loop_while_begin);

                              return;
                        }

                        /* Emits while routine too refrence (pre compiled compare). */
                        __inline void emit_while_pre(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_while(lua_langkeyword_loop_while);
                              buffer += compiled;

                              luramas::emitter_ir::lua::loop::emit_do(buffer, format);

                              buffer += format->spacing.format_scope_begin(lua_langkeyword_loop_while_begin);

                              return;
                        }

                        /* Emits until routine too refrence. */
                        __inline void emit_until(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_until(lua_langkeyword_loop_until);

                              luramas::ir::data::refrence_string ref("");
                              luramas::emitter_ir::lua::logical::emit_logical_operation(ref, op, lvalue, rvalue, format);
                              buffer += format->spacing.format_parenthesis_ref(lua_langkeyword_parenthesis_open, ref, lua_langkeyword_parenthesis_close);

                              return;
                        }

                        /* Emits until routine too refrence (pre compiled compare). */
                        __inline void emit_until_pre(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += format->spacing.format_until(lua_langkeyword_loop_until);
                              buffer += compiled;

                              return;
                        }

                  } // namespace loop

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas