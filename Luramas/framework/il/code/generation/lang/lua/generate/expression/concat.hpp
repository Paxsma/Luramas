#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace concat {

                        /* Emits concat too refrence. */
                        __inline void emit_concat(luramas::ir::data::refrence_string &buffer, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              const auto count = vect.size();
                              for (auto i = 0u; i < count; ++i) {

                                    buffer += vect[i];

                                    if ((i + 1u) != count) {
                                          buffer += format->spacing.format_parameter_seperator(lua_langkeyword_concat);
                                    }
                              }

                              return;
                        }

                  } // namespace concat

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas