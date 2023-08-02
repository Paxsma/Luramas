#include "../../transformers_post.hpp"

void luramas::ast::transformers::post::table::set_indexs(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            if (i->lex->kind == luramas::il::lexer::inst_kinds::table_get) {
                  i->add_expr<luramas::ast::element_kinds::table_index>();
            }
      }

      return;
}

void luramas::ast::transformers::post::table::fill_elements(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto table_starts = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_expr<luramas::ast::element_kinds::table_start>(true));
      for (const auto &i : table_starts) {

            const auto range = ast->body->visit_range_current(i->address, ast->body->visit_relative_next_expr_scope_current<luramas::ast::element_kinds::table_end>(i->address, {luramas::ast::element_kinds::table_start})->address);
            for (const auto &node : range) {

                  if (node->lex->kind == luramas::il::lexer::inst_kinds::set_table) {
                        node->add_existance<luramas::ast::element_kinds::table_element>();
                  }
            }
      }

      return;
}
