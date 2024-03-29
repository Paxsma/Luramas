#include "../transformers.hpp"

void luramas::ast::transformers::returns::set(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            if (i->lex->kind == luramas::il::lexer::inst_kinds::return_) {
                  i->add_safe<luramas::ast::element_kinds::desc_return>();
            }
      }

      return;
}
