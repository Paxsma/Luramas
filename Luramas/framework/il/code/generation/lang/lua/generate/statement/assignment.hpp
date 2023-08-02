#pragma once
#include "../../../../../../../ast/ast/kinds.hpp"
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

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
                                          lvalues += format->spacing.format_assignment_initial_lvalue(lua_langkeyword_list_lvalue);
                                    }
                              }

                              count = rvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    rvalues += rvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          rvalues += format->spacing.format_assignment_initial_rvalue(lua_langkeyword_list_lvalue);
                                    }
                              }

                              buffer += format->spacing.format_assignment_initial_datatype(lua_langkeyword_dynamic_datatype);
                              buffer += lvalues;
                              buffer += format->spacing.format_assignment_initial(lua_langkeyword_assignment);
                              buffer += rvalues;

                              return;
                        }

                        /* Emits dynamic assignment too refrence. */
                        __inline void assignment(std::string &buffer, const std::vector<std::string> &lvalue, const std::vector<std::string> &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format, const char *const assignment = lua_langkeyword_assignment) {

                              std::string lvalues = "";
                              std::string rvalues = "";

                              auto count = lvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    lvalues += lvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          lvalues += format->spacing.format_assignment_lvalue(lua_langkeyword_list_lvalue);
                                    }
                              }

                              count = rvalue.size();
                              for (auto i = 0u; i < count; ++i) {

                                    rvalues += rvalue[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          rvalues += format->spacing.format_assignment_rvalue(lua_langkeyword_list_lvalue);
                                    }
                              }

                              buffer += lvalues;
                              buffer += format->spacing.format_assignment(assignment);
                              buffer += rvalues;

                              return;
                        }

                  } // namespace assignment

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas