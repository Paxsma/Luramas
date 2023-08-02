#pragma once
#include "../../../../il/il.hpp"
#if luramas_target == luramas_target_luau_v3

#include "../../../../../disassembler/luau/V3/disassembler.hpp"

namespace luramas {

      namespace il {

            namespace lifter {

                  /* Lifts current closure and child closures too IL disassembly. */
                  void lift(Proto *p, const std::vector<std::shared_ptr<luau_v3_disassembler::disassembly>> &dism, std::shared_ptr<luramas::il::ilang> &buffer);

                  /* Lifts proto and child closures too IL disassembly. */
                  std::shared_ptr<luramas::il::ilang> lift(Proto *p);

            } // namespace lifter

      } // namespace il

} // namespace luramas

#endif