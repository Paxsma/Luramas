#include "resolver.hpp"
#include "../../../../../il/architecture/emitter.hpp"

#if luramas_target == luramas_target_luau_v3

void luau_v3_resolvers::resolve_instruction_operands(luramas::il::lifter::resolver::resolver_manager<Proto> &pm) {

      /* Resolve instructions. */
      for (const auto &i : pm.il->dis) {

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

/* Resolves jump locations operands. */
void luau_v3_resolvers::resolve_jump_operands(luramas::il::lifter::resolver::resolver_manager<Proto> &rm) {

      for (const auto &i : rm.il->mutate_dis()) {

            for (const auto &operand : i->operands) {

                  if (operand->type == luramas::il::arch::operand::operand_kind::jmp) {
                        operand->ref_addr = i->ref->addr;
                        operand->dis.jmp = signed(operand->ref_addr) - signed(i->addr);
                        break;
                  }
            }
      }

      return;
}

/* Resolves instructions. */
void luau_v3_resolvers::resolve_instructions(luramas::il::lifter::resolver::resolver_manager<Proto> &rm) {

      /* Emit instruction. */
      auto dis = rm.il->mutate_dis();
      for (auto i = 0u; i < dis.size(); ++i) {

            auto il = dis[i];

            switch (il->op) {

                  case luramas::il::arch::opcodes::OP_SELF: {

                        const auto dest = il->operands.front()->dis.reg;

                        /* Dest == source emit dest too pop. */
                        if (dest == il->operands[1]->dis.reg) {
                              auto ptr = std::make_shared<luramas::il::disassembly>();
                              luramas::il::emitter::emit_opcode<luramas::il::arch::opcodes::OP_POPARG>(rm.il, il->addr, ptr, dest);
                              rm.append(i, ptr);
                        }

                        break;
                  }

                  default: {
                        break;
                  }
            }
      }
}
#endif