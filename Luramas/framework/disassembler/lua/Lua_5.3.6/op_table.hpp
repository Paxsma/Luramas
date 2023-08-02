#include "3rdparty/imports.hpp"
#include <array>
#include <cstdint>
#include <vector>

namespace op_table {

      enum class operands : std::uint8_t {
            A,
            B,
            C,
            Ax,
            Bx,
            sBx
      };

      enum class type : std::uint8_t {
            reg,         /* Register. */
            val,         /* Value. (signed 64 bit integer) */
            jmp,         /* Jump. */
            k_idx,       /* Kvalue index. */
            upvalue,     /* Upvalue. */
            table_size,  /* Table size. */
            k_idx_pp,    /* K idx p->p relative (Valid for function K). */
            val_multret, /* Value that can be a multret. (-1) */
            dest         /* Dest register. */
      };

      struct optable {
            OpCode op;
            std::vector<op_table::operands> operands;
            std::vector<op_table::type> types;
      };

      static optable op_table[] = {

          {OpCode::OP_MOVE, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},                                                 // 00
          {OpCode::OP_LOADK, {op_table::operands::A, op_table::operands::Bx}, {op_table::type::dest, op_table::type::k_idx}},                                             // 01
          {OpCode::OP_LOADKX, {op_table::operands::Ax}, {op_table::type::dest}},                                                                                          // 02
          {OpCode::OP_LOADBOOL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::val, op_table::type::jmp}}, // 03
          {OpCode::OP_LOADNIL, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::val}},                                              // 04
          {OpCode::OP_GETUPVAL, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::upvalue}},                                         // 05

          {OpCode::OP_GETTABUP, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::upvalue, op_table::type::val}}, // 06
          {OpCode::OP_GETTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::val, op_table::type::val}},     // 07

          {OpCode::OP_SETTABUP, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::upvalue, op_table::type::val, op_table::type::val}}, // 08
          {OpCode::OP_SETUPVAL, {op_table::operands::A, op_table::operands::B}, {op_table::type::upvalue, op_table::type::val}},                                             // 09
          {OpCode::OP_SETTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::val, op_table::type::val, op_table::type::val}},     // 0A

          {OpCode::OP_NEWTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::table_size, op_table::type::table_size}}, // 0B

          {OpCode::OP_SELF, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::val, op_table::type::val}}, // 0C

          {OpCode::OP_ADD, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 0D
          {OpCode::OP_SUB, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 0E
          {OpCode::OP_MUL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 0F
          {OpCode::OP_MOD, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 10
          {OpCode::OP_POW, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 11
          {OpCode::OP_DIV, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 12
          {OpCode::OP_IDIV, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 13
          {OpCode::OP_BAND, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 14
          {OpCode::OP_BOR, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 15
          {OpCode::OP_BXOR, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 16
          {OpCode::OP_SHL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 17
          {OpCode::OP_SHR, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 18
          {OpCode::OP_UNM, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},                                              // 19
          {OpCode::OP_BNOT, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},                                             // 1A
          {OpCode::OP_NOT, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},                                              // 1B
          {OpCode::OP_LEN, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},                                              // 1C

          {OpCode::OP_CONCAT, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 1D

          {OpCode::OP_JMP, {op_table::operands::A, op_table::operands::sBx}, {op_table::type::val, op_table::type::jmp}},                                          // 1F
          {OpCode::OP_EQ, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::val, op_table::type::reg, op_table::type::reg}}, // 20
          {OpCode::OP_LT, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::val, op_table::type::reg, op_table::type::reg}}, // 21
          {OpCode::OP_LE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::val, op_table::type::reg, op_table::type::reg}}, // 22

          {OpCode::OP_TEST, {op_table::operands::A, op_table::operands::C}, {op_table::type::reg, op_table::type::val}},                                                // 23
          {OpCode::OP_TESTSET, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::reg, op_table::type::reg, op_table::type::val}}, // 24

          {OpCode::OP_CALL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::reg, op_table::type::val_multret, op_table::type::val_multret}},     // 25
          {OpCode::OP_TAILCALL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::reg, op_table::type::val_multret, op_table::type::val_multret}}, // 26
          {OpCode::OP_RETURN, {op_table::operands::A, op_table::operands::B}, {op_table::type::reg, op_table::type::val_multret}},                                                       // 27

          {OpCode::OP_FORLOOP, {op_table::operands::A, op_table::operands::sBx}, {op_table::type::reg, op_table::type::jmp}}, // 28
          {OpCode::OP_FORPREP, {op_table::operands::A, op_table::operands::sBx}, {op_table::type::reg, op_table::type::jmp}}, // 29

          {OpCode::OP_TFORCALL, {op_table::operands::A, op_table::operands::C}, {op_table::type::reg, op_table::type::val}}, // 2A

          {OpCode::OP_TFORLOOP, {op_table::operands::A, op_table::operands::sBx}, {op_table::type::reg, op_table::type::jmp}}, // 2B

          {OpCode::OP_SETLIST, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::reg, op_table::type::val_multret, op_table::type::val_multret}}, // 2C

          {OpCode::OP_CLOSURE, {op_table::operands::A, op_table::operands::Bx}, {op_table::type::reg, op_table::type::k_idx_pp}},  // 2D
          {OpCode::OP_VARARG, {op_table::operands::A, op_table::operands::B}, {op_table::type::reg, op_table::type::val_multret}}, // 2E

          {OpCode::OP_EXTRAARG, {op_table::operands::Ax}, {op_table::type::reg}}, // 2F

      };

} // namespace op_table