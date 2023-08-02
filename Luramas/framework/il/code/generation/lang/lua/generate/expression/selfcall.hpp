#pragma once
#include "../emit.hpp"

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  namespace selfcall {

                        /* Emits selfcall too refrence. */
                        __inline void emit_selfcall(luramas::ir::data::refrence_string &buffer, const bool mark, luramas::ir::data::refrence_string &lvalue, luramas::ir::data::refrence_string &rvalue, const std::shared_ptr<luramas::ir::data::format::format> &format) {

                              buffer += lvalue;
                              buffer += lua_langkeyword_selfcall;

                              if (mark) {
                                    buffer.append_mark(rvalue.str());
                              } else {
                                    buffer += rvalue;
                              }

                              return;
                        }

                  } // namespace selfcall

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas