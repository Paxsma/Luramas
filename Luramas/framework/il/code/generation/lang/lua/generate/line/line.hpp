#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace line {

                        /* Emits line break too refrence. */
                        __inline void emit_linebreak(std::string &buffer, const std::size_t amt, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += std::string(amt, lua_langkeyword_line_break);
                              return;
                        }

                        /* Emits semicolon too refrence. */
                        __inline void emit_semicolon(std::string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += format->spacing.format_semicolon(lua_langkeyword_semicolon);
                              return;
                        }
                  } // namespace line

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas