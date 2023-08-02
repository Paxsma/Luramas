#include "parser.hpp"
#include <unordered_map>
#include <vector>

#if luramas_target == luramas_target_lua_53_6

/* Parses instructions. */
void parser::parse_instruction(std::shared_ptr<luramas::il::ilang> &buffer, const std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> &dism, std::uint16_t &stack_top) {

      std::uintptr_t pc = 0u;
      for (auto idx = 0u; idx < dism.size(); ++idx) {

            const auto i = dism[idx];
            auto ptr = std::make_shared<luramas::il::disassembly>();

            auto op = luramas::il::arch::opcodes::OP_NOP;
            std::vector<std::shared_ptr<lua_53_6_disassembler::operand>> sorted_operands;

            std::uint16_t append_top = 0u; /* Append top at the end of operands. */

            /* Set data (Can not be optable because some stuff may need too resolved dynamically) */
            switch (i->op) {

                  default: {
                        break;
                  }
            }

            /* Data */
            ptr->op = op;
            ptr->len = i->len;
            ptr->data = i;
            ptr->addr = pc;

            /* Parse operands */
            for (const auto &oper : sorted_operands) {

                  auto operand_ptr = std::make_shared<luramas::il::arch::operand::operand>();

                  /* Data */
                  switch (oper->type) {

                        case op_table::type::reg: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::reg;
                              operand_ptr->dis.reg = oper->reg;
                              break;
                        }

                        case op_table::type::dest: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::reg;
                              operand_ptr->dis.reg = oper->reg;
                              append_top = operand_ptr->dis.reg;
                              break;
                        }

                        case op_table::type::val: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::value;
                              operand_ptr->dis.val = oper->val;
                              break;
                        }

                        case op_table::type::val_multret: {

                              operand_ptr->type = luramas::il::arch::operand::operand_kind::value;
                              operand_ptr->dis.val = oper->val;

                              /* Multret value. */
                              if (operand_ptr->dis.val == LUA_MULTRET) {
                                    operand_ptr->dis.val = stack_top;
                                    operand_ptr->multret = true;
                              }

                              break;
                        }

                        case op_table::type::upvalue: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::upvalue;
                              operand_ptr->dis.upvalue_idx = oper->upvalue;
                              break;
                        }

                        case op_table::type::k_idx:
                        case op_table::type::k_idx_pp: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::kval;
                              operand_ptr->k_value = oper->k_value;
                              operand_ptr->dis.kvalue_idx = oper->k_idx;
                              break;
                        }

                        case op_table::type::table_size: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::table_size;
                              operand_ptr->dis.table_size = oper->table_size;
                              break;
                        }

                        case op_table::type::jmp: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::jmp;
                              operand_ptr->dis.jmp = oper->jmp;
                              operand_ptr->ref_addr = oper->ref_addr;
                              break;
                        }

                        default: {
                              throw std::runtime_error("Unkown operand type.");
                        }
                  }

                  /* Append */
                  ptr->operands.emplace_back(operand_ptr);
            }

            buffer->il.emplace_back(ptr);

            /* Ignore for self. */
            if (ptr->op != luramas::il::arch::opcodes::OP_SELF || ptr->operands.front()->dis.reg != ptr->operands[1]->dis.reg) { /* Namecall with same dest and source dont get logged. */
                  stack_top = append_top;
            }

            pc += ptr->len;
      }

      return;
}

/* Maps out jumps too be fixed for there relatives. */
std::vector<std::pair<std::shared_ptr<luramas::il::disassembly> /* Jumper */, std::shared_ptr<luramas::il::disassembly> /* Jump location */>> parser::parse_jumps(std::shared_ptr<luramas::il::ilang> &il) {

      std::vector<std::pair<std::shared_ptr<luramas::il::disassembly> /* Jumper */, std::shared_ptr<luramas::il::disassembly> /* Jump location */>> retn;

      for (auto &node : il->il) {

            for (auto operand : node->operands) {

                  if (operand->type == luramas::il::arch::operand::operand_kind::jmp) {
                        retn.emplace_back(std::make_pair(node, il->visit_addr(operand->ref_addr)));
                  }
            }
      }

      return retn;
}

#endif