#include "parser.hpp"
#include "../../../../../il/architecture/emitter.hpp"
#include <unordered_map>
#include <vector>

#if luramas_target == luramas_target_luau_v3

/* Parse compare instruction. */
void luau_v3_parsers::parse_compares(luramas::il::lifter::parser::parser_manager<luau_v3_disassembler::disassembly> &pm) {

      for (auto i = 0u; i < pm.dism.size(); ++i) {

            const auto curr = pm.dism[i];

            /* Jump with compare == logical compare. */
            const auto setcmp = (pm.dism.back() != curr && pm.dism[i + 1u]->op == LuauOpcode::LOP_LOADB && pm.dism[i + 1u]->operands.back()->jmp && pm.dism[i + 2u]->op == LuauOpcode::LOP_LOADB);

            switch (curr->op) {

                  case LuauOpcode::LOP_JUMPIF: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMPS>(pm.il, curr->addr, ptr, curr->operands.front()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIF>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIF>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFNOT: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMPS>(pm.il, curr->addr, ptr, curr->operands.front()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFNOT>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFNOT>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFEQ: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFEQUAL>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands[1u]->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFEQUAL>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFLE: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFLESSEQUAL>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFLT: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFLESS>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFLESS>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFNOTEQ: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFNOTEQUAL>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFNOTLE: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  case LuauOpcode::LOP_JUMPIFNOTLT: {

                        auto ptr = std::make_shared<luramas::il::disassembly>();
                        luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_CMP>(pm.il, curr->addr, ptr, curr->operands.front()->reg, curr->operands.back()->reg);
                        pm.append(i, ptr);

                        if (setcmp) {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_SETIFGREATER>(pm.il, curr->addr, ptr, pm.dism[i + 1u]->operands.front()->reg);
                              pm.append(i, ptr);
                              pm.remove(i + 1u);
                              pm.remove(i + 2u);
                        } else {
                              ptr = std::make_shared<luramas::il::disassembly>();
                              ptr->ref = pm.il->visit_addr(curr->operands.back()->ref_addr);
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_JUMPIFGREATER>(pm.il, curr->addr, ptr, ptr->ref->addr);
                              pm.append(i, ptr);
                        }

                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return;
}

/* Inits parser, does not remove or add any new instructions, everything is alligned.*/
void luau_v3_parsers::parse_instructions(luramas::il::lifter::parser::parser_manager<luau_v3_disassembler::disassembly> &pm) {

      std::uint16_t stack_top = 0u;
      std::uintptr_t pc = 0u;
      for (auto idx = 0u; idx < pm.dism.size(); ++idx) {

            const auto i = pm.dism[idx];
            auto ptr = std::make_shared<luramas::il::disassembly>();

            auto op = luramas::il::arch::opcodes::OP_NOP;
            std::vector<std::shared_ptr<luau_v3_disassembler::operand>> sorted_operands;

            std::uint16_t append_top = 0u; /* Append top at the end of operands. */

            /* Set data (Can not be optable because some stuff may need too resolved dynamically) */
            switch (i->op) {

                  case LuauOpcode::LOP_LOADNIL: {
                        op = luramas::il::arch::opcodes::OP_LOADNONE;
                        sorted_operands = {i->operands.front()};
                        break;
                  }
                  case LuauOpcode::LOP_LOADB: {
                        op = luramas::il::arch::opcodes::OP_LOADBOOL;
                        sorted_operands = {i->operands.front(), i->operands[1]};
                        break;
                  }
                  case LuauOpcode::LOP_LOADN: {
                        op = luramas::il::arch::opcodes::OP_LOADINT;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_LOADK: {
                        op = luramas::il::arch::opcodes::OP_LOADKVAL;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_LOADKX: {
                        op = luramas::il::arch::opcodes::OP_LOADKVAL;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_MOVE: {
                        op = luramas::il::arch::opcodes::OP_MOVE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_GETGLOBAL: {
                        op = luramas::il::arch::opcodes::OP_LOADGLOBAL;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETGLOBAL: {
                        op = luramas::il::arch::opcodes::OP_SETGLOBAL;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_GETUPVAL: {
                        op = luramas::il::arch::opcodes::OP_GETUPVALUE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETUPVAL: {
                        op = luramas::il::arch::opcodes::OP_SETUPVALUE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_GETTABLE: {
                        op = luramas::il::arch::opcodes::OP_GETTABLE;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETTABLE: {
                        op = luramas::il::arch::opcodes::OP_SETTABLE;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_GETTABLEN: {
                        op = luramas::il::arch::opcodes::OP_GETTABLEN;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETTABLEN: {
                        op = luramas::il::arch::opcodes::OP_SETTABLEN;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_GETTABLEKS: {
                        op = luramas::il::arch::opcodes::OP_GETTABLEK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETTABLEKS: {
                        op = luramas::il::arch::opcodes::OP_SETTABLEK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_ADD: {
                        op = luramas::il::arch::opcodes::OP_ADD;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SUB: {
                        op = luramas::il::arch::opcodes::OP_SUB;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_MUL: {
                        op = luramas::il::arch::opcodes::OP_MUL;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_DIV: {
                        op = luramas::il::arch::opcodes::OP_DIV;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_MOD: {
                        op = luramas::il::arch::opcodes::OP_MOD;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_POW: {
                        op = luramas::il::arch::opcodes::OP_POW;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_AND: {
                        op = luramas::il::arch::opcodes::OP_AND;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_OR: {
                        op = luramas::il::arch::opcodes::OP_OR;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_ADDK: {
                        op = luramas::il::arch::opcodes::OP_ADDK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SUBK: {
                        op = luramas::il::arch::opcodes::OP_SUBK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_MULK: {
                        op = luramas::il::arch::opcodes::OP_MULK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_DIVK: {
                        op = luramas::il::arch::opcodes::OP_DIVK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_MODK: {
                        op = luramas::il::arch::opcodes::OP_MODK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_POWK: {
                        op = luramas::il::arch::opcodes::OP_POWK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_ANDK: {
                        op = luramas::il::arch::opcodes::OP_ANDK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_ORK: {
                        op = luramas::il::arch::opcodes::OP_ORK;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_CONCAT: {
                        op = luramas::il::arch::opcodes::OP_CONCAT;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_CALL: {
                        op = luramas::il::arch::opcodes::OP_CCALL;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_RETURN: {
                        op = luramas::il::arch::opcodes::OP_RETURN;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_MINUS: {
                        op = luramas::il::arch::opcodes::OP_MINUS;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_LENGTH: {
                        op = luramas::il::arch::opcodes::OP_LEN;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_NOT: {
                        op = luramas::il::arch::opcodes::OP_NOT;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_CAPTURE: {
                        op = luramas::il::arch::opcodes::OP_ADDUPVALUE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_CLOSEUPVALS: {
                        op = luramas::il::arch::opcodes::OP_DESTROYUPVALUESA;
                        sorted_operands = {i->operands.front()};
                        break;
                  }

                  case LuauOpcode::LOP_NEWCLOSURE: {
                        op = luramas::il::arch::opcodes::OP_NEWCLOSURE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_DUPCLOSURE: {
                        op = luramas::il::arch::opcodes::OP_REFCLOSURE;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_NAMECALL: {
                        op = luramas::il::arch::opcodes::OP_SELF;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_NEWTABLE: {
                        op = luramas::il::arch::opcodes::OP_NEWTABLEA;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_DUPTABLE: {
                        op = luramas::il::arch::opcodes::OP_REFTABLEA;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_SETLIST: {
                        op = luramas::il::arch::opcodes::OP_SETLIST;
                        sorted_operands = {i->operands.front(), i->operands[1], i->operands[2]};
                        break;
                  }

                  case LuauOpcode::LOP_GETVARARGS: {
                        op = luramas::il::arch::opcodes::OP_GETVARARGS;
                        sorted_operands = {i->operands.front(), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_PREPVARARGS: {
                        op = luramas::il::arch::opcodes::OP_INITVARARGS;
                        break;
                  }

                  case LuauOpcode::LOP_FORNPREP: {
                        op = luramas::il::arch::opcodes::OP_INITFORLOOPN;
                        sorted_operands = {i->operands.back()};
                        break;
                  } // FIX FIX FIX
                  case LuauOpcode::LOP_FORGPREP_INEXT: {
                        op = luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL;
                        sorted_operands = {i->operands.front()};
                        break;
                  }
                  case LuauOpcode::LOP_FORGPREP_NEXT: {
                        op = luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL;
                        sorted_operands = {i->operands.front()};
                        break;
                  }
                  case LuauOpcode::LOP_FORGPREP: {
                        op = luramas::il::arch::opcodes::OP_INITFORLOOPG;
                        sorted_operands = {i->operands.back()};
                        break;
                  }

                  case LuauOpcode::LOP_FORNLOOP: {
                        /* CONTAINS STATICALLY TYPED OPERANDS (FOLLOWS FORMAT) */
                        op = luramas::il::arch::opcodes::OP_FORLOOPN;
                        sorted_operands = {luau_v3_disassembler::make_operand::make_operand_reg(i->operands.front()->reg + 2u), i->operands.front(), luau_v3_disassembler::make_operand::make_operand_reg(i->operands.front()->reg + 1u), i->operands.back()};
                        break;
                  }
                  case LuauOpcode::LOP_FORGLOOP: {
                        op = luramas::il::arch::opcodes::OP_FORLOOPG;
                        sorted_operands = {i->operands.front(), i->operands.back(), i->operands[1]};
                        break;
                  }

                  case LuauOpcode::LOP_GETIMPORT: {
                        op = luramas::il::arch::opcodes::OP_GETTABUPVALUE;
                        sorted_operands = {i->operands.front(), i->operands[1]};
                        break;
                  }

                  case LuauOpcode::LOP_JUMPBACK: {
                        op = luramas::il::arch::opcodes::OP_JUMPBACK;
                        sorted_operands = {i->operands.front()};
                        break;
                  }
                  case LuauOpcode::LOP_JUMP: {
                        op = (i->operands.back()->jmp >= 0) ? luramas::il::arch::opcodes::OP_JUMP : luramas::il::arch::opcodes::OP_JUMPBACK;
                        sorted_operands = {i->operands.front()};
                        break;
                  }
                  case LuauOpcode::LOP_JUMPX: {
                        op = (i->operands.back()->jmp >= 0) ? luramas::il::arch::opcodes::OP_JUMP : luramas::il::arch::opcodes::OP_JUMPBACK;
                        sorted_operands = {i->operands.front()};
                        break;
                  }

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

                        case op_table::type::capture_idx: {

                              operand_ptr->type = luramas::il::arch::operand::operand_kind::upvalue_kind;

                              /* LuaU capture types. */
                              switch (oper->capture_ref) {

                                    case LuauCaptureType::LCT_UPVAL:
                                    case LuauCaptureType::LCT_REF: {
                                          operand_ptr->dis.upvalue_kind = luramas::il::arch::operand::upvalue_kind::reg;
                                          break;
                                    }

                                    case LuauCaptureType::LCT_VAL: {
                                          operand_ptr->dis.upvalue_kind = luramas::il::arch::operand::upvalue_kind::contents;
                                          break;
                                    }
                              }

                              break;
                        }

                        case op_table::type::capture_ref: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::upvalue_kind;
                              operand_ptr->dis.upvalue_reg = oper->capture_reg;
                              break;
                        }

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

                        case op_table::type::integer: {

                              operand_ptr->type = luramas::il::arch::operand::operand_kind::integer;
                              operand_ptr->dis.integer = oper->val;

                              /* String and remove trailing 0's */
                              operand_ptr->integer_str = std::to_string(operand_ptr->dis.integer);
                              operand_ptr->integer_str.erase(operand_ptr->integer_str.find_last_not_of('0') + 1, std::string::npos);
                              operand_ptr->integer_str.erase(operand_ptr->integer_str.find_last_not_of('.') + 1, std::string::npos);

                              break;
                        }

                        case op_table::type::import_idx:
                        case op_table::type::k_value_nstr: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::kval_embeded;
                              operand_ptr->k_value = oper->k_value;
                              operand_ptr->dis.kvalue_idx = oper->k_idx;
                              break;
                        }

                        case op_table::type::closure: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::closure_index;
                              operand_ptr->dis.closure_idx = oper->closure;
                              break;
                        }
                        case op_table::type::upvalue: {
                              operand_ptr->type = luramas::il::arch::operand::operand_kind::upvalue;
                              operand_ptr->dis.upvalue_idx = oper->upvalue;
                              break;
                        }

                        case op_table::type::k_idx:
                        case op_table::type::k_idx_pp:
                        case op_table::type::k_idx_aux: {
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

            pm.il->append_dis(ptr);

            /* Ignore for self. */
            if (ptr->op != luramas::il::arch::opcodes::OP_SELF || ptr->operands.front()->dis.reg != ptr->operands[1]->dis.reg) { /* Namecall with same dest and source dont get logged. */
                  stack_top = append_top;
            }

            pc += ptr->len;
      }

      return;
};

#endif