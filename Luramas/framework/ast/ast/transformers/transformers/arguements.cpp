#include "../transformers.hpp"

void luramas::ast::transformers::arguments::set(std::shared_ptr<luramas::ast::ast> &ast) {

      auto set_args = [&](std::shared_ptr<luramas::ast::ast> &closure, const bool dont_set = false /* Used for main moves. */) mutable {
            /* Already been analyzed. */
            if (!closure->arg_regs.empty()) {
                  return;
            }

            const auto all = closure->body->visit_all();

            luramas::ast::transformers::scopes::scope_data::scope scope(closure, true);
            scope = all;

            luramas::ast::transformers::scopes::scope_data::scope_vector<std::uint32_t> dests(scope);                                                                           /* (Scoped) Registers used in dest. **getting written too** */
            luramas::ast::transformers::scopes::scope_data::scope_vector<std::shared_ptr<luramas::ast::node>> dests_nodes(scope);                                               /* (INIT) Dest nodes relative to dests (Can't be pair as too this is used for something entirely different from arguments) */
            luramas::ast::transformers::scopes::scope_data::scope_unorderedmap<std::uint16_t /* Reg */, std::shared_ptr<luramas::ast::node> /* Node */> dests_nodes_map(scope); /* When reg was last set. */
            std::vector<std::uint32_t> source_no_dest;                                                                                                                          /* Registers used in source, value but not dest. **Not written too yet but been used** */
            std::vector<std::uint32_t> dests_not_scoped;                                                                                                                        /* (NOT-Scoped) Registers used in dest. */

            for (const auto &node : all) {

                  /* Fix scope */
                  dests[node];
                  dests_nodes[node];
                  dests_nodes_map[node];

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

                  /* Append sources */
                  for (const auto source : node->extract_source_regs()) {

                        if (dests_nodes_map.find(source)) {

                              node->source_nodes.emplace_back(dests_nodes_map.get().front()[source]);
                        }

                        /* Append unused reg. */
                        if (!dests.find(source) && std::find(source_no_dest.begin(), source_no_dest.end(), source) == source_no_dest.end()) {

                              source_no_dest.emplace_back(source);

                        } else if (dests.find(source)) {

                              node->source_nodes_init.emplace_back(dests_nodes.idx_get(dests.index(source)));
                        }
                  }

                  /* Append dests */
                  for (const auto dest : node->extract_dest_regs()) {

                        /* Add node too dest_nodes_map. */
                        if (dests_nodes_map.find(dest)) {

                              dests_nodes_map.set_all_idx(dest, node);

                        } else {

                              dests_nodes_map.insert(std::make_pair(dest, node));
                        }

                        /* Set target nodes. */
                        if (!dests.find(dest)) { /* Didn't find. */

                              /* Assign */
                              dests.push_back(dest);

                              dests_not_scoped.emplace_back(dest);

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

            /* Used for main skips arguments set. */
            if (dont_set) {
                  return;
            }

            std::size_t max = 0u; /* Find max. */
            const auto min = 0;   /* Args are on bottom of stack. */

            /* Max could be wrong. See if stack is consitance if theres a hole dec and use that as max. */
            if (!dests_not_scoped.empty()) {
                  std::sort(dests_not_scoped.begin(), dests_not_scoped.end());
                  dests_not_scoped.erase(std::unique(dests_not_scoped.begin(), dests_not_scoped.end()), dests_not_scoped.end());
                  std::sort(dests_not_scoped.begin(), dests_not_scoped.end());
            }

            /* Check for holes. */
            std::int16_t hole_target = 0;
            if (!dests_not_scoped.empty()) {

                  /* Max hole in dest. */
                  auto start = dests_not_scoped.front();
                  for (auto i = 0u; i < dests_not_scoped.size(); ++i) {
                        if ((i + 1u) != dests_not_scoped.size() && dests_not_scoped[i + 1u] != (dests_not_scoped[i] + 1u)) {
                              start = dests_not_scoped[i + 1u];
                        }
                  }

                  hole_target = (start) ? (start - 1) : -1;
            }

            /* No dests->source */
            if (source_no_dest.empty()) {

                  /* No dests */
                  if (dests_not_scoped.empty()) {
                        return;
                  }

                  /* Nothing */
                  if (hole_target < 0) {
                        return;
                  }

                  /* Check front for hole. */
                  if (!std::binary_search(source_no_dest.begin(), source_no_dest.end(), static_cast<std::uint32_t>(hole_target))) {
                        max = hole_target;
                  }

            } else {

                  /* Remove dupes */
                  std::sort(source_no_dest.begin(), source_no_dest.end());
                  source_no_dest.erase(std::unique(source_no_dest.begin(), source_no_dest.end()), source_no_dest.end());

                  max = *std::max_element(source_no_dest.begin(), source_no_dest.end());

                  /* Check for holes. */
                  if (hole_target != -1) {

                        /* Check front for hole. */
                        if (!std::binary_search(source_no_dest.begin(), source_no_dest.end(), static_cast<std::uint32_t>(hole_target))) {
                              max = hole_target;
                        }
                  }
            }

            /* Set args (fill stack) */
            for (auto reg = min; reg <= max; reg++)
                  closure->arg_regs.emplace_back(std::make_pair(reg, luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(ast->lifter_config->arg_prefix, reg, ast->lifter_config->arg_suffix_char)));

            /* Varargs ("...") */
            if (ast->body->has_inst<luramas::il::arch::opcodes::OP_GETVARARGS>()) {
                  closure->arg_regs.emplace_back(std::make_pair(-1, "{VARARG_PLACEHOLDER}"));
            }

            return;
      };

      /* Set moves and sources for main. */
      if (ast->closure_kind == luramas::ast::closure_kinds::main) {
            set_args(ast, true);
      }

      /* Everything needs to go through based on control flow. */
      for (auto &closure : ast->closures) {

            if (closure->arg_regs.empty()) {

#if DEBUG_AST_DISPLAY_PASS_THROUGH
                  std::printf("[AST] For routines for children closure.\n");
#endif
                  luramas::ast::transformers::loops::set_for_routines(closure); /* Add forloops */

#if DEBUG_AST_DISPLAY_PASS_THROUGH
                  std::printf("[AST] For prep for children closure.\n");
#endif
                  luramas::ast::transformers::loops::set_for_prep_exprs(closure); /* Add for inits. */

#if DEBUG_AST_DISPLAY_PASS_THROUGH
                  std::printf("[AST] Setting args.\n");
#endif
                  set_args(closure); /* Set arguements. */
            }
      }

      return;
}
