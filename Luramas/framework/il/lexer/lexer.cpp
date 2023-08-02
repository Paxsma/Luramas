#include "lexer.hpp"

std::shared_ptr<luramas::il::lexer::lexeme> luramas::il::lexer::lexer(std::shared_ptr<luramas::il::disassembly> &disassembly) {

      auto retn = std::make_shared<luramas::il::lexer::lexeme>();
      retn->disassembly = disassembly;

      switch (disassembly->op) {

            /* Nothing */
            case luramas::il::arch::opcodes::OP_NOP: {
                  retn->kind = luramas::il::lexer::inst_kinds::nothing;
                  retn->operands = {};
                  break;
            }

            /* Call and return. */
            case luramas::il::arch::opcodes::OP_CCALL: {
                  retn->kind = luramas::il::lexer::inst_kinds::ccall;
                  retn->operands = {(retn->disassembly->operands.back()->dis.val ? luramas::il::lexer::operand_kinds::dest : luramas::il::lexer::operand_kinds::reg) /* Has return so its dest else its reg. */, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::integer}; /* Operands are dest, int(arg), int(return) */
                  break;
            }
            case luramas::il::arch::opcodes::OP_RETURN: {
                  retn->kind = luramas::il::lexer::inst_kinds::return_;
                  retn->operands = {luramas::il::lexer::operand_kinds::reg, luramas::il::lexer::operand_kinds::value}; /* Operands are dest, int */
                  break;
            }

            /* Branch */
            case luramas::il::arch::opcodes::OP_JUMPIF:
            case luramas::il::arch::opcodes::OP_JUMPIFNOT:
            case luramas::il::arch::opcodes::OP_JUMPIFEQUAL:
            case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL:
            case luramas::il::arch::opcodes::OP_JUMPIFLESS:
            case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL:
            case luramas::il::arch::opcodes::OP_JUMPIFGREATER:
            case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::branch_condition;
                  retn->operands = {luramas::il::lexer::operand_kinds::jmpaddr}; /* Operands are addr */
                  break;
            }

            case luramas::il::arch::opcodes::OP_JUMP:
            case luramas::il::arch::opcodes::OP_JUMPBACK: {
                  retn->kind = luramas::il::lexer::inst_kinds::branch;
                  retn->operands = {luramas::il::lexer::operand_kinds::jmpaddr}; /* Operands are addr */
                  break;
            }

            case luramas::il::arch::opcodes::OP_CMP: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::compare, luramas::il::lexer::operand_kinds::compare};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPK: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::compare, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPN: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::compare, luramas::il::lexer::operand_kinds::integer};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPNONE: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::compare};
                  break;
            }

            case luramas::il::arch::opcodes::OP_CMPS: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::compare};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPSK: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPSN: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  retn->operands = {luramas::il::lexer::operand_kinds::value};
                  break;
            }
            case luramas::il::arch::opcodes::OP_CMPSNONE: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare;
                  break;
            }

            case luramas::il::arch::opcodes::OP_SETIF:
            case luramas::il::arch::opcodes::OP_SETIFNOT:
            case luramas::il::arch::opcodes::OP_SETIFEQUAL:
            case luramas::il::arch::opcodes::OP_SETIFNOTEQUAL:
            case luramas::il::arch::opcodes::OP_SETIFLESS:
            case luramas::il::arch::opcodes::OP_SETIFLESSEQUAL:
            case luramas::il::arch::opcodes::OP_SETIFGREATER:
            case luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::compare_dest;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest};
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
            case luramas::il::arch::opcodes::OP_OR: {
                  retn->kind = luramas::il::lexer::inst_kinds::arith;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::source}; /* Operands are dest, source, source */
                  break;
            }
            /* Arith with constant still follows same logic as normal but with a constant. */
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
                  retn->kind = luramas::il::lexer::inst_kinds::arith;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::kvalue}; /* Operands are dest, source, kvalue */
                  break;
            }
            /* Arith with constant integer follows same logic as normal but with a constant. */
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
                  retn->kind = luramas::il::lexer::inst_kinds::arith;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::integer}; /* Operands are dest, source, integer */
                  break;
            }

            /* Load */
            case luramas::il::arch::opcodes::OP_LOADINT: {
                  retn->kind = luramas::il::lexer::inst_kinds::load;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::integer};
                  break;
            }
            case luramas::il::arch::opcodes::OP_GETTABUPVALUE:
            case luramas::il::arch::opcodes::OP_LOADKVAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::load;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::kvalue_embeded};
                  break;
            }
            case luramas::il::arch::opcodes::OP_LOADATTR: {
                  retn->kind = luramas::il::lexer::inst_kinds::load;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::value};
                  break;
            }
            case luramas::il::arch::opcodes::OP_LOADNONE: {
                  retn->kind = luramas::il::lexer::inst_kinds::load;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest};
                  break;
            }
            case luramas::il::arch::opcodes::OP_LOADBOOL: {
                  retn->kind = luramas::il::lexer::inst_kinds::load;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::value};
                  break;
            }

            /* Global */
            case luramas::il::arch::opcodes::OP_LOADGLOBAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::get_global;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETGLOBAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::set_global;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }

            /* Attribute */
            case luramas::il::arch::opcodes::OP_SETATTR: {
                  retn->kind = luramas::il::lexer::inst_kinds::set_attr;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::value};
                  break;
            }
            case luramas::il::arch::opcodes::OP_ADDATTR: {
                  retn->kind = luramas::il::lexer::inst_kinds::set_attr;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::value};
                  break;
            }

            /* Move */
            case luramas::il::arch::opcodes::OP_MOVE: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source};
                  break;
            }

            /* Upvalue */
            case luramas::il::arch::opcodes::OP_GETUPVALUE: {
                  retn->kind = luramas::il::lexer::inst_kinds::upvalue_gs;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::upvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETUPVALUE: {
                  retn->kind = luramas::il::lexer::inst_kinds::upvalue_gs;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::upvalue};
                  break;
            }

            /* Closeupvalues, Getimport */
            case luramas::il::arch::opcodes::OP_DESTROYUPVALUES: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  break;
            }
            case luramas::il::arch::opcodes::OP_DESTROYUPVALUESA: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::reg};
                  break;
            }

            /* Table */
            case luramas::il::arch::opcodes::OP_GETTABLE: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_get;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::table_reg};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETTABLE: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_set;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::reg, luramas::il::lexer::operand_kinds::table_reg};
                  break;
            }

            /* TableK */
            case luramas::il::arch::opcodes::OP_GETTABLEK: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_get;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETTABLEK: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_set;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::reg, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }

            /* TablenN */
            case luramas::il::arch::opcodes::OP_GETTABLEN: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_get;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::table_idx};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETTABLEN: {
                  retn->kind = luramas::il::lexer::inst_kinds::table_set;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::reg, luramas::il::lexer::operand_kinds::table_idx};
                  break;
            }

            /* Newclosure, Selfcall */
            case luramas::il::arch::opcodes::OP_NEWCLOSURE: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::closure};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SELF: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }

            /* Concat */
            case luramas::il::arch::opcodes::OP_CONCAT: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::source};
                  break;
            }

            /* Not, Minus, Length, Bitnot */
            case luramas::il::arch::opcodes::OP_LEN:
            case luramas::il::arch::opcodes::OP_NOT:
            case luramas::il::arch::opcodes::OP_MINUS:
            case luramas::il::arch::opcodes::OP_BITNOT:
            case luramas::il::arch::opcodes::OP_PLUS: {
                  retn->kind = luramas::il::lexer::inst_kinds::unary;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source};
                  break;
            }

                  /* Table stuff */
            case luramas::il::arch::opcodes::OP_NEWTABLE:
            case luramas::il::arch::opcodes::OP_NEWTABLEA: {
                  retn->kind = luramas::il::lexer::inst_kinds::new_table;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::value};
                  break;
            }
            case luramas::il::arch::opcodes::OP_REFTABLE:
            case luramas::il::arch::opcodes::OP_REFTABLEA: {
                  retn->kind = luramas::il::lexer::inst_kinds::new_table;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_SETLIST: {
                  retn->kind = luramas::il::lexer::inst_kinds::set_table;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::value};
                  break;
            }

            case luramas::il::arch::opcodes::OP_POPARG: {
                  retn->kind = luramas::il::lexer::inst_kinds::special_flag;
                  retn->operands = {luramas::il::lexer::operand_kinds::reg};
                  break;
            }

            /* Forloop */
            case luramas::il::arch::opcodes::OP_INITFORLOOPN:
            case luramas::il::arch::opcodes::OP_INITFORLOOPG: {
                  retn->kind = luramas::il::lexer::inst_kinds::for_;
                  retn->operands = {luramas::il::lexer::operand_kinds::jmpaddr};
                  break;
            }
            case luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL: {
                  retn->kind = luramas::il::lexer::inst_kinds::for_;
                  retn->operands = {luramas::il::lexer::operand_kinds::jmpaddr};
                  break;
            }
            case luramas::il::arch::opcodes::OP_FORLOOPG: {
                  retn->kind = luramas::il::lexer::inst_kinds::for_;
                  retn->operands = {luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::value, luramas::il::lexer::operand_kinds::jmpaddr};
                  break;
            }
            case luramas::il::arch::opcodes::OP_FORLOOPN: {
                  retn->kind = luramas::il::lexer::inst_kinds::for_;
                  retn->operands = {luramas::il::lexer::operand_kinds::source /* Dest is not needed because it is already set before hand by IL instruction. */, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::source, luramas::il::lexer::operand_kinds::jmpaddr};
                  break;
            }

            /* Getvarargs, Dupclosure, Prepvarargs */
            case luramas::il::arch::opcodes::OP_GETVARARGS: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::value};
                  break;
            }
            case luramas::il::arch::opcodes::OP_REFCLOSURE: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  retn->operands = {luramas::il::lexer::operand_kinds::dest, luramas::il::lexer::operand_kinds::kvalue};
                  break;
            }
            case luramas::il::arch::opcodes::OP_INITVARARGS: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  break;
            }

            case luramas::il::arch::opcodes::OP_POPTOP: {
                  retn->kind = luramas::il::lexer::inst_kinds::expression;
                  break;
            }

            case luramas::il::arch::opcodes::OP_ADDUPVALUE: {
                  retn->kind = luramas::il::lexer::inst_kinds::capture;
                  retn->operands = {luramas::il::lexer::operand_kinds::upvalue_kind, luramas::il::lexer::operand_kinds::source};
                  break;
            }

            default: {
                  throw std::runtime_error("Unkown opcode when lexing.");
            }
      }

      return retn;
}