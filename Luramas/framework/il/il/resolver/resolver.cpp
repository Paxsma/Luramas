#include "../il.hpp"

void luramas::il::ilang::resolve_addresses() {

      std::uintptr_t addr = 0u;

      for (const auto &i : this->p_dis) {
            i->addr = addr++;
            i->len = 1u;
      }

      return;
}

void luramas::il::ilang::resolve_jumps() {

      for (const auto &i : this->p_dis) {
            for (const auto &operand : i->operands) {
                  if (operand->type == il::arch::operand::operand_kind::jmp) { /* Refrences */
                        /* Mutate address refrence */
                        operand->dis.jmp = i->ref->addr - i->addr;
                        operand->ref_addr = i->ref->addr;
                  }
            }
      }

      return;
}
