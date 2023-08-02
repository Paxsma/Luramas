#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace logical {

                        __inline void emit_operator(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (op) {

                                    case luramas::ir::data::constant::logical_operations_kinds::and_: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_and);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::or_: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_or);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::lt: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_lessthan);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::lte: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_lessthanequal);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::eq: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_equal);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::ne: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_notequal);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::gt: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_greaterthan);
                                          break;
                                    }

                                    case luramas::ir::data::constant::logical_operations_kinds::gte: {
                                          buffer += format->spacing.format_logical_operation(luau_langkeyword_compare_greaterthanequal);
                                          break;
                                    }

                                    default: {
                                          break;
                                    }
                              }

                              return;
                        }

                        /* Emits logical operation too refrence. */
                        __inline void emit_logical_operation(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += lvalue;
                              luramas::emitter_ir::luau::logical::emit_operator(buffer, op, format);
                              buffer += rvalue;

                              return;
                        }

                        /* Emits single logical operation too refrence. */
                        __inline void emit_single_logical_operation(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &lvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const bool open = true, const bool close = true) {

                              if (open) {
                                    buffer += format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_open);
                              }

                              buffer += lvalue;

                              if (close) {
                                    format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_close);
                              }

                              return;
                        }

                        /* Emits logical compare too refrence. e.g. (?? ?? ??) */
                        __inline void emit_logical_compare(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::constant::logical_operations_kinds op, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const bool open = true, const bool close = true) {

                              if (open) {
                                    buffer += format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_open);
                              }

                              luramas::emitter_ir::luau::logical::emit_logical_operation(buffer, op, lvalue, rvalue, format);

                              if (close) {
                                    buffer += format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_close);
                              }

                              return;
                        }

                        /* Emits logical compare open/close too refrence. e.g. (/) */
                        template <bool open = true /* True for open, False for close*/>
                        __inline void emit_logical_compare_parenthesis(luramas::ir::data::refrence_string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (open) {
                                    buffer += format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_open);
                              } else {
                                    buffer += format->spacing.format_parenthesis_open(luau_langkeyword_parenthesis_close);
                              }

                              return;
                        }

                  } // namespace logical

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas