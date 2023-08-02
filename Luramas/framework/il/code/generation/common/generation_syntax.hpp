/*
* framework/il/code/generation/common/generation_syntax.hpp
* 
* IL Code Generation Syntax
* 10/7/2023
* 
* IL code generation syntax selection.
*/
#pragma once

#ifndef luramas_code_generation_common_syntax
#define luramas_code_generation_common_syntax

#include <cstdint>
#include <iostream>

namespace luramas {

      namespace emitter_ir {

            namespace syntax {

                  /* Syntax of code for IL to emit. */
                  enum class emitter_syntax : std::uint8_t {
                        nothing, /* Nothing*/
                        python,  /* Python-3.11.0 */
                        rust,    /* Rust-1.68.0 */
                        c,       /* C-23 */
                        cpp,     /* Cpp-23 */
                        java,    /* Java-20 */
                        lua,     /* Lua-5.4 */
                        luau,    /* Lua-V3 */
                  };

                  /* Syntax emitter enum to string. */
                  __inline const char *const emitter_syntax_str(const luramas::emitter_ir::syntax::emitter_syntax syn) {

                        switch (syn) {

                              case luramas::emitter_ir::syntax::emitter_syntax::c: {
                                    return "C-23";
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::cpp: {
                                    return "Cpp-23";
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::java: {
                                    return "Java-20";
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::lua: {
                                    return "Lua-5.4";
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::python: {
                                    return "Python-3.11.0";
                              }

                              case luramas::emitter_ir::syntax::emitter_syntax::rust: {
                                    return "Rust-1.68.0";
                              }

                              default: {
                                    throw std::runtime_error("Unkown emittion syntax.");
                              }
                        }
                  }

            } // namespace syntax

      } // namespace emitter_ir

} // namespace luramas

#endif