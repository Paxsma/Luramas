#pragma once
#include "op_table.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace luau_v3_disassembler {

      struct operand {

            op_table::operands operand; /* Operand encoding type */
            op_table::type type;        /* Operand kind */

            union {
                  std::uint16_t reg;         /* Register */
                  std::intptr_t val;         /* Signed value */
                  std::intptr_t jmp;         /* Jump, not address */
                  std::size_t k_idx;         /* Kvalue index */
                  std::intptr_t aux;         /* Aux */
                  std::uintptr_t slot;       /* Slot */
                  std::uintptr_t upvalue;    /* Upvalue index */
                  std::uintptr_t table;      /* Table value*/
                  std::uintptr_t closure;    /* Closure index */
                  std::uintptr_t table_size; /* Table size */
                  std::uint8_t fastcall_idx; /* Fastcall index */
                  std::uint8_t capture_ref;  /* Caputure refrence kind */
                  std::uint8_t capture_reg;  /* Capture register */
                  std::size_t import_idx;    /* Import index */
                  double integer;            /* Integer */
            };

            std::uintptr_t ref_addr = 0u;               /* Refrence address */
            std::string k_value = "";                   /* Seperate value to represent as a string and idx. Will also serve as import str. */
            lua_Type k_value_type = lua_Type::LUA_TNIL; /* If operand refrences kvalue index this will be its type. */
      };

      struct disassembly {

            /* Total calculated value based on regs, constants etc. Completly irrelavent too anything useful other than a loss function. */
            float total = 0.0f;

            std::uintptr_t addr = 0u;

            LuauOpcode op;

            const char *mnenomic = "";
            const char *hint = "";

            std::string data = "";
            std::uint8_t len = 0u;

            Instruction *code;

            std::vector<std::shared_ptr<operand>> operands;
            std::vector<std::uint8_t> bytes;

            /* Returns disassembly string. */
            template <bool include_hint = false>
            std::string disassemble() {

                  std::string result = std::to_string(this->addr) + " " + this->data;

                  if (include_hint) {
                        result += " " + std::string(this->hint);
                  }

                  return result;
            }
      };

      /* Disassemble address */
      void disassemble(const std::uintptr_t addr, const Proto *p, std::shared_ptr<disassembly> &buffer);

      /* Disassemble everything */
      void disassemble(const Proto *p, std::vector<std::shared_ptr<disassembly>> &buffer);

      namespace make_operand {

            __inline std::shared_ptr<luau_v3_disassembler::operand> make_operand_integer(const double v) {
                  auto operand = std::make_shared<luau_v3_disassembler::operand>();
                  operand->type = op_table::type::integer;
                  operand->integer = v;
                  return operand;
            }

            __inline std::shared_ptr<luau_v3_disassembler::operand> make_operand_val(const std::intptr_t v) {
                  auto operand = std::make_shared<luau_v3_disassembler::operand>();
                  operand->type = op_table::type::val;
                  operand->integer = static_cast<double>(v);
                  return operand;
            }

            __inline std::shared_ptr<luau_v3_disassembler::operand> make_operand_reg(const std::uint16_t v) {
                  auto operand = std::make_shared<luau_v3_disassembler::operand>();
                  operand->type = op_table::type::reg;
                  operand->reg = v;
                  return operand;
            }

      } // namespace make_operand

} // namespace luau_v3_disassembler