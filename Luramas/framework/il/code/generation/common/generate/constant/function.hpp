/*
* framework/il/code/generation/common/generate/constant/function.hpp
* 
* IL Code Generation Functions
* 10/7/2023
* 
* IL code generation functions.
*/
#pragma once

#ifndef luramas_code_generation_common_function
#define luramas_code_generation_common_function

#include "../generate.hpp"
#include "../supported.hpp"
#include <vector>

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace function {

                        namespace virtual_functions {

                              __inline void emit_virtual_function(const luramas::emitter_ir::syntax::emitter_syntax syn, const luramas::il::arch::virtual_functions vt, const std::vector<luramas::ir::data::refrence_string> &args) {

                                    switch (syn) {

                                          case luramas::emitter_ir::syntax::emitter_syntax::python: {
                                                break;
                                          }

                                          default: {
                                                throw std::runtime_error("Virtual function statement emissions are not supported by current target syntax.");
                                          }
                                    }

                                    return;
                              }

                        } // namespace virtual_functions

                  } // namespace function

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif