#include "transformers.hpp"

void luramas::il::transformers::eliminate::junk_code(const std::shared_ptr<luramas::il::ilang> &il) {

      const auto dis = il->mutate_dis();
      for (const auto &i : dis) {

            switch (i->op) {

                  case arch::opcodes::OP_NOP: {

                        /* No refrences? remove nop. */
                        if (il->visit_ref(i->addr).empty()) {
                              il->remove_dis(i);
                        }

                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      il->commit_dis();
      return;
}