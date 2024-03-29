#include "../transformers.hpp"

void luramas::ast::transformers::nodes::set_exit_exprs(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uintptr_t> scopes;

      bool hit_deadcode = false; /* Code after return that never gets runned. */

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            /* Hit */
            if (hit_deadcode) {
                  i->add_safe<luramas::ast::element_kinds::desc_exit_dead>();
            }

            /* Append */
            if ((i->lex->kind == luramas::il::lexer::inst_kinds::branch || i->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) && i->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp >= 0) {
                  scopes.emplace_back(i->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
            }

            /* Found */
            if (!scopes.empty() && std::find(scopes.begin(), scopes.end(), i->address) != scopes.end()) {
                  scopes.erase(std::remove(scopes.begin(), scopes.end(), i->address), scopes.end());
            }

            /* End */
            if (scopes.empty() && i->lex->kind == luramas::il::lexer::inst_kinds::return_) {
                  i->add_safe<luramas::ast::element_kinds::desc_exit_post>();
                  hit_deadcode = true;
            }
      }

      return;
}

void luramas::ast::transformers::nodes::set_bin_kinds(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            switch (i->lex->disassembly->op) {

                  case luramas::il::arch::opcodes::OP_ADD:
                  case luramas::il::arch::opcodes::OP_ADDK:
                  case luramas::il::arch::opcodes::OP_ADDN: {
                        i->bin_kind = luramas::ast::bin_kinds::add_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_SUB:
                  case luramas::il::arch::opcodes::OP_SUBK:
                  case luramas::il::arch::opcodes::OP_SUBN: {
                        i->bin_kind = luramas::ast::bin_kinds::sub_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_MUL:
                  case luramas::il::arch::opcodes::OP_MULK:
                  case luramas::il::arch::opcodes::OP_MULN: {
                        i->bin_kind = luramas::ast::bin_kinds::mul_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_DIV:
                  case luramas::il::arch::opcodes::OP_DIVK:
                  case luramas::il::arch::opcodes::OP_DIVN: {
                        i->bin_kind = luramas::ast::bin_kinds::div_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_MOD:
                  case luramas::il::arch::opcodes::OP_MODK:
                  case luramas::il::arch::opcodes::OP_MODN: {
                        i->bin_kind = luramas::ast::bin_kinds::mod_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_POW:
                  case luramas::il::arch::opcodes::OP_POWK:
                  case luramas::il::arch::opcodes::OP_POWN: {
                        i->bin_kind = luramas::ast::bin_kinds::pow_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_AND:
                  case luramas::il::arch::opcodes::OP_ANDK:
                  case luramas::il::arch::opcodes::OP_ANDN: {
                        i->bin_kind = luramas::ast::bin_kinds::and_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_XOR:
                  case luramas::il::arch::opcodes::OP_XORK:
                  case luramas::il::arch::opcodes::OP_XORN: {
                        i->bin_kind = luramas::ast::bin_kinds::xor_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_SHL:
                  case luramas::il::arch::opcodes::OP_SHLK:
                  case luramas::il::arch::opcodes::OP_SHLN: {
                        i->bin_kind = luramas::ast::bin_kinds::shl_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_SHR:
                  case luramas::il::arch::opcodes::OP_SHRK:
                  case luramas::il::arch::opcodes::OP_SHRN: {
                        i->bin_kind = luramas::ast::bin_kinds::shr_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_IDIV:
                  case luramas::il::arch::opcodes::OP_IDIVK:
                  case luramas::il::arch::opcodes::OP_IDIVN: {
                        i->bin_kind = luramas::ast::bin_kinds::idiv_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_OR:
                  case luramas::il::arch::opcodes::OP_ORK:
                  case luramas::il::arch::opcodes::OP_ORN: {
                        i->bin_kind = luramas::ast::bin_kinds::or_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_LEN: {
                        i->bin_kind = luramas::ast::bin_kinds::len_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_NOT: {
                        i->bin_kind = luramas::ast::bin_kinds::not_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_MINUS: {
                        i->bin_kind = luramas::ast::bin_kinds::minus_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_BITNOT: {
                        i->bin_kind = luramas::ast::bin_kinds::bitnot_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_PLUS: {
                        i->bin_kind = luramas::ast::bin_kinds::plus_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFEQUAL:
                  case luramas::il::arch::opcodes::OP_SETIFEQUAL: {
                        i->bin_kind = luramas::ast::bin_kinds::eq_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL:
                  case luramas::il::arch::opcodes::OP_SETIFNOTEQUAL: {
                        i->bin_kind = luramas::ast::bin_kinds::ne_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFLESS:
                  case luramas::il::arch::opcodes::OP_SETIFLESS: {
                        i->bin_kind = luramas::ast::bin_kinds::lt_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL:
                  case luramas::il::arch::opcodes::OP_SETIFLESSEQUAL: {
                        i->bin_kind = luramas::ast::bin_kinds::lte_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFGREATER:
                  case luramas::il::arch::opcodes::OP_SETIFGREATER: {
                        i->bin_kind = luramas::ast::bin_kinds::gt_;
                        break;
                  }

                  case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL:
                  case luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL: {
                        i->bin_kind = luramas::ast::bin_kinds::gte_;
                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return;
}