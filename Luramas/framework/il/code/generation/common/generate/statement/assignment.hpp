/*
* framework/il/code/generation/common/generate/statement/assignment.hpp
* 
* IL Code Generation Assignment Statements
* 10/7/2023
* 
* IL code generation assignment statements.
*/
#pragma once

#ifndef luramas_code_generation_common_statement_assignment
#define luramas_code_generation_common_statement_assignment

#include "../../../../../../ast/ast/kinds.hpp"
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace assignment {

                        /* Emits dynamic initial assignment too refrence. */
                        __inline void initial_assignment(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::assignment::initial_assignment(buffer, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::assignment::initial_assignment(buffer, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Initial dynamic assignment statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits dynamic assignment too refrence. */
                        __inline void assignment(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const char *const assignment = NULL /* Default syntax specific. */) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {

                                          if (assignment != NULL) {
                                                luramas::emitter_ir::lua::assignment::assignment(buffer, lvalue, rvalue, format, assignment);
                                          } else {
                                                luramas::emitter_ir::lua::assignment::assignment(buffer, lvalue, rvalue, format);
                                          }

                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {

                                          if (assignment != NULL) {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, assignment);
                                          } else {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format);
                                          }

                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Assignment statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        namespace arith {

                              /* Emits arith assignment too refrence. */
                              __inline void emit_arith_assignment(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const luramas::ast::bin_kinds op, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                                    switch (syn) {

                                          case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                                luramas::emitter_ir::luau::assignment::arith::emit_arith_assignment(buffer, op, lvalue, rvalue, format);
                                                break;
                                          }

                                          default: {
                                                throw std::runtime_error("Arithmetic assignment statement emissions are not supported by current target syntax.");
                                          }
                                    }

                                    return;
                              }

                        } // namespace arith

                  } // namespace assignment

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif