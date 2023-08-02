#include "disassembler.hpp"
#include <iostream>

enum set_action : std::uint8_t {
      instruction, /* Sets all instruction info, op, mnenomic, hint. */
      operands     /* Sets all operands including details about it. */
};

template <set_action n>
void set_data(std::shared_ptr<luau_v3_disassembler::disassembly> &buffer, const Proto *p, const op_table::optable op_table) {

      switch (n) {

            case set_action::instruction: {

                  buffer->op = op_table.op;

                  switch (op_table.op) {

                        case LuauOpcode::LOP_ADD: {
                              buffer->mnenomic = "add";
                              buffer->hint = "Addition.";
                              break;
                        }

                        case LuauOpcode::LOP_ADDK: {
                              buffer->mnenomic = "addk";
                              buffer->hint = "Addition with constant.";
                              break;
                        }

                        case LuauOpcode::LOP_AND: {
                              buffer->mnenomic = "and";
                              buffer->hint = "And.";
                              break;
                        }

                        case LuauOpcode::LOP_ANDK: {
                              buffer->mnenomic = "andk";
                              buffer->hint = "And with constant.";
                              break;
                        }

                        case LuauOpcode::LOP_BREAK: {
                              buffer->mnenomic = "break";
                              buffer->hint = "Break.";
                              break;
                        }

                        case LuauOpcode::LOP_CALL: {
                              buffer->mnenomic = "call";
                              buffer->hint = "Call routine.";
                              break;
                        }

                        case LuauOpcode::LOP_CAPTURE: {
                              buffer->mnenomic = "capture";
                              buffer->hint = "Capture operation.";
                              break;
                        }

                        case LuauOpcode::LOP_CLOSEUPVALS: {
                              buffer->mnenomic = "closeupvals";
                              buffer->hint = "Close upvalues.";
                              break;
                        }

                        case LuauOpcode::LOP_CONCAT: {
                              buffer->mnenomic = "concat";
                              buffer->hint = "Concat strings.";
                              break;
                        }

                        case LuauOpcode::LOP_DEP_FORGLOOP_INEXT: {
                              buffer->mnenomic = "forgloop_inext";
                              buffer->hint = "For loop with (i)next (Depricated).";
                              break;
                        }

                        case LuauOpcode::LOP_DEP_FORGLOOP_NEXT: {
                              buffer->mnenomic = "forgloop_next";
                              buffer->hint = "For loop with next (Depricated).";
                              break;
                        }

                        case LuauOpcode::LOP_FORGPREP_INEXT: {
                              buffer->mnenomic = "forgprep_inext";
                              buffer->hint = "For loop with (i)next.";
                              break;
                        }

                        case LuauOpcode::LOP_FORGPREP_NEXT: {
                              buffer->mnenomic = "forgprep_next";
                              buffer->hint = "For loop prep with next.";
                              break;
                        }

                        case LuauOpcode::LOP_DEP_JUMPIFEQK: {
                              buffer->mnenomic = "jumpifeqk";
                              buffer->hint = "Jump if equal constant (Depricated).";
                              break;
                        }

                        case LuauOpcode::LOP_DEP_JUMPIFNOTEQK: {
                              buffer->mnenomic = "jumpifbnoteqk";
                              buffer->hint = "Jump if not equal constant (Depricated).";
                              break;
                        }

                        case LuauOpcode::LOP_DIV: {
                              buffer->mnenomic = "div";
                              buffer->hint = "Divide.";
                              break;
                        }

                        case LuauOpcode::LOP_DIVK: {
                              buffer->mnenomic = "divk";
                              buffer->hint = "Divide with constant.";
                              break;
                        }

                        case LuauOpcode::LOP_DUPCLOSURE: {
                              buffer->mnenomic = "dupclosure";
                              buffer->hint = "Dupe closure.";
                              break;
                        }

                        case LuauOpcode::LOP_DUPTABLE: {
                              buffer->mnenomic = "duptable";
                              buffer->hint = "Dupe table.";
                              break;
                        }

                        case LuauOpcode::LOP_FASTCALL: {
                              buffer->mnenomic = "fastcall";
                              buffer->hint = "Fastcall.";
                              break;
                        }

                        case LuauOpcode::LOP_FASTCALL1: {
                              buffer->mnenomic = "fastcall1";
                              buffer->hint = "Fastcall.";
                              break;
                        }

                        case LuauOpcode::LOP_FASTCALL2: {
                              buffer->mnenomic = "fastcall2";
                              buffer->hint = "Fastcall.";
                              break;
                        }

                        case LuauOpcode::LOP_FASTCALL2K: {
                              buffer->mnenomic = "fastcall2k";
                              buffer->hint = "Fastcall with constant.";
                              break;
                        }

                        case LuauOpcode::LOP_FORGLOOP: {
                              buffer->mnenomic = "forgloop";
                              buffer->hint = "For generic loop.";
                              break;
                        }

                        case LuauOpcode::LOP_FORGPREP: {
                              buffer->mnenomic = "forgprep";
                              buffer->hint = "For generic prepare.";
                              break;
                        }

                        case LuauOpcode::LOP_FORNLOOP: {
                              buffer->mnenomic = "fornloop";
                              buffer->hint = "For numeric loop.";
                              break;
                        }

                        case LuauOpcode::LOP_FORNPREP: {
                              buffer->mnenomic = "fornprep";
                              buffer->hint = "For numeric loop.";
                              break;
                        }

                        case LuauOpcode::LOP_GETGLOBAL: {
                              buffer->mnenomic = "getglobal";
                              buffer->hint = "Get global.";
                              break;
                        }

                        case LuauOpcode::LOP_GETIMPORT: {
                              buffer->mnenomic = "getimport";
                              buffer->hint = "Get import.";
                              break;
                        }

                        case LuauOpcode::LOP_GETTABLE: {
                              buffer->mnenomic = "gettable";
                              buffer->hint = "Get table.";
                              break;
                        }

                        case LuauOpcode::LOP_GETTABLEKS: {
                              buffer->mnenomic = "gettableks";
                              buffer->hint = "Get table constant.";
                              break;
                        }

                        case LuauOpcode::LOP_GETTABLEN: {
                              buffer->mnenomic = "gettablen";
                              buffer->hint = "Get tabble numeric.";
                              break;
                        }

                        case LuauOpcode::LOP_GETUPVAL: {
                              buffer->mnenomic = "getupval";
                              buffer->hint = "Get upvalue.";
                              break;
                        }

                        case LuauOpcode::LOP_GETVARARGS: {
                              buffer->mnenomic = "getvarargs";
                              buffer->hint = "Get variable args.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMP: {
                              buffer->mnenomic = "jump";
                              buffer->hint = "Jump.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPBACK: {
                              buffer->mnenomic = "jumpback";
                              buffer->hint = "Jumpback.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIF: {
                              buffer->mnenomic = "jumpif";
                              buffer->hint = "Jump if.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFEQ: {
                              buffer->mnenomic = "jumpifeq";
                              buffer->hint = "Jump if equal.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFLE: {
                              buffer->mnenomic = "jumpifle";
                              buffer->hint = "Jump if less then or equal.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFLT: {
                              buffer->mnenomic = "jumpiflt";
                              buffer->hint = "Jump if less then.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFNOT: {
                              buffer->mnenomic = "jumpifnot";
                              buffer->hint = "Jump if not.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFNOTEQ: {
                              buffer->mnenomic = "jumpifnoteq";
                              buffer->hint = "Jump if not equal.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFNOTLE: {
                              buffer->mnenomic = "jumpifnotle";
                              buffer->hint = "Jump if not less then or equal.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPIFNOTLT: {
                              buffer->mnenomic = "jumpifnotlt";
                              buffer->hint = "Jump if not less then.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPX: {
                              buffer->mnenomic = "jumpx";
                              buffer->hint = "Jump long.";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPXEQKB: {
                              buffer->mnenomic = "jumpxeqkb";
                              buffer->hint = "Jump long if equal (constant, bool).";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPXEQKN: {
                              buffer->mnenomic = "jumpxeqkn";
                              buffer->hint = "Jump long if equal (constant, numeric).";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPXEQKNIL: {
                              buffer->mnenomic = "jumpxeqknil";
                              buffer->hint = "Jump long if equal (constant, nil).";
                              break;
                        }

                        case LuauOpcode::LOP_JUMPXEQKS: {
                              buffer->mnenomic = "jumpxeqks";
                              buffer->hint = "Jump long if equal (constant, string).";
                              break;
                        }

                        case LuauOpcode::LOP_LENGTH: {
                              buffer->mnenomic = "lenght";
                              buffer->hint = "Lenght.";
                              break;
                        }

                        case LuauOpcode::LOP_LOADB: {
                              buffer->mnenomic = "loadb";
                              buffer->hint = "Load boolean.";
                              break;
                        }

                        case LuauOpcode::LOP_LOADK: {
                              buffer->mnenomic = "loadk";
                              buffer->hint = "Load constant.";
                              break;
                        }

                        case LuauOpcode::LOP_LOADKX: {
                              buffer->mnenomic = "loadkx";
                              buffer->hint = "Load extra constant.";
                              break;
                        }

                        case LuauOpcode::LOP_LOADN: {
                              buffer->mnenomic = "loadn";
                              buffer->hint = "Load numeric.";
                              break;
                        }

                        case LuauOpcode::LOP_LOADNIL: {
                              buffer->mnenomic = "loadnil";
                              buffer->hint = "Load nil.";
                              break;
                        }

                        case LuauOpcode::LOP_MINUS: {
                              buffer->mnenomic = "minus";
                              buffer->hint = "Minus.";
                              break;
                        }

                        case LuauOpcode::LOP_MOD: {
                              buffer->mnenomic = "mod";
                              buffer->hint = "Mod.";
                              break;
                        }

                        case LuauOpcode::LOP_MODK: {
                              buffer->mnenomic = "modk";
                              buffer->hint = "Mod constant.";
                              break;
                        }

                        case LuauOpcode::LOP_MOVE: {
                              buffer->mnenomic = "move";
                              buffer->hint = "Move.";
                              break;
                        }

                        case LuauOpcode::LOP_MUL: {
                              buffer->mnenomic = "mul";
                              buffer->hint = "Mul.";
                              break;
                        }

                        case LuauOpcode::LOP_MULK: {
                              buffer->mnenomic = "mulk";
                              buffer->hint = "Mul constant.";
                              break;
                        }

                        case LuauOpcode::LOP_NAMECALL: {
                              buffer->mnenomic = "namecall";
                              buffer->hint = "Namecall.";
                              break;
                        }

                        case LuauOpcode::LOP_NEWCLOSURE: {
                              buffer->mnenomic = "newclosure";
                              buffer->hint = "New closure.";
                              break;
                        }

                        case LuauOpcode::LOP_NEWTABLE: {
                              buffer->mnenomic = "newtable";
                              buffer->hint = "New table.";
                              break;
                        }

                        case LuauOpcode::LOP_NOP: {
                              buffer->mnenomic = "nop";
                              buffer->hint = "Nop.";
                              break;
                        }

                        case LuauOpcode::LOP_NOT: {
                              buffer->mnenomic = "not";
                              buffer->hint = "Not.";
                              break;
                        }

                        case LuauOpcode::LOP_OR: {
                              buffer->mnenomic = "or";
                              buffer->hint = "Or.";
                              break;
                        }

                        case LuauOpcode::LOP_ORK: {
                              buffer->mnenomic = "ork";
                              buffer->hint = "Or constant.";
                              break;
                        }

                        case LuauOpcode::LOP_POW: {
                              buffer->mnenomic = "pow";
                              buffer->hint = "Pow.";
                              break;
                        }

                        case LuauOpcode::LOP_POWK: {
                              buffer->mnenomic = "powk";
                              buffer->hint = "Pow constant.";
                              break;
                        }

                        case LuauOpcode::LOP_PREPVARARGS: {
                              buffer->mnenomic = "prepvarargs";
                              buffer->hint = "Prepare variable args.";
                              break;
                        }

                        case LuauOpcode::LOP_RETURN: {
                              buffer->mnenomic = "return";
                              buffer->hint = "Return.";
                              break;
                        }

                        case LuauOpcode::LOP_SETGLOBAL: {
                              buffer->mnenomic = "setglobal";
                              buffer->hint = "Set global.";
                              break;
                        }

                        case LuauOpcode::LOP_SETLIST: {
                              buffer->mnenomic = "setlist";
                              buffer->hint = "Set list table.";
                              break;
                        }

                        case LuauOpcode::LOP_SETTABLE: {
                              buffer->mnenomic = "settable";
                              buffer->hint = "Set table.";
                              break;
                        }

                        case LuauOpcode::LOP_SETTABLEKS: {
                              buffer->mnenomic = "settableks";
                              buffer->hint = "Set table constant.";
                              break;
                        }

                        case LuauOpcode::LOP_SETTABLEN: {
                              buffer->mnenomic = "settablen";
                              buffer->hint = "Settable numeric.";
                              break;
                        }

                        case LuauOpcode::LOP_SETUPVAL: {
                              buffer->mnenomic = "setupval";
                              buffer->hint = "Set upvalue.";
                              break;
                        }

                        case LuauOpcode::LOP_SUB: {
                              buffer->mnenomic = "sub";
                              buffer->hint = "Sub.";
                              break;
                        }

                        case LuauOpcode::LOP_SUBK: {
                              buffer->mnenomic = "subk";
                              buffer->hint = "Sub constant.";
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

                        auto current_operand = std::make_shared<luau_v3_disassembler::operand>();

                        std::intptr_t operand_value = 0;

                        const auto operand = op_table.operands[i];
                        const auto type = op_table.types[i];

                        const auto split = ((i + 1u) == op_count) ? " " : ", ";

                        /* Set operand value. */
                        switch (operand) {

                              case op_table::operands::A: {
                                    operand_value = decode_A(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::B: {
                                    operand_value = decode_B(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::C: {
                                    operand_value = decode_C(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::C_dec: {
                                    operand_value = decode_C(buffer->code[0]) - 1;
                                    break;
                              }

                              case op_table::operands::C_inc: {
                                    operand_value = decode_C(buffer->code[0]) + 1;
                                    break;
                              }

                              case op_table::operands::D: {
                                    operand_value = decode_D(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::E: {
                                    operand_value = decode_E(buffer->code[0]);
                                    break;
                              }

                              case op_table::operands::AUX: {
                                    ++buffer->code;
                                    operand_value = buffer->code[0];
                                    break;
                              }

                              case op_table::operands::AUX_24: {
                                    ++buffer->code;
                                    operand_value = buffer->code[0] & 0xffffff;
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

                              /* Capture_ref follows. */
                              case op_table::type::capture_idx: {

                                    const auto on = buffer->code[0];
                                    const auto tt = decode_A(on);

                                    current_operand->capture_ref = tt;

                                    /* Upvalue. */
                                    if (tt == 2u) {
                                          buffer->data += std::to_string(tt) + split;
                                    } else {
                                          buffer->data += std::to_string(tt) + split;
                                    }

                                    buffer->total += tt;

                                    break;
                              }

                              case op_table::type::capture_ref: {

                                    const auto on = buffer->code[0];
                                    const auto tt = decode_A(on);
                                    const auto v = decode_B(on);

                                    current_operand->capture_reg = v;

                                    /* Upvalue. */
                                    if (tt == 2u) {
                                          buffer->data += "upvalue_" + std::to_string(v) + split;
                                    } else {
                                          buffer->data += "r" + std::to_string(v) + split;
                                    }

                                    buffer->total += current_operand->capture_reg;

                                    break;
                              }

                              case op_table::type::fastcall_idx: {
                                    buffer->data += std::string(op_table::fastcall_array[operand_value]) + split;
                                    current_operand->fastcall_idx = std::uint8_t(operand_value);
                                    buffer->total += current_operand->fastcall_idx;
                                    break;
                              }

                              case op_table::type::table_size: {
                                    const auto size = (!operand_value ? 0 : (1 << (operand_value - 1)));
                                    buffer->data += std::to_string(size) + split;
                                    current_operand->table_size = size;
                                    buffer->total += current_operand->table_size;
                                    break;
                              }

                              case op_table::type::closure: {
                                    buffer->data += "closure_index_" + std::to_string(operand_value) + split;
                                    current_operand->closure = operand_value;
                                    buffer->total += current_operand->closure;
                                    break;
                              }

                              case op_table::type::upvalue: {
                                    buffer->data += "upvalue_" + std::to_string(operand_value) + split;
                                    current_operand->upvalue = operand_value;
                                    buffer->total += current_operand->upvalue;
                                    break;
                              }

                              case op_table::type::slot: {
                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->slot = operand_value;
                                    break;
                              }

                              case op_table::type::aux: {
                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->aux = operand_value;
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

                              case op_table::type::integer: {
                                    current_operand->integer = operand_value;
                                    buffer->total += current_operand->integer;
                              }

                              case op_table::type::k_idx_aux: {
                                    buffer->data += std::to_string(operand_value) + split;
                                    current_operand->val = operand_value;
                                    current_operand->k_value = std::to_string(current_operand->val);
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

                              case op_table::type::import_idx: {

                                    const auto source = buffer->code[1];
                                    const std::int32_t brco = source >> 30;

                                    const auto id1 = (brco > 0) ? std::int32_t(source >> 20) & 1023 : -1;
                                    const auto id2 = (brco > 1) ? std::int32_t(source >> 10) & 1023 : -1;
                                    const auto id3 = (brco > 2) ? std::int32_t(source) & 1023 : -1;

                                    if (id1 >= 0) {
                                          current_operand->k_value += gco2ts(p->k[id1].value.gc)->data;
                                    }
                                    if (id2 >= 0) {
                                          current_operand->k_value += std::string(".") + std::string(gco2ts(p->k[id2].value.gc)->data);
                                    }
                                    if (id3 >= 0) {
                                          current_operand->k_value += std::string(".") + std::string(gco2ts(p->k[id3].value.gc)->data);
                                    }

                                    buffer->data += current_operand->k_value + split;
                                    current_operand->import_idx = id3;
                                    buffer->total += current_operand->import_idx;

                                    break;
                              }

                              case op_table::type::k_value_nstr: {

                                    const auto kv = p->k[operand_value];
                                    current_operand->k_value = std::string(kv.value.gc->ts.data);

                                    buffer->data += current_operand->k_value + split;

                                    current_operand->k_idx = operand_value;
                                    buffer->total += current_operand->k_idx;

                                    break;
                              }

                              case op_table::type::k_idx_pp:
                              case op_table::type::k_idx: {

                                    buffer->total += operand_value;

                                    const auto kv = p->k[operand_value];
                                    current_operand->k_value_type = static_cast<lua_Type>(kv.tt);

                                    switch (kv.tt) {

                                          case lua_Type::LUA_TNIL: {
                                                current_operand->k_value = "nil";
                                                break;
                                          }

                                          case lua_Type::LUA_TBOOLEAN: {
                                                current_operand->k_value = std::to_string(kv.value.b);
                                                break;
                                          }

                                          case lua_Type::LUA_TNUMBER: {
                                                if (std::floor(kv.value.n) == kv.value.n) {
                                                      current_operand->k_value = std::to_string(std::intptr_t(kv.value.n));
                                                } else {
                                                      current_operand->k_value = std::to_string(kv.value.n);
                                                }
                                                break;
                                          }

                                          case lua_Type::LUA_TSTRING: {
                                                current_operand->k_value = std::string("\"") + kv.value.gc->ts.data + '\"';
                                                break;
                                          }

                                          case lua_Type::LUA_TLIGHTUSERDATA: {
                                                current_operand->k_value = "lightuserdata_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case lua_Type::LUA_TFUNCTION: {

                                                if (type == op_table::type::k_idx_pp) {

                                                      for (auto i = 0u; i < p->sizep; i++)
                                                            if (p->p[i] == gco2cl(kv.value.gc)->l.p) {
                                                                  operand_value = i;
                                                                  break;
                                                            }
                                                }

                                                current_operand->k_value = "closure_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case lua_Type::LUA_TTABLE: {
                                                current_operand->k_value = "table_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case lua_Type::LUA_TUSERDATA: {
                                                current_operand->k_value = "userdata_" + std::to_string(operand_value);
                                                break;
                                          }

                                          case lua_Type::LUA_TUPVAL: {
                                                current_operand->k_value = "upval_" + std::to_string(operand_value);
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

void luau_v3_disassembler::disassemble(const std::uintptr_t addr, const Proto *p, std::shared_ptr<luau_v3_disassembler::disassembly> &buffer) {

      /* Get intruction and set instruction. */
      const auto code = op_table::op_table[decode_opcode(p->code[addr])];
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

void luau_v3_disassembler::disassemble(const Proto *p, std::vector<std::shared_ptr<luau_v3_disassembler::disassembly>> &buffer) {

      for (auto pc = 0u; pc < unsigned(p->sizecode);) {
            auto dism = std::make_shared<luau_v3_disassembler::disassembly>();
            disassemble(pc, p, dism);
            buffer.emplace_back(dism);
            pc += dism->len;
      }

      return;
}