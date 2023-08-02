#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace selfcall {

                        /* Emits selfcall too refrence. */
                        __inline void emit_selfcall(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::selfcall::emit_selfcall(buffer, mark, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::selfcall::emit_selfcall(buffer, mark, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Selfcall statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace selfcall

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas