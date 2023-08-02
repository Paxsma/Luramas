#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace table {

                        /* Emits index too refrence. */
                        __inline void emit_index(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &table, luramas::ir::data::refrence_string &index, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += table;
                              buffer += format->spacing.format_index_ref(lua_langkeyword_index_open, index, lua_langkeyword_index_close);
                              return;
                        }

                        /* Emits generic index too refrence. */
                        __inline void emit_index_generic(luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &table, luramas::ir::data::refrence_string &index, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += table;
                              buffer += lua_langkeyword_index_generic;
                              buffer += index;
                              return;
                        }

                        /* Emits table too refrence. */
                        __inline void emit_table(luramas::ir::data::refrence_string &buffer, std::vector<std::shared_ptr<luramas::ir::data::data_refrence::table_refrence_member>> &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              auto make_member = [&](const bool seperate, std::shared_ptr<luramas::ir::data::data_refrence::table_refrence_member> &on) mutable -> void {
                                    if (on->table_end) {
                                          buffer += format->spacing.format_table_end(lua_langkeyword_table_end);
                                    }

                                    if (on->table_start) {
                                          buffer += format->spacing.format_table_start(lua_langkeyword_table_start);
                                    }

                                    if (!on->index.empty()) {
                                          buffer += format->spacing.format_table_index(lua_langkeyword_table_index_start, on->index, lua_langkeyword_table_index_end, lua_langkeyword_table_index_assignment);
                                    }

                                    buffer += on->value;

                                    if (seperate) {
                                          buffer += format->spacing.format_table_seperator(lua_langkeyword_table_seperator);
                                    }
                              };

                              for (auto i = 0u; i < data.size(); ++i) {

                                    auto member = data[i];

                                    /* Split? */
                                    if ((i + 1u) < data.size()) {

                                          auto next = data[i + 1u];

                                          /* curr: "}" && next: != "}" */
                                          if (member->table_end && !next->table_end) {
                                                make_member(true, member);
                                          } else {

                                                /* curr = "}" && next = "}" */
                                                if (member->table_end && next->table_end) {
                                                      make_member(false /* Last member in nested table. */, member);
                                                } else {

                                                      /* Current = value */
                                                      if (!member->value.empty()) {
                                                            make_member(!next->table_end /* Last member in table if next is an end. */, member);
                                                      } else {
                                                            make_member(false /* Last member in nested table. */, member);
                                                      }
                                                }
                                          }

                                    } else { /* Last member in parent table. */
                                          make_member(false, member);
                                    }
                              }

                              return;
                        }

                  } // namespace table

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas