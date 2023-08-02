#pragma once
#include "transformers.hpp"
#include "transformers_post.hpp"

namespace luramas {

      namespace ast {

            namespace transformers {

                  /* Appends default transformers to ast. */
                  __inline void add_default_transformers(std::shared_ptr<luramas::ast::ast> &ast) {

#pragma region passthroughs

                        ast->add("nodes set exit exprs", luramas::ast::transformers::nodes::set_exit_exprs);
                        ast->add("nodes set binary operation kinds", luramas::ast::transformers::nodes::set_bin_kinds);
                        ast->add("tables set exprs", luramas::ast::transformers::tables::set_table_exprs);
                        ast->add("loops set for prep exprs", luramas::ast::transformers::loops::set_for_prep_exprs);
                        ast->add("flags set pop registers", luramas::ast::transformers::flags::set_pops);
                        ast->add("arguements set children arguements", luramas::ast::transformers::arguments::set);
                        ast->add("branches set jumpout exprs", luramas::ast::transformers::branches::set_jumpout_exprs);
                        ast->add("loops set for routines", luramas::ast::transformers::loops::set_for_routines);
                        ast->add("registers set source scope exprs", luramas::ast::transformers::registers::set_source_scope_expr);
                        ast->add("calls set call routines", luramas::ast::transformers::calls::set_routines);
                        ast->add("concats set concat routines", luramas::ast::transformers::concats::set_routines);
                        ast->add("registers set statement exprs", luramas::ast::transformers::registers::set_statement_expr);
                        ast->add("tables set table routines", luramas::ast::transformers::tables::set_routines);
                        ast->add("tables set table node end", luramas::ast::transformers::tables::set_node_end);
                        ast->add("branches set valid routines", luramas::ast::transformers::branches::set_valid_branch_routine);
                        ast->add("loops set loop start/end routines", luramas::ast::transformers::loops::set_loop_startend_routines);
                        ast->add("locvars set logical operations", luramas::ast::transformers::locvars::set_logical_operations);
                        ast->add("logicals set logical expressions", luramas::ast::transformers::logicals::set_logical_expression);
                        ast->add("branches set branch statements", luramas::ast::transformers::branches::set_branch_statements);
                        ast->add("locvars set scoped variables", luramas::ast::transformers::locvars::set_lv);
                        ast->add("upvalues set upvalue statements", luramas::ast::transformers::upvalues::set);
                        ast->add("loops sort loops", luramas::ast::transformers::loops::sort_loops);
                        ast->add("calls set multiple return call routine", luramas::ast::transformers::calls::set_multret_routines);

#pragma endregion

#pragma region post_passthroughs

                        ast->add("post table set index exprs", luramas::ast::transformers::post::table::set_indexs);
                        ast->add("post table fill elements", luramas::ast::transformers::post::table::fill_elements);
                        ast->add("post arith exprs", luramas::ast::transformers::post::arith::set_arith_exprs);

#pragma endregion

                        return;
                  }

            } // namespace transformers

      } // namespace ast

} // namespace luramas