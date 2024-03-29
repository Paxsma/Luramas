#include "../../transformers_post.hpp"
#include <tuple>

void luramas::ast::transformers::post::arith::set_arith_exprs(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_inst_kinds<luramas::il::lexer::inst_kinds::arith>(true));
      for (const auto &i : all) {

            if (i->lex->has_operand_kind<luramas::il::lexer::operand_kinds::kvalue>()) {
                  i->add_elem<luramas::ast::element_kinds::expr_arithk>();
            } else {
                  i->add_elem<luramas::ast::element_kinds::expr_arith>();
            }
      }

      return;
}