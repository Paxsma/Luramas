#pragma once
#include "../generate.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace unary {

                        /* Emits unary too refrence string. */
                        __inline void emit_unary(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::data::refrence_string &buffer, const luramas::ast::bin_kinds unary, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::unary::emit_unary(buffer, unary, format);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::unary::emit_unary(buffer, unary, format);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Format operation statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace unary

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas