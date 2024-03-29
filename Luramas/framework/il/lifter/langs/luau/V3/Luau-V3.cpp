#include "Luau-V3.hpp"
#if luramas_target == luramas_target_luau_v3

#include "../../../../../disassembler/luau/V3/disassembler.hpp"
#include "../../../../il/architecture/emitter.hpp"
#include "parser/parser.hpp"
#include "resolver/resolver.hpp"
#include <iostream>

void luramas::il::lifter::lift(Proto *p, const std::vector<std::shared_ptr<luau_v3_disassembler::disassembly>> &dism, std::shared_ptr<luramas::il::ilang> &buffer) {

      lifter::parser::parser_manager<luau_v3_disassembler::disassembly> pm(buffer, dism, luau_v3_parsers::parse_instructions, false);
      lifter::resolver::resolver_manager<Proto> rm(buffer, p);

      auto lift_closure = [](Proto *p) -> std::shared_ptr<luramas::il::ilang> {
            auto retn = std::make_shared<luramas::il::ilang>();
            std::vector<std::shared_ptr<luau_v3_disassembler::disassembly>> vect_dism;

            for (auto pc = 0u; pc < unsigned(p->sizecode);) {
                  auto dism = std::make_shared<luau_v3_disassembler::disassembly>();
                  luau_v3_disassembler::disassemble(pc, p, dism);
                  vect_dism.emplace_back(dism);
                  pc += dism->len;
            }

            luramas::il::lifter::lift(p, vect_dism, retn);
            return retn;
      };

      pm.add(luau_v3_parsers::parse_compares);
      pm.add(luau_v3_parsers::parse_jumps);
      pm.run();

      for (const auto &i : pm.il->dis)
            std::cout << "BT " << i->disassemble() << std::endl;

      rm.add(luau_v3_resolvers::resolve_instruction_operands);
      rm.add(luau_v3_resolvers::resolve_jump_operands);
      rm.add(luau_v3_resolvers::resolve_instructions);
      rm.run();

      /* Parses Kvalues */
      for (auto i = 0; i < p->sizek; ++i) {

            const auto kval = p->k[i];

            auto ptr = std::make_shared<luramas::il::ilang::kvalue>();

            switch (kval.tt) {

                  case lua_Type::LUA_TNIL:
                  case lua_Type::LUA_TDEADKEY: {
                        ptr->type = luramas::il::arch::kval_kinds::none;
                        break;
                  }
                  case lua_Type::LUA_TBOOLEAN: {
                        ptr->type = luramas::il::arch::kval_kinds::boolean;
                        ptr->boolean.b = kval.value.b;
                        break;
                  }

                  case lua_Type::LUA_TLIGHTUSERDATA: {
                        ptr->type = luramas::il::arch::kval_kinds::userdata;
                        ptr->userdata.str = "lightuserdata_" + std::to_string(i);
                        break;
                  }
                  case lua_Type::LUA_TNUMBER: {

                        ptr->type = luramas::il::arch::kval_kinds::integer;
                        ptr->integer.i = kval.value.n;

                        /* String and remove trailing 0's */
                        ptr->integer.str = std::to_string(ptr->integer.i);
                        ptr->integer.str.erase(ptr->integer.str.find_last_not_of('0') + 1, std::string::npos);
                        ptr->integer.str.erase(ptr->integer.str.find_last_not_of('.') + 1, std::string::npos);

                        break;
                  }
                  case lua_Type::LUA_TVECTOR: {
                        ptr->type = luramas::il::arch::kval_kinds::none; /* Ignored */
                        break;
                  }

                  case lua_Type::LUA_TSTRING: {

                        ptr->type = luramas::il::arch::kval_kinds::string;
                        ptr->string.str = gco2ts(kval.value.gc)->data;

                        /* Clean string. */
                        constexpr std::array str_arr = {'\"', '\''};

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

                  case lua_Type::LUA_TTABLE: {
                        const auto t = gco2h(kval.value.gc);
                        ptr->type = luramas::il::arch::kval_kinds::table;
                        ptr->table.array_size = t->sizearray;
                        ptr->table.node_size = t->lsizenode;
                        break;
                  }
                  case lua_Type::LUA_TFUNCTION: {
                        ptr->type = luramas::il::arch::kval_kinds::upvalue;
                        ptr->upvalue.str = "closure_" + std::to_string(i);
                        break;
                  }
                  case lua_Type::LUA_TUSERDATA: {
                        ptr->type = luramas::il::arch::kval_kinds::userdata;
                        ptr->userdata.str = "userdata_" + std::to_string(i);
                        break;
                  }
                  case lua_Type::LUA_TTHREAD: {
                        ptr->type = luramas::il::arch::kval_kinds::none; /* Ignored */
                        break;
                  }

                  case lua_Type::LUA_TPROTO: {
                        ptr->type = luramas::il::arch::kval_kinds::function;
                        ptr->closure.id = i;
                        ptr->closure.closure = lift_closure(gco2p(kval.value.gc));
                        break;
                  }
                  case lua_Type::LUA_TUPVAL: {
                        ptr->type = luramas::il::arch::kval_kinds::upvalue;
                        ptr->upvalue.str = "upval_" + std::to_string(i);
                        break;
                  }

                  default: {
                        throw std::runtime_error("Unkown kvalue type.");
                  }
            }

            buffer->kval.emplace_back(ptr);
      }

      /* Protos */
      for (auto i = 0; i < p->sizep; ++i)
            buffer->closures.emplace_back(lift_closure(p->p[i]));

      return;
}

std::shared_ptr<luramas::il::ilang> luramas::il::lifter::lift(Proto *p) {
      auto buffer = std::make_shared<luramas::il::ilang>();
      std::vector<std::shared_ptr<luau_v3_disassembler::disassembly>> dism;
      luau_v3_disassembler::disassemble(p, dism);
      luramas::il::lifter::lift(p, dism, buffer);
      return buffer;
};

#endif