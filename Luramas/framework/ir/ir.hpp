#pragma once
#include "../../framework/il/il/il.hpp"
#include "../il/code/generation/common/generate/generate.hpp"
#include "data/data.hpp"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/* 

    Lua -> Ir

    * All Ir enums are based on lua.
    * Emitter can be changed for certain profiles ie. lua -> c++/c/rust/go/js/py anything everything is there but some corners may need too be cut.

*/
namespace luramas {

      namespace ir {

            namespace ir_expr_data {

                  /* Type of compare for expr ?? expr. */
                  enum class expr_compare : std::uint8_t {
                        string /* Turns both exprs too string and compare. */
                  };

                  struct line {

                        struct expression_data {

                              luramas::ir::data::constant::constant_kinds constant = luramas::ir::data::constant::constant_kinds::nothing;
                              std::vector<luramas::ast::bin_kinds> unary;

                              struct constants {

                                    struct function {

                                          std::shared_ptr<expression_data> name = nullptr;
                                          std::vector<std::string> args;

                                          luramas::ir::data::constant::function_kinds type = luramas::ir::data::constant::function_kinds::nothing;
                                          std::vector<std::shared_ptr<line>> lines;

                                    } function;

                                    struct for_loop {

                                          std::vector<std::shared_ptr<expression_data>> vars;
                                          std::vector<std::shared_ptr<expression_data>> iterator; /* Contains start, maximum/target, and interations for numeric loops. */

                                    } for_loop;

                                    struct return_ {

                                          std::vector<std::shared_ptr<expression_data>> returns;

                                    } return_;

                                    /* Usable for anything that uses compares like if/elseif/until/while/etc (very hacky way of this working better ways of course but it works fine for lifter for deciding whos who). */
                                    struct compare {

                                          bool operator_only = false; /* Only uses operator only, "&&, ||" */
                                          bool open = false;          /* ( */
                                          bool close = false;         /* ) */

                                          luramas::ir::data::constant::logical_operations_kinds operation = luramas::ir::data::constant::logical_operations_kinds::nothing;
                                          std::shared_ptr<expression_data> rvalue = nullptr;
                                          std::shared_ptr<expression_data> lvalue = nullptr;
                                    };

                                    /* Close, open, etc is determined by ast not by lifter. (Everything is seperated besides: "?? == ??" or "??") */
                                    std::vector<std::shared_ptr<compare>> compares;

                                    std::shared_ptr<compare> make() {
                                          return std::make_shared<compare>();
                                    }

                                    /* Builds expression for given constant. (DOES NOT INCLUDE CONSTANT KEYWORD) */
                                    template <luramas::ir::data::constant::constant_kinds t>
                                    luramas::ir::data::refrence_string build_expression(const luramas::emitter_ir::syntax::emitter_syntax syn, const std::vector<luramas::ir::data::expr::refrence_kinds> &refrence, std::shared_ptr<luramas::ir::data::format::format> &format) {

                                          luramas::ir::data::refrence_string retn("");

                                          /* Compares */
                                          if (t == luramas::ir::data::constant::constant_kinds::if_ ||
                                              t == luramas::ir::data::constant::constant_kinds::elseif_ ||
                                              t == luramas::ir::data::constant::constant_kinds::while_ ||
                                              t == luramas::ir::data::constant::constant_kinds::until ||
                                              t == luramas::ir::data::constant::constant_kinds::compares) {
                                                for (auto &c : this->compares) {

                                                      if (c->operator_only) {

                                                            if (c->operation != luramas::ir::data::constant::logical_operations_kinds::nothing) {
                                                                  luramas::emitter_ir::common::logical::emit_operator(syn, retn, c->operation, format);
                                                            }

                                                      } else {

                                                            /* (?? ?? ??) */
                                                            if (c->open && c->close && c->lvalue != nullptr && c->rvalue != nullptr) {

                                                                  auto lvalue = c->lvalue->build(syn, refrence, format);
                                                                  auto rvalue = c->rvalue->build(syn, refrence, format);

                                                                  luramas::emitter_ir::common::logical::emit_logical_compare(syn, retn, c->operation, lvalue, rvalue, format);

                                                            } else {

                                                                  /* ( */
                                                                  if (c->lvalue == nullptr && c->rvalue == nullptr) {

                                                                        /* ( */
                                                                        if (c->open) {
                                                                              luramas::emitter_ir::common::logical::emit_logical_compare_parenthesis(syn, retn, format);
                                                                        }

                                                                        /* ) */
                                                                        if (c->close) {
                                                                              luramas::emitter_ir::common::logical::emit_logical_compare_parenthesis<false>(syn, retn, format);
                                                                        }

                                                                  } else {

                                                                        /* (Lvalue) */
                                                                        if (c->lvalue != nullptr && c->rvalue == nullptr) {
                                                                              auto lvalue = c->lvalue->build(syn, refrence, format);
                                                                              luramas::emitter_ir::common::logical::emit_single_logical_operation(syn, retn, lvalue, format, c->open, c->close);
                                                                        } else {

                                                                              /* (Lvalue ?? Rvalue) */
                                                                              auto lvalue = c->lvalue->build(syn, refrence, format);
                                                                              auto rvalue = c->rvalue->build(syn, refrence, format);

                                                                              luramas::emitter_ir::common::logical::emit_logical_compare(syn, retn, c->operation, lvalue, rvalue, format, c->open, c->close);
                                                                        }
                                                                  }
                                                            }
                                                      }
                                                }
                                          }

                                          return retn;
                                    }

                              } constants;

                              struct exprs {

                                    luramas::ir::data::data::data_kinds datatype_kind = luramas::ir::data::data::data_kinds::nothing;
                                    luramas::ir::data::data::type_kinds type_data_e = luramas::ir::data::data::type_kinds::nothing;
                                    luramas::ir::data::data::concat_data_kinds concat_data_e = luramas::ir::data::data::concat_data_kinds::nothing;

                                    struct data {

                                          /* Operator */
                                          struct concat {

                                                struct arith {

                                                      luramas::ast::bin_kinds operation = luramas::ast::bin_kinds::nothing;

                                                } arith;

                                                struct compare {

                                                      luramas::ir::data::constant::logical_operations_kinds operation = luramas::ir::data::constant::logical_operations_kinds::nothing;

                                                } compare;

                                                struct table {

                                                      struct table_member {

                                                            bool table_start = false; /* { */
                                                            bool table_end = false;   /*  } */

                                                            std::shared_ptr<expression_data> index = nullptr;
                                                            std::shared_ptr<expression_data> value = nullptr;
                                                      };

                                                      std::vector<std::shared_ptr<table_member>> table;

                                                } table;

                                                struct exprs {

                                                      std::shared_ptr<expression_data> rvalue = nullptr;
                                                      std::shared_ptr<expression_data> lvalue = nullptr;

                                                } exprs;

                                          } concat;

                                          struct kval {

                                                std::string kval = "";

                                          } kval;

                                          struct selfcall {

                                                std::shared_ptr<expression_data> rvalue = nullptr;
                                                std::shared_ptr<expression_data> lvalue = nullptr;

                                          } selfcall;

                                          struct idx {

                                                std::shared_ptr<expression_data> idx = nullptr;
                                                std::shared_ptr<expression_data> table = nullptr;

                                          } idx;

                                          struct var {

                                                std::string var = "";

                                          } var;

                                          struct arg {

                                                std::string arg = "";

                                          } arg;

                                          struct vararg {

                                                std::string vararg = "";

                                          } vararg;

                                          struct upvalue {

                                                std::string upvalue = "";

                                          } upvalue;

                                          struct string {

                                                std::string string = "";

                                          } string;

                                          struct global {

                                                std::string global = "";

                                          } global;

                                          struct integer {

                                                double num = 0;
                                                std::string num_str = "";

                                          } integer;

                                          struct boolean {

                                                bool boolean = false;

                                          } boolean;

                                          struct call {

                                                std::shared_ptr<expression_data> function;
                                                std::vector<std::shared_ptr<expression_data>> args; /* This will be removed ahead of time by lifter for selfcall. */

                                          } call;

                                          struct concat_string {

                                                std::vector<std::shared_ptr<expression_data>> members;

                                          } concat_string;

                                    } data;

                                    struct data_type {

                                          std::vector<std::shared_ptr<expression_data>> refrence;
                                    };

                                    /* 
                        
                                * Singulars default too left. e.g. r1(statement lvalue)(expr lvalue) = r2(statement rvalue)(expr rvalue) 
                                * Complicated stuff gets represented as e.g. ((a1 + a2) - a3) and (-(a4 - a5) - (call(1234) - 10)) 
                            
                             "(-(a4 - a5) - call(1234))":
                                * lvalue(expr): "-(a4 - a5)" (unary minus), (concat/operator("-") exprs): 
                                    * lvalue(expr): "a4" (arg): a4
                                    * rvalue(expr): "a5" (arg): a5
                                * rvalue(expr): "(call(1234) - 10)" (concat/operator("-") exprs):
                                    * lvalue(expr): "call(1234)" (call): { function = (call), args = (1234) }
                                    * rvalue(expr): "10" (integer): 10
                                
                              */
                                    data_type value_R;
                                    data_type value_L;

                                    /* Append rvalue */
                                    template <bool concat = false /* Emit too concat value. */>
                                    void set_r(const std::shared_ptr<expression_data> &expr) {
                                          if (!concat) {
                                                this->value_R.refrence.emplace_back(expr);
                                          } else {
                                                this->data.concat.exprs.rvalue = expr;
                                          }
                                          return;
                                    }

                                    /* Append lvalue */
                                    template <bool concat = false /* Emit too concat value. */>
                                    void set_l(const std::shared_ptr<expression_data> &expr) {
                                          if (!concat) {
                                                this->value_L.refrence.emplace_back(expr);
                                          } else {
                                                this->data.concat.exprs.lvalue = expr;
                                          }
                                          return;
                                    }

                              } exprs;

