/*
* framework/il/code/generation/common/generate/constant/for.hpp
* 
* IL Code Generation For Loops
* 10/7/2023
* 
* IL code generation for loops.
*/
#pragma once

#ifndef luramas_code_generation_common_for
#define luramas_code_generation_common_for

#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace for_ {

                        /* Emits return too refrence. */
                        template <bool numeric = false /* Numeric(true) or Generic(false) */>
                        __inline void emit_for(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, std::vector<luramas::ir::data::refrence_string> &vars, std::vector<luramas::ir::data::refrence_string> &iterator, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::for_::emit_for<numeric>(buffer, vars, iterator, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::for_::emit_for<numeric>(buffer, vars, iterator, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("For statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace for_

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif