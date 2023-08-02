#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace line {

                        /* Emits line break too refrence. */
                        __inline void emit_linebreak(std::string &buffer, const std::size_t amt, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += std::string(amt, luau_langkeyword_line_break);
                              return;
                        }

                        /* Emits semicolon too refrence. */
                        __inline void emit_semicolon(std::string &buffer, const std::shared_ptr<luramas::ir::data::format::format> &format) {
                              buffer += format->spacing.format_semicolon(luau_langkeyword_semicolon);
                              return;
                        }
                  } // namespace line

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas