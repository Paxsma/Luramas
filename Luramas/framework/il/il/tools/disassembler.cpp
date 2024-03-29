#include "disassembler.hpp"

const char *const luramas::il::disassembler::mnemonic_string(const luramas::il::arch::opcodes op) {

      switch (op) {

            case luramas::il::arch::opcodes::OP_NOP: {
                  return "nop";
            }

            case luramas::il::arch::opcodes::OP_LOADBOOL: {
                  return "loadbool";
            }

            case luramas::il::arch::opcodes::OP_LOADINT: {
                  return "loadint";
            }
            case luramas::il::arch::opcodes::OP_LOADNONE: {
                  return "loadnone";
            }
            case luramas::il::arch::opcodes::OP_LOADKVAL: {
                  return "loadkval";
            }
            case luramas::il::arch::opcodes::OP_LOADGLOBAL: {
                  return "loadglobal";
            }
            case luramas::il::arch::opcodes::OP_LOADATTR: {
                  return "loadattr";
            }
            case luramas::il::arch::opcodes::OP_GETTABUPVALUE: {
                  return "gettabupvalue";
            }

            case luramas::il::arch::opcodes::OP_SETGLOBAL: {
                  return "setglobal";
            }

            case luramas::il::arch::opcodes::OP_SETATTR: {
                  return "setattr";
            }
            case luramas::il::arch::opcodes::OP_ADDATTR: {
                  return "addattr";
            }

            case luramas::il::arch::opcodes::OP_MOVE: {
                  return "move";
            }

            case luramas::il::arch::opcodes::OP_ADD: {
                  return "add";
            }
            case luramas::il::arch::opcodes::OP_SUB: {
                  return "sub";
            }
            case luramas::il::arch::opcodes::OP_MUL: {
                  return "mul";
            }
            case luramas::il::arch::opcodes::OP_DIV: {
                  return "div";
            }
            case luramas::il::arch::opcodes::OP_MOD: {
                  return "mod";
            }
            case luramas::il::arch::opcodes::OP_POW: {
                  return "pow";
            }
            case luramas::il::arch::opcodes::OP_AND: {
                  return "and";
            }
            case luramas::il::arch::opcodes::OP_XOR: {
                  return "xor";
            }
            case luramas::il::arch::opcodes::OP_SHL: {
                  return "shl";
            }
            case luramas::il::arch::opcodes::OP_SHR: {
                  return "shr";
            }
            case luramas::il::arch::opcodes::OP_IDIV: {
                  return "idiv";
            }
            case luramas::il::arch::opcodes::OP_OR: {
                  return "or";
            }

            case luramas::il::arch::opcodes::OP_ADDK: {
                  return "addk";
            }
            case luramas::il::arch::opcodes::OP_SUBK: {
                  return "subk";
            }
            case luramas::il::arch::opcodes::OP_MULK: {
                  return "mulk";
            }
            case luramas::il::arch::opcodes::OP_DIVK: {
                  return "divk";
            }
            case luramas::il::arch::opcodes::OP_MODK: {
                  return "modk";
            }
            case luramas::il::arch::opcodes::OP_POWK: {
                  return "powk";
            }
            case luramas::il::arch::opcodes::OP_ANDK: {
                  return "andk";
            }
            case luramas::il::arch::opcodes::OP_XORK: {
                  return "xork";
            }
            case luramas::il::arch::opcodes::OP_SHLK: {
                  return "shlk";
            }
            case luramas::il::arch::opcodes::OP_SHRK: {
                  return "shrk";
            }
            case luramas::il::arch::opcodes::OP_IDIVK: {
                  return "idivk";
            }
            case luramas::il::arch::opcodes::OP_ORK: {
                  return "ork";
            }

            case luramas::il::arch::opcodes::OP_ADDN: {
                  return "addn";
            }
            case luramas::il::arch::opcodes::OP_SUBN: {
                  return "subn";
            }
            case luramas::il::arch::opcodes::OP_MULN: {
                  return "muln";
            }
            case luramas::il::arch::opcodes::OP_DIVN: {
                  return "divn";
            }
            case luramas::il::arch::opcodes::OP_MODN: {
                  return "modn";
            }
            case luramas::il::arch::opcodes::OP_POWN: {
                  return "pown";
            }
            case luramas::il::arch::opcodes::OP_ANDN: {
                  return "andn";
            }
            case luramas::il::arch::opcodes::OP_XORN: {
                  return "xorn";
            }
            case luramas::il::arch::opcodes::OP_SHLN: {
                  return "shln";
            }
            case luramas::il::arch::opcodes::OP_SHRN: {
                  return "shrn";
            }
            case luramas::il::arch::opcodes::OP_IDIVN: {
                  return "idivn";
            }
            case luramas::il::arch::opcodes::OP_ORN: {
                  return "orn";
            }

            case luramas::il::arch::opcodes::OP_LEN: {
                  return "len";
            }
            case luramas::il::arch::opcodes::OP_NOT: {
                  return "not";
            }
            case luramas::il::arch::opcodes::OP_MINUS: {
                  return "minus";
            }
            case luramas::il::arch::opcodes::OP_BITNOT: {
                  return "bitnot";
            }
            case luramas::il::arch::opcodes::OP_PLUS: {
                  return "plus";
            }

            case luramas::il::arch::opcodes::OP_CCALL: {
                  return "ccall";
            }

            case luramas::il::arch::opcodes::OP_VCALL: {
                  return "vcall";
            }
            case luramas::il::arch::opcodes::OP_VPUSH: {
                  return "vpush";
            }

            case luramas::il::arch::opcodes::OP_SELF: {
                  return "self";
            }

            case luramas::il::arch::opcodes::OP_RETURN: {
                  return "return";
            }

            case luramas::il::arch::opcodes::OP_CONCAT: {
                  return "concat";
            }

            case luramas::il::arch::opcodes::OP_JUMP: {
                  return "jump";
            }
            case luramas::il::arch::opcodes::OP_JUMPBACK: {
                  return "jumpback";
            }

            case luramas::il::arch::opcodes::OP_CMP: {
                  return "cmp";
            }
            case luramas::il::arch::opcodes::OP_CMPK: {
                  return "cmpk";
            }
            case luramas::il::arch::opcodes::OP_CMPN: {
                  return "cmpn";
            }
            case luramas::il::arch::opcodes::OP_CMPNONE: {
                  return "cmpnone";
            }

            case luramas::il::arch::opcodes::OP_CMPS: {
                  return "cmps";
            }
            case luramas::il::arch::opcodes::OP_CMPSK: {
                  return "cmpsk";
            }
            case luramas::il::arch::opcodes::OP_CMPSN: {
                  return "cmpsn";
            }
            case luramas::il::arch::opcodes::OP_CMPSNONE: {
                  return "cmpsnone";
            }

            case luramas::il::arch::opcodes::OP_JUMPIF: {
                  return "jumpif";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFNOT: {
                  return "jumpifnot";
            }

            case luramas::il::arch::opcodes::OP_JUMPIFEQUAL: {
                  return "jumpifequal";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL: {
                  return "jumpifnotequal";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFLESS: {
                  return "jumpiflessthan";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL: {
                  return "jumpiflessthanequal";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFGREATER: {
                  return "jumpifgreater";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL: {
                  return "jumpifgreaterequal";
            }

            case luramas::il::arch::opcodes::OP_SETIF: {
                  return "setif";
            }
            case luramas::il::arch::opcodes::OP_SETIFNOT: {
                  return "setifnot";
            }

            case luramas::il::arch::opcodes::OP_SETIFEQUAL: {
                  return "setifequal";
            }
            case luramas::il::arch::opcodes::OP_SETIFNOTEQUAL: {
                  return "setifnotequal";
            }
            case luramas::il::arch::opcodes::OP_SETIFLESS: {
                  return "setifless";
            }
            case luramas::il::arch::opcodes::OP_SETIFLESSEQUAL: {
                  return "setiflessequal";
            }
            case luramas::il::arch::opcodes::OP_SETIFGREATER: {
                  return "setifgreater";
            }
            case luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL: {
                  return "setifgreaterequal";
            }

            case luramas::il::arch::opcodes::OP_SETUPVALUE: {
                  return "setupvalue";
            }
            case luramas::il::arch::opcodes::OP_GETUPVALUE: {
                  return "getupvalue";
            }

            case luramas::il::arch::opcodes::OP_DESTROYUPVALUES: {
                  return "destroyupvalues";
            }
            case luramas::il::arch::opcodes::OP_DESTROYUPVALUESA: {
                  return "destroyupvaluesa";
            }

            case luramas::il::arch::opcodes::OP_ADDUPVALUE: {
                  return "addupvalue";
            }

            case luramas::il::arch::opcodes::OP_RESUME: {
                  return "resume";
            }

            case luramas::il::arch::opcodes::OP_GETVARARGS: {
                  return "getvarargs";
            }

            case luramas::il::arch::opcodes::OP_SETTABLE: {
                  return "settable";
            }
            case luramas::il::arch::opcodes::OP_GETTABLE: {
                  return "gettable";
            }

            case luramas::il::arch::opcodes::OP_SETTABLEN: {
                  return "settablen";
            }
            case luramas::il::arch::opcodes::OP_GETTABLEN: {
                  return "gettablen";
            }

            case luramas::il::arch::opcodes::OP_SETTABLEK: {
                  return "settablek";
            }
            case luramas::il::arch::opcodes::OP_GETTABLEK: {
                  return "gettablek";
            }

            case luramas::il::arch::opcodes::OP_INITFORLOOPN: {
                  return "initforloopn";
            }
            case luramas::il::arch::opcodes::OP_INITFORLOOPG: {
                  return "initforloopg";
            }
            case luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL: {
                  return "initforloopspecial";
            }

            case luramas::il::arch::opcodes::OP_NEWCLOSURE: {
                  return "newclosure";
            }
            case luramas::il::arch::opcodes::OP_REFCLOSURE: {
                  return "refclosure";
            }

            case luramas::il::arch::opcodes::OP_NEWTABLE: {
                  return "newtable";
            }
            case luramas::il::arch::opcodes::OP_REFTABLE: {
                  return "reftable";
            }

            case luramas::il::arch::opcodes::OP_NEWTABLEA: {
                  return "newtablea";
            }
            case luramas::il::arch::opcodes::OP_REFTABLEA: {
                  return "reftablea";
            }

            case luramas::il::arch::opcodes::OP_SETLIST: {
                  return "setlist";
            }

            case luramas::il::arch::opcodes::OP_FORLOOPG: {
                  return "forloopg";
            }
            case luramas::il::arch::opcodes::OP_FORLOOPN: {
                  return "forloopn";
            }

            case luramas::il::arch::opcodes::OP_POPTOP: {
                  return "poptop";
            }
            case luramas::il::arch::opcodes::OP_POPARG: {
                  return "poparg";
            }

            default: {
                  throw std::runtime_error("Unkown IL opcode.");
            }
      }

      return NULL;
}

const char *const luramas::il::disassembler::mnemonic_hint_string(const luramas::il::arch::opcodes op) {

      switch (op) {

            case luramas::il::arch::opcodes::OP_NOP: {
                  return "Nothing";
            }

            case luramas::il::arch::opcodes::OP_LOADBOOL: {
                  return "Loads a boolean to a register.";
            }
            case luramas::il::arch::opcodes::OP_LOADINT: {
                  return "Loads a integer to a register.";
            }
            case luramas::il::arch::opcodes::OP_LOADNONE: {
                  return "Loads a none object to a register.";
            }
            case luramas::il::arch::opcodes::OP_LOADKVAL: {
                  return "Loads a kvalue to a register.";
            }
            case luramas::il::arch::opcodes::OP_LOADGLOBAL: {
                  return "Loads a global to a register.";
            }
            case luramas::il::arch::opcodes::OP_LOADATTR: {
                  return "Loads a attribute from attribute stack to a register.";
            }
            case luramas::il::arch::opcodes::OP_GETTABUPVALUE: {
                  return "Loads an embeded kvalue to a register.";
            }

            case luramas::il::arch::opcodes::OP_SETGLOBAL: {
                  return "Sets global with a register value.";
            }

            case luramas::il::arch::opcodes::OP_SETATTR: {
                  return "Sets attribute on attribute stack";
            }
            case luramas::il::arch::opcodes::OP_ADDATTR: {
                  return "Adds attribute on attribute stack";
            }

            case luramas::il::arch::opcodes::OP_MOVE: {
                  return "Copys register contents into another register.";
            }

            case luramas::il::arch::opcodes::OP_ADD: {
                  return "Arithmetic add with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_SUB: {
                  return "Arithmetic sub with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_MUL: {
                  return "Arithmetic mul with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_DIV: {
                  return "Arithmetic div with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_MOD: {
                  return "Arithmetic mod with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_POW: {
                  return "Arithmetic pow with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_XOR: {
                  return "Bitwise xor with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_AND: {
                  return "Bitwise and with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_SHL: {
                  return "Bitwise shift left with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_SHR: {
                  return "Bitwise shift right with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_IDIV: {
                  return "Floor division shift right with a destination register and two source operands.";
            }
            case luramas::il::arch::opcodes::OP_OR: {
                  return "Bitwise or with a destination register and two source operands.";
            }

            case luramas::il::arch::opcodes::OP_ADDK: {
                  return "Arithmetic add with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_SUBK: {
                  return "Arithmetic sub with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_MULK: {
                  return "Arithmetic mul with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_DIVK: {
                  return "Arithmetic div with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_MODK: {
                  return "Arithmetic mod with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_POWK: {
                  return "Arithmetic pow with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_XORK: {
                  return "Bitwise xor with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_ANDK: {
                  return "Bitwise and with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_SHLK: {
                  return "Bitwise shift left with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_SHRK: {
                  return "Bitwise shift right with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_IDIVK: {
                  return "Floor division with a destination register, a source register, and a source kvalue operand.";
            }
            case luramas::il::arch::opcodes::OP_ORK: {
                  return "Bitwise or with a destination register, a source register, and a source kvalue operand.";
            }

            case luramas::il::arch::opcodes::OP_ADDN: {
                  return "Arithmetic add with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_SUBN: {
                  return "Arithmetic sub with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_MULN: {
                  return "Arithmetic mul with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_DIVN: {
                  return "Arithmetic div with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_MODN: {
                  return "Arithmetic mod with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_POWN: {
                  return "Arithmetic pow with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_XORN: {
                  return "Bitwise xor with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_ANDN: {
                  return "Bitwise and with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_SHLN: {
                  return "Bitwise shift left with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_SHRN: {
                  return "Bitwise shift right with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_IDIVN: {
                  return "Floor division right with a destination register, a source register, and a source integer operand.";
            }
            case luramas::il::arch::opcodes::OP_ORN: {
                  return "Bitwise or with a destination register, a source register, and a source integer operand.";
            }

            case luramas::il::arch::opcodes::OP_LEN: {
                  return "Unary table lenght.";
            }
            case luramas::il::arch::opcodes::OP_NOT: {
                  return "Unary not.";
            }
            case luramas::il::arch::opcodes::OP_MINUS: {
                  return "Unary minus.";
            }
            case luramas::il::arch::opcodes::OP_BITNOT: {
                  return "Unary bitwise not.";
            }
            case luramas::il::arch::opcodes::OP_PLUS: {
                  return "Unary plus.";
            }

            case luramas::il::arch::opcodes::OP_CCALL: {
                  return "C function call instruction that uses arguement stack for its arguements.";
            }

            case luramas::il::arch::opcodes::OP_VCALL: {
                  return "Virtual function call instruction that uses isolated arguement stack for its arguements.";
            }
            case luramas::il::arch::opcodes::OP_VPUSH: {
                  return "Pushes register to virtual arguement call stack.";
            }

            case luramas::il::arch::opcodes::OP_SELF: {
                  return "Loads table function too register.";
            }

            case luramas::il::arch::opcodes::OP_RETURN: {
                  return "Return instruction.";
            }

            case luramas::il::arch::opcodes::OP_CONCAT: {
                  return "String concat instruction.";
            }

            case luramas::il::arch::opcodes::OP_JUMP: {
                  return "Jump too address.";
            }
            case luramas::il::arch::opcodes::OP_JUMPBACK: {
                  return "Jump backwards in code too address.";
            }

            case luramas::il::arch::opcodes::OP_CMP: {
                  return "Sets cmp flag with two compare registers.";
            }
            case luramas::il::arch::opcodes::OP_CMPK: {
                  return "Sets cmp flag with a compare register and a kvalue.";
            }
            case luramas::il::arch::opcodes::OP_CMPN: {
                  return "Sets cmp flag with a compare register and integer.";
            }
            case luramas::il::arch::opcodes::OP_CMPNONE: {
                  return "Sets cmp flag with a compare register and a none object.";
            }

            case luramas::il::arch::opcodes::OP_CMPS: {
                  return "Sets cmp flag with a compare register.";
            }
            case luramas::il::arch::opcodes::OP_CMPSK: {
                  return "Sets cmp flag with a compare kvalue.";
            }
            case luramas::il::arch::opcodes::OP_CMPSN: {
                  return "Sets cmp flag with a compare integer.";
            }
            case luramas::il::arch::opcodes::OP_CMPSNONE: {
                  return "Sets cmp flag with a compare none object.";
            }

            case luramas::il::arch::opcodes::OP_JUMPIF: {
                  return "Jump if register compare with register is true.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFNOT: {
                  return "Jump if register compare with register is not true.";
            }

            case luramas::il::arch::opcodes::OP_JUMPIFEQUAL: {
                  return "Jump if register compare with register is equal.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFNOTEQUAL: {
                  return "Jump if register compare with register is not equal.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFLESS: {
                  return "Jump if register compare with register is less than.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFLESSEQUAL: {
                  return "Jump if register compare with register is less than or equal too.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFGREATER: {
                  return "Jump if register compare with register is greater than.";
            }
            case luramas::il::arch::opcodes::OP_JUMPIFGREATEREQUAL: {
                  return "Jump if register compare with register is greater than or equal too.";
            }

            case luramas::il::arch::opcodes::OP_SETIF: {
                  return "Sets register with true or false if cmp flag is true.";
            }
            case luramas::il::arch::opcodes::OP_SETIFNOT: {
                  return "Sets register with true or false if cmp flag is not true.";
            }

            case luramas::il::arch::opcodes::OP_SETIFEQUAL: {
                  return "Sets register with true or false if cmp flag if equal too.";
            }
            case luramas::il::arch::opcodes::OP_SETIFNOTEQUAL: {
                  return "Sets register with true or false if cmp flag if not equal too.";
            }
            case luramas::il::arch::opcodes::OP_SETIFLESS: {
                  return "Sets register with true or false if cmp flag if less than.";
            }
            case luramas::il::arch::opcodes::OP_SETIFLESSEQUAL: {
                  return "Sets register with true or false if cmp flag if less than or equal too.";
            }
            case luramas::il::arch::opcodes::OP_SETIFGREATER: {
                  return "Sets register with true or false if cmp flag if greater than.";
            }
            case luramas::il::arch::opcodes::OP_SETIFGREATEREQUAL: {
                  return "Sets register with true or false if cmp flag if greater than or equal too.";
            }

            case luramas::il::arch::opcodes::OP_SETUPVALUE: {
                  return "Set upvalue with an register as the value.";
            }
            case luramas::il::arch::opcodes::OP_GETUPVALUE: {
                  return "Gets upvalue and sets it too a register.";
            }

            case luramas::il::arch::opcodes::OP_DESTROYUPVALUES: {
                  return "Destroys all upvalue in current closure.";
            }

            case luramas::il::arch::opcodes::OP_DESTROYUPVALUESA: {
                  return "Destroys all upvalue with target in current closure.";
            }

            case luramas::il::arch::opcodes::OP_ADDUPVALUE: {
                  return "Adds upvalue too next closure in following code.";
            }

            case luramas::il::arch::opcodes::OP_POPTOP: {
                  return "Pops register from top of the stack.";
            }

            case luramas::il::arch::opcodes::OP_RESUME: {
                  return "Initiates script.";
            }

            case luramas::il::arch::opcodes::OP_GETVARARGS: {
                  return "Gets varargs and sets it a register.";
            }

            case luramas::il::arch::opcodes::OP_SETTABLE: {
                  return "Sets value in table with a register index.";
            }
            case luramas::il::arch::opcodes::OP_GETTABLE: {
                  return "Gets value in table with a register index.";
            }

            case luramas::il::arch::opcodes::OP_SETTABLEN: {
                  return "Sets value in table with a integer index.";
            }
            case luramas::il::arch::opcodes::OP_GETTABLEN: {
                  return "Gets value in table with a integer index.";
            }

            case luramas::il::arch::opcodes::OP_SETTABLEK: {
                  return "Sets value in table with a kvalue index.";
            }
            case luramas::il::arch::opcodes::OP_GETTABLEK: {
                  return "Gets value in table with a kvalue index.";
            }

            case luramas::il::arch::opcodes::OP_INITFORLOOPN: {
                  return "Initiates numeric for loop.";
            }
            case luramas::il::arch::opcodes::OP_INITFORLOOPG: {
                  return "Initiates generic for loop.";
            }
            case luramas::il::arch::opcodes::OP_INITFORLOOPSPECIAL: {
                  return "Initiates special generic for loop.";
            }

            case luramas::il::arch::opcodes::OP_NEWCLOSURE: {
                  return "Creates a new closure for register.";
            }
            case luramas::il::arch::opcodes::OP_REFCLOSURE: {
                  return "Loads register with kvalue closure. ";
            }

            case luramas::il::arch::opcodes::OP_NEWTABLE: {
                  return "Creates new table for register with exact sizes.";
            }
            case luramas::il::arch::opcodes::OP_REFTABLE: {
                  return "Loads register with kvalue table that has approximate sizes.";
            }

            case luramas::il::arch::opcodes::OP_NEWTABLEA: {
                  return "Creates new table for register with approximate sizes.";
            }
            case luramas::il::arch::opcodes::OP_REFTABLEA: {
                  return "Loads register with kvalue table that has approximate sizes.";
            }

            case luramas::il::arch::opcodes::OP_SETLIST: {
                  return "Appends elements in table.";
            }

            case luramas::il::arch::opcodes::OP_POPARG: {
                  return "Adds register too pop arguement flag.";
            }

            case luramas::il::arch::opcodes::OP_FORLOOPG: {
                  return "For generic loop.";
            }
            case luramas::il::arch::opcodes::OP_FORLOOPN: {
                  return "For numeric loop.";
            }
            default: {
                  throw std::runtime_error("Unkown IL opcode.");
            }
      }

      return NULL;
}

std::string luramas::il::disassembler::operand_string(const std::shared_ptr<luramas::il::arch::operand::operand> &operand) {

      std::string retn = "";

      switch (operand->type) {

            case luramas::il::arch::operand::operand_kind::none: {
                  retn = "none";
                  break;
            }
            case luramas::il::arch::operand::operand_kind::reg: {
                  retn = "r" + std::to_string(operand->dis.reg);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::kval: { /* Will still be represented as string. */
                  retn = operand->k_value;
                  break;
            }
            case luramas::il::arch::operand::operand_kind::kval_embeded: {
                  retn = operand->k_value;
                  break;
            }
            case luramas::il::arch::operand::operand_kind::table_size: {
                  retn = std::to_string(operand->dis.table_size);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::jmp: {
                  retn = std::to_string(operand->ref_addr);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::closure_index: {
                  retn = std::to_string(operand->dis.closure_idx);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::upvalue: {
                  retn = std::to_string(operand->dis.upvalue_idx);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::integer: {
                  retn = operand->integer_str;
                  break;
            }
            case luramas::il::arch::operand::operand_kind::value: {
                  retn = std::to_string(operand->dis.val);
                  break;
            }
            case luramas::il::arch::operand::operand_kind::boolean: {
                  retn = (operand->dis.boolean) ? "true" : "false";
                  break;
            }
            case luramas::il::arch::operand::operand_kind::upvalue_kind: {

                  switch (operand->dis.upvalue_kind) {

                        case luramas::il::arch::operand::upvalue_kind::reg: {
                              retn = "register";
                              break;
                        }

                        case luramas::il::arch::operand::upvalue_kind::contents: {
                              retn = "contents";
                              break;
                        }

                        default: {
                              throw std::runtime_error("Unkown upvalue kind.");
                        }
                  }

                  break;
            }

            case luramas::il::arch::operand::operand_kind::closure: {
                  retn = "cls_" + std::to_string(operand->dis.closure_id);
                  break;
            }
            default: {
                  throw std::runtime_error("Unkown IL operand type.");
            }
      }

      return retn;
}