                              std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> clone() {
                                    return std::make_shared<luramas::ir::ir_expr_data::line::expression_data>(*this);
                              }

                              void reset() {

                                    this->constant = luramas::ir::data::constant::constant_kinds::nothing;
                                    this->unary.clear();

                                    this->constants.function.name = nullptr;
                                    this->constants.function.args.clear();
                                    this->constants.function.type = luramas::ir::data::constant::function_kinds::nothing;
                                    this->constants.function.lines.clear();

                                    this->constants.for_loop.vars.clear();
                                    this->constants.for_loop.iterator.clear();

                                    this->constants.return_.returns.clear();

                                    this->constants.compares.clear();

                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::nothing;
                                    this->exprs.type_data_e = luramas::ir::data::data::type_kinds::nothing;
                                    this->exprs.concat_data_e = luramas::ir::data::data::concat_data_kinds::nothing;
                                    this->exprs.data.concat.arith.operation = ast::bin_kinds::nothing;
                                    this->exprs.data.concat.compare.operation = luramas::ir::data::constant::logical_operations_kinds::nothing;

                                    this->exprs.data.concat.table.table.clear();
                                    this->exprs.data.concat.exprs.rvalue = nullptr;
                                    this->exprs.data.concat.exprs.lvalue = nullptr;

                                    this->exprs.data.kval.kval.clear();

                                    this->exprs.data.selfcall.rvalue = nullptr;
                                    this->exprs.data.selfcall.lvalue = nullptr;

                                    this->exprs.data.idx.idx = nullptr;
                                    this->exprs.data.idx.table = nullptr;

                                    this->exprs.data.var.var.clear();
                                    this->exprs.data.arg.arg.clear();
                                    this->exprs.data.vararg.vararg.clear();
                                    this->exprs.data.upvalue.upvalue.clear();
                                    this->exprs.data.string.string.clear();
                                    this->exprs.data.global.global.clear();
                                    this->exprs.data.integer.num = 0;
                                    this->exprs.data.integer.num_str.clear();
                                    this->exprs.data.boolean.boolean = false;

                                    this->exprs.data.call.function = nullptr;
                                    this->exprs.data.call.args.clear();

                                    this->exprs.data.concat_string.members.clear();

                                    this->exprs.value_R.refrence.clear();
                                    this->exprs.value_L.refrence.clear();

                                    return;
                              }

                              luramas::ir::data::refrence_string build(const luramas::emitter_ir::syntax::emitter_syntax syn, const std::vector<luramas::ir::data::expr::refrence_kinds> &refrence, std::shared_ptr<luramas::ir::data::format::format> &format) {

                                    luramas::ir::data::refrence_string retn("");

                                    /* Unary */
                                    for (const auto &unary : this->unary)
                                          luramas::emitter_ir::common::unary::emit_unary(syn, retn, unary, format);

                                    if (this->constant == luramas::ir::data::constant::constant_kinds::nothing) {

                                          switch (this->exprs.datatype_kind) {

                                                case luramas::ir::data::data::data_kinds::idx: {

                                                      auto table = this->exprs.data.idx.table->build(syn, refrence, format);
                                                      auto index = this->exprs.data.idx.idx->build(syn, refrence, format);

                                                      luramas::emitter_ir::common::table::emit_index(syn, retn, table, index, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::vararg: {

                                                      luramas::emitter_ir::common::datatype::emit_vararg(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::vararg) != refrence.end(), this->exprs.data.vararg.vararg, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::idx_generic: {

                                                      auto table = this->exprs.data.idx.table->build(syn, refrence, format);
                                                      auto idx = this->exprs.data.idx.idx->build(syn, refrence, format);

                                                      luramas::emitter_ir::common::table::emit_index_generic(syn, retn, table, idx, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::concat_exprs: {

                                                      switch (this->exprs.concat_data_e) {

                                                            case luramas::ir::data::data::concat_data_kinds::arith: {

                                                                  auto lvalue = this->exprs.data.concat.exprs.lvalue->build(syn, refrence, format);
                                                                  auto rvalue = this->exprs.data.concat.exprs.rvalue->build(syn, refrence, format);

                                                                  luramas::emitter_ir::common::arith_statement::emit_arith(syn, retn, this->exprs.data.concat.arith.operation, lvalue, rvalue, format);

                                                                  break;
                                                            }

                                                            case luramas::ir::data::data::concat_data_kinds::compare: {

                                                                  auto lvalue = this->exprs.data.concat.exprs.lvalue->build(syn, refrence, format);
                                                                  auto rvalue = (this->exprs.data.concat.exprs.rvalue != nullptr) ? this->exprs.data.concat.exprs.rvalue->build(syn, refrence, format) : std::string("");

                                                                  luramas::emitter_ir::common::logical::emit_logical_compare(syn, retn, this->exprs.data.concat.compare.operation, lvalue, rvalue, format);

                                                                  break;
                                                            }

                                                            case luramas::ir::data::data::concat_data_kinds::table: {

                                                                  std::vector<std::shared_ptr<luramas::ir::data::data_refrence::table_refrence_member>> vect;

                                                                  for (auto &data : this->exprs.data.concat.table.table) {

                                                                        auto ptr = std::make_shared<luramas::ir::data::data_refrence::table_refrence_member>();

                                                                        ptr->table_start = data->table_start;
                                                                        ptr->table_end = data->table_end;

                                                                        if (data->index != nullptr) {
                                                                              ptr->index = data->index->build(syn, refrence, format);
                                                                        }

                                                                        if (data->value != nullptr) {
                                                                              ptr->value = data->value->build(syn, refrence, format);
                                                                        }

                                                                        vect.emplace_back(ptr);
                                                                  }

                                                                  luramas::emitter_ir::common::table::emit_table(syn, retn, vect, format);

                                                                  break;
                                                            }

                                                            default: {
                                                                  throw std::runtime_error("Unkown concat expr.");
                                                            }
                                                      }

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::arg: {

                                                      luramas::emitter_ir::common::datatype::emit_arg(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::arg) != refrence.end(), this->exprs.data.arg.arg, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::var: {

                                                      luramas::emitter_ir::common::datatype::emit_var(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::var) != refrence.end(), this->exprs.data.var.var, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::kval: {

                                                      luramas::emitter_ir::common::datatype::emit_kval(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::kval) != refrence.end(), this->exprs.data.kval.kval, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::integer: {

                                                      luramas::emitter_ir::common::datatype::emit_integer(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::integer) != refrence.end(), this->exprs.data.integer.num_str, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::global: {

                                                      luramas::emitter_ir::common::datatype::emit_global(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::global) != refrence.end(), this->exprs.data.global.global, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::upvalue: {

                                                      luramas::emitter_ir::common::datatype::emit_upvalue(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::upvalue) != refrence.end(), this->exprs.data.upvalue.upvalue, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::boolean: {

                                                      luramas::emitter_ir::common::datatype::emit_boolean(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::boolean) != refrence.end(), this->exprs.data.boolean.boolean, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::none: {

                                                      luramas::emitter_ir::common::datatype::emit_none(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::none) != refrence.end(), format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::string: {

                                                      luramas::emitter_ir::common::datatype::emit_string(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::string) != refrence.end(), this->exprs.data.string.string, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::selfcall: {

                                                      auto lvalue = this->exprs.data.selfcall.lvalue->build(syn, refrence, format);
                                                      auto rvalue = this->exprs.data.selfcall.rvalue->build(syn, refrence, format);

                                                      luramas::emitter_ir::common::selfcall::emit_selfcall(syn, retn, std::find(refrence.begin(), refrence.end(), luramas::ir::data::expr::refrence_kinds::selfcall) != refrence.end(), lvalue, rvalue, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::call: {

                                                      std::vector<luramas::ir::data::refrence_string> vect;

                                                      for (auto &i : this->exprs.data.call.args)
                                                            vect.emplace_back(i->build(syn, refrence, format));

                                                      const auto call = this->exprs.data.call.function->build(syn, refrence, format);
                                                      luramas::emitter_ir::common::call::emit_call(syn, retn, call, vect, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::concat: {

                                                      std::vector<luramas::ir::data::refrence_string> vect;

                                                      for (auto &i : this->exprs.data.concat_string.members)
                                                            vect.emplace_back(i->build(syn, refrence, format));

                                                      luramas::emitter_ir::common::concat::emit_concat(syn, retn, vect, format);

                                                      break;
                                                }

                                                case luramas::ir::data::data::data_kinds::nothing: {
                                                      luramas::emitter_ir::common::datatype::emit_nothing(syn, retn);
                                                      break;
                                                }

                                                default: {
                                                      throw std::exception("Unkown expr type.");
                                                }
                                          }

                                    } else {

                                          switch (this->constant) {

                                                case luramas::ir::data::constant::constant_kinds::break_: {
                                                      luramas::emitter_ir::common::loop::emit_loop_break(syn, retn, format);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::end_: {
                                                      luramas::emitter_ir::common::branch::emit_scope_end(syn, retn, format);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::do_: {
                                                      luramas::emitter_ir::common::loop::emit_do(syn, retn, format);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::repeat_: {
                                                      luramas::emitter_ir::common::loop::emit_repeat(syn, retn, format);
                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::while_: {

                                                      auto compare = this->constants.build_expression<luramas::ir::data::constant::constant_kinds::while_>(syn, refrence, format);
                                                      luramas::emitter_ir::common::loop::emit_while_pre(syn, retn, compare, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::until: {

                                                      auto compare = this->constants.build_expression<luramas::ir::data::constant::constant_kinds::until>(syn, refrence, format);
                                                      luramas::emitter_ir::common::loop::emit_until_pre(syn, retn, compare, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::if_: {

                                                      auto compare = this->constants.build_expression<luramas::ir::data::constant::constant_kinds::if_>(syn, refrence, format);
                                                      luramas::emitter_ir::common::branch::emit_if_pre(syn, retn, compare, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::elseif_: {

                                                      auto compare = this->constants.build_expression<luramas::ir::data::constant::constant_kinds::elseif_>(syn, refrence, format);
                                                      luramas::emitter_ir::common::branch::emit_elseif_pre(syn, retn, compare, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::else_: {

                                                      luramas::emitter_ir::common::branch::emit_else(syn, retn, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::return_: {

                                                      std::vector<luramas::ir::data::refrence_string> vect;

                                                      for (auto &i : this->constants.return_.returns)
                                                            vect.emplace_back(i->build(syn, refrence, format));

                                                      luramas::emitter_ir::common::return_::emit_return(syn, retn, vect, format);

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::for_:
                                                case luramas::ir::data::constant::constant_kinds::for_numeric: {

                                                      std::vector<luramas::ir::data::refrence_string> iterator;
                                                      std::vector<luramas::ir::data::refrence_string> vars;

                                                      for (auto &i : this->constants.for_loop.iterator)
                                                            iterator.emplace_back(i->build(syn, refrence, format));

                                                      for (auto &i : this->constants.for_loop.vars)
                                                            vars.emplace_back(i->build(syn, refrence, format));

                                                      if (this->constant == luramas::ir::data::constant::constant_kinds::for_numeric) {
                                                            luramas::emitter_ir::common::for_::emit_for<true>(syn, retn, vars, iterator, format);
                                                      } else {
                                                            luramas::emitter_ir::common::for_::emit_for(syn, retn, vars, iterator, format);
                                                      }

                                                      break;
                                                }

                                                case luramas::ir::data::constant::constant_kinds::function_: {

                                                      break;
                                                }

                                                default: {
                                                      throw std::runtime_error("Unkown constant.");
                                                }
                                          }
                                    }

                                    return retn;
                              }

                              std::string str(const luramas::emitter_ir::syntax::emitter_syntax syn, std::shared_ptr<luramas::ir::data::format::format> &format) {
                                    return this->build(syn, {}, format).str();
                              }

                              /* Checks same string type as expr. */
                              template <luramas::ir::ir_expr_data::expr_compare cmpt = luramas::ir::ir_expr_data::expr_compare::string>
                              bool cmp(const luramas::emitter_ir::syntax::emitter_syntax syn, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &expr, std::shared_ptr<luramas::ir::data::format::format> &format) {

                                    switch (cmpt) {

                                          case luramas::ir::ir_expr_data::expr_compare::string: {
                                                return (this->build(syn, {}, format).str() == expr->build(syn, {}, format).str());
                                          }

                                          default: {
                                                return false;
                                          }
                                    }
                              }

#pragma region isa

                              template <luramas::ir::data::data::data_kinds target>
                              bool isa_type() {
                                    return target == this->exprs.datatype_kind;
                              }

#pragma endregion

#pragma region emitter

#pragma region constant

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_do() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = luramas::ir::data::constant::constant_kinds::do_;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_break() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = luramas::ir::data::constant::constant_kinds::break_;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_repeat() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = luramas::ir::data::constant::constant_kinds::repeat_;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_else() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = luramas::ir::data::constant::constant_kinds::else_;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_end() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = luramas::ir::data::constant::constant_kinds::end_;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_if() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->constant = luramas::ir::data::constant::constant_kinds::if_;

                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_while() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->constant = luramas::ir::data::constant::constant_kinds::while_;

                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_until() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->constant = luramas::ir::data::constant::constant_kinds::until;

                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_constant_elseif() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->constant = luramas::ir::data::constant::constant_kinds::elseif_;

                                    return;
                              }

                              template <luramas::ir::data::constant::function_kinds type>
                              void emitter_constant_function(const std::vector<std::shared_ptr<line>> &lines, const std::shared_ptr<expression_data> &name = nullptr) {

                                    this->constant = luramas::ir::data::constant::constant_kinds::function_;
                                    this->constants.function.name = name;
                                    this->constants.function.type = type;
                                    this->constants.function.lines = lines;

                                    return;
                              }

                              void emitter_constant_function_arg(const std::string &arg) {

                                    this->constants.function.args.emplace_back(arg);
                                    return;
                              }

#pragma endregion

#pragma region expr

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_unary(const luramas::ast::bin_kinds unary) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->unary.insert(this->unary.begin(), unary);
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_arith(const luramas::ast::bin_kinds kind, const std::shared_ptr<expression_data> &lvalue, const std::shared_ptr<expression_data> &rvalue) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::concat_exprs;
                                    this->exprs.concat_data_e = luramas::ir::data::data::concat_data_kinds::arith;
                                    this->exprs.data.concat.arith.operation = kind;
                                    this->exprs.set_l<true>(lvalue);
                                    this->exprs.set_r<true>(rvalue);
                                    return;
                              }

                              /* ??[??] */
                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_table_idx(const std::shared_ptr<expression_data> &table, const std::shared_ptr<expression_data> &index) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::idx;
                                    this->exprs.data.idx.table = table;
                                    this->exprs.data.idx.idx = index;
                                    return;
                              }

                              /* ??.?? */
                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_table_idx_generic(const std::shared_ptr<expression_data> &table, const std::shared_ptr<expression_data> &index) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::idx_generic;
                                    this->exprs.data.idx.table = table;
                                    this->exprs.data.idx.idx = index;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_self(const std::shared_ptr<expression_data> &lvalue, const std::shared_ptr<expression_data> &rvalue) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::selfcall;
                                    this->exprs.data.selfcall.lvalue = lvalue;
                                    this->exprs.data.selfcall.rvalue = rvalue;
                                    return;
                              }

#pragma region concat

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_table_new() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::concat_exprs;
                                    this->exprs.concat_data_e = luramas::ir::data::data::concat_data_kinds::table;

                                    auto ptr = std::make_shared<luramas::ir::ir_expr_data::line::expression_data::exprs::data::concat::table::table_member>();
                                    ptr->table_start = true;
                                    this->exprs.data.concat.table.table.emplace_back(ptr);

                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_table_end() {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::concat_exprs;
                                    this->exprs.concat_data_e = luramas::ir::data::data::concat_data_kinds::table;

                                    auto ptr = std::make_shared<luramas::ir::ir_expr_data::line::expression_data::exprs::data::concat::table::table_member>();
                                    ptr->table_end = true;
                                    this->exprs.data.concat.table.table.emplace_back(ptr);

                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_call(const std::shared_ptr<expression_data> &call) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::call;
                                    this->exprs.data.call.function = call;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_call_arg(const std::shared_ptr<expression_data> &arg) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.data.call.args.emplace_back(arg);
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_table_member(const std::shared_ptr<expression_data> &value, const std::shared_ptr<expression_data> &index = nullptr) {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::concat_exprs;
                                    this->exprs.concat_data_e = luramas::ir::data::data::concat_data_kinds::table;

                                    auto ptr = std::make_shared<luramas::ir::ir_expr_data::line::expression_data::exprs::data::concat::table::table_member>();
                                    ptr->index = index;
                                    ptr->value = value;
                                    this->exprs.data.concat.table.table.emplace_back(ptr);

                                    return;
                              }

                              template <bool integer = false /* Integer(true) or Generic(false) */, bool reset = false /* Resets expression */>
                              void emitter_datatype_for_var(const std::string &var) {

                                    if (reset) {
                                          this->reset();
                                    }

                                    this->constant = (integer) ? luramas::ir::data::constant::constant_kinds::for_numeric : luramas::ir::data::constant::constant_kinds::for_;

                                    auto data = luramas::ir::ir_expr_data::make::expression_data();
                                    data->emitter_datatype_var(var);
                                    this->constants.for_loop.vars.emplace_back(data);

                                    return;
                              }

                              template <bool integer = false /* Integer(true) or Generic(false) */, bool reset = false /* Resets expression */>
                              void emitter_datatype_for_iterator(const std::shared_ptr<expression_data> &iter) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->constant = (integer) ? luramas::ir::data::constant::constant_kinds::for_numeric : luramas::ir::data::constant::constant_kinds::for_;
                                    this->constants.for_loop.iterator.emplace_back(iter);
                                    return;
                              }

#pragma endregion

#pragma region data_type

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_boolean(const bool boolean) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::boolean;
                                    this->exprs.data.boolean.boolean = boolean;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_integer(const double integer) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::integer;
                                    this->exprs.data.integer.num = integer;
                                    this->exprs.data.integer.num_str = std::to_string(integer);
                                    this->exprs.data.integer.num_str.erase(this->exprs.data.integer.num_str.find_last_not_of('0') + 1, std::string::npos);
                                    this->exprs.data.integer.num_str.erase(this->exprs.data.integer.num_str.find_last_not_of('.') + 1, std::string::npos);
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_arg(const std::string &arg) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::arg;
                                    this->exprs.data.arg.arg = arg;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_upvalue(const std::string &upvalue) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::upvalue;
                                    this->exprs.data.upvalue.upvalue = upvalue;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_vararg_list(const std::string &vararg) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::vararg;
                                    this->exprs.data.vararg.vararg = vararg;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_global(const std::string &global) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::global;
                                    this->exprs.data.global.global = global;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_var(const std::string &var) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::var;
                                    this->exprs.data.var.var = var;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_kval(const std::string &kval) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::kval;
                                    this->exprs.data.kval.kval = kval;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_string(const std::string &string) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::string;
                                    this->exprs.data.string.string = string;
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_datatype_none() {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.datatype_kind = luramas::ir::data::data::data_kinds::none;
                                    return;
                              }

                              void emitter_datatype_kvalue(const luramas::il::arch::kval_kinds type, const std::string &str) {

                                    switch (type) {

                                          case luramas::il::arch::kval_kinds::string: {
                                                this->emitter_datatype_string(str);
                                                break;
                                          }

                                          case luramas::il::arch::kval_kinds::integer: {
                                                this->emitter_datatype_integer(std::stod(str));
                                                break;
                                          }

                                          case luramas::il::arch::kval_kinds::boolean: {
                                                this->emitter_datatype_boolean(std::stoi(str));
                                                break;
                                          }

                                          default: {
                                                this->emitter_datatype_kval(str);
                                                break;
                                          }
                                    }

                                    return;
                              }

#pragma endregion

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_lvalue(const std::shared_ptr<expression_data> &lvalue) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.set_l(lvalue);
                                    return;
                              }

                              template <bool reset = false /* Resets expression */>
                              void emitter_expr_rvalue(const std::shared_ptr<expression_data> &rvalue) {
                                    if (reset) {
                                          this->reset();
                                    }
                                    this->exprs.set_r(rvalue);
                                    return;
                              }

#pragma endregion

#pragma endregion
                        };

                        std::int32_t scope_id = 0;

                        /* 
                    
                      Statment enum follows statement struct respective too datatypes.
                  
                  */
                        luramas::ir::data::kinds line_type = luramas::ir::data::kinds::expression;

                        template <luramas::ir::data::kinds t, bool new_expression_pointer = false /* Automatically inits expression pointer. */>
                        void set(const std::int32_t scope_id) {

                              this->line_type = t;
                              this->scope_id = scope_id;

                              if (new_expression_pointer) {
                                    this->expression = luramas::ir::ir_expr_data::make::expression_data();
                              }

                              return;
                        }

                        std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> expression = nullptr;

                        /* ?? ?? ?? e.g. a1 = a2; */
                        struct statement {

                              struct data_types {

                                    bool is_function = false;
                                    bool is_arith = false;

                                    struct closure {

                                          std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> function = nullptr; /* Variable is set as expr in constant not variable in statement. */

                                    } closure;

                                    struct arith {

                                          luramas::ast::bin_kinds kind = luramas::ast::bin_kinds::nothing;

                                    } arith;

                              } data_types;

                              struct variable {
                                    bool new_variable = false;
                                    std::vector<std::string> name;
                              } variable;

                              /* Hiddens lvalues */
                              struct hidden {

                                    std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> lvalue; /* lvalue */

                              } hidden;

                              std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> lvalue; /* Lvalues */
                              std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> rvalue; /* Rvalues */

                              /* Sets lvalue, rvalue */
                              template <bool newvar = false>
                              void set(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue = nullptr /* Ignored for new variable. */, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue = nullptr, const std::string &name = "") {

                                    if (rvalue == nullptr) {
                                          throw std::runtime_error("Can't set null rvalue too assignment.");
                                    }

                                    if (newvar) {
                                          variable.new_variable = true;
                                          variable.name.emplace_back(name);
                                    } else {
                                          this->lvalue.clear();
                                          if (lvalue != nullptr) {
                                                this->lvalue.emplace_back(lvalue);
                                          }
                                    }

                                    if (rvalue != nullptr) {
                                          this->rvalue.emplace_back(rvalue);
                                    }

                                    return;
                              }

                              void set_closure(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &expr) {
                                    this->data_types.is_function = true;
                                    this->data_types.closure.function = expr;
                                    return;
                              }

                              /* Sets lvalue, rvalue */
                              template <bool clear = false /* Clears lvalue and rvalue */>
                              void set(const luramas::ast::bin_kinds kind, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue) {
                                    if (clear) {
                                          this->lvalue.clear();
                                          this->rvalue.clear();
                                    }
                                    this->lvalue.emplace_back(lvalue);
                                    this->rvalue.emplace_back(rvalue);
                                    this->data_types.is_arith = true;
                                    this->data_types.arith.kind = kind;
                                    return;
                              }

                              /* Sets { lvalue }, rvalue */
                              void set(const std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> &lvalue, const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &rvalue) {
                                    this->lvalue.clear();
                                    this->lvalue.insert(this->lvalue.end(), lvalue.begin(), lvalue.end());
                                    this->rvalue.emplace_back(rvalue);
                                    return;
                              }

                              /* Appends an lvalue. */
                              void append(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue) {
                                    this->lvalue.emplace_back(lvalue);
                                    return;
                              }

                              /* Sets hidden lvalue. */
                              void set_hidden(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue) {
                                    this->hidden.lvalue.clear();
                                    this->hidden.lvalue.emplace_back(lvalue);
                                    return;
                              }

                              /* Appends hidden lvalue. */
                              void append_hidden(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &lvalue) {
                                    this->hidden.lvalue.emplace_back(lvalue);
                                    return;
                              }

                        } statement;

                        struct comment {

                              std::string data = "";

                              void set(const std::string &str) {
                                    this->data = str;
                                    return;
                              }

                              void append(const std::string &str) {
                                    this->data += str;
                                    return;
                              }

                        } comment;
                  };

                  namespace cache {

                        namespace vect {

                              extern std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data>> expression_data;

                        }

                        namespace deconstruct {

                              /* Destroys all pointers in cache. */
                              __inline void deconstruct() {
                                    vect::expression_data.clear();
                                    return;
                              }

                        } // namespace deconstruct

                        namespace cache {

                              /* Appends expression too cache. */
                              __inline void expression(const std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> &ptr) {
                                    vect::expression_data.emplace_back(ptr);
                                    return;
                              }

                        } // namespace cache

                  } // namespace cache

                  namespace make {

                        /* Makes expression data pointer and caches it. */
                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> expression_data() {
                              auto ptr = std::make_shared<luramas::ir::ir_expr_data::line::expression_data>();
                              cache::cache::expression(ptr);
                              return ptr;
                        }

                  } // namespace make

                  namespace make_expression {

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_kval(const std::string &kval) {
                              auto ptr = luramas::ir::ir_expr_data::make::expression_data();
                              ptr->emitter_datatype_kval(kval);
                              return ptr;
                        }

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_none() {
                              auto ptr = luramas::ir::ir_expr_data::make::expression_data();
                              ptr->emitter_datatype_none();
                              return ptr;
                        }

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_boolean(const bool b) {
                              auto ptr = luramas::ir::ir_expr_data::make::expression_data();
                              ptr->emitter_datatype_boolean(b);
                              return ptr;
                        }

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_integer(const double n) {
                              auto ptr = luramas::ir::ir_expr_data::make::expression_data();
                              ptr->emitter_datatype_integer(n);
                              return ptr;
                        }

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_kvalue(const std::shared_ptr<luramas::il::ilang::kvalue> &kvalue) {
                              auto data = luramas::ir::ir_expr_data::make::expression_data();
                              data->emitter_datatype_kvalue(kvalue->type, kvalue->str());
                              return data;
                        }

                        __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> datatype_string(const std::string &str) {
                              auto ptr = luramas::ir::ir_expr_data::make::expression_data();
                              ptr->emitter_datatype_string(str);
                              return ptr;
                        }

                  } // namespace make_expression

            } // namespace ir_expr_data

            class lines {

                public:

                  void push_back(const std::shared_ptr<luramas::ir::ir_expr_data::line> &ptr) {
                        this->vect.push_back(ptr);
                        return;
                  }

                  void emplace_back(const std::shared_ptr<luramas::ir::ir_expr_data::line> &ptr) {
                        this->vect.emplace_back(ptr);
                        return;
                  }

                  std::shared_ptr<luramas::ir::ir_expr_data::line> operator[](const std::size_t idx) {
                        return this->vect[idx];
                  }

                  std::shared_ptr<luramas::ir::ir_expr_data::line> back() {
                        return this->vect.back();
                  }

                  std::shared_ptr<luramas::ir::ir_expr_data::line> front() {
                        return this->vect.front();
                  }

                  std::shared_ptr<luramas::ir::ir_expr_data::line> make() {
                        return std::make_shared<luramas::ir::ir_expr_data::line>();
                  }

                  std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line>> get() {
                        return this->vect;
                  }

                  std::unordered_map<std::size_t, std::string> get_map() {
                        return this->line_data_map;
                  }

                  /* New ptr and appended too vector. */
                  std::shared_ptr<luramas::ir::ir_expr_data::line> operator()() {
                        const auto ptr = this->make();
                        this->emplace_back(ptr);
                        return ptr;
                  }

                  void insert(const std::shared_ptr<luramas::ir::ir_expr_data::line> &line, const std::size_t pos) {
                        this->vect.insert(this->vect.begin() + pos, line);
                        return;
                  }

                  /* See if key exists in map. */
                  bool key_exist(const std::size_t pos) {
                        return this->line_data_map.find(pos) != this->line_data_map.end();
                  }

                  void insert_map(const std::size_t pos, const std::string &data) {

                        if (this->line_data_map.find(pos) != this->line_data_map.end()) {
                              this->line_data_map[pos] += data;
                        } else {
                              this->line_data_map.insert(std::make_pair(pos, data));
                        }

                        return;
                  }

                  luramas::emitter_ir::syntax::emitter_syntax syntax = luramas::emitter_ir::syntax::emitter_syntax::nothing;

                private:
                  std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line>> vect;
                  std::unordered_map<std::size_t /* Line */, std::string /* Data */> line_data_map; /* Append stuff at vect index. (Needs too be seperate as too prevent bugs) */
            };

            /* Constructs expression pointer. */
            __inline std::shared_ptr<luramas::ir::ir_expr_data::line::expression_data> make_expression() {
                  return luramas::ir::ir_expr_data::make::expression_data();
            }

            /* Generates code from IR. */
            std::string generate(const luramas::emitter_ir::syntax::emitter_syntax syn, const std::vector<std::shared_ptr<luramas::ir::ir_expr_data::line>> &data, std::unordered_map<std::size_t, std::string> &line_data_map, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations);

      } // namespace ir

} // namespace luramas