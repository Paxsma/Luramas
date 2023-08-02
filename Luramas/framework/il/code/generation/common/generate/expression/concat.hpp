#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace concat {

                        /* Emits concat too refrence. */
                        __inline void emit_concat(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::concat::emit_concat(buffer, vect, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::concat::emit_concat(buffer, vect, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Concat emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace concat

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas