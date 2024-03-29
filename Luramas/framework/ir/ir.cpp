#include "ir.hpp"
#include <algorithm>
#include <memory>
#include <string>

std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> luramas::ir::ir_expr_data::cache::vect::expression_data;

namespace ir_format {

      struct global {

            std::string indent = "";
            std::size_t linebreak = 0u;
            bool linebreak_changed = false;

            template <bool dec = false /* true for dec(amt for index), false for inc(amt) */, bool post = false /* Post indent */>
            void indent_mut(std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t amt) {

                  if (post) {

                        for (auto i = 0u; i < amt; ++i)
                              this->indent += format->indent.indent_space;

                        this->indent_vect.back() = this->indent;

                  } else {

                        if (dec) {

                              for (auto i = 0u; i < amt; ++i)
                                    if (!this->indent_vect.empty()) {
                                          this->indent_vect.pop_back();
                                    }

                              this->indent = indent_vect.back();

                        } else {

                              for (auto i = 0u; i < amt; ++i)
                                    this->indent += format->indent.indent_space;

                              this->indent_vect.emplace_back(this->indent);
                        }
                  }
            }

            template <bool dec = false /* true for dec(amt for index), false for inc(amt) */, bool post = false /* Post indent */>
            void linebreak_mut(std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t amt) {

                  const auto cached = this->linebreak;

                  if (post) {

                        this->linebreak += amt;
                        linebreak_vect.back() = this->linebreak;

                  } else {

                        if (dec) {

                              for (auto i = 0u; i < amt; ++i)
                                    if (!linebreak_vect.empty()) {
                                          linebreak_vect.pop_back();
                                    }

                              this->linebreak = linebreak_vect.back();

                        } else {

                              this->linebreak += amt;
                              linebreak_vect.emplace_back(this->linebreak);
                        }
                  }

                  this->linebreak_changed = (cached != this->linebreak);
                  return;
            }

            void clear() {
                  this->indent_vect.clear();
                  this->linebreak_vect.clear();
                  this->indent.clear();
                  this->linebreak = 0u;
                  return;
            }

          private:
            std::vector<std::string> indent_vect = {indent};
            std::vector<std::size_t> linebreak_vect = {linebreak};

      } global;

      template <bool global_indent = true>
      __inline std::string indent(const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t amount) {

            std::string retn = (global_indent) ? global.indent : "";

            if (format->indent.disabled) {
                  return retn;
            }

            for (auto i = 0u; i < amount; ++i)
                  retn += format->indent.indent_space;

            return retn;
      }

      template <bool global_indent = true>
      __inline void format_string(std::string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format, const std::size_t amount = 0u) {

            const auto indent_str = ir_format::indent<global_indent>(format, amount);

            std::string::size_type pos = 0;
            while ((pos = buffer.find(common_langkeyword_line_break, pos)) != std::string::npos) {
                  buffer.insert(pos + 1u, indent_str);
                  pos += indent_str.length() + 1u;
            }

            buffer = indent_str + buffer;
            return;
      }

} // namespace ir_format

/* Generates code from IR. */
std::string luramas::ir::generate(const luramas::emitter_ir::syntax::emitter_syntax syn, const std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line>> &data, std::unordered_map<std::size_t, std::string> &line_data_map, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations) {

      std::string code = "";

      ir_format::global.indent_mut(format, format->indent.indent_start);
      ir_format::global.linebreak_mut(format, format->linebreak.start_linebreak_amount);
      ir_format::global.linebreak_changed = false; /* Ignore change */

#pragma region data_vars

#pragma region comment
      bool comment_concat = false;
      std::size_t comment_amt = 0u;
      std::string comment_data = "";
#pragma endregion

#pragma endregion

      for (auto i = 0u; i < data.size(); ++i) {

            const auto line = data[i];

            /* String */
            if (line_data_map.find(i) != line_data_map.end()) {
                  code += line_data_map[i];
            }

            /* Comment concat */
            if (comment_concat && line->line_type != luramas::ir::data::kinds::comment && !format->comments.disable_comments) {

                  /* Indent collapsed disabled?? */
                  if (!format->indent.collapse.comment.disabled) {

                        /* Optimize disabled?? and comment with amount being 1?? */
                        if (!optimizations->disabled && optimizations->comment.comments && comment_amt == 1u) {

                              /* Emit expandable comment if possible. */

                              const auto count = std::count(comment_data.begin(), comment_data.end(), common_langkeyword_line_break);

                              if (count >= 2u) {
                                    goto expa;
                              } else if (comment_data.back() == common_langkeyword_line_break) {
                                    ir_format::format_string<false>(comment_data, format, format->indent.indent_comment);
                                    luramas::emitter_ir::common::comment::comment(syn, code, comment_data, format, ir_format::global.linebreak, !format->linebreak.disable);
                              } else {
                                    goto expa;
                              }

                        } else {
                        expa:
                              ir_format::format_string(comment_data, format, format->indent.indent_comment_expandable);
                              luramas::emitter_ir::common::comment::expandable_comment(syn, code, comment_data, format, ir_format::global.linebreak, format->linebreak.linebreak_expandable_comment_pre, format->linebreak.linebreak_expandable_comment_post, !format->linebreak.disable);
                        }

                  } else {
                        ir_format::format_string<false>(comment_data, format, format->indent.indent_comment);
                        luramas::emitter_ir::common::comment::comment(syn, code, comment_data, format, ir_format::global.linebreak, !format->linebreak.disable);
                  }

                  comment_concat = false;
                  comment_data.clear();
            }

            switch (line->line_type) {

                  case luramas::ir::data::kinds::expandable_comment: {

                        comment_data = line->comment.data;

                        ir_format::format_string(comment_data, format, format->indent.indent_comment_expandable);
                        luramas::emitter_ir::common::comment::expandable_comment(syn, code, comment_data, format, ir_format::global.linebreak, format->linebreak.linebreak_expandable_comment_pre, format->linebreak.linebreak_expandable_comment_post, !format->linebreak.disable);

                        comment_data.clear();

                        break;
                  }

                  case luramas::ir::data::kinds::comment: {

                        if (!format->comments.disable_comments) {

                              if (format->comments.disabled) {

                                    /* Emit singular comment. */
                                    comment_data = line->comment.data;

                                    ir_format::format_string<false>(comment_data, format, format->indent.indent_comment);
                                    luramas::emitter_ir::common::comment::comment(syn, code, comment_data, format, ir_format::global.linebreak, !format->linebreak.disable);

                                    comment_data.clear();

                              } else {

                                    /* Cache expandable comment. */

                                    comment_data += line->comment.data;

                                    comment_concat = !format->indent.collapse.comment.concat.disabled;

                                    if (!format->indent.collapse.comment.disabled && !format->linebreak.disable) {
                                          luramas::emitter_ir::common::line::emit_linebreak(syn, comment_data, 1u, format);
                                    }

                                    ++comment_amt;
                              }
                        }

                        break;
                  }

                  case luramas::ir::data::kinds::expression: {

                        /* Build */
                        auto str = line->expression->build(syn, {}, format).str();

                        /* Adjust indent. [PRE] */
                        if (line->expression->constant != luramas::ir::data::constant::constant_kinds::nothing) {

                              switch (line->expression->constant) {

                                    case luramas::ir::data::constant::constant_kinds::for_: {
                                          ir_format::global.indent_mut(format, format->indent.indent_generic_for_loop_pre);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_generic_for_loop_pre);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::for_numeric: {
                                          ir_format::global.indent_mut(format, format->indent.indent_numeric_for_loop_pre);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_numeric_for_loop_pre);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::do_:
                                    case luramas::ir::data::constant::constant_kinds::repeat_:
                                    case luramas::ir::data::constant::constant_kinds::while_: {
                                          ir_format::global.indent_mut(format, format->indent.indent_loop_pre);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_loop_pre);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::if_: {
                                          ir_format::global.indent_mut(format, format->indent.indent_if_pre);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_if_pre);
                                          break;
                                    }
                                    case luramas::ir::data::constant::constant_kinds::elseif_: {
                                          ir_format::global.indent_mut<true>(format, 1u);
                                          break;
                                    }
                                    case luramas::ir::data::constant::constant_kinds::else_: {
                                          ir_format::global.indent_mut<true>(format, 1u);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::end_:
                                    case luramas::ir::data::constant::constant_kinds::until: {

                                          switch (format->indent.indent_format_scope_end) {

                                                case 1u: {
                                                      ir_format::global.indent_mut<true>(format, 1u);
                                                      break;
                                                }

                                                case 3u: {
                                                      ir_format::global.indent_mut(format, format->indent.indent_end);
                                                      break;
                                                }

                                                default: {
                                                      break;
                                                }
                                          }

                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::function_: {

                                          switch (line->expression->constants.function.type) {

                                                case luramas::ir::data::constant::function_kinds::scoped: {
                                                      ir_format::global.indent_mut(format, format->indent.indent_scope_function_pre);
                                                      ir_format::global.linebreak_mut(format, format->linebreak.linebreak_scope_function_pre);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::function_kinds::global: {
                                                      ir_format::global.indent_mut(format, format->indent.indent_global_function_pre);
                                                      ir_format::global.linebreak_mut(format, format->linebreak.linebreak_global_function_pre);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::function_kinds::anonymous: {
                                                      ir_format::global.indent_mut(format, format->indent.indent_anonymous_function_pre);
                                                      ir_format::global.linebreak_mut(format, format->linebreak.linebreak_anonymous_function_pre);
                                                      break;
                                                }

                                                default: {
                                                      break;
                                                }
                                          }

                                          break;
                                    }

                                    default: {
                                          break;
                                    }
                              }
                        }

                        /* \n */
                        if (!format->linebreak.disable && ir_format::global.linebreak_changed /* Emit if changed */) {
                              luramas::emitter_ir::common::line::emit_linebreak(syn, code, ir_format::global.linebreak, format);
                        }

                        ir_format::format_string(str, format);
                        code += str;

                        /* Adjust indent. [POST] */
                        if (line->expression->constant != luramas::ir::data::constant::constant_kinds::nothing) {

                              switch (line->expression->constant) {

                                    case luramas::ir::data::constant::constant_kinds::for_: {
                                          ir_format::global.indent_mut<false, true>(format, format->indent.indent_generic_for_loop_post);
                                          ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_generic_for_loop_post);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::for_numeric: {
                                          ir_format::global.indent_mut<false, true>(format, format->indent.indent_numeric_for_loop_post);
                                          ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_numeric_for_loop_post);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::do_:
                                    case luramas::ir::data::constant::constant_kinds::repeat_:
                                    case luramas::ir::data::constant::constant_kinds::while_: {
                                          ir_format::global.indent_mut<false, true>(format, format->indent.indent_loop_post);
                                          ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_loop_post);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::if_: {
                                          ir_format::global.indent_mut<false, true>(format, format->indent.indent_if_post);
                                          ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_if_post);
                                          break;
                                    }
                                    case luramas::ir::data::constant::constant_kinds::elseif_: {
                                          ir_format::global.indent_mut(format, format->indent.indent_elseif_post);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_elseif_post);
                                          break;
                                    }
                                    case luramas::ir::data::constant::constant_kinds::else_: {
                                          ir_format::global.indent_mut(format, format->indent.indent_else_post);
                                          ir_format::global.linebreak_mut(format, format->linebreak.linebreak_else_post);
                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::end_:
                                    case luramas::ir::data::constant::constant_kinds::until: {

                                          switch (format->indent.indent_format_scope_end) {

                                                case 0u: {
                                                      ir_format::global.indent_mut<true>(format, 1u);
                                                      break;
                                                }

                                                case 4u: {
                                                      ir_format::global.indent_mut(format, format->indent.indent_end);
                                                      break;
                                                }

                                                default: {
                                                      break;
                                                }
                                          }

                                          ir_format::global.linebreak_mut<true>(format, 1u);

                                          break;
                                    }

                                    case luramas::ir::data::constant::constant_kinds::function_: {

                                          switch (line->expression->constants.function.type) {

                                                case luramas::ir::data::constant::function_kinds::scoped: {
                                                      ir_format::global.indent_mut<false, true>(format, format->indent.indent_scope_function_post);
                                                      ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_scope_function_post);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::function_kinds::global: {
                                                      ir_format::global.indent_mut<false, true>(format, format->indent.indent_global_function_post);
                                                      ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_global_function_post);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::function_kinds::anonymous: {
                                                      ir_format::global.indent_mut<false, true>(format, format->indent.indent_anonymous_function_post);
                                                      ir_format::global.linebreak_mut<false, true>(format, format->linebreak.linebreak_anonymous_function_post);
                                                      break;
                                                }

                                                default: {
                                                      break;
                                                }
                                          }

                                          break;
                                    }

                                    default: {
                                          break;
                                    }
                              }

                              /* ; */
                              if (!optimizations->disabled && !optimizations->semicolon.disable) {

                                    switch (line->expression->constant) {

                                          case luramas::ir::data::constant::constant_kinds::end_: {
                                                if (optimizations->semicolon.constant.semicolon_end) {
                                                      luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                                                }
                                                break;
                                          }

                                          case luramas::ir::data::constant::constant_kinds::until: {
                                                if (optimizations->semicolon.constant.semicolon_until) {
                                                      luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                                                }
                                                break;
                                          }

                                          case luramas::ir::data::constant::constant_kinds::repeat_: {
                                                if (optimizations->semicolon.constant.semicolon_repeat) {
                                                      luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                                                }
                                                break;
                                          }

                                          case luramas::ir::data::constant::constant_kinds::break_: {
                                                if (optimizations->semicolon.constant.semicolon_break) {
                                                      luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                                                }
                                                break;
                                          }

                                          case luramas::ir::data::constant::constant_kinds::return_: {
                                                if (optimizations->semicolon.constant.semicolon_return) {
                                                      luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                                                }
                                                break;
                                          }

                                          default: {
                                                break;
                                          }
                                    }
                              }

                        } else {

                              /* ; */
                              if (!optimizations->disabled && !optimizations->semicolon.disable) {
                                    luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                              }
                        }

                        /* \n */
                        if (!format->linebreak.disable) {
                              luramas::emitter_ir::common::line::emit_linebreak(syn, code, ir_format::global.linebreak, format);
                        }

                        break;
                  }

                  case luramas::ir::data::kinds::statement: {

                        if (line->statement.data_types.is_function) {

                        } else {

                              std::string buffer = "";

                              std::vector<std::string> lvalue;
                              std::vector<std::string> rvalue;

                              if (line->statement.variable.new_variable) {

                                    /* Var */
                                    lvalue = line->statement.variable.name;

                              } else {

                                    /* Lvalue */
                                    for (const auto &l : line->statement.lvalue) {
                                          lvalue.emplace_back(l->build(syn, {}, format).str());
                                    }
                              }

                              /* Rvalue */
                              for (const auto &l : line->statement.rvalue) {
                                    rvalue.emplace_back(l->build(syn, {}, format).str());
                              }

                              if (line->statement.variable.new_variable) {
                                    luramas::emitter_ir::common::assignment::initial_assignment(syn, buffer, lvalue, rvalue, format);
                              } else if (line->statement.data_types.is_arith) {
                                    luramas::emitter_ir::common::assignment::arith::emit_arith_assignment(syn, buffer, line->statement.data_types.arith.kind, {lvalue}, {rvalue}, format);
                              } else {
                                    luramas::emitter_ir::common::assignment::assignment(syn, buffer, {lvalue}, {rvalue}, format);
                              }

                              ir_format::format_string(buffer, format);
                              code += buffer;

                              /* ; */
                              if (!optimizations->semicolon.disable) {
                                    luramas::emitter_ir::common::line::emit_semicolon(syn, code, format);
                              }

                              /* \n */
                              if (!format->linebreak.disable) {
                                    luramas::emitter_ir::common::line::emit_linebreak(syn, code, ir_format::global.linebreak, format);
                              }
                        }

                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return code;
}