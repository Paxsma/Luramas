#include "../transformers.hpp"

void luramas::ast::transformers::flags::set_pops(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uint16_t> append_regs;

      auto all = ast->body->visit_all();
      for (auto &i : all) {

            switch (i->lex->disassembly->op) {

                  case luramas::il::arch::opcodes::OP_POPARG: {
                        append_regs.emplace_back(i->lex->operand_kind<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg);
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_CCALL: { /* Set for call. */
                        i->flags.poparg_flag.call_pop = append_regs;
                        append_regs.clear();
                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return;
}