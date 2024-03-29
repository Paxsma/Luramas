#include "../transformers.hpp"

void luramas::ast::transformers::concats::set_string_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto concats = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst<luramas::il::arch::opcodes::OP_CONCAT>(true));

      /* Set concat info. */
      for (const auto &node : concats) {
            ast->body->visit_previous_dest_register(node->address, node->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg)->add_safe<luramas::ast::element_kinds::desc_concat_routine_start>(); /* Concat start. */
            node->add_safe<luramas::ast::element_kinds::desc_concat_routine_end>();                                                                                                                                            /* Concat end. */
      }

      return;
}
