#include "3rdparty/imports.hpp"
#include <array>
#include <cstdint>
#include <vector>

#define decode_opcode(inst) (inst & 0xFF)
#define decode_A(inst) (((inst) >> 8) & 0xFF)
#define decode_B(inst) (((inst) >> 16) & 0xFF)
#define decode_C(inst) (((inst) >> 24) & 0xFF)
#define decode_D(inst) (std::int32_t(inst) >> 16)
#define decode_E(inst) (std::int32_t(inst) >> 8)

namespace op_table {

      static constexpr const char *const fastcall_array[] = {

          "assert",

          "abs",
          "acos",
          "asin",
          "atan2",
          "atan",
          "ceil",
          "cosh",
          "cos",
          "deg",
          "exp",
          "floor",
          "fmod",
          "frexp",
          "ldexp",
          "log10",
          "log",
          "max",
          "min",
          "modf",
          "pow",
          "rad",
          "sinh",
          "sin",
          "sqrt",
          "tanh",
          "tan",

          "arshift",
          "band",
          "bnot",
          "bor",
          "bxor",
          "btest",
          "extract",
          "lrotate",
          "lshift",
          "replace",
          "rrotate",
          "rshift",

          "type",

          "byte",
          "char",
          "len",

          "typeof",

          "sub",

          "clamp",
          "sign",
          "round",

          "rawset",
          "rawget",
          "rawequal",

          "tinsert",
          "tunpack",

          "vector",

          "countlz",
          "countrz",

          "select",

          "rawlen",

          "extractk",

          "getmetatable",
          "setmetatable",

      };

      enum class operands : std::uint8_t {
            A,
            B,
            C,
            C_dec, /* C - 1*/
            C_inc, /* C + 1*/
            D,
            E,
            AUX,
            AUX_24
      };

      enum class type : std::uint8_t {
            reg,          /* Register. */
            val,          /* Value. (signed 64 bit integer) */
            k_idx_aux,    /* Value written to kvalue str if low bits. */
            jmp,          /* Jump. */
            k_idx,        /* Kvalue index. */
            aux,          /* Aux. */
            slot,         /* Slot. */
            upvalue,      /* Upvalue. */
            closure,      /* Proto. */
            table_size,   /* Table size. */
            fastcall_idx, /* Fastcall index. */
            capture_ref,  /* Capture refrence type. */
            capture_idx,  /* Capture index. */
            import_idx,   /* Luau Import index. */
            k_value_nstr, /* Kvalue string. */
            k_idx_pp,     /* K idx p->p relative (Valid for function K). */
            integer,      /* Generic integer. (double) */
            val_multret,  /* Value that can be a multret. (-1) */
            dest          /* Dest register. */
      };

      struct optable {
            LuauOpcode op;
            std::vector<op_table::operands> operands;
            std::vector<op_table::type> types;
      };

      static optable op_table[] = {

          {LuauOpcode::LOP_NOP, {}, {}},   // 00
          {LuauOpcode::LOP_BREAK, {}, {}}, // 01

          {LuauOpcode::LOP_LOADNIL, {op_table::operands::A}, {op_table::type::dest}},                                                                                       // 02
          {LuauOpcode::LOP_LOADB, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::val, op_table::type::jmp}}, // 03
          {LuauOpcode::LOP_LOADN, {op_table::operands::A, op_table::operands::D}, {op_table::type::dest, op_table::type::integer}},                                         // 04
          {LuauOpcode::LOP_LOADK, {op_table::operands::A, op_table::operands::D}, {op_table::type::dest, op_table::type::k_idx}},                                           // 05

          {LuauOpcode::LOP_MOVE, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}}, // 06

          {LuauOpcode::LOP_GETGLOBAL, {op_table::operands::A, op_table::operands::C, op_table::operands::AUX}, {op_table::type::dest, op_table::type::slot, op_table::type::k_value_nstr}}, // 07
          {LuauOpcode::LOP_SETGLOBAL, {op_table::operands::A, op_table::operands::C, op_table::operands::AUX}, {op_table::type::reg, op_table::type::slot, op_table::type::k_value_nstr}},  // 08

          {LuauOpcode::LOP_GETUPVAL, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::upvalue}}, // 09
          {LuauOpcode::LOP_SETUPVAL, {op_table::operands::A, op_table::operands::B}, {op_table::type::reg, op_table::type::upvalue}},  // 0A

          {LuauOpcode::LOP_CLOSEUPVALS, {op_table::operands::A}, {op_table::type::reg}},                                                                                                 // 0B
          {LuauOpcode::LOP_GETIMPORT, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::dest, op_table::type::import_idx, op_table::type::val}}, // 0C

          {LuauOpcode::LOP_GETTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 0D
          {LuauOpcode::LOP_SETTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::reg, op_table::type::reg, op_table::type::reg}},  // 0E

          {LuauOpcode::LOP_GETTABLEKS, {op_table::operands::A, op_table::operands::B, op_table::operands::C, op_table::operands::AUX}, {op_table::type::dest, op_table::type::reg, op_table::type::slot, op_table::type::k_idx}}, // 0F
          {LuauOpcode::LOP_SETTABLEKS, {op_table::operands::A, op_table::operands::B, op_table::operands::C, op_table::operands::AUX}, {op_table::type::reg, op_table::type::reg, op_table::type::slot, op_table::type::k_idx}},  // 10

          {LuauOpcode::LOP_GETTABLEN, {op_table::operands::A, op_table::operands::B, op_table::operands::C_inc}, {op_table::type::dest, op_table::type::reg, op_table::type::val}}, // 11
          {LuauOpcode::LOP_SETTABLEN, {op_table::operands::A, op_table::operands::B, op_table::operands::C_inc}, {op_table::type::reg, op_table::type::reg, op_table::type::val}},  // 12

          {LuauOpcode::LOP_NEWCLOSURE, {op_table::operands::A, op_table::operands::D}, {op_table::type::dest, op_table::type::closure}},                                                                                        // 13
          {LuauOpcode::LOP_NAMECALL, {op_table::operands::A, op_table::operands::B, op_table::operands::C, op_table::operands::AUX}, {op_table::type::dest, op_table::type::reg, op_table::type::slot, op_table::type::k_idx}}, // 14

          {LuauOpcode::LOP_CALL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::val_multret, op_table::type::val_multret}}, // 15
          {LuauOpcode::LOP_RETURN, {op_table::operands::A, op_table::operands::B}, {op_table::type::reg, op_table::type::val_multret}},                                                    // 16

          {LuauOpcode::LOP_JUMP, {op_table::operands::D}, {op_table::type::jmp}},     // 17
          {LuauOpcode::LOP_JUMPBACK, {op_table::operands::D}, {op_table::type::jmp}}, // 18

          {LuauOpcode::LOP_JUMPIF, {op_table::operands::A, op_table::operands::D}, {op_table::type::reg, op_table::type::jmp}},    // 19
          {LuauOpcode::LOP_JUMPIFNOT, {op_table::operands::A, op_table::operands::D}, {op_table::type::reg, op_table::type::jmp}}, // 1A

          {LuauOpcode::LOP_JUMPIFEQ, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}},    // 1B
          {LuauOpcode::LOP_JUMPIFLE, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}},    // 1C
          {LuauOpcode::LOP_JUMPIFLT, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}},    // 1D
          {LuauOpcode::LOP_JUMPIFNOTEQ, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}}, // 1E
          {LuauOpcode::LOP_JUMPIFNOTLE, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}}, // 1F
          {LuauOpcode::LOP_JUMPIFNOTLT, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::reg}}, // 20

          {LuauOpcode::LOP_ADD, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 21
          {LuauOpcode::LOP_SUB, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 22
          {LuauOpcode::LOP_MUL, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 23
          {LuauOpcode::LOP_DIV, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 24
          {LuauOpcode::LOP_MOD, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 25
          {LuauOpcode::LOP_POW, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 26

          {LuauOpcode::LOP_ADDK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 27
          {LuauOpcode::LOP_SUBK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 28
          {LuauOpcode::LOP_MULK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 29
          {LuauOpcode::LOP_DIVK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 2A
          {LuauOpcode::LOP_MODK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 2B
          {LuauOpcode::LOP_POWK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 2C

          {LuauOpcode::LOP_AND, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 2D
          {LuauOpcode::LOP_OR, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}},  // 2E

          {LuauOpcode::LOP_ANDK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}}, // 2F
          {LuauOpcode::LOP_ORK, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::k_idx}},  // 30

          {LuauOpcode::LOP_CONCAT, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::dest, op_table::type::reg, op_table::type::reg}}, // 31

          {LuauOpcode::LOP_NOT, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},    // 32
          {LuauOpcode::LOP_MINUS, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}},  // 33
          {LuauOpcode::LOP_LENGTH, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::reg}}, // 34

          {LuauOpcode::LOP_NEWTABLE, {op_table::operands::A, op_table::operands::B, op_table::operands::AUX}, {op_table::type::dest, op_table::type::table_size, op_table::type::val}},                                                // 35
          {LuauOpcode::LOP_DUPTABLE, {op_table::operands::A, op_table::operands::D}, {op_table::type::dest, op_table::type::k_idx}},                                                                                                   // 36
          {LuauOpcode::LOP_SETLIST, {op_table::operands::A, op_table::operands::B, op_table::operands::C_dec, op_table::operands::AUX}, {op_table::type::reg, op_table::type::reg, op_table::type::val_multret, op_table::type::val}}, // 37

          {LuauOpcode::LOP_FORNPREP, {op_table::operands::A, op_table::operands::D}, {op_table::type::reg, op_table::type::jmp}},                                               // 38
          {LuauOpcode::LOP_FORNLOOP, {op_table::operands::A, op_table::operands::D}, {op_table::type::reg, op_table::type::jmp}},                                               // 39
          {LuauOpcode::LOP_FORGLOOP, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::val}}, // 3A

          {LuauOpcode::LOP_FORGPREP_INEXT, {op_table::operands::A}, {op_table::type::reg}}, // 3B
          {LuauOpcode::LOP_DEP_FORGLOOP_INEXT, {}, {}},                                     // 3C : Depricated

          {LuauOpcode::LOP_FORGPREP_NEXT, {op_table::operands::A}, {op_table::type::reg}}, // 3D
          {LuauOpcode::LOP_DEP_FORGLOOP_NEXT, {}, {}},                                     // 3E : Depricated

          {LuauOpcode::LOP_GETVARARGS, {op_table::operands::A, op_table::operands::B}, {op_table::type::dest, op_table::type::val_multret}}, // 3F
          {LuauOpcode::LOP_DUPCLOSURE, {op_table::operands::A, op_table::operands::D}, {op_table::type::dest, op_table::type::k_idx_pp}},    // 40
          {LuauOpcode::LOP_PREPVARARGS, {op_table::operands::A}, {op_table::type::val}},                                                     // 41

          {LuauOpcode::LOP_LOADKX, {op_table::operands::A, op_table::operands::AUX}, {op_table::type::dest, op_table::type::k_idx}}, // 42
          {LuauOpcode::LOP_JUMPX, {op_table::operands::E}, {op_table::type::jmp}},                                                   // 43

          {LuauOpcode::LOP_FASTCALL, {op_table::operands::A, op_table::operands::C}, {op_table::type::fastcall_idx, op_table::type::jmp}},       // 44
          {LuauOpcode::LOP_COVERAGE, {op_table::operands::E}, {op_table::type::val}},                                                            // 45
          {LuauOpcode::LOP_CAPTURE, {op_table::operands::A, op_table::operands::B}, {op_table::type::capture_idx, op_table::type::capture_ref}}, // 46

          {LuauOpcode::LOP_DEP_JUMPIFEQK, {}, {}},    // 47
          {LuauOpcode::LOP_DEP_JUMPIFNOTEQK, {}, {}}, // 48

          {LuauOpcode::LOP_FASTCALL1, {op_table::operands::A, op_table::operands::B, op_table::operands::C}, {op_table::type::fastcall_idx, op_table::type::reg, op_table::type::jmp}},                                                  // 49
          {LuauOpcode::LOP_FASTCALL2, {op_table::operands::A, op_table::operands::B, op_table::operands::C, op_table::operands::AUX}, {op_table::type::fastcall_idx, op_table::type::reg, op_table::type::jmp, op_table::type::val}},    // 4A
          {LuauOpcode::LOP_FASTCALL2K, {op_table::operands::A, op_table::operands::B, op_table::operands::C, op_table::operands::AUX}, {op_table::type::fastcall_idx, op_table::type::reg, op_table::type::jmp, op_table::type::k_idx}}, // 4B

          {LuauOpcode::LOP_FORGPREP, {op_table::operands::A, op_table::operands::D}, {op_table::type::reg, op_table::type::jmp}}, // 4C

          {LuauOpcode::LOP_JUMPXEQKNIL, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::k_idx_aux}}, // 4D
          {LuauOpcode::LOP_JUMPXEQKB, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX}, {op_table::type::reg, op_table::type::jmp, op_table::type::k_idx_aux}},   // 4E

          {LuauOpcode::LOP_JUMPXEQKN, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX_24}, {op_table::type::reg, op_table::type::jmp, op_table::type::k_idx_aux}}, // 4F
          {LuauOpcode::LOP_JUMPXEQKS, {op_table::operands::A, op_table::operands::D, op_table::operands::AUX_24}, {op_table::type::reg, op_table::type::jmp, op_table::type::k_idx}}      // 50

      };

} // namespace op_table