#include "../transformers.hpp"

void luramas::ast::transformers::flags::set_pops(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uint16_t> append_regs;

      auto all = ast->body->visit_all();
      for (auto &i : all) {

            /* Append for poparg. */
            if (i->lex->disassembly->op == luramas::il::arch::opcodes::OP_POPARG) {

                  append_regs.emplace_back(i->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg);

            } else if (i->lex->disassembly->op == luramas::il::arch::opcodes::OP_CCALL) {

                  /* Set for call. */
                  i->flags.poparg_flag.call_pop = append_regs;
                  append_regs.clear();
            }
      }

      return;
}