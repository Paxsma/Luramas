#pragma once
#include "../Lua_5.3.6.hpp"

#if luramas_target == luramas_target_lua_53_6

/* Maps everything to their relative translation. (Does not add or remove any instructions not relative to the translation) */
namespace parser {

      /* Parses instructions. */
      void parse_instruction(std::shared_ptr<luramas::il::ilang> &buffer, const std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> &dism, std::uint16_t &stack_top);

      /* Maps out jumps too be fixed for there relatives. */
      std::vector<std::pair<std::shared_ptr<luramas::il::disassembly> /* Jumper */, std::shared_ptr<luramas::il::disassembly> /* Jump location */>> parse_jumps(std::shared_ptr<luramas::il::ilang> &il);

} // namespace parser

#endif