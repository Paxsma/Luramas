#include "../transformers.hpp"

void luramas::ast::transformers::closure::process_closure_info(const std::shared_ptr<luramas::ast::ast> &current_closure, const std::size_t child_closure_id) {

      std::shared_ptr<luramas::ast::node> end_capture = nullptr;
      std::shared_ptr<luramas::ast::node> closure_node = nullptr;

      /* Newcloure, Dupclosures node. */
      auto closures = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(current_closure->body->visit_inst<luramas::il::arch::opcodes::OP_NEWCLOSURE>(true));
      const auto dupclosures = std::get<std::vector<std::shared_ptr<luramas::ast::node>>>(current_closure->body->visit_inst<luramas::il::arch::opcodes::OP_REFCLOSURE>(true));

      /* All closures. */
      closures.insert(closures.end(), dupclosures.begin(), dupclosures.end());

      /* Iterate through closures and get expression for it relative to closure given. */
      for (const auto &i : closures) {

            /* Already analyzed. */
            if (i->has_elem(luramas::ast::element_kinds::stat_closure_global) || i->has_elem(luramas::ast::element_kinds::stat_closure_scoped) || i->has_elem(luramas::ast::element_kinds::stat_closure_anonymous)) {
                  continue;
            }

            /* Expression has been set. */
            if (closure_node != nullptr) {
                  break;
            }

            /* Set newcclosure node. */
            if (i->lex->disassembly->operands[1]->dis.closure_idx == child_closure_id) {

                  closure_node = i; /* Set node. */

                  /* Skip captures if any. */
                  auto next = current_closure->body->visit_next(i);
                  while (next != nullptr && next->lex->disassembly->op == luramas::il::arch::opcodes::OP_ADDUPVALUE) {
                        next = current_closure->body->visit_next(next);
                  }

                  const auto next_prev = current_closure->body->visit_previous_addr(next->address);
                  end_capture = (next != nullptr && next->lex->disassembly->op == luramas::il::arch::opcodes::OP_ADDUPVALUE) ? next : (next_prev != nullptr && next_prev->lex->disassembly->op == luramas::il::arch::opcodes::OP_ADDUPVALUE) ? next_prev
                                                                                                                                                                                                                                             : closure_node;

                  /* Next is null */
                  if (next == nullptr) {
                        closure_node->add_elem<luramas::ast::element_kinds::stat_closure_scoped>(1u, luramas::ast::element::front); /* scoped function ?? (??) [MUTABLE] */
                  } else {

                        /* Next is setglobal and uses reg as source. */
                        if (next->lex->disassembly->op == luramas::il::arch::opcodes::OP_SETGLOBAL && next->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg == i->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg) {
                              closure_node->add_elem<luramas::ast::element_kinds::stat_closure_global>(1u, luramas::ast::element::front);
                              closure_node->closures.setglobal_node = next;
                              closure_node = next;
                              closure_node->add_elem<luramas::ast::element_kinds::stat_closure_global>(1u, luramas::ast::element::front); /* global function ?? (??) */
                        } else {
                              closure_node->add_elem<luramas::ast::element_kinds::stat_closure_scoped>(1u, luramas::ast::element::front); /* scoped function ?? (??) [MUTABLE] */
                        }
                  }

                  closure_node->closures.closure_idx = child_closure_id;
            }
      }

      /* Turn expression to closure type. */

      auto closure = current_closure->closures[child_closure_id];

      /* Check for getimport followed by potential gettable. */
      std::int16_t target = -1;
      std::shared_ptr<luramas::ast::node> next = end_capture;
      std::string name = "";
      do {

            next = current_closure->body->visit_next(next);

            if (next != nullptr) {

                  if (next->lex->disassembly->op == luramas::il::arch::opcodes::OP_GETTABUPVALUE) {

                        /* Can't use getimport twice. */
                        if (target != -1) {
                              next = nullptr;
                              break;
                        }

                        target = next->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                        name += next->lex->operand_kind<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value;

                  } else if (next->lex->kind == luramas::il::lexer::inst_kinds::table_get && next->lex->has_operand_kind<luramas::il::lexer::operand_kinds::kvalue>()) {

                        const auto dest = next->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                        const auto source = next->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;

                        if (target == source) {
                              target = dest;
                        } else {
                              next = nullptr;
                              break;
                        }

                        name += '.' + next->lex->operand_kind<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value;

                  } else if (next->lex->kind == luramas::il::lexer::inst_kinds::table_set) {

                        const auto source = next->lex->operand_kind<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
                        const auto reg = next->lex->operand_kind<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;

                        if (source == closure_node->lex->operand_kind<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg && reg == target) {
                              name += '.' + next->lex->operand_kind<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value;
                              break;
                        }

                        next = nullptr;
                        break;
                  } else {
                        /* Only getimport or table get/set. */
                        next = nullptr;
                        break;
                  }
            }

      } while (next != nullptr);

      /* Change too global function with set name. */
      if (next != nullptr) {

            /* Erase , ", ' */
            if (name.find('\"') != std::string::npos) {
                  name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());
            }

            if (name.find('\'') != std::string::npos) {
                  name.erase(std::remove(name.begin(), name.end(), '\''), name.end());
            }

            closure->closure_kind = luramas::ast::closure_kinds::global;
            closure->closure_name->emitter_datatype_kval(name);

            /* Replace next */
            if (closure_node->has_elem(luramas::ast::element_kinds::stat_closure_scoped)) {

                  closure_node->replace_next<luramas::ast::element_kinds::stat_closure_scoped, luramas::ast::element_kinds::stat_closure_global>();

            } else if (closure_node->has_elem(luramas::ast::element_kinds::stat_closure_anonymous)) {

                  closure_node->replace_next<luramas::ast::element_kinds::stat_closure_anonymous, luramas::ast::element_kinds::stat_closure_global>();
            }

            /* Fill with dead instructions. */
            for (const auto &i : current_closure->body->visit_range_current((closure_node->address + closure_node->lex->disassembly->len), next->address))
                  i->add_elem<luramas::ast::element_kinds::desc_dead_instruction>(); /* Handled by before hand. */

            closure_node->closures.idx_nodes = std::make_pair(end_capture, next);

      } else {

            /* Comes with closure name. */
            if (closure_node->has_elem(luramas::ast::element_kinds::stat_closure_global)) {

                  closure->closure_kind = luramas::ast::closure_kinds::global;
                  closure->closure_name->emitter_datatype_kval(closure_node->lex->operand_kind<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value);
                  closure_node->add_elem<luramas::ast::element_kinds::desc_dead_instruction>(); /* Handled by before hand. */

            } else if (closure_node->has_elem(luramas::ast::element_kinds::stat_closure_scoped)) { /* Closure name doesn't get compiled unless specified. */

                  closure->closure_kind = luramas::ast::closure_kinds::scoped;

            } else if (closure_node->has_elem(luramas::ast::element_kinds::stat_closure_anonymous)) { /* No closure name. */

                  closure->closure_kind = luramas::ast::closure_kinds::anonymous;

            } else { /* Shouldn't happen but incase it does. */
                  throw std::runtime_error("Unkown expression for closure_kinds.");
            }
      }

      return;
}
