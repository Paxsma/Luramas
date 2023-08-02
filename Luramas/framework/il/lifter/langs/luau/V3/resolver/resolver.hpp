#pragma once
#include "../../../../resolver/resolver.hpp"
#include "../Luau-V3.hpp"
#include <functional>
#include <unordered_map>

#if luramas_target == luramas_target_luau_v3

namespace luau_v3_resolvers {

      /* Resolves instructions operands. */
      void resolve_instruction_operands(luramas::il::lifter::resolver::resolver_manager<Proto> &rm);

      /* Resolves jump locations operands. */
      void resolve_jump_operands(luramas::il::lifter::resolver::resolver_manager<Proto> &rm);

      /* Resolves instructions. */
      void resolve_instructions(luramas::il::lifter::resolver::resolver_manager<Proto> &rm);

} // namespace luau_v3_resolvers

#endif