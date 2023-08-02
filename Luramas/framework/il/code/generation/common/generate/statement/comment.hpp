/*
* framework/il/code/generation/common/generate/statement/comment.hpp
* 
* IL Code Generation Comment Statements
* 10/7/2023
* 
* IL code generation comment statements.
*/
#pragma once

#ifndef luramas_code_generation_common_statement_comment
#define luramas_code_generation_common_statement_comment

#include "../generate.hpp"
#include "../line/line.hpp"
#include "../supported.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace comment {

                        /* Emit expandable comment. */
                        template <bool post = true /* Emits post. */>
                        __inline void expandable_comment(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::string &src, const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t current_amt = 0u, const std::size_t pre_amt = 0u, const std::size_t post_amt = 0u, const bool linebreak = true) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::comment::expandable_comment<post>(buffer, src, format, current_amt, pre_amt, post_amt, linebreak);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::comment::expandable_comment<post>(buffer, src, format, current_amt, pre_amt, post_amt, linebreak);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Expandable comment statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                        /* Emit comment. */
                        template <bool post = true /* Emits post. */>
                        __inline void comment(const luramas::emitter_ir::syntax::emitter_syntax syn, std::string &buffer, const std::string &src, const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t current_amt = 0u, const bool linebreak = true) {

                              switch (syn) {

                                    case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                          luramas::emitter_ir::lua::comment::comment<post>(buffer, src, format, current_amt, linebreak);
                                          break;
                                    }

                                    case luramas::emitter_ir::syntax::emitter_syntax::luau: {
                                          luramas::emitter_ir::luau::comment::comment<post>(buffer, src, format, current_amt, linebreak);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Comment statement emissions are not supported by current target syntax.");
                                    }
                              }

                              return;
                        }

                  } // namespace comment

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif