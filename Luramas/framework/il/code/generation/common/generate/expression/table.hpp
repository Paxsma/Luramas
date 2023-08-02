#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace table {

                        /* Emits index too refrence. */
                        __inline void emit_index(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &table, luramas::ir::data::refrence_string &index, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::table::emit_index(buffer, table, index, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::table::emit_index(buffer, table, index, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Table index operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits generic index too refrence. */
                        __inline void emit_index_generic(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, luramas::ir::data::refrence_string &table, luramas::ir::data::refrence_string &index, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::table::emit_index_generic(buffer, table, index, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::table::emit_index_generic(buffer, table, index, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Table generic index operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits table too refrence. */
                        __inline void emit_table(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, std::vector<std::shared_ptr<luramas::ir::data::data_refrence::table_refrence_member>> &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::table::emit_table(buffer, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::table::emit_table(buffer, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Table statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace table

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas