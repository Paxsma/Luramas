/*
* framework/il/code/generation/common/generate/support.hpp
* 
* IL Code Generation Support
* 10/7/2023
* 
* IL code generation keyword support.
*/
#pragma once

#ifndef luramas_code_generation_common_support
#define luramas_code_generation_common_support

#include "generate.hpp"
#include <array>

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  __inline bool supported_arith_assignment(const luramas::emitter_ir::syntax::emitter_syntax syn, const ast::bin_kinds kind) {

                        switch (syn) {

                              case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                    return (!luramas::emitter_ir::lua::supported_arith_assignment.empty() && std::find(luramas::emitter_ir::lua::supported_arith_assignment.begin(), luramas::emitter_ir::lua::supported_arith_assignment.end(), kind) != luramas::emitter_ir::lua::supported_arith_assignment.end());
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                    return (!luramas::emitter_ir::luau::supported_arith_assignment.empty() && std::find(luramas::emitter_ir::luau::supported_arith_assignment.begin(), luramas::emitter_ir::luau::supported_arith_assignment.end(), kind) != luramas::emitter_ir::luau::supported_arith_assignment.end());
                              }

                              default: {
                                    break;
                              }
                        }

                        return false;
                  }

                  /* Ternarys are supported? */
                  constexpr bool supported_ternarys = false;

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif