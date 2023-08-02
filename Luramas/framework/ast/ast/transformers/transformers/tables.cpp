#include "../transformers.hpp"

void luramas::ast::transformers::tables::set_table_exprs(std::shared_ptr<luramas::ast::ast> &ast) {

      const auto all = ast->body->visit_all();
      for (const auto &i : all) {

            switch (i->lex->disassembly->op) {

                  case luramas::il::arch::opcodes::OP_NEWTABLE:
                  case luramas::il::arch::opcodes::OP_REFTABLE:
                  case luramas::il::arch::opcodes::OP_REFTABLEA:
                  case luramas::il::arch::opcodes::OP_NEWTABLEA: {
                        debug_success("Setting table expr at: %s", i->str().c_str());
                        i->add_expr<luramas::ast::element_kinds::table>();
                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      return;
}

void luramas::ast::transformers::tables::set_routines(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uintptr_t> ends; /* Used to avoid encapsulation with tables as this is for ends of already analyzed tables. */

      const auto tables = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_expr<luramas::ast::element_kinds::table>(true));
      for (const auto &table : tables) {

            /* Check */
            bool valid = true;
            for (const auto i : ends)
                  if (table->lex->disassembly->addr <= i) {
                        valid = false;
                  }

            if (!valid) {
                  continue;
            }

            std::intptr_t table_count = 0;
            std::uint16_t table_reg = 0u;  /* Current register table target. */
            std::int32_t nested_count = 0; /* Used for node analysis. */
            std::uintptr_t node_size = 0u;
            std::uintptr_t array_size = 0u;
            std::uintptr_t predicted_size = 0u; /* Previous power of 2 for array_size, gives us more context on end. */

            /* Sizes for scopes of table. (Used for scopes of nested tables) */
            std::vector<std::uintptr_t> predicted_sizes;
            std::vector<std::uintptr_t> node_sizes;
            std::vector<std::uintptr_t> array_sizes;
            std::vector<std::uint16_t> table_target;
            std::vector<std::uint16_t> table_target_uf; /* Unfinished reg set table set when hit table end. */
            std::unordered_map<std::uint16_t /* reg */, std::shared_ptr<luramas::ast::node> /* node */> dest_map;

            std::shared_ptr<luramas::ast::node> last_set = table;

            /* Cache data for scopes. */
            auto cache = [&](const bool start /* Start new cache or set old cache? */) mutable -> void {
                  if (start) {
                        predicted_sizes.emplace_back(predicted_size);
                        node_sizes.emplace_back(node_size);
                        array_sizes.emplace_back(array_size);
                        table_target.emplace_back(table_reg);
                  } else {

                        predicted_sizes.pop_back();
                        node_sizes.pop_back();
                        array_sizes.pop_back();
                        table_target.pop_back();

                        /* Check size */
                        if (!predicted_sizes.empty())
                              predicted_size = predicted_sizes.back();

                        if (!node_sizes.empty())
                              node_size = node_sizes.back();

                        if (!array_sizes.empty())
                              array_size = array_sizes.back();

                        if (!table_target.empty())
                              table_reg = table_target.back();
                  }
            };

            /* Set size for node_size and array_size also sets table elements/start/end. */
            auto set_size = [&](const std::shared_ptr<luramas::ast::node> &node) mutable -> void {
                  switch (node->lex->disassembly->op) {

                        case luramas::il::arch::opcodes::OP_NEWTABLE:
                        case luramas::il::arch::opcodes::OP_NEWTABLEA: {

                              const auto operands = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>();
                              auto x = operands.front()->dis.table_size;

                              if (!x && !operands.back()->dis.table_size) {
                                    node->add_expr<luramas::ast::element_kinds::table_start>();
                                    node->add_expr<luramas::ast::element_kinds::table_end>();
                                    return;
                              }

                              node_size = x;
                              array_size = operands.back()->dis.table_size;

                              if (x) {
                                    --x;
                                    x = x | (x >> 1);
                                    x = x | (x >> 2);
                                    x = x | (x >> 4);
                                    x = x | (x >> 8);
                                    x = x | (x >> 16);
                                    predicted_size = x - (x >> 1);
                              }

                              table_reg = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                              debug_result("Set size for %s, node size %" PRIuPTR ", array size %" PRIuPTR ", predicted size %" PRIuPTR ", target %d", node->str().c_str(), node_size, array_size, predicted_size, table_reg);

                              cache(true);
                              node->add_existance<luramas::ast::element_kinds::table_start>();
                              ++table_count;

                              debug_success("Added table start expr too %s", node->str().c_str());

                              break;
                        }

                        case luramas::il::arch::opcodes::OP_REFTABLE:
                        case luramas::il::arch::opcodes::OP_REFTABLEA: {

                              const auto kvalue = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx];

                              if (kvalue->type != luramas::il::arch::kval_kinds::table) {
                                    throw std::runtime_error("Kvalue is not table.");
                              }

                              /* Opposite to newtable vise versa. */
                              auto x = kvalue->table.node_size;
                              const auto pow = 1 << x;

                              if (pow) {
                                    predicted_size = std::uintptr_t(std::pow(std::log2(pow) - 1u, 2u)); /* Last too next power of too from pow. */
                              }

                              array_size = kvalue->table.array_size;
                              node_size = pow;
                              table_reg = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                              debug_result("Set size for %s, node size %" PRIuPTR ", array size %" PRIuPTR ", predicted size %" PRIuPTR ", target %d ", node->str().c_str(), node_size, array_size, predicted_size, table_reg);

                              cache(true);
                              node->add_existance<luramas::ast::element_kinds::table_start>();
                              ++table_count;
                              debug_success("Added table start expr too %s", node->str().c_str());

                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETLIST: {

                              const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                              auto amt = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.val;

                              array_size -= std::uintptr_t(amt);

                              /* Target */
                              if (std::find(table_target_uf.begin(), table_target_uf.end(), dest) != table_target_uf.end()) {

                                    for (auto i = 0u; i < std::count(table_target_uf.begin(), table_target_uf.end(), dest); ++i) {
                                          node->add_expr<luramas::ast::element_kinds::table_end>();
                                          cache(false);
                                          --nested_count;
                                          --table_count;
                                          debug_success("Added extra table end expr.");
                                    }
                              }

                              /* Could be set by end if end? */
                              if (!node_sizes.empty() /* Base line must have members. */ && (predicted_size < (node_size - 1u)) /* May have some nodes left? */) {
                                    node->add_expr<luramas::ast::element_kinds::table_end>();
                                    --table_count;
                                    --nested_count;
                                    ends.emplace_back(node->address);
                                    cache(false);
                                    debug_success("Added table end expr.");
                              }

                              debug_result("Decreased array size for %s, array size %" PRIuPTR ", node size %" PRIuPTR, node->str().c_str(), array_size, node_size);
                              debug_success("Added table end expr too %s", node->str().c_str());

                              break;
                        }

                        default: {
                              break;
                        }
                  }

                  return;
            };

            const auto inside_call_routine = ast->body->inside_routine<luramas::ast::element_kinds::call_multret_start, luramas::ast::element_kinds::call_multret_end>(table->address);

            /* Add first info */
            if (table->has_expr(luramas::ast::element_kinds::table)) {

                  /* Has table members? */
                  set_size(table);

                  if (array_size || node_size) {

                        auto nodes = ast->body->visit_rest(table->address);
                        for (auto &node : nodes) {

                              /* Map out dest */
                              if (!inside_call_routine && node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {

                                    const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                                    if (dest_map.find(dest) == dest_map.end()) {
                                          dest_map.insert(std::make_pair(dest, node)); /* Add */
                                    } else {
                                          dest_map[dest] = node; /* Change */
                                    }
                              }

                              /* New/set table instruction inc/dec for nested. */
                              if (node->has_expr(luramas::ast::element_kinds::table)) {

                                    ++nested_count;
                                    debug_line("Hit table increased nested count too %d from %s", nested_count, node->str().c_str());
                              }

                              set_size(node);

                              /* Node_size equals predicted_size or is less then predicted_size that means that it exceeded predicted_size. */
                              if (node_size) {

                                    /* Table set so check if it's the end. */
                                    if (node->lex->kind == luramas::il::lexer::inst_kinds::table_set) {

                                          debug_result("Current table %s, node size %" PRIuPTR ", array size %" PRIuPTR ", predicted size %" PRIuPTR ", target %d, nested %d", node->str().c_str(), node_size, array_size, predicted_size, table_reg, nested_count);

                                          /* Double check new set table. */
                                          if (node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg != table_reg) {

                                                debug_line("Current reg and table target does not match.");

                                                --table_count;
                                                --node_size;                                              /* Dec for node. */
                                                node->add_expr<luramas::ast::element_kinds::table_end>(); /* End of table. */
                                                last_set = node;
                                                cache(false); /* Revert back cache. */

                                                debug_success("Added table end expr too %s", node->str().c_str());

                                                /* End of a nested table. */
                                                if (nested_count) {
                                                      --nested_count;
                                                      node->add_existance<luramas::ast::element_kinds::table_element>(); /* Member */
                                                } else {                                                                 /* Nested table is 0 and we entered a new table. */
                                                      break;
                                                }

                                          } else {

                                                /* Check next from last set. */
                                                const auto source = node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
                                                if (dest_map.find(source) != dest_map.end() && node != ast->body->visit_next(dest_map[source])) {

                                                      debug_warning("Dest set is not next from node %s and set %s", node->str().c_str(), ast->body->visit_next(dest_map[source])->str().c_str());

                                                      node = dest_map[source];
                                                      goto node_end;
                                                }

                                                /* Dec not predicted. */
                                                if (predicted_size < (node_size - 1u)) {

                                                      --node_size;
                                                      node->add_existance<luramas::ast::element_kinds::table_element>();
                                                      last_set = node;
                                                      debug_line("Decreased node size too %" PRIuPTR " from %s", node_size, node->str().c_str());

                                                } else {

                                                      bool hit_nested = false; /* Hit nested table while analysis? */
                                                      std::intptr_t routine = 0;

                                                      /* Really just visiting future instructions too see if table source, dest, or idx gets set twice indicating end. */
                                                      const auto rest_nodes = ast->body->visit_rest(node->address);
                                                      for (const auto &i : rest_nodes) {

                                                            /* Log routines */
                                                            routine_inc_basic(i, routine);
                                                            routine_dec_basic(i, routine);

                                                            /* Skip */
                                                            if (routine) {
                                                                  continue;
                                                            }

                                                            if (!hit_nested) {
                                                                  hit_nested = i->lex->kind == luramas::il::lexer::inst_kinds::new_table;
                                                            }

                                                            /* End (new/end nested table) */
                                                            if (nested_count > 0) {

                                                                  debug_line("Nested is greater then 0, %d.", nested_count);

                                                                  if (i->lex->kind == luramas::il::lexer::inst_kinds::set_table) {

                                                                        debug_line("Hit setlist for nested table set on %s", i->str().c_str());

                                                                        --nested_count;
                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        node->add_existance<luramas::ast::element_kinds::table_end>();
                                                                        last_set = node;
                                                                        cache(false); /* Revert back cache. */

                                                                  } else if (i->has_expr(luramas::ast::element_kinds::table)) {

                                                                        debug_line("Hit new forming nested table set on %s", i->str().c_str());

                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        last_set = node;
                                                                  }

                                                                  /* Normal */
                                                                  if (!hit_nested && i->lex->kind == luramas::il::lexer::inst_kinds::table_set && table_reg != i->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg) {

                                                                        debug_line("Hit normal table element for %s", node->str().c_str());

                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        last_set = node;

                                                                        break;
                                                                  }

                                                                  /* New index */
                                                                  if (!hit_nested && i->lex->kind == luramas::il::lexer::inst_kinds::table_set && table_reg != i->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg) {

                                                                        --nested_count;

                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        node->add_existance<luramas::ast::element_kinds::table_end>();
                                                                        last_set = node;
                                                                        cache(false); /* Revert back cache. */

                                                                        debug_line("New table was hit for nested and haven't hit a new table in analysis changed nested count %d on %s for %s", nested_count, i->str().c_str(), node->str().c_str());
                                                                  }

                                                                  continue;
                                                            }

                                                            if (i->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>() && predicted_size > node_size) {

                                                                  debug_line("Hit dest from predicted on %s for %s", i->str().c_str(), node->str().c_str());

                                                                  /* Dest is logical out of table. */
                                                                  if (registers::logical_dest_register(ast, i, i->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg)) {

                                                                        debug_warning("Reg is logical end.");

                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        last_set = node;
                                                                        node_size = 0;
                                                                        goto node_end;
                                                                  }

                                                            } else {

                                                                  debug_line("No dest and predicted size isnt bigger then node size on %s for %s", i->str().c_str(), node->str().c_str());

                                                                  /* Current is table Set. */
                                                                  if (i->lex->kind == luramas::il::lexer::inst_kinds::table_set) {

                                                                        debug_line("Hit table set on %s for %s", i->str().c_str(), node->str().c_str());

                                                                        /* Table has same table source as current table source valid element.*/
                                                                        if (table_reg == i->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg) {

                                                                              debug_line("Same table source as current valid element.");

                                                                              --node_size; /* Dec for node. */
                                                                              node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                              last_set = node;

                                                                              break;
                                                                        } else { /* New table */

                                                                              if (nested_count) {

                                                                                    /* Inside nested */

                                                                                    debug_line("New nested table.");

                                                                                    --node_size; /* Dec for node. */

                                                                                    node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                                    last_set = i;
                                                                                    cache(false); /* Revert back cache. */

                                                                                    if (array_size) {

                                                                                          table_target_uf.emplace_back(table_reg);
                                                                                          debug_warning("Current table still has array size apppended reg to cache for later set on %s", i->str().c_str());

                                                                                    } else {

                                                                                          i->add_expr<luramas::ast::element_kinds::table_end>(); /* End of table. */

                                                                                          /* Dec nested table */
                                                                                          node->add_existance<luramas::ast::element_kinds::table_element>(); /* Member */
                                                                                          --nested_count;
                                                                                          --table_count;
                                                                                          debug_success("Added table end expr too %s", i->str().c_str());
                                                                                    }

                                                                              } else {

                                                                                    debug_line("New table.");

                                                                                    /* End */
                                                                                    if (array_size) {

                                                                                          table_target_uf.emplace_back(table_reg);
                                                                                          debug_warning("Array size still not 0, cached table reg target.");

                                                                                    } else {
                                                                                          node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                                          last_set = node;
                                                                                          node_size = 0;
                                                                                          goto node_end;
                                                                                    }
                                                                              }

                                                                              break;
                                                                        }

                                                                  } else if (no_dest(i)) {
                                                                        /* Something different */

                                                                        debug_warning("Jumping too end because of not table set or dest on %s", i->str().c_str());

                                                                        node->add_existance<luramas::ast::element_kinds::table_element>();
                                                                        last_set = node;
                                                                        node_size = 0;
                                                                        goto node_end;
                                                                  }
                                                            }
                                                      }

                                                      /* Fail case */
                                                      if (!node->has_expr(luramas::ast::element_kinds::table_element)) {
                                                            node->add_existance<luramas::ast::element_kinds::table_element>();
                                                            last_set = node;
                                                            --node_size;
                                                      }
                                                }
                                          }

                                    } else if (no_dest(node)) {

                                          debug_line("Current has no dest %s", node->str().c_str());

                                          if (node->lex->kind == luramas::il::lexer::inst_kinds::set_global && node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg == table_reg && !luramas::ast::transformers::registers::logical_dest_register(ast, last_set, table_reg)) {

                                                debug_success("Next is setglobal with same table reg and isn't a locvar it is a valid table.");
                                                last_set->add_existance<luramas::ast::element_kinds::table_element>();

                                          } else {

                                                debug_warning("No dest adbrupt end.");
                                                node = last_set;
                                          }

                                          goto node_end;
                                    }
                              }

                              /* Found end, end anlysis. */
                              if (!node_size && !array_size && table_count == 1) {
                              node_end:
                                    debug_success("Adding table end expr too %s", node->str().c_str());
                                    node->add_expr<luramas::ast::element_kinds::table_end>();
                                    ends.emplace_back(node->address);
                                    cache(false); /* Revert back cache. */
                                    break;
                              }
                        }

                  } else {
                        /* No table members. */
                        ends.emplace_back(table->address);
                  }

            } else {
                  throw std::runtime_error("Expected NEWTABLE or DUPTABLE instruction to init table.");
            }
      }

      return;
}

void luramas::ast::transformers::tables::set_node_end(std::shared_ptr<luramas::ast::ast> &ast) {

      std::vector<std::uintptr_t> nodes_end;
      auto table_starts = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(ast->body->visit_expr<luramas::ast::element_kinds::table_start>(true));
      std::reverse(table_starts.begin(), table_starts.end());

      for (const auto &i : table_starts) {

            /* No elements */
            if (i->has_expr(luramas::ast::element_kinds::table_end)) {
                  i->tables.end_table = i->address;
                  nodes_end.emplace_back(i->address);
                  continue;
            }

            const auto start_node = i->address;
            const auto end_node = ast->body->visit_relative_next_expr_scope_current<luramas::ast::element_kinds::table_end>(i->address, {luramas::ast::element_kinds::table_start})->address;

            const auto range = ast->body->visit_range_current(start_node, end_node);
            for (const auto &node : range) {

                  /* Not set */
                  if (!node->tables.end_table) {
                        node->tables.end_table = end_node;
                  }
            }

            nodes_end.emplace_back(end_node);
      }

      return;
}