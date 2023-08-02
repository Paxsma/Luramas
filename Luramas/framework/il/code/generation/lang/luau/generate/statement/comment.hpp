#pragma once
#include "../emit.hpp"
#include "../line/line.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace comment {

                        /* Emit expandable comment. */
                        template <bool post = true /* Emits post. */>
                        __inline void expandable_comment(std::string &buffer, const std::string &src, const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t current_amt = 0u, const std::size_t pre_amt = 0u, const std::size_t post_amt = 0u, const bool linebreak = true) {

                              std::string line_break_pre = "";
                              if (linebreak) {
                                    luramas::emitter_ir::luau::line::emit_linebreak(line_break_pre, pre_amt, format);
                              }

                              std::string line_break_post = "";
                              if (linebreak) {
                                    luramas::emitter_ir::luau::line::emit_linebreak(line_break_post, post_amt, format);
                              }

                              std::string line_break_end = "";
                              if (src.back() != '\n' && linebreak) {
                                    luramas::emitter_ir::luau::line::emit_linebreak(line_break_end, current_amt, format);
                              }

                              if (post) {
                                    buffer += std::string(luau_langkeyword_comment_expandable_start) + line_break_pre + src + line_break_post + std::string(luau_langkeyword_comment_expandable_end) + line_break_end;
                              } else {
                                    buffer = std::string(luau_langkeyword_comment_expandable_start) + line_break_pre + src + line_break_post + std::string(luau_langkeyword_comment_expandable_end) + line_break_end + buffer;
                              }

                              return;
                        }

                        /* Emit comment. */
                        template <bool post = true /* Emits post. */>
                        __inline void comment(std::string &buffer, const std::string &src, const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t current_amt = 0u, const bool linebreak = true) {

                              std::string line_break_end = "";
                              if (src.back() != '\n' && linebreak) {
                                    luramas::emitter_ir::luau::line::emit_linebreak(line_break_end, current_amt, format);
                              }

                              if (post) {
                                    buffer += std::string(luau_langkeyword_comment) + std::string(" ") + src + line_break_end;
                              } else {
                                    buffer = std::string(luau_langkeyword_comment) + std::string(" ") + src + line_break_end + buffer;
                              }

                              return;
                        }

                  } // namespace comment

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas