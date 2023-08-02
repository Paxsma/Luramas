#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace unary {

                        /* Emits unary too refrence string. */
                        __inline void emit_unary(luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds unary, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (unary) {

                                    case luramas::ast::bin_kinds::not_: {
                                          buffer.push_front(format->spacing.format_unary_not(luau_langkeyword_not));
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::len_: {
                                          buffer.push_front(format->spacing.format_unary_length(luau_langkeyword_lenght));
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::minus_: {
                                          buffer.push_front(format->spacing.format_unary_minus(luau_langkeyword_minus));
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::bitnot_: {
                                          buffer.push_front(format->spacing.format_unary_bitnot(luau_langkeyword_bitnot));
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Unkown unary for unary IR emitter.");
                                    }
                              }

                              return;
                        }

                  } // namespace unary

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas