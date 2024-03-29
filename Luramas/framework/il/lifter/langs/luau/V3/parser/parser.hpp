#pragma once
#include "../../../../parser/parser.hpp"
#include "../Luau-V3.hpp"
#include <functional>
#include <vector>

#if luramas_target == luramas_target_luau_v3

namespace luau_v3_parsers {

      /* Parse compare instruction. */
      void parse_compares(luramas::il::lifter::parser::parser_manager<luau_v3_disassembler::disassembly> &pm);

      /* Inits parser, does not remove or add any new instructions, everything is alligned. */
      void parse_instructions(luramas::il::lifter::parser::parser_manager<luau_v3_disassembler::disassembly> &pm);

      /* Parse jumps */
      void parse_jumps(luramas::il::lifter::parser::parser_manager<luau_v3_disassembler::disassembly> &pm);

} // namespace luau_v3_parsers

#endif