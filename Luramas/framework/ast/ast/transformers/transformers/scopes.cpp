#include "../transformers.hpp"

std::shared_ptr<luramas::ast::node> luramas::ast::transformers::scopes::end_of_scope(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &start) {

      /* Check cached */
      if (global_cache::scopes::cached_ends.find(start) != global_cache::scopes::cached_ends.end()) {
            return global_cache::scopes::cached_ends[start]; /* Append */
      }

      std::vector<std::uintptr_t> addr_scopes; /* Scopes */

      std::intptr_t scope = 0;

      const auto all = ast->body->visit_rest(start->address);
      for (const auto &node : all) {

            /* End of scope. */
            if (!scope && (node->lex->kind == luramas::il::lexer::inst_kinds::return_ || node->lex->kind == luramas::il::lexer::inst_kinds::branch)) {
                  global_cache::scopes::cached_ends.insert(std::make_pair(start, node)); /* Append */
                  return node;
            }

            /* Track scopes. */
            if (std::find(addr_scopes.begin(), addr_scopes.end(), node->address) != addr_scopes.end()) {
                  scope -= std::count(addr_scopes.begin(), addr_scopes.end(), node->address);
            }

            /* Way out of scope.*/
            if (scope < 0) {
                  global_cache::scopes::cached_ends.insert(std::make_pair(start, node));
                  return node;
            }

            /* Add for loop. */
            const auto for_node = node->loops.end_node;
            if (for_node != nullptr && node->loops.start_node == node) {
                  addr_scopes.emplace_back(for_node->address);
            }

            /* Append scope */
            if (node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                  /* Scope isnt jumpback? */
                  const auto ref_addr = node->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                  if (ref_addr > node->address) {
                        const auto end = luramas::ast::transformers::scopes::end_of_scope(ast, node);
                        addr_scopes.emplace_back((end != nullptr) ? end->address : ref_addr);
                        ++scope;
                  }
            }
      }

      global_cache::scopes::cached_ends.insert(std::make_pair(start, nullptr)); /* Append */
      return nullptr;
}
