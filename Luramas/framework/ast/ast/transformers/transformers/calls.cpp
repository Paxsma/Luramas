#include "../transformers.hpp"

void luramas::ast::transformers::calls::set_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto calls = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_CCALL>(true));

      /* Set call info. */
      for (const auto &node : calls) {

            auto prev = ast->body->visit_previous_dest_register(node->address, node->lex->disassembly->operands.front()->dis.reg);

            /* Namecall gets special treatment. */
            if (prev->lex->disassembly->op == luramas::il::arch::opcodes::OP_SELF) {

                  const auto data = prev->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                  const auto data_1 = prev->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;

                  /* Call first operand will be the same as previous. */
                  if (data == data_1) {
                        ast->body->visit_previous_dest_register(prev->address, node->lex->disassembly->operands.front()->dis.reg)->add_expr<luramas::ast::element_kinds::call_routine_start>(); /* Call start */
                  } else {

                        const auto args = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.integer - 1u;

                        if (args) {
                              /* Set previous as call register + 2(1 is reserved, other is slot) */
                              ast->body->visit_previous_dest_register(node->address, node->lex->disassembly->operands.front()->dis.reg + 2u)->add_expr<luramas::ast::element_kinds::call_routine_start>(); /* Call start */
                        } else {
                              /* No args namecall is end. */
                              prev->add_expr<luramas::ast::element_kinds::call_routine_start>(); /* Call start */
                        }
                  }

            } else {

                  /* Next is end and has args fix prev. */
                  if (ast->body->visit_next(prev) == node && node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val) {

                        prev = ast->body->visit_previous_dest_register(prev->address, node->lex->disassembly->operands.front()->dis.reg + 1);
                  }

                  prev->add_expr<luramas::ast::element_kinds::call_routine_start>(); /* Call start */
            }

            node->add_expr<luramas::ast::element_kinds::call_routine_end>(); /* Call end. */
      }

      return;
}

void luramas::ast::transformers::calls::set_multret_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      std::intptr_t routine = 0;

      /* Checking for regs and vector reg. */
      bool first = false;
      std::vector<std::uint16_t> check_regs;

      const auto all = ast->body->visit_all();
      for (const auto &node : all) {

            /* Inc for concat start, call start, and table start. Dec for concat end, call end, and start end. */
            routine_inc_lv(node, routine);
            routine_dec_lv(node, routine);

            /* Call? */
            if (!routine && node->lex->kind == luramas::il::lexer::inst_kinds::ccall && !node->has_expr(luramas::ast::element_kinds::locvar)) {

                  auto retn = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().back()->dis.val;
                  const auto start = node->lex->disassembly->operands.front()->dis.reg;

                  if (retn > 1) {

                        /* Add multrets. */
                        for (auto i = start; i < (start + retn); ++i) {
                              check_regs.emplace_back(i);
                        }

                        first = true;
                  }
            }

            /* Looks for regs. */
            if (!check_regs.empty() && node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::source>()) {

                  const auto sources = node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>();
                  for (const auto &operand : sources) {

                        const auto reg = operand->dis.reg;
                        if (std::find(check_regs.begin(), check_regs.end(), reg) != check_regs.end()) {

                              /* First */
                              if (first) {
                                    node->add_expr<luramas::ast::element_kinds::call_multret_start>();
                                    first = false;
                              } else {

                                    if (check_regs.size() == 1u) {
                                          node->add_expr<luramas::ast::element_kinds::call_multret_end>();
                                    } else {
                                          node->add_expr<luramas::ast::element_kinds::call_multret_member>();
                                    }
                              }

                              check_regs.erase(std::remove(check_regs.begin(), check_regs.end(), reg), check_regs.end());
                        }
                  }
            }
      }

      return;
}

bool luramas::ast::transformers::calls::reg_arg(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &call, const std::uint16_t target) {

      /* Call */
      if (call->lex->kind == luramas::il::lexer::inst_kinds::ccall && call->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

            const auto dest = call->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
            const auto arg = call->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

            if (arg) {
                  return ((1u + dest) <= target && (1u + dest + arg) >= target);
            }
      }

      return false;
}