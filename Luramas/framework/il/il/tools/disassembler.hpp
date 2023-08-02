/*
* framework/il/il/tools/disassembler.hpp
* 
* IL Disassembler
* 10/7/2023
* 
* Provides the tools that allows users to easily disassemble the IL.
*/
#pragma once

#ifndef luramas_il_disassembler
#define luramas_il_disassembler

#include "../architecture/architecture.hpp"
#include <iostream>

namespace luramas {

      namespace il {

            namespace disassembler {

                  /* Returns mnemonic string for IL opcode. */
                  const char *const mnemonic_string(const luramas::il::arch::opcodes op);

                  /* Returns mnemonic hint string for IL opcode. */
                  const char *const mnemonic_hint_string(const luramas::il::arch::opcodes op);

                  /* Returns operand string for IL operand. */
                  std::string operand_string(const std::shared_ptr<luramas::il::arch::operand::operand> &operand);

            } // namespace disassembler

      } // namespace il

} // namespace luramas

#endif