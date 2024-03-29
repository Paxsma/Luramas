#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace datatype {

                        /* Emits arguement too refrence. */
                        __inline void emit_arg(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits variable too refrence. */
                        __inline void emit_var(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits global too refrence. */
                        __inline void emit_global(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits upvalue too refrence. */
                        __inline void emit_upvalue(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }
                              return;
                        }

                        /* Emits string too refrence. */
                        __inline void emit_string(luramas::ir::data::refrence_string &buffer, const bool mark, std::string data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              /* Fix string */
                              if (data.back() != lua_langkeyword_string_end) {
                                    data += lua_langkeyword_string_end;
                              }
                              if (data.front() != lua_langkeyword_string_start) {
                                    data = lua_langkeyword_string_start + data;
                              }

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits vararg too refrence. */
                        __inline void emit_vararg(luramas::ir::data::refrence_string &buffer, const bool mark, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(lua_langkeyword_vararg);
                              } else {
                                    buffer += lua_langkeyword_vararg;
                              }

                              return;
                        }

                        /* Emits boolean too refrence. */
                        __inline void emit_boolean(luramas::ir::data::refrence_string &buffer, const bool mark, const bool data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark((data) ? lua_langkeyword_true : lua_langkeyword_false);
                              } else {
                                    buffer += (data) ? lua_langkeyword_true : lua_langkeyword_false;
                              }

                              return;
                        }

                        /* Emits integer too refrence. */
                        __inline void emit_integer(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits kvalue too refrence. */
                        __inline void emit_kval(luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(data);
                              } else {
                                    buffer += data;
                              }

                              return;
                        }

                        /* Emits none too refrence. */
                        __inline void emit_none(luramas::ir::data::refrence_string &buffer, const bool mark, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              if (mark) {
                                    buffer.append_mark(lua_langkeyword_nil);
                              } else {
                                    buffer += lua_langkeyword_nil;
                              }

                              return;
                        }

                        /* Emits nothing too refrence. */
                        __inline void emit_nothing(luramas::ir::data::refrence_string &buffer) {
                              buffer += lua_langkeyword_nothing;
                              return;
                        }

                  } // namespace datatype

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas