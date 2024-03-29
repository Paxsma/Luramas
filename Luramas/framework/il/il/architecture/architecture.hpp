/*
* framework/il/il/architecture/architecture.hpp
* 
* IL Architecture
* 10/7/2023
* 
* Provides the schematics, ISA, and functionality of how the IL is represented.
*/
#pragma once

#ifndef luramas_il_architecture
#define luramas_il_architecture

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using il_register = std::uint16_t;

namespace luramas {

      namespace il {

            namespace arch {

                  namespace data {

                        enum class data_kinds : std::uint8_t {
                              integer,
                              string,
                              global
                        };

                  }

                  namespace operand {

                        /* Encoding translation | Encoding */
                        enum class operand_kind : std::uint8_t {
                              none,          /* Nothing | None object. */
                              reg,           /* reg | Register. */
                              kval,          /* idx | Kvalue. */
                              kval_embeded,  /* idx | Kvalue string. */
                              jmp,           /* jmp | Jump. */
                              integer,       /* integer | Integer. */
                              boolean,       /* boolean | Boolean. */
                              upvalue_kind,  /* idx | Upvalue kind. */
                              upvalue,       /* idx | Upvalue. */
                              closure,       /* idx | Closure. */
                              table_size,    /* size | Table size. */
                              value,         /* value | Generic value. */
                              closure_index, /* idx | Closure index for closure stack. */
                        };

                        /* Operand encoding kinds (Bits, Functionality) */
                        enum class operand_encoding : std::uint8_t {
                              reg,      /* [(signed)16-Bits] Register. */
                              idx,      /* [(unsigned)32-Bits] Generic index. */
                              jmp,      /* [(signed)64-Bits] Jump. */
                              integer,  /* [double] Integer. */
                              boolean,  /* [(unsigned)8-Bits] Boolean. */
                              enumeral, /* [(unsigned)8-Bits] Generic 8-bit enumeral type. */
                              value,    /* [(signed)32-Bits] Generic value. */
                        };

                        /* Upvalue kinds */
                        enum class upvalue_kind : std::uint8_t {
                              contents, /* Capture contents of register. */
                              reg,      /* Capture register as upvalue. */
                        };

                        /* Operand disassembly */
                        union operand_disassembly {
                              std::uint16_t reg;          /* Generic register. */
                              std::intptr_t kvalue_idx;   /* Kvalue index for kvalue stack. */
                              std::intptr_t closure_idx;  /* Closure index for closure stack. */
                              std::intptr_t upvalue_idx;  /* Upvalue index for upvalue stack. */
                              std::intptr_t jmp;          /* Jump. */
                              double integer;             /* Integer value. */
                              bool boolean;               /* Boolean value. */
                              upvalue_kind upvalue_kind;  /* Upvalue captured type. */
                              std::uintptr_t upvalue_reg; /* Upvalue register. */
                              std::size_t closure_id;     /* Closure index for Kvalues. */
                              std::size_t table_size;     /* Size of table element. */
                              std::intptr_t val;          /* Generic value. */
                        };

                        /* Disassembly struct for operands */
                        struct operand {

                              operand_kind type = operand_kind::none;

                              operand_disassembly dis;

                              std::uintptr_t ref_addr = 0u; /* Refrence address by operand. */
                              std::string k_value = "";     /* Seperate value to represent as a string and idx. Will also serve as import str. */
                              std::string integer_str = ""; /* Fixed integer string. */
                              bool multret = false;         /* Original value was changed by multret. */
                        };

                  } // namespace operand

#pragma region opcodes

                  /* Opcode Legend (Opcode, Hint, Operands):
                Register: "r??"
                Kvalue: "??"
                Integer(Double): "??"
                Jump: "+/-??" 
                UpvalueID: "u??"
                GlobalID: "??"
                Boolean: "true/false"
                UpvalueKind: see upvalue_kind
                ClosureID: "c??"
                Val(Intptr): "??",
                Virtual_Function: "??",
                Table_Size: "??" ,
                Size: sizeof(int8_t), sizeof(int16_t), sizeof(in32_t), sizeof(int64_t)
                 */
                  enum class opcodes : std::uint8_t {

                        OP_NOP, /* (00) | Nothing */

                        OP_LOADBOOL,   /* (01) | Loads a boolean to the destination register | * Dest(Register), * Source(Boolean) */
                        OP_LOADINT,    /* (02) | Loads a integer to the destination register | * Dest(Register), * Source(Integer) */
                        OP_LOADNONE,   /* (03) | Loads a None to the destination register | * Dest(Register) */
                        OP_LOADKVAL,   /* (04) | Loads a kvalue to the destination register | * Dest(Register), * Source(Kvalue) */
                        OP_LOADGLOBAL, /* (05) | Loads a global to the destination register | * Dest(Register), * Source(GlobalID) */
                        OP_LOADATTR,   /* (06) | Loads a attribute from attribute stack to the destination register | * Dest(Register), * Attribute stack identifier(Value), * Attribute index(Value) */

                        OP_GETTABUPVALUE, /* (07) | Loads a kvalue from structure to the destination register | * Dest(Register), * Source(Kvalue) */
                        OP_SETGLOBAL,     /* (08) | Set global | * Source(Register), * Dest(GlobalID) */

                        OP_SETATTR, /* (09) | Sets attribute on attribute stack (class) | * Source(Register), * Attribute stack identifier(Value), * Attribute index(Value) */
                        OP_ADDATTR, /* (0A) | Adds attribute on attribute stack (class) | * Source(Register), * Attribute stack identifier(Value), * Attribute index(Value) */

                        /* (0B) | Move | Dest(Register), Source(Register) */
                        OP_MOVE,

                        /* Arith/Bitwise |
                        * Dest(Register)
                        * Source(Register)
                        * Value(Register)  */
                        OP_ADD,  /* (0C) |  +  */
                        OP_SUB,  /* (0D) |  -  */
                        OP_MUL,  /* (0E) |  *  */
                        OP_DIV,  /* (0F) |  /  */
                        OP_MOD,  /* (10) |  %  */
                        OP_POW,  /* (11) |  ^  */
                        OP_AND,  /* (12) |  &  */
                        OP_XOR,  /* (13) |  ~  */
                        OP_SHL,  /* (14) |  << */
                        OP_SHR,  /* (15) |  >> */
                        OP_IDIV, /* (16) |  // */
                        OP_OR,   /* (17) |  |  */

                        /* Arith/Bitwise Kvalue |
                        * Dest(Register)
                        * Source(Register)
                        * Value(Kvalue) */
                        OP_ADDK,  /* (18) |  +  */
                        OP_SUBK,  /* (19) |  -  */
                        OP_MULK,  /* (1A) |  *  */
                        OP_DIVK,  /* (1B) |  /  */
                        OP_MODK,  /* (1C) |  %  */
                        OP_POWK,  /* (1D) |  ^  */
                        OP_ANDK,  /* (1E) |  &  */
                        OP_XORK,  /* (1F) |  ~  */
                        OP_SHLK,  /* (20) |  << */
                        OP_SHRK,  /* (21) |  >> */
                        OP_IDIVK, /* (22) |  // */
                        OP_ORK,   /* (23) |  |  */

                        /* Arith/Bitwise Integer |
                        * Dest(Register)
                        * Source(Register)
                        * Value(Integer) */
                        OP_ADDN,  /* (24) |  +  */
                        OP_SUBN,  /* (25) |  -  */
                        OP_MULN,  /* (26) |  *  */
                        OP_DIVN,  /* (27) |  /  */
                        OP_MODN,  /* (28) |  %  */
                        OP_POWN,  /* (29) |  ^  */
                        OP_ANDN,  /* (2A) |  &  */
                        OP_XORN,  /* (2B) |  ~  */
                        OP_SHLN,  /* (2C) |  << */
                        OP_SHRN,  /* (2D) |  >> */
                        OP_IDIVN, /* (2E) |  // */
                        OP_ORN,   /* (2F) |  |  */

                        /* Unary:
                        * Dest(Register)
                        * Source(Register) */
                        OP_LEN,    /* (30) |  #   */
                        OP_NOT,    /* (31) |  not */
                        OP_MINUS,  /* (32) |  -   */
                        OP_BITNOT, /* (33) |  ~   */
                        OP_PLUS,   /* (34) |  +   */

                        /* (35) | C function and closure call | * Dest(Register), * Arguement count(Val), * Return count(Val) */
                        OP_CCALL,

                        /* (36) | Virtual function call (clears virtual function arguement stack once executed) | * Dest(Register), * Virtual function(Virtual_Function) * Arguement count(Val), * Return count(Val) */
                        OP_VCALL,

                        /* (37) | Pushes arguement to virtual function stack | * Dest(Register) */
                        OP_VPUSH,

                        /* (38) | Loads function in table to a register | * Dest(Register), * Source(Register), * Kvalue(Kvalue) */
                        OP_SELF,

                        /* (39) | Return from a function | * Start register(Register), * Return count(Val) */
                        OP_RETURN,

                        /* (3A) | String concatation | * Dest(Register), * Start register(Val), * End register(Val) */
                        OP_CONCAT,

                        /* (3B) | Jump to a memory address | * Target(Jump) */
                        OP_JUMP,

                        /* (3C) | Jumpback in memory | * Target(Jump =< 0) */
                        OP_JUMPBACK,

                        OP_CMP,     /* (3D) | Writes two compare registers too a cmp flag | * Compare(Register), * Compare(Register) */
                        OP_CMPK,    /* (3E) | Writes compare register and a kvalue too cmp flag | * Compare(Register), * Compare(Kvalue) */
                        OP_CMPN,    /* (3F) | Writes compare register and a integer too cmp flag | * Compare(Register), * Compare(Val) */
                        OP_CMPNONE, /* (40) | Writes compare register and a none object too cmp flag | * Compare(Register) */

                        OP_CMPS,     /* (41) | Writes one compare register too cmp flag | * Compare(Register) */
                        OP_CMPSK,    /* (42) | Writes one compare kvalue too cmp flag | * Compare(Kvalue) */
                        OP_CMPSN,    /* (43) | Writes one compare integer too cmp flag | * Compare(Val) */
                        OP_CMPSNONE, /* (44) | Writes one compare none object cmp flag */

                        /* (45) | Jump if cmp flag | * Jump address(jump) */
                        OP_JUMPIF,

                        /* (46) | Jump if not cmp flag | * Jump address(jump) */
                        OP_JUMPIFNOT,

                        /* Jump if ?? cmp flag | * Jump address(jump) */
                        OP_JUMPIFEQUAL,        /* (47) |  == */
                        OP_JUMPIFNOTEQUAL,     /* (48) |  != */
                        OP_JUMPIFLESS,         /* (49) |  <  */
                        OP_JUMPIFLESSEQUAL,    /* (4A) |  <= */
                        OP_JUMPIFGREATER,      /* (4B) |  >  */
                        OP_JUMPIFGREATEREQUAL, /* (4C) |  >= */

                        /* (4D) | Set true or false if cmp flag | * Dest(Register) */
                        OP_SETIF,

                        /* (4E) | Set true or false if not cmp flag | * Dest(Register) */
                        OP_SETIFNOT,

                        /* Set true or false if ?? cmp flag | * Dest(Register) */
                        OP_SETIFEQUAL,        /* (4F) |  == */
                        OP_SETIFNOTEQUAL,     /* (50) |  != */
                        OP_SETIFLESS,         /* (51) |  <  */
                        OP_SETIFLESSEQUAL,    /* (52) |  <= */
                        OP_SETIFGREATER,      /* (53) |  >  */
                        OP_SETIFGREATEREQUAL, /* (54) |  >= */

                        OP_SETUPVALUE, /*  (55) | Set Upvalue | * Source(Register), * Upvalue ID(UpvalueID) */
                        OP_GETUPVALUE, /*  (56) | Get Upvalue | * Dest(Register), * Upvalue ID(UpvalueID) */

                        /* (57) | Destroy all upvalues with target. */
                        OP_DESTROYUPVALUES,

                        /* (58) | Destroy upvalues with target | * Start(Register) */
                        OP_DESTROYUPVALUESA,

                        /* (59) | Sets upvalue for next closure instruction(Follows closure instruction) | * Type(UpvalueKind), * Source(Register)   */
                        OP_ADDUPVALUE,

                        /* (5A) | Inits */
                        OP_RESUME,

                        /* (5B) |  Gets varargs | * Dest(Register), * Amount(Val) */
                        OP_GETVARARGS,

                        OP_SETTABLE, /* (5C) | Set table | * Source(Register), * Table(Register), * Index(Register) */
                        OP_GETTABLE, /* (5D) | Get table | * Dest(Register), * Table(Register), * Index(Register) */

                        OP_SETTABLEN, /* (5E) | Set table | * Source(Register), * Table(Register), * Index(Val) */
                        OP_GETTABLEN, /* (5F) | Get table | * Dest(Register), * Table(Register), * Index(Val) */

                        OP_SETTABLEK, /* (60) | Set table | * Source(Register), * Table(Register), * Index(Kvalue) */
                        OP_GETTABLEK, /* (61) | Get table | * Dest(Register), * Table(Register), * Index(Kvalue) */

                        OP_INITFORLOOPN, /* (62) | Inits numeric for loop | * Loop iteration location(Jump) */
                        OP_INITFORLOOPG, /* (63) | Inits generic for loop | * Loop iteration location(Jump) */

                        OP_INITFORLOOPSPECIAL, /* (64) | Inits abstract special generic for loop refrence generic loop instruction for data | * Loop iteration location(Jump) */

                        OP_NEWCLOSURE, /* (65) | Creates new closure | * Dest(Register), * Closure ID(ClosureID) */
                        OP_REFCLOSURE, /* (66) | Gets closure from kvalue | * Dest(Register), * Closure(Kvalue) */

                        OP_NEWTABLE, /* (67) | Creates new table | * Dest(Register), * Exact table size(Table_Size), * Exact array size(Table_Size) */
                        OP_REFTABLE, /* (68) | Gets table from kvalue | * Dest(Register), * Table(Kvalue) (Exact size, node) */

                        OP_NEWTABLEA, /* (69) | Creates new table | * Dest(Register), * Approximate table size(Table_Size), * Approximate array size(Table_Size) */
                        OP_REFTABLEA, /* (6A) | Gets table from kvalue | * Dest(Register), * Table(Kvalue) (Approximate size, node) */

                        OP_SETLIST, /* (6B) | Appends elements in a table | * Dest table(Register), * Start register(Register), * Exact table size(Val), * Index(Val) */

                        /*
                            Format |
                                Start register (generator),
                                Start register + 1 (start),
                                Start register + 2 (index),
                                Start register + 3 >= (variables)
                        */
                        OP_FORLOOPG, /* (6C) | For loop generic | * Start register(Register) follows format above, * Loop variable count(Val), * Jump back(Jump)  */
                        OP_FORLOOPN, /* (6D) | For loop numeric | * Start value register(Register), * Maximum/target value register(Register),  * Incrementation value register(Register), * Jump back(Jump) */

                        OP_POPTOP, /* (6E) | Pops register from top of the stack. */
                        OP_POPARG, /* (6F) | Adds arguement too poparg flag too get poped when next OP_CALL instruction is hit | * Ignore Register(Register) */

                        OP_STACKSTORE, /* (70) | Store value on the stack | * Offset Register(Register), * Data(Val), * Size(Size)  */
                        OP_STACKGET,   /* (71) | Get value from the stack | * Dest(Register), * Offset Register(Register), * Size(Size) */

                        OP_STACKSTOREN, /* (72) | Store value on the stack | * Offset Register(val), * Data(Val), * Size(Size)  */
                        OP_STACKGETN,   /* (73) | Get value from the stack | * Dest(Register), * Offset Register(Val), * Size(Size)  */

                        OP_AMT /* Amount */
                  };

                  /* Virtual functions legend (Hint, Arguements, Return) */
                  enum class virtual_functions : std::uint8_t {

                        VTF_TABLE_SLICE, /* Slices table | */
                        VTF_TABLE_UNPACK,

                        VTF_PRINT
                  };

#pragma endregion

                  /* Kvalue kinds | Encoding */
                  enum class kval_kinds : std::uint8_t {
                        none,     /* None kind | *KIND(unsigned 8-bits) */
                        boolean,  /* Boolean kind | *KIND(unsigned 8-bits), *[unsigned 8-Bits] */
                        integer,  /* Boolean kind | *KIND(unsigned 8-bits), *[double 64-Bits] */
                        string,   /* String kind | *KIND(unsigned 8-bits), *STRING LENGTH(unsigned 64-Bits), *STRING(UTF-8 CHARACTERS, DOES NOT INCLUDE NULL TERMINATOR) */
                        userdata, /* Userdata kind | *KIND(unsigned 8-bits) */
                        table,    /* Table kind | *KIND(unsigned 8-bits), *NODE SIZE(unsigned 64-Bits), *ARRAY SIZE(unsigned 64-Bits) */
                        function, /* Function kind | *KIND(unsigned 8-bits), *FUNCTION STRING LENGTH(unsigned 32-Bits), FUNCTION STRING(UTF-8 CHARACTERS, DOES NOT INCLUDE NULL TERMINATOR)  */
                        closure,  /* Proto kind | *KIND(unsigned 8-bits), *CLOSURE ID(unsigned 32-Bits)  */
                        thread,   /* Threade kind | *KIND(unsigned 8-bits) */
                        upvalue   /* Proto kind | *KIND(unsigned 8-bits)  */
                  };

#pragma region table_structs

                  struct optable_kind {
                        opcodes op;
                        std::vector<operand::operand_kind> kinds;
                  };

                  struct optable_encoding {
                        opcodes op;
                        std::vector<operand::operand_encoding> encodings;
                  };

                  struct optable_descriptor {

                        /* Names */
                        const char *const opname;
                        const char *const mnemonic;
                        const char *const hint;

                        /* Kinds */
                        std::vector<const char *> operand_encoding; /* Kind(Operand encoding) */
                  };

#pragma endregion

#pragma region optables

                  /* IL operand kinds */
                  static optable_kind opkinds[] = {

                      {opcodes::OP_NOP, {}}, /* 00 */

                      {opcodes::OP_LOADBOOL, {operand::operand_kind::reg, operand::operand_kind::boolean}},                             /* 01 */
                      {opcodes::OP_LOADINT, {operand::operand_kind::reg, operand::operand_kind::integer}},                              /* 02 */
                      {opcodes::OP_LOADNONE, {operand::operand_kind::reg}},                                                             /* 03 */
                      {opcodes::OP_LOADKVAL, {operand::operand_kind::reg, operand::operand_kind::kval_embeded}},                        /* 04 */
                      {opcodes::OP_LOADGLOBAL, {operand::operand_kind::reg, operand::operand_kind::kval}},                              /* 05 */
                      {opcodes::OP_LOADATTR, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}}, /* 06 */
                      {opcodes::OP_GETTABUPVALUE, {operand::operand_kind::reg, operand::operand_kind::kval_embeded}},                   /* 07 */

                      {opcodes::OP_SETGLOBAL, {operand::operand_kind::reg, operand::operand_kind::value}}, /* 08 */

                      {opcodes::OP_SETATTR, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}}, /* 09 */
                      {opcodes::OP_ADDATTR, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}}, /* 0A */

                      {opcodes::OP_MOVE, {operand::operand_kind::reg, operand::operand_kind::reg}}, /* 0B */

                      {opcodes::OP_ADD, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 0C */
                      {opcodes::OP_SUB, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 0D */
                      {opcodes::OP_MUL, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 0E */
                      {opcodes::OP_DIV, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 0F */
                      {opcodes::OP_MOD, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 10 */
                      {opcodes::OP_POW, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 11 */
                      {opcodes::OP_AND, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 12 */
                      {opcodes::OP_XOR, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 13 */
                      {opcodes::OP_SHL, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 14 */
                      {opcodes::OP_SHR, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},  /* 15 */
                      {opcodes::OP_IDIV, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}}, /* 16 */
                      {opcodes::OP_OR, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}},   /* 17 */

                      {opcodes::OP_ADDK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 18 */
                      {opcodes::OP_SUBK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 19 */
                      {opcodes::OP_MULK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1A */
                      {opcodes::OP_DIVK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1B */
                      {opcodes::OP_MODK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1C */
                      {opcodes::OP_POWK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1D */
                      {opcodes::OP_ANDK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1E */
                      {opcodes::OP_XORK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 1F */
                      {opcodes::OP_SHLK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 20 */
                      {opcodes::OP_SHRK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},  /* 21 */
                      {opcodes::OP_IDIVK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}}, /* 22 */
                      {opcodes::OP_ORK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}},   /* 23 */

                      {opcodes::OP_ADDN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 24 */
                      {opcodes::OP_SUBN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 25 */
                      {opcodes::OP_MULN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 26 */
                      {opcodes::OP_DIVN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 27 */
                      {opcodes::OP_MODN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 28 */
                      {opcodes::OP_POWN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 29 */
                      {opcodes::OP_ANDN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 2A */
                      {opcodes::OP_XORN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 2B */
                      {opcodes::OP_SHLN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 2C */
                      {opcodes::OP_SHRN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},  /* 2D */
                      {opcodes::OP_IDIVN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}}, /* 2E */
                      {opcodes::OP_ORN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::integer}},   /* 2F */

                      {opcodes::OP_LEN, {operand::operand_kind::reg, operand::operand_kind::reg}},    /* 30 */
                      {opcodes::OP_NOT, {operand::operand_kind::reg, operand::operand_kind::reg}},    /* 31 */
                      {opcodes::OP_MINUS, {operand::operand_kind::reg, operand::operand_kind::reg}},  /* 32 */
                      {opcodes::OP_BITNOT, {operand::operand_kind::reg, operand::operand_kind::reg}}, /* 33 */
                      {opcodes::OP_PLUS, {operand::operand_kind::reg, operand::operand_kind::reg}},   /* 34 */

                      {opcodes::OP_CCALL, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}},                               /* 35 */
                      {opcodes::OP_VCALL, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value, operand::operand_kind::value}}, /* 36 */

                      {opcodes::OP_VPUSH, {operand::operand_kind::reg}}, /* 37 */

                      {opcodes::OP_SELF, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}}, /* 38 */
                      {opcodes::OP_RETURN, {operand::operand_kind::reg, operand::operand_kind::value}},                          /* 39 */

                      {opcodes::OP_CONCAT, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}}, /* 3A */

                      {opcodes::OP_JUMP, {operand::operand_kind::jmp}},     /* 3B */
                      {opcodes::OP_JUMPBACK, {operand::operand_kind::jmp}}, /* 3C */

                      {opcodes::OP_CMP, {operand::operand_kind::reg, operand::operand_kind::reg}},      /* 3D */
                      {opcodes::OP_CMPK, {operand::operand_kind::reg, operand::operand_kind::kval}},    /* 3E */
                      {opcodes::OP_CMPN, {operand::operand_kind::reg, operand::operand_kind::integer}}, /* 3F */
                      {opcodes::OP_CMPNONE, {operand::operand_kind::reg}},                              /* 40 */

                      {opcodes::OP_CMPS, {operand::operand_kind::reg}},      /* 41 */
                      {opcodes::OP_CMPSK, {operand::operand_kind::kval}},    /* 42 */
                      {opcodes::OP_CMPSN, {operand::operand_kind::integer}}, /* 43 */
                      {opcodes::OP_CMPSNONE, {}},                            /* 44 */

                      {opcodes::OP_JUMPIF, {operand::operand_kind::jmp}},    /* 45 */
                      {opcodes::OP_JUMPIFNOT, {operand::operand_kind::jmp}}, /* 46 */

                      {opcodes::OP_JUMPIFEQUAL, {operand::operand_kind::jmp}},        /* 47 */
                      {opcodes::OP_JUMPIFNOTEQUAL, {operand::operand_kind::jmp}},     /* 48 */
                      {opcodes::OP_JUMPIFLESS, {operand::operand_kind::jmp}},         /* 49 */
                      {opcodes::OP_JUMPIFLESSEQUAL, {operand::operand_kind::jmp}},    /* 4A */
                      {opcodes::OP_JUMPIFGREATER, {operand::operand_kind::jmp}},      /* 4B */
                      {opcodes::OP_JUMPIFGREATEREQUAL, {operand::operand_kind::jmp}}, /* 4C */

                      {opcodes::OP_SETIF, {operand::operand_kind::reg}},    /* 4D */
                      {opcodes::OP_SETIFNOT, {operand::operand_kind::reg}}, /* 4E */

                      {opcodes::OP_SETIFEQUAL, {operand::operand_kind::reg}},        /* 4F */
                      {opcodes::OP_SETIFNOTEQUAL, {operand::operand_kind::reg}},     /* 50 */
                      {opcodes::OP_SETIFLESS, {operand::operand_kind::reg}},         /* 51 */
                      {opcodes::OP_SETIFLESSEQUAL, {operand::operand_kind::reg}},    /* 52 */
                      {opcodes::OP_SETIFGREATER, {operand::operand_kind::reg}},      /* 53 */
                      {opcodes::OP_SETIFGREATEREQUAL, {operand::operand_kind::reg}}, /* 54 */

                      {opcodes::OP_SETUPVALUE, {operand::operand_kind::reg, operand::operand_kind::upvalue}}, /* 55 */
                      {opcodes::OP_GETUPVALUE, {operand::operand_kind::reg, operand::operand_kind::upvalue}}, /* 56 */

                      {opcodes::OP_DESTROYUPVALUES, {}},                            /* 57 */
                      {opcodes::OP_DESTROYUPVALUESA, {operand::operand_kind::reg}}, /* 58 */

                      {opcodes::OP_ADDUPVALUE, {operand::operand_kind::upvalue_kind, operand::operand_kind::reg}}, /* 59 */
                      {opcodes::OP_RESUME, {}},                                                                    /* 5A */

                      {opcodes::OP_GETVARARGS, {operand::operand_kind::reg, operand::operand_kind::value}}, /* 5B */

                      {opcodes::OP_SETTABLE, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}}, /* 5C */
                      {opcodes::OP_GETTABLE, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg}}, /* 5D */

                      {opcodes::OP_SETTABLEN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::value}}, /* 5E */
                      {opcodes::OP_GETTABLEN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::value}}, /* 5F */

                      {opcodes::OP_SETTABLEK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}}, /* 60 */
                      {opcodes::OP_GETTABLEK, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::kval}}, /* 61 */

                      {opcodes::OP_INITFORLOOPN, {operand::operand_kind::jmp}},       /* 62 */
                      {opcodes::OP_INITFORLOOPG, {operand::operand_kind::jmp}},       /* 63 */
                      {opcodes::OP_INITFORLOOPSPECIAL, {operand::operand_kind::jmp}}, /* 64 */

                      {opcodes::OP_NEWCLOSURE, {operand::operand_kind::reg, operand::operand_kind::closure}}, /* 65 */
                      {opcodes::OP_REFCLOSURE, {operand::operand_kind::reg, operand::operand_kind::kval}},    /* 66 */

                      {opcodes::OP_NEWTABLE, {operand::operand_kind::reg, operand::operand_kind::table_size, operand::operand_kind::table_size}}, /* 67 */
                      {opcodes::OP_REFTABLE, {operand::operand_kind::reg, operand::operand_kind::kval}},                                          /* 68 */

                      {opcodes::OP_NEWTABLEA, {operand::operand_kind::reg, operand::operand_kind::table_size, operand::operand_kind::table_size}}, /* 69 */
                      {opcodes::OP_REFTABLEA, {operand::operand_kind::reg, operand::operand_kind::kval}},                                          /* 6A */

                      {opcodes::OP_SETLIST, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::value}}, /* 6B */

                      {opcodes::OP_FORLOOPG, {operand::operand_kind::reg, operand::operand_kind::value, operand::operand_kind::jmp}},                           /* 6C */
                      {opcodes::OP_FORLOOPN, {operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::reg, operand::operand_kind::jmp}}, /* 6D */

                      {opcodes::OP_POPTOP, {}},                           /* 6E */
                      {opcodes::OP_POPARG, {operand::operand_kind::reg}}, /* 6F */
                  };

                  /* IL operand encodings */
                  static optable_encoding opencodings[] = {

                      {opcodes::OP_NOP, {}}, /* 00 */

                      {opcodes::OP_LOADBOOL, {operand::operand_encoding::reg, operand::operand_encoding::boolean}},                                 /* 01 */
                      {opcodes::OP_LOADINT, {operand::operand_encoding::reg, operand::operand_encoding::integer}},                                  /* 02 */
                      {opcodes::OP_LOADNONE, {operand::operand_encoding::reg}},                                                                     /* 03 */
                      {opcodes::OP_LOADKVAL, {operand::operand_encoding::reg, operand::operand_encoding::idx}},                                     /* 04 */
                      {opcodes::OP_LOADGLOBAL, {operand::operand_encoding::reg, operand::operand_encoding::idx}},                                   /* 05 */
                      {opcodes::OP_LOADATTR, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 06 */
                      {opcodes::OP_GETTABUPVALUE, {operand::operand_encoding::reg, operand::operand_encoding::idx}},                                /* 07 */

                      {opcodes::OP_SETGLOBAL, {operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 08 */

                      {opcodes::OP_SETATTR, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 09 */
                      {opcodes::OP_ADDATTR, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 0A */

                      {opcodes::OP_MOVE, {operand::operand_encoding::reg, operand::operand_encoding::reg}}, /* 0B */

                      {opcodes::OP_ADD, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 0C */
                      {opcodes::OP_SUB, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 0D */
                      {opcodes::OP_MUL, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 0E */
                      {opcodes::OP_DIV, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 0F */
                      {opcodes::OP_MOD, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 10 */
                      {opcodes::OP_POW, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 11 */
                      {opcodes::OP_AND, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 12 */
                      {opcodes::OP_XOR, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 13 */
                      {opcodes::OP_SHL, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 14 */
                      {opcodes::OP_SHR, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 15 */
                      {opcodes::OP_IDIV, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}}, /* 16 */
                      {opcodes::OP_OR, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}},   /* 17 */

                      {opcodes::OP_ADDK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 18 */
                      {opcodes::OP_SUBK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 19 */
                      {opcodes::OP_MULK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1A */
                      {opcodes::OP_DIVK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1B */
                      {opcodes::OP_MODK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1C */
                      {opcodes::OP_POWK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1D */
                      {opcodes::OP_ANDK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1E */
                      {opcodes::OP_XORK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 1F */
                      {opcodes::OP_SHLK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 20 */
                      {opcodes::OP_SHRK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},  /* 21 */
                      {opcodes::OP_IDIVK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}}, /* 22 */
                      {opcodes::OP_ORK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}},   /* 23 */

                      {opcodes::OP_ADDN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 24 */
                      {opcodes::OP_SUBN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 25 */
                      {opcodes::OP_MULN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 26 */
                      {opcodes::OP_DIVN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 27 */
                      {opcodes::OP_MODN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 28 */
                      {opcodes::OP_POWN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 29 */
                      {opcodes::OP_ANDN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 2A */
                      {opcodes::OP_XORN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 2B */
                      {opcodes::OP_SHLN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 2C */
                      {opcodes::OP_SHRN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},  /* 2D */
                      {opcodes::OP_IDIVN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}}, /* 2E */
                      {opcodes::OP_ORN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::integer}},   /* 2F */

                      {opcodes::OP_LEN, {operand::operand_encoding::reg, operand::operand_encoding::reg}},    /* 30 */
                      {opcodes::OP_NOT, {operand::operand_encoding::reg, operand::operand_encoding::reg}},    /* 31 */
                      {opcodes::OP_MINUS, {operand::operand_encoding::reg, operand::operand_encoding::reg}},  /* 32 */
                      {opcodes::OP_BITNOT, {operand::operand_encoding::reg, operand::operand_encoding::reg}}, /* 33 */
                      {opcodes::OP_PLUS, {operand::operand_encoding::reg, operand::operand_encoding::reg}},   /* 34 */

                      {opcodes::OP_CCALL, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}},                                   /* 35 */
                      {opcodes::OP_VCALL, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 36 */

                      {opcodes::OP_VPUSH, {operand::operand_encoding::reg}}, /* 37 */

                      {opcodes::OP_SELF, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}}, /* 38 */
                      {opcodes::OP_RETURN, {operand::operand_encoding::reg, operand::operand_encoding::value}},                             /* 39 */

                      {opcodes::OP_CONCAT, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 3A */

                      {opcodes::OP_JUMP, {operand::operand_encoding::jmp}},     /* 3B */
                      {opcodes::OP_JUMPBACK, {operand::operand_encoding::jmp}}, /* 3C */

                      {opcodes::OP_CMP, {operand::operand_encoding::reg, operand::operand_encoding::reg}},      /* 3D */
                      {opcodes::OP_CMPK, {operand::operand_encoding::reg, operand::operand_encoding::idx}},     /* 3E */
                      {opcodes::OP_CMPN, {operand::operand_encoding::reg, operand::operand_encoding::integer}}, /* 3F */
                      {opcodes::OP_CMPNONE, {operand::operand_encoding::reg}},                                  /* 40 */

                      {opcodes::OP_CMPS, {operand::operand_encoding::reg}},      /* 41 */
                      {opcodes::OP_CMPSK, {operand::operand_encoding::idx}},     /* 42 */
                      {opcodes::OP_CMPSN, {operand::operand_encoding::integer}}, /* 43 */
                      {opcodes::OP_CMPSNONE, {}},                                /* 44 */

                      {opcodes::OP_JUMPIF, {operand::operand_encoding::jmp}},    /* 45 */
                      {opcodes::OP_JUMPIFNOT, {operand::operand_encoding::jmp}}, /* 46 */

                      {opcodes::OP_JUMPIFEQUAL, {operand::operand_encoding::jmp}},        /* 47 */
                      {opcodes::OP_JUMPIFNOTEQUAL, {operand::operand_encoding::jmp}},     /* 48 */
                      {opcodes::OP_JUMPIFLESS, {operand::operand_encoding::jmp}},         /* 49 */
                      {opcodes::OP_JUMPIFLESSEQUAL, {operand::operand_encoding::jmp}},    /* 4A */
                      {opcodes::OP_JUMPIFGREATER, {operand::operand_encoding::jmp}},      /* 4B */
                      {opcodes::OP_JUMPIFGREATEREQUAL, {operand::operand_encoding::jmp}}, /* 4C */

                      {opcodes::OP_SETIF, {operand::operand_encoding::reg}},    /* 4D */
                      {opcodes::OP_SETIFNOT, {operand::operand_encoding::reg}}, /* 4E */

                      {opcodes::OP_SETIFEQUAL, {operand::operand_encoding::reg}},        /* 4F */
                      {opcodes::OP_SETIFNOTEQUAL, {operand::operand_encoding::reg}},     /* 50 */
                      {opcodes::OP_SETIFLESS, {operand::operand_encoding::reg}},         /* 51 */
                      {opcodes::OP_SETIFLESSEQUAL, {operand::operand_encoding::reg}},    /* 52 */
                      {opcodes::OP_SETIFGREATER, {operand::operand_encoding::reg}},      /* 53 */
                      {opcodes::OP_SETIFGREATEREQUAL, {operand::operand_encoding::reg}}, /* 54 */

                      {opcodes::OP_SETUPVALUE, {operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 55 */
                      {opcodes::OP_GETUPVALUE, {operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 56 */

                      {opcodes::OP_DESTROYUPVALUES, {}},                                /* 57 */
                      {opcodes::OP_DESTROYUPVALUESA, {operand::operand_encoding::reg}}, /* 58 */

                      {opcodes::OP_ADDUPVALUE, {operand::operand_encoding::value, operand::operand_encoding::reg}}, /* 59 */
                      {opcodes::OP_RESUME, {}},                                                                     /* 5A */

                      {opcodes::OP_GETVARARGS, {operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 5B */

                      {opcodes::OP_SETTABLE, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}}, /* 5C */
                      {opcodes::OP_GETTABLE, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg}}, /* 5D */

                      {opcodes::OP_SETTABLEN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 5E */
                      {opcodes::OP_GETTABLEN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 5F */

                      {opcodes::OP_SETTABLEK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}}, /* 60 */
                      {opcodes::OP_GETTABLEK, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::idx}}, /* 61 */

                      {opcodes::OP_INITFORLOOPN, {operand::operand_encoding::jmp}},       /* 62 */
                      {opcodes::OP_INITFORLOOPG, {operand::operand_encoding::jmp}},       /* 63 */
                      {opcodes::OP_INITFORLOOPSPECIAL, {operand::operand_encoding::jmp}}, /* 64 */

                      {opcodes::OP_NEWCLOSURE, {operand::operand_encoding::reg, operand::operand_encoding::value}}, /* 65 */
                      {opcodes::OP_REFCLOSURE, {operand::operand_encoding::reg, operand::operand_encoding::idx}},   /* 66 */

                      {opcodes::OP_NEWTABLE, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 67 */
                      {opcodes::OP_REFTABLE, {operand::operand_encoding::reg, operand::operand_encoding::idx}},                                     /* 68 */

                      {opcodes::OP_NEWTABLEA, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 69 */
                      {opcodes::OP_REFTABLEA, {operand::operand_encoding::reg, operand::operand_encoding::idx}},                                     /* 6A */

                      {opcodes::OP_SETLIST, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::value}}, /* 6B */

                      {opcodes::OP_FORLOOPG, {operand::operand_encoding::reg, operand::operand_encoding::value, operand::operand_encoding::jmp}},                               /* 6C */
                      {opcodes::OP_FORLOOPN, {operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::reg, operand::operand_encoding::jmp}}, /* 6D */

                      {opcodes::OP_POPTOP, {}},                               /* 6E */
                      {opcodes::OP_POPARG, {operand::operand_encoding::reg}}, /* 6F */
                  };

                  /* IL discriptor */
                  static optable_descriptor opdiscriptor[] = {

                      {"OP_NOP", "nop", "Does nothing, no operation.", {}},

                      {"OP_LOADBOOL", "loadbool", "Loads a boolean into a destination register.", {"Dest(Register)", "Source(Boolean)"}},
                      {"OP_LOADINT", "loadint", "Loads a integer into a destination register.", {"Dest(Register)", "Source(Integer)"}},
                      {"OP_LOADNONE", "loadnone", "Loads a None into a destination register.", {"Dest(Register)", "Source(None)"}},
                      {"OP_LOADKVAL", "loadkval", "Loads a kvalue into a destination register.", {"Dest(Register)", "Source(Kvalue)"}},
                      {"OP_LOADGLOBAL", "loadglobal", "Loads a global into a destination register.", {"Dest(Register)", "Source(GlobalID)"}},
                      {"OP_LOADATTR", "loadattr", "Loads a attribute into a destination register.", {"Dest(Register)", "Attribute stack identifier(Value)"}},

                      {"OP_GETTABUPVALUE", "gettableupvalue", "Loads a kvalue from structure to the destination register.", {"Dest(Register)", "Source(Kvalue)"}},
                      {"OP_SETGLOBAL", "setglobal", "Set global to global table with kvalue string being source and a register being dest.", {"Source(Register)", "Source(GlobalID)"}},

                      {"OP_SETATTR", "setattr", "Sets attribute on attribute stack for class.", {"Source(Register)", "Attribute stack identifier(Value)", "Attribute index(Value)"}},
                      {"OP_ADDATTR", "addattr", "Adds attribute on attribute stack for class.", {"Source(Register)", "Attribute stack identifier(Value)", "Attribute index(Value)"}},

                      {"OP_MOVE", "move", "Moves register to another register.", {"Dest(Register)", "Source(Register)"}},

                      {"OP_ADD", "add", "Add a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_SUB", "sub", "Suctract a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_MUL", "mul", "Multiply a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_DIV", "div", "Divide a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_MOD", "mod", "Modulo a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_POW", "pow", "Power a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_AND", "and", "And a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_XOR", "xor", "Xor a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_SHL", "shl", "Shift left a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_SHR", "shr", "Shift right a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_IDIV", "idiv", "Floor division a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},
                      {"OP_OR", "or", "Or a register to another register.", {"Dest(Register)", "Source(Register)", "Value(Register)"}},

                      {"OP_ADDK", "addk", "Add a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_SUBK", "subk", "Suctract a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_MULK", "mulk", "Multiply a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_DIVK", "divk", "Divide a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_MODK", "modk", "Modulo a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_POWK", "powk", "Power a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_ANDK", "andk", "And a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_XORK", "xork", "Xor a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_SHLK", "shlk", "Shift left a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_SHRK", "shrk", "Shift right a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_IDIVK", "idivk", "Floor division a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},
                      {"OP_ORK", "ork", "Or a register to a kvalue.", {"Dest(Register)", "Source(Register)", "Value(Kvalue)"}},

                      {"OP_ADDN", "addn", "Add a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_SUBN", "subn", "Suctract a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_MULN", "muln", "Multiply a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_DIVN", "divn", "Divide a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_MODN", "modn", "Modulo a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_POWN", "pown", "Power a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_ANDN", "andn", "And a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_XORN", "xorn", "Xor a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_SHLN", "shln", "Shift left a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_SHRN", "shrn", "Shift right a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_IDIVN", "idivn", "Floor division a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},
                      {"OP_ORN", "orn", "Or a register to a integer.", {"Dest(Register)", "Source(Register)", "Value(Integer)"}},

                      {"OP_LEN", "len", "Unary table length with register source.", {"Dest(Register)", "Source(Register)"}},
                      {"OP_NOT", "not", "Unary not with register source.", {"Dest(Register)", "Source(Register)"}},
                      {"OP_MINUS", "minus", "Unary minus with register source.", {"Dest(Register)", "Source(Register)"}},
                      {"OP_BITNOT", "bitnot", "Unary bit not with register source.", {"Dest(Register)", "Source(Register)"}},
                      {"OP_PLUS", "plus", "Unary plus with register source.", {"Dest(Register)", "Source(Register)"}},

                      {"OP_CCALL", "ccall", "C function and closure call with __cdecl calling convention, arguements and returns are on the stack.", {"Dest(Register)", "Arguement count(Val)", "Return count(Val)"}},
                      {"OP_VCALL", "vcall", "Virtual function call (clears virtual function arguement stack once executed).", {"Dest(Register)", "Virtual function(Virtual_Function)", "Arguement count(Val)", "Return count(Val)"}},

                      {"OP_VPUSH", "vpush", "Push arguement to virtual function stack.", {"Dest(Register)", "Virtual function(Virtual_Function)", "Arguement count(Val)", "Return count(Val)"}},

                      {"OP_SELF", "self", "Loads function in table to a register.", {"Dest(Register)", "Source(Register)", "Kvalue(Kvalue)"}},
                      {"OP_RETURN", "return", "Return values from a function.", {"Start(Register)", "End(Register)"}},

                      {"OP_CONCAT", "concat", "Concat string.", {"Dest(Register)", "Start(Register)", "End(Register)"}},

                      {"OP_JUMP", "jump", "Jump forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPBACK", "jumpback", "Jump back to an address.", {"Jump(Address)"}},

                      {"OP_CMP", "cmp", "Compare register with another register in compare flag.", {"Compare(Register)", "Compare(Register)"}},
                      {"OP_CMPK", "cmps", "Compare register with an kvalue in compare flag.", {"Compare(Register)", "Compare(Kvalue)"}},
                      {"OP_CMPN", "cmpn", "Compare register with an integer in compare flag.", {"Compare(Register)", "Compare(Integer)"}},
                      {"OP_CMPNONE", "cmpnone", "Compare register in compare flag.", {"Compare(Register)"}},

                      {"OP_CMPS", "cmps", "Compare register in compare flag.", {"Compare(Register)"}},
                      {"OP_CMPKS", "cmpsk", "Compare kvalue in compare flag.", {"Compare(Kvalue)"}},
                      {"OP_CMPNS", "cmpsn", "Compare integer in compare flag.", {"Compare(Integer)"}},
                      {"OP_CMPNONES", "cmpsnone", "Compare none in compare flag.", {}},

                      {"OP_JUMPIF", "jumpif", "Jump (if) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFNOT", "jumpifnot", "Jump (if not) compare with compare flag, forward to an address.", {"Jump(Address)"}},

                      {"OP_JUMPIFEQUAL", "jumpifequal", "Jump (==) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFNOTEQUAL", "jumpifnotequal", "Jump (!=) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFLESS", "jumpifless", "Jump (<) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFLESSEQUAL", "jumpiflessequal", "Jump (<=) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFGREATER", "jumpifgreater", "Jump (>) compare with compare flag, forward to an address.", {"Jump(Address)"}},
                      {"OP_JUMPIFGREATEREQUAL", "jumpifgreaterequal", "Jump (>=) compare with compare flag, forward to an address.", {"Jump(Address)"}},

                      {"OP_SETIF", "setif", "Set (if) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFNOT", "setifnot", "Set (if not) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},

                      {"OP_SETIFEQUAL", "setifequal", "Set (==) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFNOTEQUAL", "setifnotequal", "Set (!=) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFLESS", "setifless", "Set (<) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFLESSEQUAL", "setiflessequal", "Set (<=) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFGREATER", "setifgreater", "Set (>) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},
                      {"OP_SETIFGREATEREQUAL", "setifgreaterequal", "Set (>=) compare with compare flag, set compare outcome to a register.", {"Dest(Register)"}},

                      {"OP_SETUPVALUE", "setupvalue", "Set upvalue.", {"Source(Register)", "Upvalue IDX(Upvalue)"}},
                      {"OP_GETUPVALUE", "getupvalue", "Get upvalue.", {"Dest(Register)", "Upvalue IDX(Upvalue)"}},
                  };

#pragma endregion

            } // namespace arch

      } // namespace il

} // namespace luramas

#endif