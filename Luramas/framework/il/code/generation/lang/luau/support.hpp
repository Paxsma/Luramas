#pragma once
#include "generate/emit.hpp"
#include <array>

namespace luramas {

      namespace emitter_ir {

            namespace luau {

                  /* Arith instruction that are supported as assignments. (You can leave it empty if every assignment is supported). */
                  constexpr std::array supported_arith_assignment = {
                      ast::bin_kinds::add_,
                      ast::bin_kinds::sub_,
                      ast::bin_kinds::mod_,
                      ast::bin_kinds::div_,
                      ast::bin_kinds::mul_,
                      ast::bin_kinds::pow_};

                  /* Ternarys are supported? */
                  constexpr bool supported_ternarys = false;

            } // namespace luau

      } // namespace emitter_ir

} // namespace luramas