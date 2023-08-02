#pragma once
#include "../../../ast/ast/ast.hpp"
#include "../../../ir/ir.hpp"

namespace luramas {

      namespace ir {

            namespace lifter {

                  /* Lifts AST too IR. */
                  luramas::ir::lines lift(const std::shared_ptr<luramas::ast::ast> &main_ast, const std::shared_ptr<lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations);

            } // namespace lifter

      } // namespace ir

} // namespace luramas