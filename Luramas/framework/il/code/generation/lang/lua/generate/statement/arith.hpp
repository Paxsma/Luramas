#pragma once
#include "../emit.hpp"
#include "../expression/arith.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace arith_statement {

                        /* Emits arith routine too refrence. */
                        __inline void emit_arith(luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds kind, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              luramas::ir::data::refrence_string ref("");

                              luramas::emitter_ir::lua::arith::emit_arith_operation(ref, kind, lvalue, rvalue, format);
                              buffer += format->spacing.format_parenthesis_ref(lua_langkeyword_parenthesis_open, ref, lua_langkeyword_parenthesis_close);

                              return;
                        }

                  } // namespace arith_statement

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas
