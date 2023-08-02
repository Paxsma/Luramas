#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace arith {

                        /* Emits arith operation too refrence. */
                        __inline void emit_arith_operation(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds kind, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::arith::emit_arith_operation(buffer, kind, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::arith::emit_arith_operation(buffer, kind, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Arith operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace arith

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas