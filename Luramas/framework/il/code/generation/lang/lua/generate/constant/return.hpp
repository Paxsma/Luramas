#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace return_ {

                        /* Emits return too refrence. */
                        __inline void emit_return(luramas::ir::data::refrence_string &buffer, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              const auto count = vect.size();
                              buffer += (count) ? format->spacing.format_return(lua_langkeyword_return) : format->spacing.format_return<true>(lua_langkeyword_return);

                              for (auto i = 0u; i < count; ++i) {

                                    buffer += vect[i];

                                    if ((i + 1u) != count) {
                                          buffer += format->spacing.format_parameter_seperator(lua_langkeyword_return_seprator);
                                    }
                              }

                              return;
                        }

                  } // namespace return_

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas