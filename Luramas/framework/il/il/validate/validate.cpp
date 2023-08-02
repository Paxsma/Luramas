#include "../il.hpp"

void luramas::il::ilang::validate(const char *const valid) {

      if (valid != NULL) {
            throw std::runtime_error(std::format("Uncommitted disassembly contains an error: {}", valid));
      }

      return;
}

const char *const luramas::il::ilang::validate_operands() {

      for (const auto &i : this->p_dis) {

            for (const auto &operand : i->operands) {

                  /* Refrence */
                  if (operand->type == il::arch::operand::operand_kind::jmp && i->ref == nullptr /* Jmp and ref is nullptr. */) {
                        return "Nullptr refrence for jump.";
                  }
            }
      }

      return NULL;
}

const char *const luramas::il::ilang::validate_instructions() {

      bool target_cmp = false; /* Hit cmp set instruction check for set or usage of cmp flag. */

      for (const auto &i : this->p_dis) {
      }

      return NULL;
}
