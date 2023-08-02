#include "resolver.hpp"

#if luramas_target == luramas_target_lua_53_6

void resolver::resolve_instructions(std::shared_ptr<luramas::il::ilang> &buffer) {

      /* Resolve instructions. */
      for (const auto &i : buffer->il) {

            switch (i->op) {

                  case luramas::il::arch::opcodes::OP_CCALL: {

                        /* Fix multret. */

                        const auto call = i->operands.front();

                        /* Fix args for multret. */
                        if (i->operands[1]->multret) {
                              i->operands[1]->dis.reg = i->operands[1]->dis.reg - call->dis.reg;
                        }

                        /* Fix args for multret. */
                        if (i->operands.back()->multret) {
                              i->operands.back()->dis.reg = i->operands.back()->dis.reg - call->dis.reg;
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

/* Resolves block data. */
void resolver::resolve_blocks(std::shared_ptr<luramas::il::ilang> &buffer) {

      std::uintptr_t pc = 0u;
      for (auto &il : buffer->il) {

            /* Fix lenght. */
            if (!il->len || il->data == nullptr) {
                  il->len = 1u;
            }

            il->addr = pc;

            /* Set total float. */
            if (il->total == 0.0f || il->data == nullptr) {
                  il->total = 0.25f; /* Static for not initialized totals. */
            } else if (il->data != nullptr) {
                  il->total = il->data->total;
            }

            pc += il->len;
      }

      return;
}

/* Resolves jump locations. */
void resolver::resolve_jumps(std::shared_ptr<luramas::il::ilang> &buffer, const std::vector<std::pair<std::shared_ptr<luramas::il::disassembly> /* Jumper */, std::shared_ptr<luramas::il::disassembly> /* Jump location */>> &map) {

      for (const auto &i : map) {

            for (const auto &operand : i.first->operands) {

                  if (operand->type == luramas::il::arch::operand::operand_kind::jmp) {
                        operand->ref_addr = i.second->addr;
                        operand->dis.jmp = signed(operand->ref_addr) - signed(i.first->addr);
                        break;
                  }
            }
      }

      return;
}

#endif