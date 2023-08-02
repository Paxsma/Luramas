#include "lifter.hpp"
#include "../../../il/code/generation/common/generate/generate.hpp"
#include "../../../ir/ir.hpp"
#include "../debug.hpp"
#include "lifter_flags.hpp"
#include "lifter_macros.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <variant>

#pragma region lifter_data

/* Suffixes for vars, args, etc. */
namespace suffixes {
      std::uintptr_t loop_variable_suffix = 0u;
      std::uintptr_t iterator_prefix_suffix = 0u;
      std::uintptr_t loop_variable_prefix_2_suffix = 0u;
      std::uintptr_t vararg_prefix = 0u;
} // namespace suffixes

/* Registers */
namespace registers {

      /* How data should be treated in lift. */
      enum class type : std::uint8_t {
            nothing, /* Nothing */
            flag,    /* Flag */
            expr,    /* Expression */
            var,     /* Vars/upvalues */
            arg,     /* Arguement */
            global   /* Global value */
      };

      struct reg {

            /* Only used by special purpose registers. */
            struct special {
                  std::size_t inside_expr = false;      /* Inc's for encapsulation. */
                  std::vector<std::uint16_t> registers; /* Registers used for popflag. */
            } special;

            registers::type type = registers::type::nothing;

            /* Will get used along side IR data. */
            std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> data = luramas::ir::ir_expr_data::make::expression_data();
            std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> sub_data = luramas::ir::ir_expr_data::make::expression_data(); /* Mostly used for stuff that needs it's own string. */

            /* Flag data */
            struct flag {

                  /* Cmp flag. */
                  struct cmp {

                        std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> cmp1 = nullptr;
                        std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> cmp2 = nullptr;

                        void clear() {
                              this->cmp1 = nullptr;
                              this->cmp2 = nullptr;
                              return;
                        }

                  } cmp;

                  /* Multiple return flag. */
                  struct mul {

                        std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> exprs; /* Clone MANUALLY per expr entry. */

                  } multret;

            } flag;

            const char *const str_type() {

                  switch (this->type) {

                        case type::nothing: {
                              return "nothing";
                        }
                        case type::flag: {
                              return "flag";
                        }
                        case type::expr: {
                              return "expr";
                        }
                        case type::var: {
                              return "var";
                        }
                        case type::arg: {
                              return "arg";
                        }
                        case type::global: {
                              return "global";
                        }

                        default: {
                              throw std::runtime_error("Unkown type for register str.");
                        }
                  }

                  return NULL;
            }

            std::shared_ptr<reg> clone() {
                  auto ptr = std::make_shared<reg>();
                  ptr->type = this->type;
                  ptr->data = this->data->clone();
                  ptr->sub_data = this->sub_data->clone();
                  ptr->special.inside_expr = this->special.inside_expr;
                  return ptr;
            }

            void replicate(const std::shared_ptr<reg> &src) {
                  this->type = src->type;
                  this->data = src->data->clone();
                  this->sub_data = src->sub_data->clone();
                  this->special.inside_expr = src->special.inside_expr;
                  return;
            }

            void clear(const bool keep_type = false) {
                  if (!keep_type) {
                        this->type = type::nothing;
                  }
                  this->special.inside_expr = false;
                  this->data->reset();
                  this->sub_data->reset();
                  this->flag.multret.exprs.clear();
                  this->flag.cmp.cmp1 = nullptr;
                  this->flag.cmp.cmp2 = nullptr;
                  return;
            }

            void new_(const bool keep_type = false) {
                  if (!keep_type) {
                        this->type = type::nothing;
                  }
                  this->special.inside_expr = false;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->sub_data = luramas::ir::ir_expr_data::make::expression_data();
                  this->flag.multret.exprs.clear();
                  this->flag.cmp.cmp1 = nullptr;
                  this->flag.cmp.cmp2 = nullptr;
                  return;
            }

            /* If lvalue is comming from another register **CLONE IT MANUALLY**!! */
            void set_lvalue(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue) {
                  this->data->emitter_expr_lvalue(lvalue);
                  return;
            }

            /* If rvalue is comming from another register **CLONE IT MANUALLY**!! */
            void set_rvalue(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue) {
                  this->data->emitter_expr_rvalue(rvalue);
                  return;
            }

            /* If source is comming from another register **CLONE IT MANUALLY**!! */
            template <registers::type tt, bool override_data = false /* If true it will clone src and set as data else just set src as data.  */>
            void set(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &src) {

                  this->type = tt;

                  if (override_data) {
                        this->data = src->clone();
                  } else {
                        this->data = src;
                  }

                  return;
            }

            template <registers::type tt>
            void set() {
                  this->type = tt;
                  return;
            }

#pragma region data_types

            template <registers::type tt>
            void set(const bool t) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_boolean(t);
                  return;
            }

            template <registers::type tt>
            void set(const double n) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_integer(n);
                  return;
            }

            template <registers::type tt>
            void set_datatype_var(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_var(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_arg(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_arg(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_vararg(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_vararg_list(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_upvalue(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_upvalue(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_global(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_global(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_kval(const std::string &s) {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_kval(s);
                  return;
            }

            template <registers::type tt>
            void set_datatype_none() {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  this->data->emitter_datatype_none();
                  return;
            }

            template <registers::type tt, const bool type /* True for new, false for end. */>
            void set_table() {
                  this->type = tt;
                  this->data = luramas::ir::ir_expr_data::make::expression_data();
                  if (type) {
                        this->data->emitter_expr_table_new();
                  } else {
                        this->data->emitter_expr_table_end();
                  }
                  return;
            }

#pragma endregion

            template <registers::type tt>
            void set_sub(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &src) {
                  this->type = tt;
                  this->sub_data = src->clone();
                  return;
            }

#pragma region compare

            template <bool front = false /* Append at front of vector? */>
            void append_compare_open() {

                  /* null */
                  if (this->data == nullptr) {
                        this->data = luramas::ir::ir_expr_data::make::expression_data();
                  }

                  this->data->constant = luramas::ir::data::constant::constant_kinds::compares;

                  auto ptr = this->data->constants.make();
                  ptr->open = true;

                  if (front) {
                        this->data->constants.compares.insert(this->data->constants.compares.begin(), ptr);
                  } else {
                        this->data->constants.compares.emplace_back(ptr);
                  }

                  return;
            }

            template <bool front = false /* Append at front of vector? */>
            void append_compare_close() {

                  /* null */
                  if (this->data == nullptr) {
                        this->data = luramas::ir::ir_expr_data::make::expression_data();
                  }

                  this->data->constant = luramas::ir::data::constant::constant_kinds::compares;

                  auto ptr = this->data->constants.make();
                  ptr->close = true;

                  if (front) {
                        this->data->constants.compares.insert(this->data->constants.compares.begin(), ptr);
                  } else {
                        this->data->constants.compares.emplace_back(ptr);
                  }

                  return;
            }

            /* Scopes all data in compare. */
            void scope_compare() {
                  this->append_compare_open<true>();
                  this->append_compare_close();
                  return;
            }

            template <bool front = false /* Append at front of vector? */>
            void append_compare(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue = nullptr, const luramas::ir::data::constant::logical_operations_kinds operation = luramas::ir::data::constant::logical_operations_kinds::nothing) {

                  /* null */
                  if (this->data == nullptr) {
                        this->data = luramas::ir::ir_expr_data::make::expression_data();
                  }

                  this->data->constant = luramas::ir::data::constant::constant_kinds::compares;

                  auto ptr = this->data->constants.make();
                  ptr->lvalue = lvalue;
                  ptr->rvalue = rvalue;
                  ptr->operation = operation;

                  if (front) {
                        this->data->constants.compares.insert(this->data->constants.compares.begin(), ptr);
                  } else {
                        this->data->constants.compares.emplace_back(ptr);
                  }

                  return;
            }

            template <luramas::ir::data::constant::logical_operations_kinds operation, bool front = false /* Append at front of vector? */>
            void append_compare_operator() {

                  /* null */
                  if (this->data == nullptr) {
                        this->data = luramas::ir::ir_expr_data::make::expression_data();
                  }

                  this->data->constant = luramas::ir::data::constant::constant_kinds::compares;

                  auto ptr = this->data->constants.make();
                  ptr->operator_only = true;
                  ptr->operation = operation;

                  if (front) {
                        this->data->constants.compares.insert(this->data->constants.compares.begin(), ptr);
                  } else {
                        this->data->constants.compares.emplace_back(ptr);
                  }

                  return;
            }

            /* Append compare opcode */
            void append_compare_op(const luramas::il::arch::opcodes op, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue = nullptr, const bool opposite = false, const bool parenth = true) {

                  /* null */
                  if (this->data == nullptr) {
                        this->data = luramas::ir::ir_expr_data::make::expression_data();
                  }

                  this->data->constant = luramas::ir::data::constant::constant_kinds::compares;
                  auto lop = luramas::ir::data::constant::logical_operations_kinds::nothing;

                  /* OP -> enum TODO put in ast stat  */
                  switch (op) {

                        case luramas::il::arch::opcodes::OP_SETIFLESS:
                        case luramas::il::arch::opcodes::OP_JUMPIFLESS: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::lt : luramas::ir::data::constant::logical_operations_kinds::gt;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFEQUAL:
                        case luramas::il::arch::opcodes::OP_JUMPIFEQUAL: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::eq : luramas::ir::data::constant::logical_operations_kinds::ne;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFLESSEQUAL:
                        case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::lte : luramas::ir::data::constant::logical_operations_kinds::gte;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFNOTEQUAL:
                        case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::ne : luramas::ir::data::constant::logical_operations_kinds::eq;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL:
                        case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::gte : luramas::ir::data::constant::logical_operations_kinds::lte;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFGREATER:
                        case luramas::il::arch::opcodes::OP_JUMPIFGREATER: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::gt : luramas::ir::data::constant::logical_operations_kinds::lt;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIF:
                        case luramas::il::arch::opcodes::OP_JUMPIF: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::nothing : luramas::ir::data::constant::logical_operations_kinds::nt;
                              break;
                        }

                        case luramas::il::arch::opcodes::OP_SETIFNOT:
                        case luramas::il::arch::opcodes::OP_JUMPIFNOT: {
                              lop = (!opposite) ? luramas::ir::data::constant::logical_operations_kinds::nt : luramas::ir::data::constant::logical_operations_kinds::nothing;
                              break;
                        }

                        default: {
                              break;
                        }
                  }

                  /* Open parenthesis: "(" */
                  if (parenth) {
                        this->append_compare_open();
                  }

                  /* Emit compare */
                  this->append_compare(lvalue, rvalue, lop);

                  /* Close parenthesis: ")" */
                  if (parenth) {
                        this->append_compare_close();
                  }

                  return;
            }

#pragma endregion

/* Is certain kind for data? */
#pragma region is_kind

            /* Is None kind? */
            bool is_none() {
                  return this->data->exprs.datatype_kind == luramas::ir::data::data::data_kinds::none;
            }

            /* Is int(double) kind? */
            bool is_int(const double n) {
                  return this->data->exprs.datatype_kind == luramas::ir::data::data::data_kinds::integer && this->data->exprs.data.integer.num == n;
            }

#pragma endregion
      };

      class reg_scope {

          private:
            /*
				-1 => : flags check: "ir_lifter_flags.hpp"
				0+ : general purpose registers.
			*/
            std::unordered_map<std::intptr_t, std::shared_ptr<reg>> registers;

          public:

            /* See if register already exists. */
            bool reg_exists(const std::intptr_t reg) {
                  return this->registers.find(reg) != this->registers.end();
            }

            std::shared_ptr<reg> operator[](const std::intptr_t reg) {

                  if (this->reg_exists(reg)) {
                        return this->registers[reg];
                  }

                  auto ptr = std::make_shared<registers::reg>();
                  this->registers.insert(std::make_pair(reg, ptr));

                  if (reg > flag_first /* First flag */) {
                        ptr->type = registers::type::flag;
                  }

                  return ptr;
            }

            /* Create a clone of current register scope. */
            reg_scope clone() {

                  reg_scope retn;

                  for (const auto &i : this->registers) {
                        retn[i.first]->replicate(i.second);
                  }

                  return retn;
            }
      };

} // namespace registers

/* Handles types. */
namespace type_handler {

      /* Returns arith operands. */
      __inline std::pair<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> /* source operand */, std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> /* source operand */> arith_operands(const luramas::il::arch::opcodes op, const std::shared_ptr<luramas::ast::node> &node, std::vector<std::shared_ptr<luramas::il::ilang::kvalue>> &kvalues, registers::reg_scope &regs) {

            std::pair<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> /* source operand */, std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> /* source operand */> retn;

            switch (op) {
                  case luramas::il::arch::opcodes::OP_ADD:
                  case luramas::il::arch::opcodes::OP_SUB:
                  case luramas::il::arch::opcodes::OP_MUL:
                  case luramas::il::arch::opcodes::OP_DIV:
                  case luramas::il::arch::opcodes::OP_MOD:
                  case luramas::il::arch::opcodes::OP_POW:
                  case luramas::il::arch::opcodes::OP_AND:
                  case luramas::il::arch::opcodes::OP_XOR:
                  case luramas::il::arch::opcodes::OP_SHL:
                  case luramas::il::arch::opcodes::OP_SHR:
                  case luramas::il::arch::opcodes::OP_IDIV:
                  case luramas::il::arch::opcodes::OP_OR: {
                        retn.first = regs[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;
                        retn.second = regs[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().back()->dis.reg]->data;
                        return retn;
                  }

                  case luramas::il::arch::opcodes::OP_ADDK:
                  case luramas::il::arch::opcodes::OP_SUBK:
                  case luramas::il::arch::opcodes::OP_MULK:
                  case luramas::il::arch::opcodes::OP_DIVK:
                  case luramas::il::arch::opcodes::OP_MODK:
                  case luramas::il::arch::opcodes::OP_POWK:
                  case luramas::il::arch::opcodes::OP_ANDK:
                  case luramas::il::arch::opcodes::OP_XORK:
                  case luramas::il::arch::opcodes::OP_SHLK:
                  case luramas::il::arch::opcodes::OP_SHRK:
                  case luramas::il::arch::opcodes::OP_IDIVK:
                  case luramas::il::arch::opcodes::OP_ORK: {
                        retn.first = regs[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;
                        retn.second = luramas::ir::ir_expr_data::make_expression::datatype_kvalue(kvalues[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().back()->dis.kvalue_idx]);
                        return retn;
                  }

                  case luramas::il::arch::opcodes::OP_ADDN:
                  case luramas::il::arch::opcodes::OP_SUBN:
                  case luramas::il::arch::opcodes::OP_MULN:
                  case luramas::il::arch::opcodes::OP_DIVN:
                  case luramas::il::arch::opcodes::OP_MODN:
                  case luramas::il::arch::opcodes::OP_POWN:
                  case luramas::il::arch::opcodes::OP_ANDN:
                  case luramas::il::arch::opcodes::OP_XORN:
                  case luramas::il::arch::opcodes::OP_SHLN:
                  case luramas::il::arch::opcodes::OP_SHRN:
                  case luramas::il::arch::opcodes::OP_IDIVN:
                  case luramas::il::arch::opcodes::OP_ORN: {
                        retn.first = regs[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;
                        retn.second = luramas::ir::ir_expr_data::make_expression::datatype_integer(node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().back()->dis.integer);
                        return retn;
                  }

                  default: {
                        throw std::runtime_error("Unkown arith instruction.");
                  }
            }

            throw std::runtime_error("No arith operand data too return.");
      }

} // namespace type_handler

#pragma endregion

void lift_blocks(const std::shared_ptr<luramas::ast::ast> &ast, const std::shared_ptr<luramas::ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations, std::vector<registers::reg_scope> &regs, luramas::ir::lines &lines) {

      /*
            Inc ON constants: while/if/etc
            Dec ON constants: end/etc.
      */
      std::int32_t scope_id = 0;

      /* Go through everything linearly everything has already passed through doing it linearly lessers headaches. */
      const auto all = ast->body->visit_all();
      for (const auto &node : all) {

            bool table_start_new_node = false; /* For old nodes append and so on. */

            /* Pass expr. */
            for (const auto &expr : node->expr) {

                  for (auto i = 0u; i < expr.second; ++i)
                        switch (expr.first) {

                              case luramas::ast::element_kinds::repeat_: {

                                    ++scope_id;

                                    /* Replicate scope. */
                                    regs.emplace_back(regs.back().clone());

                                    /* Emit repeat */
                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression->emitter_constant_repeat();

                                    break;
                              }

                              case luramas::ast::element_kinds::table_start:
                              case luramas::ast::element_kinds::concat_routine_start:
                              case luramas::ast::element_kinds::call_routine_start: {

                                    /* Write table start to reg. */
                                    if (expr.first == luramas::ast::element_kinds::table_start) {

                                          const auto reg = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                                          if (!node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::dest>()) {
                                                throw std::runtime_error("No dest operand for table start.");
                                          }

                                          if (!table_start_new_node) {
                                                regs.back()[reg]->sub_data->reset();
                                                table_start_new_node = true;
                                          }

                                          regs.back()[reg]->set<registers::type::expr>();
                                          regs.back()[reg]->sub_data->emitter_expr_table_new();
                                    }

                                    /* Inc expr */
                                    ++regs.back()[flag_compare]->special.inside_expr;
                                    ++regs.back()[flag_table]->special.inside_expr;

                                    break;
                              }

                              case luramas::ast::element_kinds::table_end:
                              case luramas::ast::element_kinds::concat_routine_end:
                              case luramas::ast::element_kinds::call_routine_end: {

                                    /* Dec expr */
                                    if (regs.back()[flag_compare]->special.inside_expr) {
                                          --regs.back()[flag_compare]->special.inside_expr;
                                    } else {
#if DEBUG_IR_LIFTER_WARNINGS
                                          std::printf("[lifter-WARNING] Trying too dec 0 unsigned flag.\n");
#endif
                                    }

                                    break;
                              }

                              case luramas::ast::element_kinds::scope_end: {

                                    --scope_id;

                                    /* Remove current register. */
                                    regs.pop_back();

                                    /* Emit end */
                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression->emitter_constant_end();

                                    break;
                              }

                              case luramas::ast::element_kinds::break_: {

                                    /* Emit break */
                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression->emitter_constant_break();

                                    break;
                              }

                              case luramas::ast::element_kinds::conditional_expression_end: {

                                    if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::source>()) {
                                          regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->set<registers::type::expr, true>(regs.back()[flag_compare]->data);
                                          regs.back()[flag_compare]->clear(true);
                                    }

                                    break;
                              }

                              case luramas::ast::element_kinds::if_:
                              case luramas::ast::element_kinds::elseif_: {

                                    ++scope_id;

                                    /* if/elseif statement */
                                    const auto cmp = regs.back()[flag_cmp]->flag.cmp;
                                    const auto cmp1 = cmp.cmp1;
                                    const auto cmp2 = cmp.cmp2;

                                    regs.back()[flag_compare]->append_compare_op(node->lex->disassembly->op, cmp1, cmp2, node->branches.opposite, false);

                                    if (optimizations->compare.parenthesis) {
                                          regs.back()[flag_compare]->scope_compare();
                                    }

                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression = regs.back()[flag_compare]->data;

                                    /* if */
                                    if (expr.first == luramas::ast::element_kinds::if_) {
                                          lines.back()->expression->emitter_constant_if();
                                    } else {
                                          lines.back()->expression->emitter_constant_elseif();
                                    }

                                    /* Clear compare flag. */
                                    regs.back()[flag_compare]->new_(true);

                                    /* Only scope for if, elseif gets handled speratly. */
                                    if (expr.first == luramas::ast::element_kinds::if_) {
                                          regs.emplace_back(regs.back().clone());
                                    }

                                    /* Add break */
                                    if (node->has_expr(luramas::ast::element_kinds::condition_break)) {

                                          /* Emit break */
                                          lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                          lines.back()->expression->emitter_constant_break();

                                          /* Emit end */
                                          lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                          lines.back()->expression->emitter_constant_end();
                                    }

                                    break;
                              }
                              case luramas::ast::element_kinds::else_: {

                                    /* Emit else. */
                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression->emitter_constant_else();

                                    /* Remove last scope and replicate next. */
                                    regs.pop_back();
                                    regs.emplace_back(regs.back().clone());

                                    /* Clear compare flag. */
                                    regs.back()[flag_compare]->clear(true);

                                    break;
                              }
                              case luramas::ast::element_kinds::jump_elseif: {

                                    /* Remove last */
                                    regs.pop_back();

                                    /* Replicate next. */
                                    regs.emplace_back(regs.back().clone());

                                    break;
                              }

                              case luramas::ast::element_kinds::condition_true: {
                                    regs.back()[flag_compare]->set<registers::type::expr>(true);
                                    break;
                              }
                              case luramas::ast::element_kinds::while_: {

                                    ++scope_id;

                                    /* Has compare **Compile compare too get emitted** */
                                    if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::compare>()) {

                                          /* While statement */
                                          const auto cmp = regs.back()[flag_cmp]->flag.cmp;
                                          const auto cmp1 = cmp.cmp1;
                                          const auto cmp2 = cmp.cmp2;

                                          regs.back()[flag_compare]->append_compare_op(node->lex->disassembly->op, cmp1, cmp2, node->branches.opposite, false);
                                    }

                                    if (optimizations->compare.parenthesis) {
                                          regs.back()[flag_compare]->scope_compare();
                                    }

                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression = regs.back()[flag_compare]->data;
                                    lines.back()->expression->emitter_constant_while();

                                    /* Clear compare flag. */
                                    regs.back()[flag_compare]->new_(true);

                                    /* Replicate next. */
                                    regs.emplace_back(regs.back().clone());

                                    break;
                              }
                              case luramas::ast::element_kinds::until_: {

                                    --scope_id;

                                    /* Has compare **Compile compare too get emitted** */
                                    if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::compare>()) {

                                          /* Until statement */
                                          const auto cmp = regs.back()[flag_cmp]->flag.cmp;
                                          const auto cmp1 = cmp.cmp1;
                                          const auto cmp2 = cmp.cmp2;

                                          regs.back()[flag_compare]->append_compare_op(node->lex->disassembly->op, cmp1, cmp2, node->branches.opposite, false);
                                    }

                                    if (optimizations->compare.parenthesis) {
                                          regs.back()[flag_compare]->scope_compare();
                                    }

                                    lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                                    lines.back()->expression = regs.back()[flag_compare]->data;
                                    lines.back()->expression->emitter_constant_until();

                                    /* Clear compare flag. */
                                    regs.back()[flag_compare]->new_(true);

                                    /* Remove current register. */
                                    regs.pop_back();

                                    break;
                              }

                              case luramas::ast::element_kinds::for_iv_start: {

                                    lines()->set<luramas::ir::data::kinds::expression, true>(++scope_id);

                                    static constexpr auto reserved = 2u + 1u; /* 2 is reserved 1 for new slot. */

                                    /* Replicate next. */
                                    regs.emplace_back(regs.back().clone());
                                    const auto begin = node->loops.end_node->lex->disassembly->operands.front()->dis.reg;

                                    /* K */
                                    const auto K = (node->loops.iteration_names.find(begin + reserved) != node->loops.iteration_names.end()) ? node->loops.iteration_names[begin + reserved] : luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(config->loop_variable_prefix, suffixes::loop_variable_suffix++, config->iteration_suffix_char);
                                    regs.back()[begin + reserved]->set_datatype_var<registers::type::var>(K);
                                    lines.back()->expression->emitter_datatype_for_var(K);

                                    /* V */
                                    const auto V = (node->loops.iteration_names.find(begin + reserved + 1u) != node->loops.iteration_names.end()) ? node->loops.iteration_names[begin + reserved + 1u] : luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(config->loop_variable_prefix_2, suffixes::loop_variable_prefix_2_suffix++, config->iteration_suffix_char);
                                    regs.back()[begin + reserved + 1u]->set_datatype_var<registers::type::var>(V);
                                    lines.back()->expression->emitter_datatype_for_var(V);

                                    /* Compile iteration statement. */
                                    for (auto i = 0u; i < 3u; ++i) {

                                          const auto &dest = regs.back()[begin + i];

                                          /* Invalid */
                                          if (dest->is_none()) {
                                                break;
                                          }

                                          lines.back()->expression->emitter_datatype_for_iterator(dest->data);

                                          /* Next not found/none so end. */
                                          if (regs.back().reg_exists(begin + i + 1u) || regs.back()[begin + i + 1u]->is_none()) {
                                                break;
                                          } else { /* Add split */
                                                lines.back()->expression->emitter_datatype_for_iterator(dest->data);
                                          }
                                    }

                                    break;
                              }
                              case luramas::ast::element_kinds::for_start: {

                                    lines()->set<luramas::ir::data::kinds::expression, true>(++scope_id);

                                    static constexpr auto reserved = 2u + 1u; /* 2 is reserved 1 for new slot. */

                                    const auto lex = node->loops.end_node->lex; /* OP_FORLOOPG */

                                    /* Replicate next. */
                                    regs.emplace_back(regs.back().clone());

                                    const auto begin = lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
                                    const auto count = lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

                                    /* Vars */
                                    for (auto i = 0u; i < count; ++i) {

                                          /* Make iterating variable name and compile it. */
                                          const auto name = (node->loops.iteration_names.find(i + begin + reserved) != node->loops.iteration_names.end()) ? node->loops.iteration_names[i + begin + reserved] : luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(config->loop_variable_prefix, suffixes::loop_variable_suffix++, config->iteration_suffix_char);

                                          regs.back()[i + begin + reserved]->set_datatype_var<registers::type::var>(name);
                                          lines.back()->expression->emitter_datatype_for_var(name);
                                    }

                                    /* Iterator */
                                    for (auto i = begin; i < (begin + reserved); ++i) {

                                          /* Make iterator and compile it. */
                                          if (regs.back().reg_exists(i)) {

                                                /* Register exists so compile it if its not none. */
                                                const auto reg = regs.back()[i];

                                                if (!reg->is_none()) {
                                                      lines.back()->expression->emitter_datatype_for_iterator(reg->data);
                                                }

                                          } else {
                                                break;
                                          }
                                    }

                                    break;
                              }
                              case luramas::ast::element_kinds::for_n_start: {

                                    lines()->set<luramas::ir::data::kinds::expression, true>(++scope_id);

                                    /* Replicate next. */
                                    regs.emplace_back(regs.back().clone());

                                    const auto sources = node->loops.end_node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>(); /* OP_FORLOOPN */
                                    const auto start = sources.front()->dis.reg;
                                    const auto max = sources[1]->dis.reg;
                                    const auto inc = sources.back()->dis.reg;

                                    /* Write start end etc and finalize. */
                                    const auto iterate = (node->loops.iteration_names.find(inc) != node->loops.iteration_names.end()) ? node->loops.iteration_names[inc] : luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(config->iterator_prefix, suffixes::iterator_prefix_suffix++, config->iteration_suffix_char);

                                    lines.back()->expression->emitter_datatype_for_iterator<true>(regs.back()[start]->data);
                                    lines.back()->expression->emitter_datatype_for_iterator<true>(regs.back()[max]->data);

                                    /* Set reg as var. */
                                    regs.back()[start]->set_datatype_var<registers::type::var>(iterate);

                                    /* Check for incrementor. */
                                    if (regs.back().reg_exists(inc) && !regs.back()[inc]->is_int(1)) {
                                          lines.back()->expression->emitter_datatype_for_iterator<true>(regs.back()[inc]->data);
                                    }

                                    lines.back()->expression->emitter_datatype_for_var<true>(iterate);

                                    break;
                              }

                              case luramas::ast::element_kinds::exit_pre: {
                                    return;
                              }

                              default: {
                                    break;
                              }
                        }
            }

#if DEBUG_IR_LIFTER_OPERANDS

            std::stringstream str;

#if DEBUG_IR_LIFTER_OPERANDS_PRINT_IL
            if (node->lex->disassembly->data != nullptr) {
                  str << "[ORIGINAL-INSTRUCTION]: " << node->lex->disassembly->data->data << std::endl;
            }

            str << "[IR-INSTRUCTION]: " << node->lex->disassembly->disassemble() << std::endl;
#else
            if (node->lex->disassembly->data != nullptr) {
                  str << "[INSTRUCTION]: " << node->lex->disassembly->data->data << std::endl;
            }
#endif

            for (auto i = 0u; i < node->lex->operands.size(); ++i) {

                  const auto oper = node->lex->disassembly->operands[i];

                  switch (node->lex->operands[i]) {

                        case luramas::il::lexer::operand_kinds::reg: {

                              if (!regs.back().reg_exists(oper->reg))
                                    str << "*	[reg( " << oper->reg << " )]: NULL\n";
                              else {
                                    const auto reg = regs.back()[oper->reg];
                                    str << "*	[reg( " << oper->reg << " )]: " << reg->str_type() << std::endl;
                                    str << "*		[data]: " << reg->data->str(format) << std::endl;
                                    str << "*		[sub_data]: " << reg->sub_data->str(format) << std::endl;
                              }

                              break;
                        }

                        case luramas::il::lexer::operand_kinds::dest: {

                              if (!regs.back().reg_exists(oper->reg))
                                    str << "*	[dest( " << oper->reg << " )]: NULL\n";
                              else {
                                    const auto reg = regs.back()[oper->reg];
                                    str << "*	[dest( " << oper->reg << " )]: " << reg->str_type() << std::endl;
                                    str << "*		[data]: " << reg->data->str(format) << std::endl;
                                    str << "*		[sub_data]: " << reg->sub_data->str(format) << std::endl;
                              }

                              break;
                        }

                        case luramas::il::lexer::operand_kinds::source: {

                              if (!regs.back().reg_exists(oper->reg)) {
                                    str << "*	[source( " << oper->reg << " )]: NULL\n";
                              } else {
                                    const auto reg = regs.back()[oper->reg];
                                    str << "*	[source( " << oper->reg << " )]: " << reg->str_type() << std::endl;
                                    str << "*		[data]: " << reg->data->str(format) << std::endl;
                                    str << "*		[sub_data]: " << reg->sub_data->str(format) << std::endl;
                              }

                              break;
                        }

                        case luramas::il::lexer::operand_kinds::integer: {
                              str << "*	[integer]: " << oper->integer_str << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::value: {
                              str << "*	[value]: " << std::to_string(oper->val) << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::compare: {

                              if (!regs.back().reg_exists(oper->reg)) {
                                    str << "*	[compare( " << oper->reg << " )]: NULL\n";
                              } else {
                                    const auto reg = regs.back()[oper->reg];
                                    str << "*	[compare( " << oper->reg << " )]: " << reg->str_type() << std::endl;
                                    str << "*		[data]: " << reg->data->str(format) << std::endl;
                                    str << "*		[sub_data]: " << reg->sub_data->str(format) << std::endl;
                              }

                              break;
                        }

                        case luramas::il::lexer::operand_kinds::jmpaddr: {
                              str << "*	[jmpaddr]: " << std::to_string(node->address + oper->ref_addr) << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::closure: {
                              str << "*	[closure]: " << std::to_string(oper->closure_idx) << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::kvalue: {
                              str << "*	[kvalue]: " << ast->il->kval[oper->kvalue_idx]->str() << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::kvalue_embeded: {
                              str << "*	[kvalue_embeded]: " << oper->k_value << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::upvalue: {
                              str << "*	[upvalue]: " << ast->upvalues[oper->upvalue_idx].first << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::table_idx: {
                              str << "*	[table_idx]: " << oper->integer_str << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::table_reg: {
                              str << "*	[table_reg]: " << std::to_string(oper->reg) << std::endl;
                              break;
                        }

                        case luramas::il::lexer::operand_kinds::upvalue_kind: {
                              str << "*	[upvalue_kind]: " << std::to_string(std::uint8_t(oper->upvalue_kind)) << std::endl;
                              break;
                        }

                        default: {
                              throw std::runtime_error("Unkown operand type for debug.");
                        }
                  }
            }

            str << "*	[exprs]: " << std::endl;
            for (const auto &p : node->expr)
                  str << "*		" << node->expr_str(p) << std::endl;

#if !DEBUG_IR_LIFTER_OPERANDS_PRINT_OVERRIDE
            std::cout << str.str() << std::endl;
#else
            lines()->set<luramas::ir::data::kinds::comment>(scope_id);
            lines.back()->comment.append(str.str());
#endif

#endif

            if (node->has_expr(luramas::ast::element_kinds::dead_instruction)) {
                  continue;
            }

/* Hex | Instruction | Hint not avialable with debug. */
#if !DEBUG_IR_LIFTER_OPERANDS

            /* Original */
            if ((config->comment.original.address || config->comment.original.bytecode || config->comment.original.disassembly || config->comment.original.disassembly_hint) && node->lex->disassembly->data != nullptr) {

                  std::vector<std::string> comments;

                  lines()->set<luramas::ir::data::kinds::comment>(scope_id);

                  if (config->comment.original.address) {
                        comments.emplace_back(std::to_string(node->lex->disassembly->addr));
                  }

                  if (config->comment.original.bytecode) {

                        std::stringstream stream;
                        for (const auto byte : node->lex->disassembly->data->bytes) {
                              stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << " ";
                        }

                        comments.emplace_back(stream.str());
                  }

                  if (config->comment.original.disassembly) {
                        comments.emplace_back(node->lex->disassembly->data->data);
                  }

                  if (config->comment.original.disassembly_hint) {
                        comments.emplace_back(node->lex->disassembly->data->hint);
                  }

                  std::string buffer = "";

                  const auto count = comments.size();
                  for (auto i = 0u; i < count; ++i) {

                        buffer += comments[i];

                        /* Add split. */
                        if ((i + 1u) != count) {
                              buffer += config->comment.original.seperator;
                        }
                  }

                  lines.back()->comment.set(buffer);
            }

#endif

            /* Instruction handler. */
            const auto current_instruction_opcode = node->lex->disassembly->op;
            switch (current_instruction_opcode) {

                  /* Nop, Break, Depricated opcodes */
                  case luramas::il::arch::opcodes::OP_NOP:
                  case luramas::il::arch::opcodes::OP_INITVARARGS: /* Unused */ {
                        break;
                  }

                  /* Load */
                  case luramas::il::arch::opcodes::OP_LOADNONE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_datatype_none();

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set({dest->data}, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  case luramas::il::arch::opcodes::OP_LOADINT:
                  case luramas::il::arch::opcodes::OP_LOADBOOL: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];

                        auto data = luramas::ir::ir_expr_data::make::expression_data();

                        if (current_instruction_opcode == luramas::il::arch::opcodes::OP_LOADINT) {
                              data->emitter_datatype_integer(node->lex->operand_expr<luramas::il::lexer::operand_kinds::integer>().front()->dis.integer);
                        } else {
                              data->emitter_datatype_boolean(node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.boolean);
                        }

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set({dest->data}, data);

                        } else {

                              /* Create var. */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose. */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  case luramas::il::arch::opcodes::OP_LOADKVAL: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto source = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue_embeded>().front()->dis.kvalue_idx];

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_datatype_kvalue(source->type, source->str());

                        /* Vararg.*/
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set({dest->data}, data);

                        } else {

                              /* Create var. */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose. */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  case luramas::il::arch::opcodes::OP_MOVE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto source = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg];

                        /* Multret call */
                        if (node->has_expr(luramas::ast::element_kinds::call_multret_start) || node->has_expr(luramas::ast::element_kinds::call_multret_member) || node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                              auto mul = regs.back()[flag_multret];
                              mul->flag.multret.exprs.emplace_back(dest->data->clone());

                              if (node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(mul->flag.multret.exprs, mul->sub_data);

                                    mul->clear();
                              }

                              continue;
                        }

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, source->data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, source->data, node->variables.name);

                              } else {

                                    /* General purpose  */
                                    dest->set<registers::type::expr, true>(source->data->clone());
                              }
                        }

                        break;
                  }

                  /* Unary  */
                  case luramas::il::arch::opcodes::OP_LEN:
                  case luramas::il::arch::opcodes::OP_NOT:
                  case luramas::il::arch::opcodes::OP_MINUS:
                  case luramas::il::arch::opcodes::OP_BITNOT:
                  case luramas::il::arch::opcodes::OP_PLUS: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto source = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg];

                        auto data = source->data->clone();
                        data->emitter_expr_unary(node->bin_kind);

                        logical_expression_dest(node, regs, data);

                        /* Vararg.*/
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set({dest->data}, data);

                        } else {

                              /* Create var. */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose. */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  /* Compare */
                  case luramas::il::arch::opcodes::OP_CMP: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg]->data;
                        flag->flag.cmp.cmp2 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().back()->dis.reg]->data;

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPN: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg]->data;
                        flag->flag.cmp.cmp2 = luramas::ir::ir_expr_data::make_expression::datatype_integer(node->lex->operand_expr<luramas::il::lexer::operand_kinds::integer>().front()->dis.integer);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPK: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg]->data;
                        flag->flag.cmp.cmp2 = luramas::ir::ir_expr_data::make_expression::datatype_kvalue(ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx]);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPNONE: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg]->data;
                        flag->flag.cmp.cmp2 = luramas::ir::ir_expr_data::make_expression::datatype_none();

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPS: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg]->data;

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPSN: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = luramas::ir::ir_expr_data::make_expression::datatype_integer(node->lex->operand_expr<luramas::il::lexer::operand_kinds::integer>().front()->dis.integer);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPSK: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = luramas::ir::ir_expr_data::make_expression::datatype_kvalue(ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx]);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CMPSNONE: {

                        auto flag = regs.back()[flag_cmp];
                        flag->flag.cmp.clear();

                        flag->flag.cmp.cmp1 = luramas::ir::ir_expr_data::make_expression::datatype_none();

                        break;
                  }

                  /* If */
                  case luramas::il::arch::opcodes::OP_JUMPIF:
                  case luramas::il::arch::opcodes::OP_JUMPIFNOT:
                  case luramas::il::arch::opcodes::OP_JUMPIFEQUAL:
                  case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL:
                  case luramas::il::arch::opcodes::OP_JUMPIFLESS:
                  case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL:
                  case luramas::il::arch::opcodes::OP_JUMPIFGREATER:
                  case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL: {

                        /* Only handles conditional for compare flag. */

                        /* Gets handled ahead of time just skip opcode. */
                        if (node->has_expr(luramas::ast::element_kinds::if_) || node->has_expr(luramas::ast::element_kinds::elseif_) ||
                            node->has_expr(luramas::ast::element_kinds::while_) || node->has_expr(luramas::ast::element_kinds::until_)) {
                              break;
                        }

                        /* Get compare */
                        const auto cmp = regs.back()[flag_cmp]->flag.cmp;
                        const auto cmp1 = cmp.cmp1;
                        const auto cmp2 = cmp.cmp2;

                        /* See pre-condition expr. */
                        for (const auto &expr : node->expr) {

                              for (auto i = 0u; i < expr.second; ++i)
                                    switch (expr.first) {

                                          case luramas::ast::element_kinds::condition_start_scope: {
                                                regs.back()[flag_compare]->append_compare_open();
                                                break;
                                          }

                                          default: {
                                                break;
                                          }
                                    }
                        }

                        /* Get next conditional loadb. */
                        std::shared_ptr<registers::reg> next_condition = nullptr;
                        if (node->has_expr(luramas::ast::element_kinds::condition_emit_next)) {

                              next_condition = regs.back()[(*(&node + 1u))->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                              next_condition->clear();
                              next_condition->type = registers::type::expr;
                        }

                        /* Emit compare next if it is not nullptr else just emit too compare flag. */
                        if (next_condition != nullptr) {
                              next_condition->append_compare_op(current_instruction_opcode, cmp1, cmp2, node->branches.opposite, false);
                        } else {
                              regs.back()[flag_compare]->append_compare_op(current_instruction_opcode, cmp1, cmp2, node->branches.opposite, false);
                        }

                        /* Automatically emit parenthesis for condition emitter. */
                        if ((node->has_expr(luramas::ast::element_kinds::condition_emit_next))) {
                              next_condition->append_compare_open<true>();
                              next_condition->append_compare_close();
                        }

                        /* See post-condition expr. */
                        for (const auto &expr : node->expr) {

                              for (auto i = 0u; i < expr.second; ++i)
                                    switch (expr.first) {

                                          case luramas::ast::element_kinds::condition_and: {
                                                regs.back()[flag_compare]->append_compare_operator<luramas::ir::data::constant::logical_operations_kinds::and_>();
                                                break;
                                          }

                                          case luramas::ast::element_kinds::condition_or: {
                                                regs.back()[flag_compare]->append_compare_operator<luramas::ir::data::constant::logical_operations_kinds::or_>();
                                                break;
                                          }

                                          case luramas::ast::element_kinds::condition_end_scope: {
                                                regs.back()[flag_compare]->append_compare_close();
                                                break;
                                          }

                                          case luramas::ast::element_kinds::condition_start_scope_post: {
                                                regs.back()[flag_compare]->append_compare_open();
                                                break;
                                          }

                                          default: {
                                                break;
                                          }
                                    }
                        }

                        /* Emit conditional */
                        if (node->has_expr(luramas::ast::element_kinds::conditional)) {

                              const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::compare>().front()->dis.reg];

                              /* Vararg */
                              if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(dest->data, regs.back()[flag_compare]->data);

                              } else {

                                    /* Create var */
                                    if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                          dest->set_datatype_var<registers::type::var>(node->variables.name);

                                          lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                          lines.back()->statement.set<true>(nullptr, regs.back()[flag_compare]->data, node->variables.name);

                                    } else {

                                          /* General purpose */
                                          dest->set<registers::type::expr, true>(regs.back()[flag_compare]->data);
                                    }
                              }

                              regs.back()[flag_compare]->clear();
                        }

                        break;
                  }

                  /* Gets handled ahead of time. */
                  case luramas::il::arch::opcodes::OP_JUMP:
                  case luramas::il::arch::opcodes::OP_JUMPBACK: {
                        break;
                  }

                  /* Arith */
                  case luramas::il::arch::opcodes::OP_ADD:
                  case luramas::il::arch::opcodes::OP_SUB:
                  case luramas::il::arch::opcodes::OP_MUL:
                  case luramas::il::arch::opcodes::OP_DIV:
                  case luramas::il::arch::opcodes::OP_MOD:
                  case luramas::il::arch::opcodes::OP_POW:
                  case luramas::il::arch::opcodes::OP_AND:
                  case luramas::il::arch::opcodes::OP_XOR:
                  case luramas::il::arch::opcodes::OP_SHL:
                  case luramas::il::arch::opcodes::OP_SHR:
                  case luramas::il::arch::opcodes::OP_IDIV:
                  case luramas::il::arch::opcodes::OP_OR:

                  case luramas::il::arch::opcodes::OP_ADDK:
                  case luramas::il::arch::opcodes::OP_SUBK:
                  case luramas::il::arch::opcodes::OP_MULK:
                  case luramas::il::arch::opcodes::OP_DIVK:
                  case luramas::il::arch::opcodes::OP_MODK:
                  case luramas::il::arch::opcodes::OP_POWK:
                  case luramas::il::arch::opcodes::OP_ANDK:
                  case luramas::il::arch::opcodes::OP_XORK:
                  case luramas::il::arch::opcodes::OP_SHLK:
                  case luramas::il::arch::opcodes::OP_SHRK:
                  case luramas::il::arch::opcodes::OP_IDIVK:
                  case luramas::il::arch::opcodes::OP_ORK:

                  case luramas::il::arch::opcodes::OP_ADDN:
                  case luramas::il::arch::opcodes::OP_SUBN:
                  case luramas::il::arch::opcodes::OP_MULN:
                  case luramas::il::arch::opcodes::OP_DIVN:
                  case luramas::il::arch::opcodes::OP_MODN:
                  case luramas::il::arch::opcodes::OP_POWN:
                  case luramas::il::arch::opcodes::OP_ANDN:
                  case luramas::il::arch::opcodes::OP_XORN:
                  case luramas::il::arch::opcodes::OP_SHLN:
                  case luramas::il::arch::opcodes::OP_SHRN:
                  case luramas::il::arch::opcodes::OP_IDIVN:
                  case luramas::il::arch::opcodes::OP_ORN: {

                        /* All arith instruction have a dest register. */
                        const auto dest_r = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;
                        const auto dest = regs.back()[dest_r];

                        const auto operands = type_handler::arith_operands(current_instruction_opcode, node, ast->il->kval, regs.back());

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_expr_arith(node->bin_kind, operands.first, operands.second);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              /* Convert too arith assignment? */
                              if ((node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::source>() && dest_r == node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg) && !optimizations->assignment.disable_arith_assignment &&
                                  luramas::emitter_ir::common::supported_arith_assignment(ast->syntax, node->bin_kind)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(node->bin_kind, operands.first, operands.second);

                              } else {
                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(dest->data, data);
                              }

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  /* Getimport, Call, Namecall */
                  case luramas::il::arch::opcodes::OP_GETTABUPVALUE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto kval = node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue_embeded>().front()->k_value;
                        auto data = luramas::ir::ir_expr_data::make_expression::datatype_kval(kval);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_CCALL: {

                        const auto prev = ast->body->visit_previous_addr(node->address);

                        const auto call = node->lex->disassembly->operands[0]->dis.reg;
                        const auto arg = node->lex->disassembly->operands[1]->dis.val;
                        const auto retn = node->lex->disassembly->operands[2]->dis.val;

                        const auto dest = regs.back()[call];

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_expr_call(dest->data);

                        /* Has args so parse them. */
                        if (arg) {

                              const auto pop = regs.back()[flag_pop]->special.registers;
                              for (auto i = 0u; i < arg; ++i) {

                                    /* Skip this with args. */
                                    if (std::find(pop.begin(), pop.end(), i) != pop.end()) {
                                          continue;
                                    } else {
                                          data->emitter_expr_call_arg(regs.back()[i + 1u + call]->data);
                                    }
                              }
                        }

                        /* Clear */
                        regs.back()[flag_pop]->special.registers.clear();

                        /* No return just emit. */
                        if (!node->lex->disassembly->operands[2]->dis.val) {
                              lines()->set<luramas::ir::data::kinds::expression>(scope_id);
                              lines.back()->expression = data;
                              continue;
                        }

                        /* Return has multiple returns fill. */
                        if (retn > 1) {

                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);

                                    for (auto i = 0u; i < retn; ++i) {
                                          regs.back()[call + i]->set_datatype_var<registers::type::var>(node->variables.multret_names[i]);
                                          lines.back()->statement.append(regs.back()[call + i]->data);
                                    }

                                    lines.back()->statement.rvalue.emplace_back(data);

                              } else {

                                    regs.back()[call]->set<registers::type::expr, true>(data);
                                    regs.back()[flag_multret]->sub_data = data;
                              }

                              continue;
                        }

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_SELF: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto source = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;
                        auto kvalue = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx];

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_self(source, luramas::ir::ir_expr_data::make_expression::datatype_kval(kvalue->str()));

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                  /* Concat, Return */
                  case luramas::il::arch::opcodes::OP_CONCAT: {

                        std::string source = "";

                        const auto sources = node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>();
                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];

                        const auto start = sources.front()->dis.reg;
                        const auto end = sources.back()->dis.reg;

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->exprs.datatype_kind = luramas::ir::data::data::data_kinds::concat;

                        /* Form data */
                        for (auto i = start; i <= end; ++i)
                              data->exprs.data.concat_string.members.emplace_back(regs.back()[i]->data);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_RETURN: {

                        auto val = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;
                        const auto original_val = val;
                        const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;

                        /* If its not main closure then write a return. */
                        if (ast->closure_kind != luramas::ast::closure_kinds::main || ast->body->has_next_inst<luramas::il::arch::opcodes::OP_RETURN>(node->address) /* Has next return. */) { /* Skip op */

                              /* Skip, "useless_return" */
                              if (!val && optimizations->return_.useless_return && node == ast->body->visit_all().back()) {
                                    continue;
                              }

                              lines()->set<luramas::ir::data::kinds::expression, true>(scope_id);
                              lines.back()->expression->constant = luramas::ir::data::constant::constant_kinds::return_;

                              /* Check to make sure if last dest reg is current dest reg and was multret if so just one return format. */
                              if (original_val == -1 && dest == val)
                                    lines.back()->expression->constants.return_.returns.emplace_back(regs.back()[dest]->data);
                              else {

                                    for (auto i = 0u; i < val; ++i)
                                          lines.back()->expression->constants.return_.returns.emplace_back(regs.back()[dest]->data);
                              }
                        }

                        break;
                  }

                  /* Set/Get global */
                  case luramas::il::arch::opcodes::OP_SETGLOBAL: {

                        const auto source = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg];
                        const auto dest = node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value;
                      
                        auto dest_data = luramas::ir::ir_expr_data::make::expression_data();
                        dest_data->emitter_datatype_global(dest);

                        /* Multret call */
                        if (node->has_expr(luramas::ast::element_kinds::call_multret_start) || node->has_expr(luramas::ast::element_kinds::call_multret_member) || node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                              auto mul = regs.back()[flag_multret];

                              mul->flag.multret.exprs.emplace_back(dest_data);

                              if (node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(mul->flag.multret.exprs, mul->sub_data);

                                    mul->clear();
                              }

                              continue;
                        }

                        lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                        lines.back()->statement.set(dest_data, source->data);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_LOADGLOBAL: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        auto source = node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->k_value;

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_datatype_global(source);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }

                        /* Set/Get/Close upvalue */
                  case luramas::il::arch::opcodes::OP_SETUPVALUE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg];
                        const auto upvalue = ast->upvalues[node->lex->operand_expr<luramas::il::lexer::operand_kinds::upvalue>().front()->dis.upvalue_idx].first;

                        auto dest_data = luramas::ir::ir_expr_data::make::expression_data();
                        dest_data->emitter_datatype_upvalue(upvalue);

                        /* Multret call */
                        if (node->has_expr(luramas::ast::element_kinds::call_multret_start) || node->has_expr(luramas::ast::element_kinds::call_multret_member) || node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                              auto mul = regs.back()[flag_multret];
                              mul->flag.multret.exprs.emplace_back(dest_data);

                              if (node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(mul->flag.multret.exprs, mul->sub_data);

                                    mul->clear();
                              }

                              continue;
                        }

                        lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                        lines.back()->statement.set(dest_data, dest->data);

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_GETUPVALUE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        auto source = ast->upvalues[node->lex->operand_expr<luramas::il::lexer::operand_kinds::upvalue>().front()->dis.upvalue_idx].first;

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_datatype_upvalue(source);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_DESTROYUPVALUESA: {

                        const auto reg = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;

                        for (const auto &i : ast->closures)
                              for (const auto &upv : i->upvalues)
                                    if (upv.second.second >= reg) {
                                          i->upvalues.erase(upv.first);
                                    }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_DESTROYUPVALUES: {

                        for (const auto &i : ast->closures)
                              i->upvalues.clear();

                        break;
                  }

                  /* Not needed upvalues are already set ahead of time by ast. */
                  case luramas::il::arch::opcodes::OP_ADDUPVALUE: {
                        break;
                  }

                  /* Closures */
                  case luramas::il::arch::opcodes::OP_NEWCLOSURE:
                  case luramas::il::arch::opcodes::OP_REFCLOSURE: {

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        std::shared_ptr<luramas::ast::ast> closure_ast = nullptr;

                        /* Get idx */
                        if (node->lex->has_operand_expr<luramas::il::lexer::operand_kinds::kvalue>()) {
                              /* FIX ME */
                              //closure_ast = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->kvalue_idx]->closure.closure;
                        } else {
                              closure_ast = ast->closures[node->lex->operand_expr<luramas::il::lexer::operand_kinds::closure>().front()->dis.closure_idx];
                        }

                        /* Compile args */
                        for (const auto &arg : closure_ast->arg_regs) {
                              data->emitter_constant_function_arg(arg.second);
                        }

                        /* Compile function */
                        switch (closure_ast->closure_kind) {

                              case luramas::ast::closure_kinds::global: {

                                    /* Line */
                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set_closure(data);

                                    /* Data expr */
                                    data->emitter_constant_function<luramas::ir::data::constant::function_kinds::global>(closure_ast->closure_ir.get(), closure_ast->closure_name);
                                    regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg]->set<registers::type::expr, true>(closure_ast->closure_name);

                                    break;
                              }

                              case luramas::ast::closure_kinds::anonymous: {

                                    /* Data expr */
                                    data->emitter_constant_function<luramas::ir::data::constant::function_kinds::anonymous>(closure_ast->closure_ir.get());
                                    regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg]->set<registers::type::expr, true>(data);

                                    break;
                              }

                              case luramas::ast::closure_kinds::scoped: {

                                    /* Line */
                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set_closure(data);

                                    /* Data expr */
                                    data->emitter_constant_function<luramas::ir::data::constant::function_kinds::scoped>(closure_ast->closure_ir.get(), closure_ast->closure_name);
                                    regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg]->set<registers::type::var, true>(closure_ast->closure_name);

                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unkown ast closure type for dupclosure/newclosure.");
                              }
                        }

                        break;
                  }

                  /* Getvarargs */
                  case luramas::il::arch::opcodes::OP_GETVARARGS: {

                        auto val = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;
                        const auto start = node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg;

                        const auto vararg = luramas::emitter_ir::common::locvar::make_locvar_alphabetical_name(config->vararg_prefix, suffixes::iterator_prefix_suffix++, config->vararg_suffix_char);

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_datatype_vararg_list(vararg);

                        /* Fix val. */
                        if (!val) {
                              val = 1u;
                        }

                        logical_expression_dest(node, regs, data);

                        /* Iterate */
                        for (auto on = start; on < (start + val); ++on) {

                              const auto dest = regs.back()[on];

                              /* Vararg */
                              if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(dest->data, data->clone());

                              } else {

                                    /* Create var */
                                    if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                          dest->set_datatype_var<registers::type::var>(node->variables.name);

                                          lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                          lines.back()->statement.set<true>(nullptr, data->clone(), node->variables.name);

                                    } else {

                                          /* General purpose */
                                          dest->set_datatype_vararg<registers::type::expr>(vararg);
                                    }
                              }
                        }

                        break;
                  }

                  /* Loop **Gets handeled ahead of time by AST expr.** */
                  case luramas::il::arch::opcodes::OP_INITFORLOOPN:
                  case luramas::il::arch::opcodes::OP_INITFORLOOPG:
                  case luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL:
                  case luramas::il::arch::opcodes::OP_FORLOOPG:
                  case luramas::il::arch::opcodes::OP_FORLOOPN: { /* Gets handled ahead of time. */
                        break;
                  }

                  /* Table get index by ??. */
                  case luramas::il::arch::opcodes::OP_GETTABLEN: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto table = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;

                        const auto idx = node->lex->operand_expr<luramas::il::lexer::operand_kinds::table_idx>().front()->dis.val;
                        auto idx_data = luramas::ir::ir_expr_data::make::expression_data();
                        idx_data->emitter_datatype_integer(idx);

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_expr_table_idx(table, idx_data);

                        logical_expression_dest(node, regs, data);

                        /* Vararg.*/
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var. */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_GETTABLEK: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto table = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;

                        auto idx = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx]->str();
                        auto idx_data = luramas::ir::ir_expr_data::make::expression_data();
                        auto data = luramas::ir::ir_expr_data::make::expression_data();

                        idx_data->emitter_datatype_kval(idx);

                        /* Check for index. */
                        if (str_idx(idx)) {

                              /* Normal idx */
                              data->emitter_expr_table_idx(table, idx_data);

                        } else {

                              /* Generic idx */
                              data->emitter_expr_table_idx_generic(table, idx_data);
                        }

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_GETTABLE: {

                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto table = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg]->data;
                        const auto idx = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::table_reg>().front()->dis.reg]->data;

                        auto data = luramas::ir::ir_expr_data::make::expression_data();
                        data->emitter_expr_table_idx(table, idx);

                        logical_expression_dest(node, regs, data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(data);
                              }
                        }

                        break;
                  }
                  /* Set */
                  case luramas::il::arch::opcodes::OP_SETTABLEN:
                  case luramas::il::arch::opcodes::OP_SETTABLEK:
                  case luramas::il::arch::opcodes::OP_SETTABLE: {

                        const auto value = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg];
                        auto table = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg];
                        bool legal = false;

                        auto idx = luramas::ir::ir_expr_data::make::expression_data();
                        auto data = luramas::ir::ir_expr_data::make::expression_data();

                        /* Set idx */
                        switch (node->lex->disassembly->op) {

                              case luramas::il::arch::opcodes::OP_SETTABLEK: {

                                    auto idx_kv = ast->il->kval[node->lex->operand_expr<luramas::il::lexer::operand_kinds::kvalue>().front()->dis.kvalue_idx];

                                    legal = idx_kv->type == luramas::il::arch::kval_kinds::string || idx_kv->type == luramas::il::arch::kval_kinds::integer;

                                    idx->emitter_datatype_kvalue(idx_kv->type, idx_kv->str());

                                    break;
                              }

                              case luramas::il::arch::opcodes::OP_SETTABLEN: {
                                    idx->emitter_datatype_integer(node->lex->operand_expr<luramas::il::lexer::operand_kinds::table_idx>().front()->dis.val);
                                    break;
                              }

                              case luramas::il::arch::opcodes::OP_SETTABLE: {
                                    idx = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::table_reg>().front()->dis.reg]->data;
                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unkown instruction for set tables.");
                              }
                        }

                        /* Non-element */
                        if (!node->has_expr(luramas::ast::element_kinds::table_element)) {

                              if (legal) {
                                    data->emitter_expr_table_idx(table->data, idx);
                              } else {
                                    data->emitter_expr_table_idx_generic(table->data, idx);
                              }
                        }

                        /* Multret call */
                        if (node->has_expr(luramas::ast::element_kinds::call_multret_start) || node->has_expr(luramas::ast::element_kinds::call_multret_member) || node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                              auto mul = regs.back()[flag_multret];
                              mul->flag.multret.exprs.emplace_back(data);

                              if (node->has_expr(luramas::ast::element_kinds::call_multret_end)) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(mul->flag.multret.exprs, mul->sub_data);

                                    mul->clear();
                              }

                              continue;
                        }

                        /* Element */
                        if (node->has_expr(luramas::ast::element_kinds::table_element)) {

                              data->emitter_expr_table_member(value->data, idx);

                              table->sub_data->emitter_expr_table_member(value->data, idx);

                              /* Not a table end so add end. */
                              const auto table_ends = node->count_expr<luramas::ast::element_kinds::table_end>();
                              for (auto i = 0u; i < table_ends; ++i)
                                    table->sub_data->emitter_expr_table_end();

                              regs.back()[flag_table]->special.inside_expr -= table_ends;
                        }

                        auto sub_data = table->sub_data;

                        logical_expression_dest(node, regs, sub_data);

                        /* Table */
                        if (node->tables.end_table == node->lex->disassembly->addr || node->tables.end_table == ast->body->visit_next(node)->address) {

                              /* Vararg */
                              if (table->type == registers::type::var || table->type == registers::type::arg) {

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set(table->data, sub_data);

                              } else {

                                    /* Create var */
                                    if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                          table->set_datatype_var<registers::type::var>(node->variables.name);

                                          lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                          lines.back()->statement.set<true>(nullptr, sub_data, node->variables.name);

                                    } else {

                                          /* General purpose */
                                          table->set<registers::type::expr, true>(sub_data);
                                    }
                              }

                        } else if (!node->tables.end_table) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(data, value->data);
                        }

                        break;
                  }
                  /* Table */
                  case luramas::il::arch::opcodes::OP_REFTABLE:
                  case luramas::il::arch::opcodes::OP_NEWTABLE:
                  case luramas::il::arch::opcodes::OP_REFTABLEA:
                  case luramas::il::arch::opcodes::OP_NEWTABLEA: {

                        /* Table is already created before hand just decide like locvar or something. */
                        auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];

                        /* Fix table ends */
                        const auto table_ends = node->count_expr<luramas::ast::element_kinds::table_end>();
                        for (auto i = 0u; i < table_ends; ++i)
                              dest->sub_data->emitter_expr_table_end();

                        regs.back()[flag_table]->special.inside_expr -= table_ends;

                        auto sub_data = dest->sub_data;
                        logical_expression_dest(node, regs, sub_data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, dest->sub_data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, dest->sub_data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(dest->sub_data);
                              }
                        }

                        break;
                  }
                  case luramas::il::arch::opcodes::OP_SETLIST: {

                        /* Table is already created before hand just decide like locvar or something. */
                        const auto dest = regs.back()[node->lex->operand_expr<luramas::il::lexer::operand_kinds::dest>().front()->dis.reg];
                        const auto start = node->lex->operand_expr<luramas::il::lexer::operand_kinds::source>().front()->dis.reg;
                        const auto end = node->count_expr<luramas::ast::element_kinds::table_end>();
                        auto amt = node->lex->operand_expr<luramas::il::lexer::operand_kinds::value>().front()->dis.val;

                        regs.back()[flag_table]->special.inside_expr -= end;

                        /* Add setlist data. */
                        for (auto i = 0u; i < unsigned(amt); ++i)
                              dest->sub_data->emitter_expr_table_member(regs.back()[start + i]->data);

                        /* Add ends */
                        for (auto i = 0u; i < end; ++i)
                              dest->sub_data->emitter_expr_table_end();

                        auto sub_data = dest->sub_data;
                        logical_expression_dest(node, regs, sub_data);

                        /* Vararg */
                        if (dest->type == registers::type::var || dest->type == registers::type::arg) {

                              lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                              lines.back()->statement.set(dest->data, dest->sub_data);

                        } else {

                              /* Create var */
                              if (node->has_expr(luramas::ast::element_kinds::locvar)) {

                                    dest->set_datatype_var<registers::type::var>(node->variables.name);

                                    lines()->set<luramas::ir::data::kinds::statement>(scope_id);
                                    lines.back()->statement.set<true>(nullptr, dest->sub_data, node->variables.name);

                              } else {

                                    /* General purpose */
                                    dest->set<registers::type::expr, true>(dest->sub_data);
                              }
                        }

                        break;
                  }

                  case luramas::il::arch::opcodes::OP_POPARG: {

                        const auto reg = node->lex->operand_expr<luramas::il::lexer::operand_kinds::reg>().front()->dis.reg;
                        regs.back()[flag_pop]->special.registers.emplace_back(reg);

                        break;
                  }
                  default: {
                        throw std::runtime_error("Unkown instruction for lifter.");
                  }
            }

            if (node->has_expr(luramas::ast::element_kinds::exit_post)) {
                  return;
            }
      }

      return;
}

void lift_ast(const std::shared_ptr<luramas::ast::ast> &main_ast, const std::shared_ptr<luramas::ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations, luramas::ir::lines &lines) {

      registers::reg_scope main_scope;
      std::vector<registers::reg_scope> scopes = {main_scope};

      /* Set args for registers. */
      auto reg = 0u;
      for (const auto &arg : main_ast->arg_regs) {
            if (arg.first != -1 /* vararg */) {
                  scopes.front()[reg++]->set_datatype_arg<registers::type::arg>(arg.second);
            }
      }

      lift_blocks(main_ast, config, format, optimizations, scopes, lines);

      return;
}

luramas::ir::lines luramas::ir::lifter::lift(const std::shared_ptr<luramas::ast::ast> &main_ast, const std::shared_ptr<lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations) {

      /* Make sure it was analyzed first. */
      if (!main_ast->analyzed) {
            throw std::runtime_error("Unanalyzed AST can not be lifted.");
      }

      /* Form closures linearly. */
      std::vector<std::shared_ptr<luramas::ast::ast>> linear_on;
      std::unordered_map<std::shared_ptr<luramas::ast::ast>, std::vector<std::shared_ptr<luramas::ast::ast>>> linear;

      linear.insert(std::make_pair(main_ast, main_ast->closures));
      linear_on.insert(linear_on.end(), main_ast->closures.begin(), main_ast->closures.end());

      while (!linear_on.empty()) {

            auto on = linear_on.back();

            /* Doesnt exists create new entry. */
            if (linear.find(on) == linear.end()) {
                  linear.insert(std::make_pair(on, on->closures));
                  linear_on.insert(linear_on.end(), on->closures.begin(), on->closures.end());
            }

            /* Remove dupes */
            std::sort(linear_on.begin(), linear_on.end());
            linear_on.erase(std::unique(linear_on.begin(), linear_on.end()), linear_on.end());

            /* Remove current */
            linear_on.erase(std::remove(linear_on.begin(), linear_on.end(), on), linear_on.end());
      }

      /* Lift by each. */
      std::vector<std::shared_ptr<luramas::ast::ast>> comleted;

      /* First do ones with no closures. */
      for (const auto &i : linear)
            if (i.second.empty() && std::find(comleted.begin(), comleted.end(), i.first) == comleted.end()) {

                  luramas::ir::lines lines;

                  /* Lift */
                  lift_ast(i.first, config, format, optimizations, i.first->closure_ir);

                  /* Add to complete. */
                  comleted.emplace_back(i.first);
                  linear.erase(i.first);

                  if (linear.empty()) {
                        break;
                  }
            }

      /* Do ones that have been completed. */
      while (!linear.empty()) {

            for (const auto &i : linear)
                  if (std::find(comleted.begin(), comleted.end(), i.first) == comleted.end()) {

                        /* Check if all closures have been analyzed. */
                        auto all = true;
                        for (const auto &p : i.second)
                              if (std::find(comleted.begin(), comleted.end(), p) == comleted.end()) {
                                    all = false;
                                    break;
                              }
                        if (!all) {
                              continue;
                        }

                        /* Lift */
                        lift_ast(i.first, config, format, optimizations, i.first->closure_ir);

                        /* Add to complete. */
                        comleted.emplace_back(i.first);
                        linear.erase(i.first);

                  } else {
                        linear.erase(i.first);
                  }
      }

      main_ast->closure_ir.syntax = main_ast->syntax;
      return main_ast->closure_ir;
}