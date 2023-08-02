#pragma once
#include "generate/emit.hpp"
#include <array>

namespace luramas {

      namespace emitter_ir {

            namespace lua {

                  /* Arith instruction that are supported as assignments. (You can leave it empty if every assignment is supported). */
                  static constexpr std::array supported_arith_assignment = {ast::bin_kinds::nothing};

                  /* Ternarys are supported? */
                  static constexpr bool supported_ternarys = false;

            } // namespace lua

      } // namespace emitter_ir

} // namespace luramas