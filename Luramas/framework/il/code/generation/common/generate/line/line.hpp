/*
* framework/il/code/generation/common/generate/line/line.hpp
* 
* IL Code Generation Lines
* 10/7/2023
* 
* IL code generation lines.
*/
#pragma once

#ifndef luramas_code_generation_common_line
#define luramas_code_generation_common_line

#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace line {

                        /* Emits line break too refrence. */
                        __inline void emit_linebreak(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::size_t amt, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::line::emit_linebreak(buffer, amt, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::line::emit_linebreak(buffer, amt, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Line break statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emits semicolon too refrence. */
                        __inline void emit_semicolon(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::line::emit_semicolon(buffer, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::line::emit_semicolon(buffer, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Semicolon statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace line

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif