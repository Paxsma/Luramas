#include "../il.hpp"

void throw_error(luramas::il::ilang *il, const luramas::il::commit_error &valid) {
      throw std::runtime_error(std::format("Uncommitted disassembly contains an error: {}, On address: {}, Dump:\n{}\n", valid.error, std::to_string(valid.il->addr), il->dump()));
}

void luramas::il::ilang::validate(const luramas::il::commit_error &valid) {
      if (!this->dis.empty() && valid.is_error) {
            throw_error(this, valid);
      }
      return;
}

luramas::il::commit_error luramas::il::ilang::validate_operands() {

      luramas::il::commit_error result;

      for (const auto &i : this->p_dis) {

            for (const auto &operand : i->operands) {

                  /* Refrence */
                  if (operand->type == il::arch::operand::operand_kind::jmp && i->ref == nullptr /* Jmp and ref is nullptr. */) {
                        result.commit(i, "Nullptr refrence for jump.");
                  }
            }
      }

      return result;
}

luramas::il::commit_error luramas::il::ilang::validate_instructions() {

      luramas::il::commit_error result;
      bool target_cmp = false; /* Hit cmp set instruction check for set or usage of cmp flag. */

      for (const auto &i : this->p_dis) {
      }

      return result;
}
