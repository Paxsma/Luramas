#pragma once
#include "../../../il/code/generation/common/generate/generate.hpp"
#include "../../../il/il/architecture/architecture.hpp"
#include "../ast.hpp"
#include "../macros.hpp"
#include "transformers_macros.hpp"
#include "transformers_post.hpp"
#include <algorithm>

namespace luramas {

      namespace ast {

            namespace transformers {

                  namespace global_cache {

                        namespace scopes {

                              extern std::unordered_map<std::shared_ptr<luramas::ast::node> /* Branch */, std::shared_ptr<luramas::ast::node> /* End */> cached_ends;

                        }

                        void clear();

                  } // namespace global_cache

                  namespace calls {

                        /*
				Calls are a bit special because they rely on parent registers for both arguments and stack. Using this call routines usally get assembled right before
				a call opcode is executed. We get the beggining of this by judging by it's parent target register stack found in the call opcode in previous code. Note you can
				assemble the call and everything ahead of time and call later but that is very impracticle and very unoptimized which is generally not done.
		   */
                        void set_routines(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets multret call routines **Only applies to variables/args initing variables will get handled by lifter automatically** */
                        void set_multret_routines(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Register gets used in call? */
                        bool reg_arg(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &call, const std::uint16_t target);

                  } // namespace calls

                  namespace scopes {

                        namespace scope_data {

                              /* Scoping vector */

                              /* Creates scopes based on idx, which is used by unordered map. (Start = next from jump, Jump target = previous from jump) */
                              class scope {

                                  public:
                                    std::vector<std::pair<std::shared_ptr<luramas::ast::node> /* Begin */, std::shared_ptr<luramas::ast::node> /* End */>> operator[](const std::shared_ptr<luramas::ast::node> &start) {

                                          std::vector<std::pair<std::shared_ptr<luramas::ast::node> /* Begin */, std::shared_ptr<luramas::ast::node> /* End */>> retn;

                                          /* Get nearest */
                                          std::pair<std::shared_ptr<luramas::ast::node> /* Begin */, std::shared_ptr<luramas::ast::node> /* End */> parent_scope;
                                          for (const auto &i : this->data) {

                                                if ((parent_scope.first == NULL || parent_scope.first->address < std::get<0>(i)->address) && std::get<0>(i)->address <= start->address && std::get<1>(i)->address >= start->address) {

                                                      parent_scope = i;
                                                }
                                          }

                                          /* Loop for near bounds */
                                          for (const auto &i : this->data) {

                                                if (std::get<0>(i)->address >= parent_scope.first->address && std::get<1>(i)->address <= parent_scope.second->address && std::get<0>(i)->address >= start->address) {

                                                      retn.emplace_back(i);
                                                }
                                          }

                                          /* Incase it didnt get */
                                          if (std::find(retn.begin(), retn.end(), parent_scope) == retn.end()) {
                                                retn.emplace_back(parent_scope);
                                          }

                                          return retn;
                                    }

                                    void operator=(const std::vector<std::shared_ptr<luramas::ast::node>> &dism /* All nodes in scope */) {

                                          if (this->linked_ast == nullptr) {
                                                throw std::runtime_error("No linked ast present.");
                                          }

                                          std::vector<std::uintptr_t> exact_jump;
                                          std::vector<std::pair<std::shared_ptr<luramas::ast::node> /* Start */, std::shared_ptr<luramas::ast::node> /* Jump */>> addr_scopes;

                                          /* Add loops */
                                          for (const auto &i : dism) {

                                                switch (i->lex->disassembly->op) {

                                                      case luramas::il::arch::opcodes::OP_FORLOOPN: {
                                                            addr_scopes.emplace_back(std::make_pair(this->linked_ast->body->visit_addr(i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr), i));
                                                            break;
                                                      }

                                                      case luramas::il::arch::opcodes::OP_FORLOOPG: {
                                                            addr_scopes.emplace_back(std::make_pair(this->linked_ast->body->visit_addr(i->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr), i));
                                                            break;
                                                      }

                                                      default: {
                                                            break;
                                                      }
                                                }
                                          }

                                          for (const auto &node : dism) {

                                                /* if (?? ?? ??) */
                                                if (node->lex->kind == luramas::il::lexer::inst_kinds::branch_condition) {

                                                      /* Scope isnt jumpback? */
                                                      const auto ref_addr = node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                                                      if (!this->jump || (this->jump && ref_addr > node->address)) {

                                                            /* Hit */
                                                            bool dead = false;
                                                            for (const auto i : exact_jump) {

                                                                  if (i == (node->lex->disassembly->len + node->address)) {
                                                                        dead = true;
                                                                  }
                                                            }

                                                            /* Dead */
                                                            if (!dead) {
                                                                  addr_scopes.emplace_back(std::make_pair(((ref_addr == (node->lex->disassembly->len + node->address) || node == dism.back()) ? node : (*(&node + 1))), this->linked_ast->body->visit_previous_addr(this->linked_ast->body->visit_addr(ref_addr)->address)));
                                                                  exact_jump.emplace_back(ref_addr);
                                                            }
                                                      }
                                                }

                                                /* jump ?? */
                                                if (node->lex->kind == luramas::il::lexer::inst_kinds::branch) {
                                                      const auto ref_addr = node->lex->operand_expr<luramas::il::lexer::operand_kinds::jmpaddr>().front()->ref_addr;
                                                      addr_scopes.emplace_back(std::make_pair(((ref_addr == (node->lex->disassembly->len + node->address) || node == dism.back()) ? node : (*(&node + 1))), this->linked_ast->body->visit_previous_addr(this->linked_ast->body->visit_addr(ref_addr)->address)));
                                                }

                                                for (auto &i : addr_scopes) {

                                                      if (i.second->address == (node->lex->disassembly->len + node->address)) {

                                                            this->data.emplace_back(std::make_pair(i.first, i.second));
                                                      }
                                                }
                                          }

                                          this->data.emplace_back(std::make_pair(dism.front(), dism.back()));

                                          return;
                                    }

                                    scope(std::shared_ptr<luramas::ast::ast> &ast, const bool jumps = false /* Includes when analyzing jumps/jumpbacks. */) {
                                          this->jump = jumps;
                                          this->linked_ast = ast;
                                          return;
                                    }

                                    std::vector<std::pair<std::shared_ptr<luramas::ast::node> /* Begin */, std::shared_ptr<luramas::ast::node> /* End */>> get() {
                                          return this->data;
                                    }

                                  private:
                                    std::vector<std::pair<std::shared_ptr<luramas::ast::node> /* Begin */, std::shared_ptr<luramas::ast::node> /* End */>> data;
                                    bool jump = false;
                                    std::shared_ptr<luramas::ast::ast> linked_ast = nullptr;
                              };

                              template <typename t>
                              class scope_vector {

                                  public:
                                    std::vector<t> get() {
                                          return this->data;
                                    }

                                    scope_vector(class scope &s)
                                        : linked(s) {

                                          for (const auto &i : this->linked.get())
                                                this->data.insert(std::make_pair(i.first, std::vector<t>()));

                                          return;
                                    }

                                    /* Find index of key. */
                                    std::size_t index(const t key) {
                                          const auto d_raw = this->raw().front();
                                          return std::find(d_raw.begin(), d_raw.end(), key) - d_raw.begin();
                                    }

                                    void idx_set(const std::size_t key, const t value) {

                                          auto d_raw = this->raw();
                                          for (auto &i : d_raw)
                                                i[key] = value;
                                    }

                                    std::vector<std::vector<t>> raw() {

                                          std::vector<std::vector<t>> vect;

                                          for (const auto &i : this->scopes)
                                                vect.emplace_back(this->data[i]);

                                          return vect;
                                    }

                                    t idx_get(const std::size_t key) {
                                          const auto d_raw = this->raw().front();
                                          return d_raw[key];
                                    }

                                    /* Sets scope */
                                    void operator[](const std::shared_ptr<luramas::ast::node> &node) {

                                          this->scopes.clear();

                                          const auto index = this->linked[node];
                                          for (const auto &i : index)
                                                this->scopes.push_back(i.first);

                                          return;
                                    }

                                    void push_back(const t data) {

                                          for (const auto &i : this->scopes)
                                                this->data[i].push_back(data);

                                          return;
                                    }

                                    template <bool all = false /* all scopes */>
                                    bool find(const t target) {

                                          if (!all) {

                                                for (const auto &i : this->scopes) {

                                                      const auto vect = this->data[i];

                                                      if (std::find(vect.begin(), vect.end(), target) != vect.end()) {
                                                            return true;
                                                      }
                                                }

                                          } else {

                                                for (const auto &i : this->linked.get()) {

                                                      const auto vect = this->data[i.first];

                                                      if (std::find(vect.begin(), vect.end(), target) != vect.end()) {
                                                            return true;
                                                      }
                                                }
                                          }

                                          return false;
                                    }

                                    /* Remove dupes */
                                    std::vector<t> vector() {

                                          std::vector<t> retn;

                                          for (const auto &i : this->scopes) {

                                                const auto vect = this->data[i];

                                                retn.insert(retn.end(), vect.begin(), vect.end());
                                          }

                                          std::sort(retn.begin(), retn.end());
                                          retn.erase(std::unique(retn.begin(), retn.end()), retn.end());

                                          return retn;
                                    }

                                  private:
                                    std::vector<std::shared_ptr<luramas::ast::node> /* Begin*/> scopes;
                                    std::unordered_map<std::shared_ptr<luramas::ast::node> /* Begin */, std::vector<t> /* Data */> data;
                                    class scope &linked;
                              };

                              /* Every unordered map is homogenous for repective scope. */
                              template <typename keyt, typename valuet>
                              class scope_umap {

                                  public:
                                    std::vector<std::unordered_map<keyt, valuet>> get() {

                                          std::vector<std::unordered_map<keyt, valuet>> retn;

                                          for (const auto &i : this->scopes)
                                                retn.emplace_back(this->data[i]);

                                          return retn;
                                    }

                                    scope_umap(class scope &s)
                                        : linked(s) {

                                          for (const auto &i : this->linked.get())
                                                this->data.insert(std::make_pair(i.first, std::unordered_map<keyt /* Key */, valuet /* Value*/>()));

                                          return;
                                    }

                                    /* Sets scope */
                                    void operator[](const std::shared_ptr<luramas::ast::node> &node) {

                                          this->scopes.clear();

                                          const auto index = this->linked[node];
                                          for (const auto &i : index)
                                                this->scopes.push_back(i.first);

                                          return;
                                    }

                                    std::vector<std::unordered_map<keyt, valuet>> raw() {

                                          std::vector<t> vect;

                                          for (const auto &i : this->scopes)
                                                vect.emplace_back(this->data[i]);

                                          return vect;
                                    }

                                    void insert(const std::pair<keyt, valuet> &p) {

                                          for (const auto &i : this->scopes)
                                                this->data[i].insert(p);

                                          return;
                                    }

                                    void set_all_idx(const keyt key, const valuet &v) {

                                          for (const auto &i : this->scopes)
                                                this->data[i][key] = v;

                                          return;
                                    }

                                    template <bool all = false /* all scopes */>
                                    bool find(const keyt target) {

                                          if (!all) {

                                                for (const auto &i : this->scopes) {

                                                      const auto map = this->data[i];

                                                      if (map.find(target) != map.end()) {
                                                            return true;
                                                      }
                                                }

                                          } else {

                                                for (const auto &i : this->linked.get()) {

                                                      const auto map = this->data[i.first];

                                                      if (map.find(target) != map.end()) {
                                                            return true;
                                                      }
                                                }
                                          }

                                          return false;
                                    }

                                  private:
                                    std::vector<std::shared_ptr<luramas::ast::node> /* Begin*/> scopes;
                                    std::unordered_map<std::shared_ptr<luramas::ast::node> /* Begin */, std::unordered_map<keyt /* Key */, valuet /* Value*/> /* Data */> data;
                                    class scope &linked;
                              };

                        } // namespace scope_data

                        /* Gets end of scope node. (Doesn't count for current, looks for ends/jumps/returns) */
                        std::shared_ptr<luramas::ast::node> end_of_scope(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &start);

                  } // namespace scopes

                  namespace registers {

                        /* Sees if destination is logical in scope. */
                        bool logical_dest_register(std::shared_ptr<luramas::ast::ast> &ast, std::shared_ptr<luramas::ast::node> start, const std::int16_t target);

                        /* 
            
                These functions can't go inside lexer as it depends on some other data for it too work constructed by ast. 
            
            */
                        /* Gets list of all dests registers being set. **Dependent on for loop data** */
                        std::vector<std::uint16_t> get_dest_list(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &node);

                        /* Gets list of all source registers being set. */
                        std::vector<std::uint16_t> get_source_list(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &node);

                        /* Sets source scope exprs. */
                        void set_source_scope_expr(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Register is used twice by either source or dest without being reset by either vice versa. */
                        bool reg_used_twice(std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ast::node> &start, const std::uint16_t target, const bool same_start = false /* Dest hit and start are the same? */);

                        /* Sets linked exprs (Dependent on set arguements functions). */
                        void set_statement_expr(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace registers

                  namespace upvalues {

                        void set(std::shared_ptr<luramas::ast::ast> &ast);

                  }

                  namespace closure {

                        void set_closure_info(const std::shared_ptr<luramas::ast::ast> &current_closure, const std::size_t child_closure_id);

                  }

                  namespace branches {

                        /* Set logical operations in routine with given range. */
                        void set(std::shared_ptr<luramas::ast::ast> &ast, const std::uintptr_t begin, const std::uintptr_t end, const std::vector<std::uintptr_t> dead /* Always opposite and when hit. */, const std::int16_t logical_operation_target = -1 /* Used for ignoring ands/ors. */, const bool check_loops = false /* Checks jumps too see if they lead too loop by expr and preform opposite. */, const bool last_include = false /* Includes last compare as or. */, const bool logical_operation = false /* ??? */);

                        /* Sets valid branch routines for a range with expr: "condition_routine" */
                        void set_valid_branch_routine(std::shared_ptr<luramas::ast::ast> &ast);

                        /*

			* Sets ifs/elseifs/elses (Logical routines loops etc must be set first).

			 Logical routines does all of the dirty work already if you want too know if it's if see if end is a compare.

			 * Sets breaks too if not there.
		*/
                        void set_branch_statements(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets jumpout exprs. */
                        void set_jumpout_exprs(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace branches

                  namespace concats {

                        /*
			Concat routines are a bit special. When a concat opcode is called the start register and end register operand assemble the concat too
			get placed as a dest. This can be used too find it's routine by getting the preceeding dest of the start and the end being the concat we can determine
			the routine and where it starts and end.
		*/
                        void set_routines(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace concats

                  namespace loops {

                        /* Sorts loops based on ends. */
                        void sort_loops(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets forprep instructions exprs. ** Can be called by parent in favor for children** */
                        void set_for_prep_exprs(std::shared_ptr<luramas::ast::ast> &ast);

                        /*
			For loops are generally easy to find based on it instruction jumpback range and where a for loop instruction is though this only finds
			start and end of the routine not where variables get assembled that is usally self handled by the lifter and IGNORED by locvar huertistics
			though special cases may apply changing this.

			** Can be called by parent in favor for children**
		*/
                        void set_for_routines(std::shared_ptr<luramas::ast::ast> &ast);

                        /*
			While and repeat loops are "pretty tricky" to detect.
				* Repeat: The compare will come at the end of the routine and is near a jumpback instruction.
				* While:  The compare will come at the beginging of the routine but its jump will go past jumpback instruction.

			This issue with this is not determing which one it is, it is determing which one is a valid while,until expression then a if/elseif.
			Though some hueristics can be used to mitigate this problem and determine if its apart of the loop or not:
				* If the jump exceeds the jumpback opcode or goes directly too it.
				* If a variable doesn't get written between the next branch if there proceeds a jump.
				* Opcodes that shouldn't be in between the next branch and proceeding jump (ie. return/nop/etc).
				* Use of any of the compares used as dest between the next branch and proceeding jump.
		*/
                        void set_loop_startend_routines(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace loops

                  namespace logicals {

                        /* Sets logical expressions. */
                        void set_logical_expression(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace logicals

                  namespace arguments {

                        /*
			*Note: This isn't perfect and only sets args useful to that closure Ie. print (arg1). Things that arent neccesarly useful like
				   an argument getting set right after or inside of a routine before getting used will most of the time just become a
				   variable depending on some conditions like if it gets used after a condition but it can be set by that condition not
				   garunteed it will become an argument. All of this is put together this way instead of basing everything off of it's
				   arg stack become random args that don't even get used.

				   **Will also set sub nodes and dest nodes with sources**
		*/
                        void set(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace arguments

                  namespace tables {

                        /* Sets table elements. */
                        void set_routines(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets table exprs. */
                        void set_table_exprs(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets every table end in every table routine node too address ending of table. */
                        void set_node_end(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace tables

                  namespace locvars {

                        /*

			Sets locvars based on certain hueristics. More detailed info about it can be found in ast_config.hpp at control_flow_vars macro.
			Changing that will effect the behavior of this function but all the information you need can be found there but judge your discision
			on how you want the code to be in the decompilation. This isn't 100% perfect because where mostly judging on hueristics and nothing else.

		    */
                        void set_lv(std::shared_ptr<luramas::ast::ast> &ast);

                        /*
            
	        Set by several hueristics.
	        Sets logical operations. (Can be used for if/elseif statements).

            */
                        void set_logical_operations(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace locvars

                  namespace classes {

                  }

                  namespace nodes {

                        /* Sets exit exprs. */
                        void set_exit_exprs(std::shared_ptr<luramas::ast::ast> &ast);

                        /* Sets binary operation kinds. */
                        void set_bin_kinds(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace nodes

                  namespace flags {

                        /* Set pop flag data for nodes. */
                        void set_pops(std::shared_ptr<luramas::ast::ast> &ast);

                  } // namespace flags

            } // namespace transformers

      } // namespace ast

} // namespace luramas