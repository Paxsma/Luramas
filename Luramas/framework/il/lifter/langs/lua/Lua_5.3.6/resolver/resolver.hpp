#pragma once
#include "../Lua_5.3.6.hpp"

#if luramas_target == luramas_target_lua_53_6

namespace resolver {

      /* Resolves instructions. */
      void resolve_instructions(std::shared_ptr<luramas::il::ilang> &buffer);

      /* Resolves block data. */
      void resolve_blocks(std::shared_ptr<luramas::il::ilang> &buffer);

      /* Resolves jump locations. */
      void resolve_jumps(std::shared_ptr<luramas::il::ilang> &buffer, const std::vector<std::pair<std::shared_ptr<luramas::il::disassembly> /* Jumper */, std::shared_ptr<luramas::il::disassembly> /* Jump location */>> &map);

} // namespace resolver

#endif