#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace arith {

                        /* Emits arith operation too refrence. */
                        __inline void emit_arith_operation(luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds kind, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += lvalue;

                              switch (kind) {

                                    case luramas::ast::bin_kinds::add_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_add);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::mod_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_mod);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::sub_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_sub);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::div_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_div);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::mul_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_mul);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::pow_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_pow);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::and_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_and);
                                          break;
                                    }

                                    case luramas::ast::bin_kinds::or_: {
                                          buffer += format->spacing.format_arith(luau_langkeyword_arith_or);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Unkown binkind when trying too emit arithmetic.");
                                    }
                              }

                              buffer += rvalue;

                              return;
                        }

                  } // namespace arith

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas