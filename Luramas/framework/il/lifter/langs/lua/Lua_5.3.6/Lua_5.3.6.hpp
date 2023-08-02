#pragma once
#include "../../../../il/il.hpp"
#if luramas_target == luramas_target_lua_53_6

#include "../../../../../disassembler/lua/Lua_5.3.6/disassembler.hpp"

namespace luramas {

      namespace il {

            namespace lifter {

                  /* Lifts current closure and child closures too IL disassembly. */
                  void lift(const Proto *p, const std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> &dism, std::shared_ptr<luramas::il::ilang> &buffer);

                  /* Lifts proto and child closures too IL disassembly. */
                  std::shared_ptr<luramas::il::ilang> lift_proto(const Proto *p);

            } // namespace lifter

      } // namespace il

} // namespace luramas

#endif