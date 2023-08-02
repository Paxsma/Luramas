#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace logical {

                        __inline void emit_operator(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::logical::emit_operator(buffer, op, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::logical::emit_operator(buffer, op, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Logical operator statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits logical operation too refrence. */
                        __inline void emit_logical_operation(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::logical::emit_logical_operation(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::logical::emit_logical_operation(buffer, op, lvalue, rvalue, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Logical operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits single logical operation too refrence. */
                        __inline void emit_single_logical_operation(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &lvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const bool open = true, const bool close = true) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::logical::emit_single_logical_operation(buffer, lvalue, format, open, close);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::logical::emit_single_logical_operation(buffer, lvalue, format, open, close);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Logical single compare statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits logical compare too refrence. e.g. (?? ?? ??) */
                        __inline void emit_logical_compare(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const bool open = true, const bool close = true) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::logical::emit_logical_compare(buffer, op, lvalue, rvalue, format, open, close);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::logical::emit_logical_compare(buffer, op, lvalue, rvalue, format, open, close);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Logical compare statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits logical compare open/close too refrence. e.g. (/) */
                        template <bool open = true /* True for open, False for close*/>
                        __inline void emit_logical_compare_parenthesis(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::logical::emit_logical_compare_parenthesis<open>(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::logical::emit_logical_compare_parenthesis<open>(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Logical compare statement parenthesis emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace logical

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas