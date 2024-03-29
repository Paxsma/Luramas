/*
* framework/il/lexer/lexer.hpp
* 
* IL Lexer
* 10/7/2023
* 
* Lexes IL.
*/
#pragma once

#ifndef luramas_il_lexer
#define luramas_il_lexer

#include "../../il/il/il.hpp"
#include <functional>
#include <stdexcept>

namespace luramas {

      namespace il {

            namespace lexer {

                  /* Operand kinds */
                  enum class operand_kinds : std::uint8_t {
                        reg,            /* [REGISTER] Generic register not a dest. */
                        dest,           /* [REGISTER] Reg dest. */
                        source,         /* [REGISTER] Reg source. */
                        integer,        /* [DOUBLE] Double type source. */
                        value,          /* [INTEGER] Intptr type source. */
                        compare,        /* [REGISTER] Compare register. */
                        jmpaddr,        /* [ADDRESS] Jump location. */
                        closure,        /* [ClOSURE INDEX] Closure index for closure stack. */
                        kvalue,         /* [KVALUE INDEX] Kvalue index for kvalue stack. */
                        kvalue_embeded, /* [KVALUE] Pre-processes kvalue source. */
                        upvalue,        /* [UPVALUE INDEX] Upvalue index for upvalue stack. */
                        table_idx,      /* [INTEGER] Table idx. */
                        table_reg,      /* [REGISTER] Table idx reg. */
                        upvalue_kind    /* [UPVALUE KIND] Upvalue capture kind. */
                  };

                  /* Instruction kinds */
                  enum class inst_kinds : std::uint8_t {
                        nothing,          /* Nop and Break opcodes. */
                        arith,            /* Arith opcodes add, sub, and, or, etc. */
                        branch_condition, /* Condition branching opcodes. */
                        branch,           /* Branching opcodes. */
                        compare,          /* Instruction writes too cmp flag. */
                        compare_dest,     /* Instruction writes compares cmp flag and writes too dest. */
                        load,             /* Load opcodes. */
                        fastcall,         /* Fastcall opcodes. */
                        for_,             /* For loop opcodes. */
                        unary,            /* Minus, Not, and Lenght. */
                        table_get,        /* Table get. */
                        table_set,        /* Table set. */
                        set_table,        /* Set table opcodes. (Not init) */
                        upvalue_gs,       /* Table get/set. */
                        expression,       /* Everything else. */
                        ccall,            /* C function call */
                        vcall,            /* Virtual function Call */
                        set_global,       /* Set global */
                        get_global,       /* Get global */
                        set_attr,         /* Set attribute */
                        new_table,        /* Creates/Refrence table */
                        capture,          /* Capture */
                        return_,          /* Return */
                        special_flag      /* Special instruction that porpose is too modify a flag. */
                  };

                  /* Lexeme */
                  struct lexeme {

                        luramas::il::lexer::inst_kinds kind = inst_kinds::expression; /* Instruction kind */
                        std::vector<luramas::il::lexer::operand_kinds> operands;      /* Operand kinds */
                        std::shared_ptr<luramas::il::disassembly> disassembly;        /* IL disassembly */

                        /* Returns if opcode starts a scope. */
                        bool scope_start() {
                              return (this->kind == luramas::il::lexer::inst_kinds::for_ || this->kind == luramas::il::lexer::inst_kinds::branch_condition || this->kind == luramas::il::lexer::inst_kinds::branch);
                        }

#pragma region operand_kinds

                        /* Returns if given kind exists. */
                        template <luramas::il::lexer::operand_kinds kind>
                        bool has_operand_kind() {
                              return std::find(this->operands.begin(), this->operands.end(), kind) != this->operands.end();
                        }

                        /* Gets first operand target if there is multiple. */
                        template <luramas::il::lexer::operand_kinds kind>
                        std::vector<std::shared_ptr<luramas::il::arch::operand::operand>> operand_kind() {

                              std::vector<std::shared_ptr<luramas::il::arch::operand::operand>> retn;

                              for (auto i = 0u; i < this->operands.size(); ++i)
                                    if (this->operands[i] == kind) {
                                          retn.emplace_back(this->disassembly->operands[i]);
                                    }

                              return retn;
                        }

                        /* Counts the occurance of a kind. */
                        template <luramas::il::lexer::operand_kinds kind>
                        std::size_t count_operand_kind() {
                              return std::count(this->operands.begin(), this->operands.end(), kind);
                        }

                        /* Calls callback for every operand kind that exists in lexeme with operand and kind being arguements with no return.  */
                        template <luramas::il::lexer::operand_kinds kind>
                        void operand_kind_callback(std::function<void(const std::shared_ptr<luramas::il::arch::operand::operand> &operand, const luramas::il::lexer::operand_kinds tt)> &callback) {

                              const auto operands = this->operand_kind<kind>();
                              for (const auto &i : operands)
                                    callback(i, kind);

                              return;
                        }

#pragma endregion Operand Kinds
                  };

                  std::shared_ptr<lexeme> lexer(std::shared_ptr<luramas::il::disassembly> &disassembly);

            } // namespace lexer

      } // namespace il

} // namespace luramas

#endif