#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  namespace call {

                        /* Emits call too refrence. */
                        __inline void emit_call(luramas::ir::data::refrence_string &buffer, const luramas::ir::data::refrence_string &call, std::vector<luramas::ir::data::refrence_string> &vect, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              luramas::ir::data::refrence_string args("");

                              buffer += call;
                              format->spacing.format_call_spacing(buffer);

                              const auto count = vect.size();
                              for (auto i = 0u; i < count; ++i) {

                                    args += vect[i];

                                    /* Add split. */
                                    if ((i + 1u) != count) {
                                          args += format->spacing.format_parameter_seperator(luau_langkeyword_call_seperator);
                                    }
                              }

                              /* ( ?? ) */
                              args.push_front(format->spacing.format_call_parameter_open(luau_langkeyword_call_parenthesis_open));
                              args += format->spacing.format_call_parameter_close(luau_langkeyword_call_parenthesis_close);

                              buffer += args;
                              return;
                        }

                  } // namespace call

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas