#pragma once
#include "../ast.hpp"

namespace luramas {

      namespace ast {

            namespace transformers {

                  namespace post {

                        namespace table {

                              /* Set indexs exprs for gettable instructions. */
                              void set_indexs(std::shared_ptr<luramas::ast::ast> &ast);

                              /* Double checks that every settable opcode in table routine is an element. */
                              void fill_elements(std::shared_ptr<luramas::ast::ast> &ast);

                        } // namespace table

                        namespace arith {

                              /* Sets arith exprs. */
                              void set_arith_exprs(std::shared_ptr<luramas::ast::ast> &ast);

                        } // namespace arith

                  } // namespace post

            } // namespace transformers

      } // namespace ast

} // namespace luramas