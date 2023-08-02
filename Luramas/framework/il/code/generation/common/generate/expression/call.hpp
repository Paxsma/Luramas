#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace call {

                        /* Emits call too refrence. */
                        __inline void emit_call(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::refrence_string &call, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::call::emit_call(buffer, call, vect, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::call::emit_call(buffer, call, vect, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Call operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace call

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas