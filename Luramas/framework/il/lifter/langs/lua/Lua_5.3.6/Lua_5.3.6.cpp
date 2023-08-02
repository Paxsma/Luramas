#include "Lua_5.3.6.hpp"
#if luramas_target == luramas_target_lua_53_6

#include "../../../il/architecture/emitter.hpp"
#include "parser/parser.hpp"
#include "resolver/resolver.hpp"
#include <iostream>

/* Lifts current closure and child closures too IL disassembly. */
void luramas::il::lifter::lift(const Proto *p, const std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> &dism, std::shared_ptr<luramas::il::ilang> &buffer) {

      std::uint16_t stack_top = 0u;

      auto lift_closure = [](const Proto *p) -> std::shared_ptr<luramas::il::ilang> {
            auto retn = std::make_shared<luramas::il::ilang>();
            std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> vect_dism;

            for (auto pc = 0u; pc < unsigned(p->sizecode);) {
                  auto dism = std::make_shared<lua_53_6_disassembler::disassembly>();
                  lua_53_6_disassembler::disassemble(pc, p, dism);
                  vect_dism.emplace_back(dism);
                  pc += dism->len;
            }

            luramas::il::lifter::lift(p, vect_dism, retn);
            return retn;
      };

      /* Parse instructions. */
      parser::parse_instruction(buffer, dism, stack_top);

      /* Parses Kvalues */
      for (auto i = 0u; i < p->sizek; ++i) {

            const auto kval = p->k[i];

            auto ptr = std::make_shared<ilang::kvalue>();

            switch (kval.tt_) {

                  case LUA_TNIL:
                  case LUA_TDEADKEY: {
                        ptr->type = arch::kval_kinds::none;
                        break;
                  }
                  case LUA_TBOOLEAN: {
                        ptr->type = arch::kval_kinds::boolean;
                        ptr->boolean.boolean = kval.value_.b;
                        break;
                  }

                  case LUA_TLIGHTUSERDATA: {
                        ptr->type = arch::kval_kinds::userdata;
                        ptr->userdata.str = "lightuserdata_" + std::to_string(i);
                        break;
                  }
                  case LUA_TNUMBER: {

                        ptr->type = arch::kval_kinds::integer;
                        ptr->integer.integer = kval.value_.n;

                        /* String and remove trailing 0's */
                        ptr->integer.str = std::to_string(ptr->integer.integer);
                        ptr->integer.str.erase(ptr->integer.str.find_last_not_of('0') + 1, std::string::npos);
                        ptr->integer.str.erase(ptr->integer.str.find_last_not_of('.') + 1, std::string::npos);

                        break;
                  }

                  case LUA_TSTRING: {

                        ptr->type = arch::kval_kinds::string;
                        ptr->string.str = getstr(gco2ts(kval.value_.gc));

                        /* Clean string. */
                        static constexpr std::array str_arr = {'\"', '\''};

                        for (const auto i : str_arr) {

                              if (ptr->string.str.back() == i) {
                                    ptr->string.str.pop_back();
                              }

                              if (ptr->string.str.front() == i) {
                                    ptr->string.str.erase(0, 1);
                              }
                        }

                        break;
                  }

                  case LUA_TTABLE: {
                        const auto t = gco2t(kval.value_.gc);
                        ptr->type = arch::kval_kinds::table;
                        ptr->table.array_size = t->sizearray;
                        ptr->table.node_size = t->lsizenode;
                        break;
                  }
                  case LUA_TFUNCTION: {
                        ptr->type = arch::kval_kinds::upvalue;
                        ptr->upvalue.str = "closure_" + std::to_string(i);
                        break;
                  }
                  case LUA_TUSERDATA: {
                        ptr->type = arch::kval_kinds::userdata;
                        ptr->userdata.str = "userdata_" + std::to_string(i);
                        break;
                  }
                  case LUA_TTHREAD: {
                        ptr->type = arch::kval_kinds::none; /* Ignored */
                        break;
                  }

                  case LUA_TPROTO: {
                        ptr->closure.id = i;
                        ptr->closure.closure = lift_closure(gco2p(kval.value_.gc));
                        break;
                  }

                  default: {
                        throw std::runtime_error("Unkown kvalue type.");
                  }
            }

            buffer->kval.emplace_back(ptr);
      }

      /* Protos */
      for (auto i = 0u; i < p->sizep; ++i) {
            buffer->closures.emplace_back(lift_closure(p->p[i]));
      }

      /* Resolve instructions */
      resolver::resolve_instructions(buffer);

      const auto jump_map = parser::parse_jumps(buffer);

      std::vector<std::pair<std::size_t /* idx*/, std::shared_ptr<luramas::il::disassembly> /* block */>> block_vect;

      /* Emit instruction. */
      for (auto i = 0u; i < buffer->il.size(); ++i) {

            auto il = buffer->il[i];

            switch (il->op) {

                  case luramas::il::arch::opcodes::OP_SELF: {

                        const auto dest = il->operands.front()->dis.reg;

                        /* Dest == source emit dest too pop. */
                        if (dest == il->operands[1]->dis.reg) {

                              auto ptr = std::make_shared<luramas::il::disassembly>();
                              // luramas::il::emitter::emit_OP_POPARG(ptr, dest);
                              block_vect.emplace_back(std::make_pair(i + block_vect.size(), ptr));
                        }

                        break;
                  }

                  default: {
                        break;
                  }
            }
      }

      /* Append */
      for (const auto &i : block_vect)
            buffer->il.insert(buffer->il.begin() + i.first, i.second);

      /* Resolve blocks */
      resolver::resolve_blocks(buffer);

      /* Resolve jumps */
      resolver::resolve_jumps(buffer, jump_map);

      return;
}

/* Lifts proto and child closures too IL disassembly. */
std::shared_ptr<luramas::il::ilang> luramas::il::lifter::lift_proto(const Proto *p) {

      auto buffer = std::make_shared<luramas::il::ilang>();
      std::vector<std::shared_ptr<lua_53_6_disassembler::disassembly>> dism;
      lua_53_6_disassembler::disassemble(p, dism);
      luramas::il::lifter::lift(p, vect_dism, buffer);
      return buffer;
};

#endif