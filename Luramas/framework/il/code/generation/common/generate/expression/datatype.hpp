#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace datatype {

                        /* Emits arguement too refrence. */
                        __inline void emit_arg(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_arg(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_arg(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype arguement statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits variable too refrence. */
                        __inline void emit_var(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_var(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_var(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype variable statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits global too refrence. */
                        __inline void emit_global(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_global(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_global(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype variable statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits upvalue too refrence. */
                        __inline void emit_upvalue(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_upvalue(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_upvalue(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype upvalue statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits string too refrence. */
                        __inline void emit_string(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_string(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_string(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype string statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits vararg too refrence. */
                        __inline void emit_vararg(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_vararg(buffer, mark, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_vararg(buffer, mark, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype vararg statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits boolean too refrence. */
                        __inline void emit_boolean(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const bool data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_boolean(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_boolean(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype boolean statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits integer too refrence. */
                        __inline void emit_integer(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_integer(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_integer(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype integer statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits kvalue too refrence. */
                        __inline void emit_kval(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::string &data, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_kval(buffer, mark, data, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_kval(buffer, mark, data, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype kval statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits none too refrence. */
                        __inline void emit_none(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const bool mark, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_none(buffer, mark, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_none(buffer, mark, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype none statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits nothing too refrence. */
                        __inline void emit_nothing(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::datatype::emit_nothing(buffer);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::datatype::emit_nothing(buffer);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Datatype nothing statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace datatype

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas