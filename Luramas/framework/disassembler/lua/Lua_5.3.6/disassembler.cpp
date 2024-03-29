#include "disassembler.hpp"
#include <iostream>

enum class set_action : std::uint8_t {
      instruction, /* Sets all instruction info, op, mnenomic, hint. */
      operands     /* Sets all operands including details about it. */
};

template <set_action n>
void set_data(std::shared_ptr<lua_53_6_disassembler::disassembly> &buffer, const Proto *p, const op_table::optable op_table) {

      switch (n) {

            case set_action::instruction: {

                  buffer->op = op_table.op;

                  switch (op_table.op) {

                        case OpCode::OP_MOVE: {
                              buffer->mnenomic = "move";
                              buffer->hint = "Move a value from one register to another.";
                              break;
                        }
                        case OpCode::OP_LOADK: {
                              buffer->mnenomic = "loadk";
                              buffer->hint = "Load a constant into a register.";
                              break;
                        }
                        case OpCode::OP_LOADKX: {
                              buffer->mnenomic = "loadkx";
                              buffer->hint = "Load a constant into a register (extended opcode).";
                              break;
                        }
                        case OpCode::OP_LOADBOOL: {
                              buffer->mnenomic = "loadbool";
                              buffer->hint = "Load a boolean value into a register.";
                              break;
                        }
                        case OpCode::OP_LOADNIL: {
                              buffer->mnenomic = "loadnil";
                              buffer->hint = "Load nil into a register.";
                              break;
                        }
                        case OpCode::OP_GETUPVAL: {
                              buffer->mnenomic = "getupval";
                              buffer->hint = "Get an upvalue and push it onto the stack.";
                              break;
                        }
                        case OpCode::OP_GETTABUP: {
                              buffer->mnenomic = "gettabup";
                              buffer->hint = "Get a table value from an upvalue and push it onto the stack.";
                              break;
                        }
                        case OpCode::OP_GETTABLE: {
                              buffer->mnenomic = "gettable";
                              buffer->hint = "Get a value from a table and push it onto the stack.";
                              break;
                        }
                        case OpCode::OP_SETTABUP: {
                              buffer->mnenomic = "settabup";
                              buffer->hint = "Set a value in a table from an upvalue.";
                              break;
                        }
                        case OpCode::OP_SETUPVAL: {
                              buffer->mnenomic = "setupval";
                              buffer->hint = "Set an upvalue from a value on the stack.";
                              break;
                        }
                        case OpCode::OP_SETTABLE: {
                              buffer->mnenomic = "settable";
                              buffer->hint = "Set a value in a table from values on the stack.";
                              break;
                        }
                        case OpCode::OP_NEWTABLE: {
                              buffer->mnenomic = "newtable";
                              buffer->hint = "Create a new table and push it onto the stack.";
                              break;
                        }
                        case OpCode::OP_SELF: {
                              buffer->mnenomic = "self";
                              buffer->hint = "Get a value from a table and push it onto the stack (function).";
                              break;
                        }
                        case OpCode::OP_ADD: {
                              buffer->mnenomic = "add";
                              buffer->hint = "Arithmetic Addition.";
                              break;
                        }
                        case OpCode::OP_SUB: {
                              buffer->mnenomic = "sub";
                              buffer->hint = "Arithmetic subtraction.";
                              break;
                        }
                        case OpCode::OP_MUL: {
                              buffer->mnenomic = "mul";
                              buffer->hint = "Arithmetic multiplication.";
                              break;
                        }
                        case OpCode::OP_MOD: {
                              buffer->mnenomic = "mod";
                              buffer->hint = "Arithmetic modulus.";
                              break;
                        }
                        case OpCode::OP_POW: {
                              buffer->mnenomic = "pow";
                              buffer->hint = "Arithmetic power.";
                              break;
                        }
                        case OpCode::OP_DIV: {
                              buffer->mnenomic = "div";
                              buffer->hint = "Arithmetic division.";
                              break;
                        }
                        case OpCode::OP_IDIV: {
                              buffer->mnenomic = "idiv";
                              buffer->hint = "Integer division.";
                              break;
                        }
                        case OpCode::OP_BAND: {
                              buffer->mnenomic = "band";
                              buffer->hint = "Bitwise AND.";
                              break;
                        }
                        case OpCode::OP_BOR: {
                              buffer->mnenomic = "bor";
                              buffer->hint = "Bitwise OR.";
                              break;
                        }
                        case OpCode::OP_BXOR: {
                              buffer->mnenomic = "bxor";
                              buffer->hint = "Bitwise XOR.";
                              break;
                        }
                        case OpCode::OP_SHL: {
                              buffer->mnenomic = "shl";
                              buffer->hint = "Left shift.";
                              break;
                        }
                        case OpCode::OP_SHR: {
                              buffer->mnenomic = "shr";
                              buffer->hint = "Right shift.";
                              break;
                        }
                        case OpCode::OP_UNM: {
                              buffer->mnenomic = "unm";
                              buffer->hint = "Unary minus.";
                              break;
                        }
                        case OpCode::OP_BNOT: {
                              buffer->mnenomic = "bnot";
                              buffer->hint = "Bitwise NOT.";
                              break;
                        }
                        case OpCode::OP_NOT: {
                              buffer->mnenomic = "not";
                              buffer->hint = "Logical NOT.";
                              break;
                        }
                        case OpCode::OP_LEN: {
                              buffer->mnenomic = "len";
                              buffer->hint = "Get length of object.";
                              break;
                        }
                        case OpCode::OP_CONCAT: {
                              buffer->mnenomic = "concat";
                              buffer->hint = "Concatenate strings.";
                              break;
                        }
                        case OpCode::OP_JMP: {
                              buffer->mnenomic = "jmp";
                              buffer->hint = "Unconditional jump.";
                              break;
                        }
                        case OpCode::OP_EQ: {
                              buffer->mnenomic = "eq";
                              buffer->hint = "Compare for equality.";
                              break;
                        }
                        case OpCode::OP_LT: {
                              buffer->mnenomic = "lt";
                              buffer->hint = "Compare for less than.";
                              break;
                        }
                        case OpCode::OP_LE: {
                              buffer->mnenomic = "le";
                              buffer->hint = "Compare for less than or equal to.";
                              break;
                        }
                        case OpCode::OP_TEST: {
                              buffer->mnenomic = "test";
                              buffer->hint = "Test a value and jump based on its boolean value.";
                              break;
                        }
                        case OpCode::OP_TESTSET: {
                              buffer->mnenomic = "testset";
                              buffer->hint = "Test a value and jump based on its boolean value, and assign a value.";
                              break;
                        }
                        case OpCode::OP_CALL: {
                              buffer->mnenomic = "call";
                              buffer->hint = "Call a function.";
                              break;
                        }
                        case OpCode::OP_TAILCALL: {
                              buffer->mnenomic = "tailcall";
                              buffer->hint = "Tail call (optimization).";
                              break;
                        }
                        case OpCode::OP_RETURN: {
                              buffer->mnenomic = "return";
                              buffer->hint = "Return from a function call.";
                              break;
                        }
                        case OpCode::OP_FORLOOP: {
                              buffer->mnenomic = "forloop";
                              buffer->hint = "Generic for loop control.";
                              break;
                        }
                        case OpCode::OP_FORPREP: {
                              buffer->mnenomic = "forprep";
                              buffer->hint = "Generic for loop control.";
                              break;
                        }
                        case OpCode::OP_TFORCALL: {
                              buffer->mnenomic = "tforcall";
                              buffer->hint = "Call a function in a for loop.";
                              break;
                        }
                        case OpCode::OP_TFORLOOP: {
                              buffer->mnenomic = "tforloop";
                              buffer->hint = "Iterate over table elements in a for loop.";
                              break;
                        }
                        case OpCode::OP_SETLIST: {
                              buffer->mnenomic = "setlist";
                              buffer->hint = "Set a range of array elements.";
                              break;
                        }
                        case OpCode::OP_CLOSURE: {
                              buffer->mnenomic = "closure";
                              buffer->hint = "Creates a closure.";
                              break;
                        }
                        case OpCode::OP_VARARG: {
                              buffer->mnenomic = "vararg";
                              buffer->hint = "Get variable arguments from function call.";
                              break;
                        }
                        case OpCode::OP_EXTRAARG: {
                              buffer->mnenomic = "extraarg";
                              buffer->hint = "Extra(larger) argument for previous opcode.";
                              break;
                        }
                        default: {
                              throw std::runtime_error("Unkown opcode in disassembler.");
                        }
                  }

                  break;
            }

            case set_action::operands: {

                  const auto op_count = op_table.operands.size();

                  for (auto i = 0u; i < op_count; ++i) {

                        auto current_operand = std::make_shared<lua_53_6_disassembler::operand>();

                        std::intptr_t operand_value = 0;

                        const auto operand = op_table.operands[i];
                        const auto type = op_table.types[i];

                        const auto split = ((i + 1u) == op_count) ? " " : ", ";

                        /* Set operand value. */
                        switch (operand) {

                              case op_table::operands::A: {
                                    operand_value = GETARG_A(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::Ax: {
                                    ++buffer->code;
                                    operand_value = GETARG_Ax(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::B: {
                                    operand_value = GETARG_B(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::Bx: {
                                    operand_value = GETARG_Bx(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::C: {
                                    operand_value = GETARG_C(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::sBx: {
                                    ++buffer->code;
                                    operand_value = GETARG_sBx(buffer->code[0]);
                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unkown operand type.");
                              }
                        }

                        /* Set type and operand. */
                        current_operand->type = type;
                        current_operand->operand = operand;

                        /* Set type. */
                        switch (current_operand->type) {

                              case op_table::type::table_size: {
                                    const auto size = (!operand_value ? 0 : (1 << (operand_value - 1)));
                                    buffer->data += std::to_string(size) + split;
                                    current_operand->table_size = size;
                                    buffer->total += current_operand->table_size;
                                    break;
                              }

                              case op_table::type::upvalue: {
                                    buffer->data += "upvalue_" + std::to_string(operand_value) + split;
                                    current_operand->upvalue = operand_value;
                                    buffer->total += current_operand->upvalue;
                                    break;
                              }

                              case op_table::type::jmp: {

                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->jmp = operand_value;
                                    current_operand->ref_addr = current_operand->jmp + buffer->addr + 1u;
                                    buffer->total += current_operand->jmp;

                                    break;
                              }

                              case op_table::type::reg:
                              case op_table::type::dest: {
                                    buffer->data += 'r' + std::to_string(operand_value) + split;
                                    current_operand->reg = std::uint16_t(operand_value);
                                    buffer->total += current_operand->reg;
                                    break;
                              }

                              case op_table::type::val: {
                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->val = operand_value;
                                    buffer->total += current_operand->val;
                                    break;
                              }

                              case op_table::type::val_multret: {
                                    --operand_value;
                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->val = operand_value;
                                    buffer->total += current_operand->val;
                                    break;
                              }

                              case op_table::type::k_idx_pp:
                              case op_table::type::k_idx: {

                                    buffer->total += operand_value;

                                    const auto kv = p->k[operand_value - 1u];
                                    current_operand->k_value_type = kv.tt_;

                                    switch (current_operand->k_value_type) {

                                          case LUA_TNIL: {
                                                current_operand->k_value = "nil";
                                                break;
                                          }

                                          case LUA_TBOOLEAN: {
                                                current_operand->k_value = std::to_string(kv.value_.b);
                                                break;
                                          }

                                          case LUA_TNUMBER: {
                                                if (std::floor(kv.value_.n) == kv.value_.n) {
                                                      current_operand->k_value = std::to_string(std::intptr_t(kv.value_.n));
                                                } else {
                                                      current_operand->k_value = std::to_string(kv.value_.n);
                                                }
                                                break;
                                          }

                                          case LUA_TSTRING: {
                                                current_operand->k_value = std::string("\"") + getstr(gco2ts(kv.value_.gc)) + '\"';
                                                break;
                                          }

                                          case LUA_TLIGHTUSERDATA: {
                                                current_operand->k_value = "lightuserdata_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case LUA_TFUNCTION: {

                                                if (type == op_table::type::k_idx_pp) {

                                                      for (auto i = 0; i < p->sizep; i++)
                                                            if (p->p[i] == gco2cl(kv.value_.gc)->l.p) {
                                                                  operand_value = i;
                                                                  break;
                                                            }
                                                }

                                                current_operand->k_value = "closure_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case LUA_TTABLE: {
                                                current_operand->k_value = "table_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case LUA_TUSERDATA: {
                                                current_operand->k_value = "userdata_" + std::to_string(operand_value);
                                                break;
                                          }

                                          default: {
                                                throw std::runtime_error("Unkown lua type.");
                                          }
                                    }

                                    buffer->data += current_operand->k_value + split;
                                    current_operand->k_idx = operand_value;

                                    break;
                              }
                        }

                        buffer->operands.emplace_back(current_operand);
                  }

                  break;
            }
      }

      return;
}

void lua_53_6_disassembler::disassemble(const std::uintptr_t addr, const Proto *p, std::shared_ptr<lua_53_6_disassembler::disassembly> &buffer) {

      /* Get intruction and set instruction. */
      const auto code = op_table::op_table[GET_OPCODE(p->code[addr])];
      set_data<set_action::instruction>(buffer, p, code);

      /* Set code. */
      const auto start_pc = p->code + addr;
      buffer->code = start_pc;
      buffer->addr = addr;
      buffer->total = std::int16_t(buffer->op);

      /* Clear for next. */
      buffer->operands.clear();

      /* Init data. */
      buffer->data = std::string(buffer->mnenomic) + ' ';
      set_data<set_action::operands>(buffer, p, code);

      /* Calculate lenght. */
      buffer->len = (std::uint8_t(reinterpret_cast<const std::uintptr_t>(buffer->code) - reinterpret_cast<const std::uintptr_t>(start_pc)) / sizeof(Instruction)) + 1u;

      /* Condense constant */
      buffer->total /= 25u;

      /* Bytes */
      for (auto i = 0u; i < buffer->len; ++i)
            buffer->bytes.emplace_back(p->code[buffer->addr + i]);

      return;
}

void lua_53_6_disassembler::disassemble(const Proto *p, std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> &buffer) {

      for (auto pc = 0u; pc < unsigned(p->sizecode);) {
            auto dism = std::make_shared<lua_53_6_disassembler::disassembly>();
            disassemble(pc, p, dism);
            buffer.emplace_back(dism);
            pc += dism->len;
      }

      return;
}