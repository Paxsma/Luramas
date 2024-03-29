/*
* framework/il/code/generation/common/generate/constant/branch.hpp
* 
* IL Code Generation Branches
* 10/7/2023
* 
* IL code generation branches.
*/
#pragma once

#ifndef luramas_code_generation_common_branches
#define luramas_code_generation_common_branches

#include "../expression/logical.hpp"
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace branch {

                        /* Emits if routine too refrence. */
                        __inline void emit_if(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_if(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_if(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Brach if statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits if routine too refrence (pre compiled compare). */
                        __inline void emit_if_pre(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_if_pre(buffer, compiled, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_if_pre(buffer, compiled, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Branch if pre statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits else if routine too refrence. */
                        __inline void emit_elseif(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_elseif(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_elseif(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Branch elseif statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits else if routine too refrence (pre compiled compare). */
                        __inline void emit_elseif_pre(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_elseif_pre(buffer, compiled, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_elseif_pre(buffer, compiled, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Branch elseif pre statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits else too refrence. */
                        __inline void emit_else(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_else(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_else(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Branch else statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits scope end too refrence. */
                        __inline void emit_scope_end(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::branch::emit_scope_end(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::branch::emit_scope_end(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Branch scope end statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace branch

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif