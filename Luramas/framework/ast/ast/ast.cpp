#include "ast.hpp"
#include "transformers/transformers_default.hpp"
#include <algorithm>

namespace blocks {

      /* Set singular block *Jumpbacks act as end. */
      std::tuple<std::vector<std::shared_ptr<luramas::ast::node>>, std::uintptr_t /* Start next pc. */, std::uintptr_t /* Final instruction. */> init_current_block(std::shared_ptr<luramas::ast::ast> &ast, std::uintptr_t pc, std::vector<std::uintptr_t> &branch_ends) {

            std::vector<std::shared_ptr<luramas::ast::node>> retn;

            /* Init basic node data. */
            do {

                  auto current_disassembly = ast->disassembly[pc];

                  /* Pc is already at a branch ending. So just return empty vector. */
                  if (std::binary_search(branch_ends.begin(), branch_ends.end(), pc)) {
                        branch_ends.erase(std::remove(branch_ends.begin(), branch_ends.end(), pc), branch_ends.end());
                        return std::make_tuple(retn, pc, pc);
                  }

                  /* Construct current node. */
                  auto node = std::make_shared<luramas::ast::node>();
                  retn.emplace_back(node);

                  /* Set node data. */
                  node->address = pc;
                  node->lex = luramas::il::lexer::lexer(current_disassembly);

                  /* Branch/end so break. */
                  if (node->lex->kind == luramas::il::lexer::inst_kinds::branch ||
                      node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition ||
                      (pc + current_disassembly->len) == ast->sizecode) {
                        break;
                  }

                  pc += current_disassembly->len;

            } while (true /* Earlier code will exit if hit a branch or passed branch ends. */);

            return std::make_tuple(retn, pc + ast->disassembly[pc]->len, pc);
      }

      /* Set blocks for current ast. */
      void set_blocks(std::shared_ptr<luramas::ast::ast> &ast) {

            std::uintptr_t pc = 0u;
            std::vector<std::uintptr_t> branch_ends;
            std::vector<std::uintptr_t> branch_ends_clone;                                                                                                                                                                           /* Same as branch ends but a clone used for certain things. */
            std::unordered_map<std::uintptr_t /* PC(start) */, std::tuple<std::uintptr_t /* PC(end) */, std::uintptr_t /* PC(end(end + curr->len)) */, std::vector<std::shared_ptr<luramas::ast::node>> /* Nodes */>> linear_blocks; /* Block data for scopes. */

            /* Set each end as every branch taken pc. */
            for (auto &dism : ast->disassembly) {

                  const auto temp_lex = luramas::il::lexer::lexer(dism.second);

                  /* Don't log if jumpback. */
                  if (temp_lex->has_operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>() && temp_lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp <= 0) {
                        continue;
                  }

                  if (temp_lex->kind == luramas::il::lexer::inst_kinds::branch || temp_lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                        /* Jump with no memaddr operand idk how this has happened. */
                        if (!temp_lex->has_operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>()) {
                              throw std::runtime_error("Jump with no memaddr operand in lexer at set_blocks.");
                        }

                        /* Jump is negative don't take. */
                        if (temp_lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp < 0) {
                              continue;
                        }

                        branch_ends.emplace_back(temp_lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr);
                  }
            }

            /* Clone branch ends. */
            if (!branch_ends.empty()) {
                  branch_ends_clone.reserve(branch_ends.size());
                  std::copy(branch_ends.begin(), branch_ends.end(), branch_ends_clone.begin());
            }

            /* Set main block. */
            ast->body = std::make_shared<luramas::ast::block>();

            /* Append blocks */
            do {

#if DEBUG_AST_DISPLAY_PASS_THROUGH_BLOCKS
                  std::printf("[AST] Begin block.\n");
#endif

                  const auto block = init_current_block(ast, pc, branch_ends_clone);
                  linear_blocks.insert(std::make_pair(pc, std::make_tuple(std::get<2>(block), std::get<1>(block), std::get<0>(block))));
                  pc = std::get<1>(block);

#if DEBUG_AST_DISPLAY_PASS_THROUGH_BLOCKS
                  std::printf("[AST] End block.\n");
                  std::printf("[AST] Next pc: %" PRIuPTR " size: %" PRIuPTR "\n", pc, ast->sizecode);
#endif

            } while (pc < ast->sizecode);

            /* Reset PC. */
            pc = 0u;

            /* Init main (Block search is dependent on main so needs to be seperate from everything). */
            ast->body->node_start = pc;
            ast->body->node_end = std::get<0>(linear_blocks[pc]);
            ast->body->nodes = std::get<2>(linear_blocks[pc]);
            ast->add(ast->body);

            std::vector<std::uintptr_t> analyzed_scopes; /* Used to prevent infinite loops when doing visits. */

            /* Assemble blocks */
            while (pc < ast->sizecode) {

                  const auto node_block = linear_blocks[pc];
                  const auto nodes = std::get<2>(node_block);

                  /* Adbrupt end */
                  if (nodes.empty()) {
                        break;
                  }

                  analyzed_scopes.emplace_back(pc);

                  /* End */
                  const auto jump_node = nodes.back();
                  if (jump_node->lex->kind == luramas::il::lexer::inst_kinds::branch || jump_node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                        const auto jmp = jump_node->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                        const auto next_node_block = linear_blocks[jmp];

                        auto current = ast->find_block(pc);                                                         /* Block too place all info in. */
                        auto jump_block = ast->find_block(jmp);                                                     /* Jump taken block */
                        auto nojump_block = ast->find_block(jump_node->address + jump_node->lex->disassembly->len); /* Branch not taken jump. */

                        /* Current is always available something bad happened. */
                        if (current == nullptr) {

/* Either something bad has happen or dead instruction. (Will never get executed no matter what branch is taken or not) */
#if DEBUG_AST_DISPLAY_WARNINGS
                              std::printf("[WARNING] Dead instruction: %" PRIuPTR " %s\n", ast->disassembly[pc]->addr, ast->disassembly[pc]->data.c_str());
#endif

                              /* Get previous from pc */
                              std::uintptr_t key = 0u;
                              for (const auto &i : ast->disassembly) {
                                    if (i.first < pc && key < i.first) {
                                          key = i.first;
                                    }
                              }

#if DEBUG_AST_DISPLAY_WARNINGS
                              std::printf("[WARNING] Appending dead instruction to block with: pc = %" PRIuPTR " at instruction : %s\n", key, ast->disassembly[key]->data.c_str());
#endif

                              current = std::make_shared<luramas::ast::block>();
                              current->node_start = pc;
                              current->node_end = pc;
                              current->nodes = std::get<2>(linear_blocks[pc]);

                              ast->find_block_addr(key)->branches.emplace_back(current);

                              /* Propegate with bad instruction expr. */
                              for (const auto &node : current->nodes) {
                                    node->add_elem<luramas::ast::element_kinds::desc_bad_instruction>();
                              }
                        }

                        switch (jump_node->lex->kind) {

                              case luramas::il::lexer::inst_kinds::branch: {

                                    /* Construct branch taken. */
                                    if (jump_block == nullptr) {
                                          jump_block = std::make_shared<luramas::ast::block>();
                                          jump_block->node_start = jmp;
                                          jump_block->node_end = std::get<0>(linear_blocks[jmp]);
                                          jump_block->nodes = std::get<2>(linear_blocks[jmp]);
                                    }

                                    /* Add jump taken. */
                                    if (jump_node->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp > 0 || std::find(analyzed_scopes.begin(), analyzed_scopes.end(), jmp) == analyzed_scopes.end()) {
                                          current->branches.emplace_back(jump_block);
                                          ast->add(jump_block);
                                    }

                                    break;
                              }

                              case luramas::il::lexer::inst_kinds::branch_condition: {

                                    /* Construct branch taken. */
                                    if (jump_block == nullptr) {
                                          jump_block = std::make_shared<luramas::ast::block>();
                                          jump_block->node_start = jmp;
                                          jump_block->node_end = std::get<0>(linear_blocks[jmp]);
                                          jump_block->nodes = std::get<2>(linear_blocks[jmp]);
                                    }

                                    /* Construct no branch taken. */
                                    if (nojump_block == nullptr) {
                                          nojump_block = std::make_shared<luramas::ast::block>();
                                          nojump_block->node_start = jump_node->address + jump_node->lex->disassembly->len;
                                          nojump_block->node_end = std::get<0>(linear_blocks[jump_node->address + jump_node->lex->disassembly->len]);
                                          nojump_block->nodes = std::get<2>(linear_blocks[jump_node->address + jump_node->lex->disassembly->len]);
                                    }

                                    /* Add jump taken. */
                                    if (jump_node->lex->operand_kind<luramas::il::lexer::operand_kinds::jmpaddr>().front()->dis.jmp > 0 || std::find(analyzed_scopes.begin(), analyzed_scopes.end(), jmp) == analyzed_scopes.end()) {
                                          current->branches.emplace_back(jump_block);
                                          ast->add(jump_block);
                                    }

                                    current->branches.emplace_back(nojump_block);
                                    ast->add(nojump_block);

                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unexpected jump inst type.");
                              }
                        }
                  }

                  /* Set pc */
                  pc = std::get<1>(node_block);
            };

            return;
      }

} // namespace blocks

std::shared_ptr<luramas::ast::ast> luramas::ast::gen_ast(const luramas::emitter_ir::syntax::emitter_syntax syn, std::shared_ptr<luramas::il::ilang> &il_data, const std::shared_ptr<ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format) {

      /* Current ast. */
      auto retn = std::make_shared<ast>();                     /* Parent AST. */
      std::vector<std::shared_ptr<ast>> closures_ast = {retn}; /* All closures including nested closures. */

      /* Set closure and closure. */
      retn->closure_kind = closure_kinds::main;                                /* Set type as main. */
      retn->il = il_data;                                                      /* Set IL as current IL. */
      retn->lifter_config = config;                                            /* Set config */
      retn->sizecode = retn->il->dis.back()->addr + retn->il->dis.back()->len; /* Last addr + len is size code. */
      retn->syntax = syn;                                                      /* Set syntax */

#if DEBUG_AST_DISPLAY_PASS_THROUGH
      std::printf("[AST] Initing main disassembly.\n");
#endif

      /* Map IL disassembly too parent AST. */
      for (const auto &i : retn->il->dis) {
            retn->disassembly.insert(std::make_pair(i->addr, i));
            retn->total = i->total;
      }

/* Set current closure blocks. */
#if DEBUG_AST_DISPLAY_PASS_THROUGH
      std::printf("[AST] Initing main blocks.\n");
#endif
      blocks::set_blocks(retn);

      do {

            debug_init("AST");

            auto current_closure = closures_ast.front(); /* Current closure target. */
            auto ast_id = current_closure->ast_id;       /* Current AST id from closure target. */

#if DEBUG_AST_DISPLAY_PASS_THROUGH
            std::printf("[AST] Initing closure [%" PRIuPTR "].\n", reinterpret_cast<std::uintptr_t>(current_closure.get()));
#endif

/* Gen children closure to analyze. */
#if DEBUG_AST_DISPLAY_PASS_THROUGH
            std::printf("[AST] Setting child closure information.\n");
#endif
            /* Loop through each closure from target. */
            for (auto i = 0u; i < current_closure->il->closures.size(); ++i) {

                  /* Make child ast and add closure and get type. */
                  auto child_ast = std::make_shared<ast>();                                               /* Child AST. */
                  child_ast->il = current_closure->il->closures[i];                                       /* Set indexed closure as IL. */
                  child_ast->lifter_config = config;                                                      /* Set config. */
                  child_ast->sizecode = child_ast->il->dis.back()->addr + child_ast->il->dis.back()->len; /* Last addr + len is size code. */

                  current_closure->closures.emplace_back(child_ast);                             /* Append uninited child AST too targets closure. */
                  luramas::ast::transformers::closure::process_closure_info(current_closure, i); /* Set closure information for the closure. */

                  /* Add child to get analyzed. */
                  closures_ast.emplace_back(child_ast); /* Append child AST too get analyzed later. */

                  child_ast->ast_id = ++ast_id; /* Increment for ID. */
            }

/* Set dism of child closure. */
#if DEBUG_AST_DISPLAY_PASS_THROUGH
            std::printf("[AST] Initing child closures.\n");
#endif
            for (auto &child : current_closure->closures) {

#if DEBUG_AST_DISPLAY_PASS_THROUGH
                  std::printf("[AST] Initing child closure [%" PRIuPTR "].\n", reinterpret_cast<std::uintptr_t>(child.get()));
#endif

                  if (child->disassembly.empty()) {

#if DEBUG_AST_DISPLAY_PASS_THROUGH
                        std::printf("[AST] Setting child dissasembly.\n");
#endif

                        /* Set current closure dissasembly. */
                        for (const auto &i : child->il->dis) {
                              child->disassembly.insert(std::make_pair(i->addr, i));
                              retn->total = i->total;
                        }
                  }

                  /* Set child closure blocks. */
                  if (child->body == nullptr || child->body->nodes.empty()) {
#if DEBUG_AST_DISPLAY_PASS_THROUGH
                        std::printf("[AST] Initing child closure main blocks.\n");
#endif
                        blocks::set_blocks(child);
                  }
            }

            /* Init ast */
            luramas::ast::transformers::add_default_transformers(current_closure);

/* Set end pc and display tree. */
#if DEBUG_AST_DISPLAY_PASS_THROUGH
            std::printf("[AST] Setting end pc.\n");
#endif
            current_closure->pc_end = current_closure->body->visit_all().back()->address;

#if DEBUG_AST_DISPLAY_PASS_THROUGH
            std::printf("[AST] Finished with current ast.\n");
#endif

#if DEBUG_AST_DISPLAY_DATA
            std::cout << current_closure->closure_information(syn, format) << std::endl;
            std::cout << "CFG:\n"
                      << current_closure->gen_cfg<true>() << std::endl;
#endif

            /* Remove current. */
            closures_ast.erase(std::remove(closures_ast.begin(), closures_ast.end(), current_closure), closures_ast.end());

            debug_close("AST");

      } while (!closures_ast.empty());

      /* Clear cache for new asts. */
      luramas::ast::transformers::global_cache::clear();

      return retn;
}