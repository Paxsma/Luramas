/*
* framework/il/code/generation/common/generate/statement/arith.hpp
* 
* IL Code Generation Arithmetic Statements
* 10/7/2023
* 
* IL code generation arithmetic statement generation.
*/
#pragma once

#ifndef luramas_code_generation_common_statement_arith
#define luramas_code_generation_common_statement_arith

#include "../expression/arith.hpp"
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace arith_statement {

                        /* Emits arith routine too refrence. */
                        __inline void emit_arith(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds kind, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::arith_statement::emit_arith(buffer, kind, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::arith_statement::emit_arith(buffer, kind, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Arithmetic statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace arith_statement

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif