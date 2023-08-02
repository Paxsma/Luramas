/*
* framework/il/code/generation/common/generate/constant/return.hpp
* 
* IL Code Generation Return
* 10/7/2023
* 
* IL code generation return.
*/
#pragma once

#ifndef luramas_code_generation_common_return
#define luramas_code_generation_common_return

#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace return_ {

                        /* Emits return too refrence. */
                        __inline void emit_return(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::return_::emit_return(buffer, vect, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::return_::emit_return(buffer, vect, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Return statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace return_

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif