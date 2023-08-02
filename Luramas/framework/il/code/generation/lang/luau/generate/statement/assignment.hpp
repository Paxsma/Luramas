#pragma once
#include "../../../../../../../ast/ast/kinds.hpp"
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace assignment {

                        /* Emits dynamic initial assignment too refrence. */
                        __inline void initial_assignment(std::string &buffer, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              std::string lvalues = "";
                              std::string rvalues = "";

                              auto count = lvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    lvalues += lvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          lvalues += format->spacing.format_assignment_initial_lvalue(luau_langkeyword_list_lvalue);
                                    }
                              }

                              count = rvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    rvalues += rvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          rvalues += format->spacing.format_assignment_initial_rvalue(luau_langkeyword_list_lvalue);
                                    }
                              }

                              buffer += format->spacing.format_assignment_initial_datatype(luau_langkeyword_dynamic_datatype);
                              buffer += lvalues;
                              buffer += format->spacing.format_assignment_initial(luau_langkeyword_assignment);
                              buffer += rvalues;

                              return;
                        }

                        /* Emits dynamic assignment too refrence. */
                        __inline void assignment(std::string &buffer, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const char *const assignment = luau_langkeyword_assignment) {

                              std::string lvalues = "";
                              std::string rvalues = "";

                              auto count = lvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    lvalues += lvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          lvalues += format->spacing.format_assignment_lvalue(luau_langkeyword_list_lvalue);
                                    }
                              }

                              count = rvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    rvalues += rvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          rvalues += format->spacing.format_assignment_rvalue(luau_langkeyword_list_lvalue);
                                    }
                              }

                              buffer += lvalues;
                              buffer += format->spacing.format_assignment(assignment);
                              buffer += rvalues;

                              return;
                        }

                        namespace arith {

                              /* Emits arith assignment too refrence. */
                              __inline void emit_arith_assignment(std::string &buffer, const luramas::ast::bin_kinds op, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                                    switch (op) {

                                          case luramas::ast::bin_kinds::add_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_add);
                                                break;
                                          }

                                          case luramas::ast::bin_kinds::mod_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_mod);
                                                break;
                                          }

                                          case luramas::ast::bin_kinds::sub_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_sub);
                                                break;
                                          }

                                          case luramas::ast::bin_kinds::div_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_div);
                                                break;
                                          }

                                          case luramas::ast::bin_kinds::mul_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_mul);
                                                break;
                                          }

                                          case luramas::ast::bin_kinds::pow_: {
                                                luramas::emitter_ir::luau::assignment::assignment(buffer, lvalue, rvalue, format, luau_langkeyword_arith_assignment_pow);
                                                break;
                                          }

                                          default: {
                                                throw std::runtime_error("Unkown binkind assignment when trying too emit arithmetic assignment.");
                                          }
                                    }

                                    return;
                              }

                        } // namespace arith

                  } // namespace assignment

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas