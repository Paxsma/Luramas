/*
* framework/il/code/generation/common/generate/constant/loop.hpp
* 
* IL Code Generation Loops
* 10/7/2023
* 
* IL code generation loops.
*/
#pragma once

#ifndef luramas_code_generation_common_loop
#define luramas_code_generation_common_loop

#include "../expression/logical.hpp"
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace loop {

                        /* Emits do routine too refrence. */
                        __inline void emit_do(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_do(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_do(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Do statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits repeat routine too refrence. */
                        __inline void emit_repeat(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_repeat(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_repeat(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Repeat statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits loop break too refrence. */
                        __inline void emit_loop_break(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_loop_break(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_loop_break(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop break statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits loop continue too refrence. */
                        __inline void emit_loop_continue(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_loop_continue(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_loop_continue(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop continue statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        __inline void emit_while(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_while(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_while(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop while statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits while routine too refrence (pre compiled compare). */
                        __inline void emit_while_pre(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_while_pre(buffer, compiled, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_while_pre(buffer, compiled, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop while pre statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits until routine too refrence. */
                        __inline void emit_until(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_until(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_until(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop until statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits until routine too refrence (pre compiled compare). */
                        __inline void emit_until_pre(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &compiled, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::loop::emit_until_pre(buffer, compiled, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::loop::emit_until_pre(buffer, compiled, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Loop until pre statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace loop

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif