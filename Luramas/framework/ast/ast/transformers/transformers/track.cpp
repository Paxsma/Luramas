#include "../transformers.hpp"

void luramas::ast::transformers::track::set(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();

      luramas::ast::transformers::scopes::scope_data::scope scope(ast, true);
      scope = all;

      luramas::ast::transformers::scopes::scope_data::scope_vector<std::uint32_t> dests(scope);                             /* (Scoped) Registers used in dest. **getting written too** */
      luramas::ast::transformers::scopes::scope_data::scope_vector<std::shared_ptr<luramas::ast::node>> dests_nodes(scope); /* (INIT) Dest nodes relative to dests (Can't be pair as too this is used for something entirely different from arguments) */

      for (const auto &node : all) {

            /* Fix scope */
            dests[node];
            dests_nodes[node];

            /* See if theres prep for for loop then add if so. */
            bool mutated = false;
            auto target_node = node;
            if (!target_node->has_elem(luramas::ast::element_kinds::stat_for_prep)) {

                  const auto prev = ast->body->visit_previous_addr(target_node->address);
                  if (prev != nullptr && prev->has_elem(luramas::ast::element_kinds::stat_for_prep)) {
                        target_node = prev;
                        mutated = true;
                  }
            }

            /* Append dests */
            for (const auto dest : node->extract_dest_regs()) {

                  /* Set target nodes. */
                  if (!dests.find(dest)) { /* Didn't find. */

                        /* Assign */
                        dests.push_back(dest);

                        dests_nodes.push_back(target_node);

                  } else { /* Found dest, mutate dest node. */

                        if (mutated) {

                              dests_nodes.idx_set(dests.index(dest), target_node);

                        } else {

                              node->dest_nodes_init.emplace_back(dests_nodes.idx_get(dests.index(dest)));
                        }
                  }
            }

            /* Add for move */
            if (node->lex->disassembly->op == luramas::il::arch::opcodes::OP_MOVE) {

                  const auto reg = node->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
                  if (dests.find(reg)) {
                        node->sub_node = dests_nodes.idx_get(dests.index(reg));
                  }
            }
      }

      return;
}