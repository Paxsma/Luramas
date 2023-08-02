/*
* framework/il/il/il.hpp
* 
* IL
* 10/7/2023
* 
* IL data, disassembly, etc.
*/
#pragma once

#ifndef luramas_il
#define luramas_il

#include "../../target/target.hpp"
#include "architecture/architecture.hpp"
#include "tools/disassembler.hpp"
#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <ranges>
#include <unordered_map>

#pragma region version

#if luramas_target == luramas_target_luau_v3

#pragma region thirdparty
#include "../../disassembler/luau/V3/3rdparty/imports.hpp"
#pragma endregion

#pragma region disassembler
#include "../../disassembler/luau/V3/disassembler.hpp"
#pragma endregion

#endif

#if luramas_target == luramas_target_lua_53_6

#pragma region thirdparty
#include "../../disassembler/lua/Lua_5.3.6/3rdparty/imports.hpp"
#pragma endregion

#pragma region disassembler
#include "../../disassembler/lua/Lua_5.3.6/disassembler.hpp"
#pragma endregion

#endif

#pragma endregion

namespace luramas {

      namespace il {

            /* IL disassembly */
            struct disassembly {

                  luramas::il::arch::opcodes op = luramas::il::arch::opcodes::OP_NOP;
                  std::size_t len = 0u;
                  std::uintptr_t addr = 0u;
                  float total = 0.0f;
                  std::vector<std::shared_ptr<luramas::il::arch::operand::operand>> operands;

                  /* All dissasembly formats are the same. **Its not always garunteed too be a valid pointer.** */
#pragma region dis
#pragma region luau

#if luramas_target == luramas_target_luau_v3
                  std::shared_ptr<luau_v3_disassembler::disassembly> data = nullptr;
#endif

#pragma endregion

#pragma region lua

#if luramas_target == luramas_target_lua_53_6
                  std::shared_ptr<lua_53_6_disassembler::disassembly> data = nullptr;
#endif

#pragma endregion
#pragma endregion

                  std::shared_ptr<disassembly> ref = nullptr; /* Refrence to the disassembly which one of the operands refrence i.e. jump/memory refrence. */

                  struct debug {

                        /* Requires dest as one of the operands. */
                        bool is_var = false;       /* Is variable? */
                        std::string var_name = ""; /* Variable name if one exists. */

                  } debug;

                  /* Returns disassembly of IL instruction. */
                  template <bool include_hint = false /* Includes mnemonic hint */,
                      bool case_mnemonic = true /* Uppercase mnemonic? */,
                      bool include_address = true /* Includes address */,
                      bool include_mnemonic = true /* Includes mnemonic */,
                      bool include_operands = true /* Includes mnemonic */>
                  std::string disassemble(const char *const comment = NULL) {

                        std::string retn = "";

                        /* Address */
                        if (include_address) {
                              retn += std::to_string(this->addr) + " ";
                        }

                        /* Mnemonic */
                        if (include_mnemonic) {
                              std::string mnemonic = luramas::il::disassembler::mnemonic_string(this->op); /* Mnemonic string */

                              /* Uppercase? */
                              if (case_mnemonic) {
                                    std::ranges::transform(mnemonic, mnemonic.begin(), [](const char c) { return std::toupper(c); });
                              }

                              /* Append */
                              retn += mnemonic + " ";
                        }

                        /* Operands */
                        if (include_operands) {
                              const auto op_count = this->operands.size();
                              for (auto i = 0u; i < op_count; ++i) {
                                    retn += luramas::il::disassembler::operand_string(this->operands[i]);
                                    retn += ((i + 1u) == op_count) ? " " : ", ";
                              }
                        }

                        /* Comment */
                        if (comment != NULL) {
                              retn += std::string("; ") + comment;
                        }

                        /* Hint */
                        if (include_hint) {
                              retn += ((comment == NULL) ? std::string("; ") : std::string("  ")) + luramas::il::disassembler::mnemonic_hint_string(this->op);
                        }

                        return retn;
                  }

#if defined(_WIN32) || defined(_WIN64)

#include "../../color.hpp"

                  /* Prints disassembly in color of IL instruction. */
                  template <bool include_hint = false /* Includes mnemonic hint */,
                      bool case_mnemonic = true /* Uppercase mnemonic? */,
                      bool include_address = true /* Includes address */,
                      bool include_mnemonic = true /* Includes mnemonic */,
                      bool include_operands = true /* Includes mnemonic */>
                  void debug_disassemble(const char *const comment = NULL) {

                        /* Address */
                        if (include_address) {
                              luramas_color_print(luramas_color_fontcolor_green, luramas_color_background_black, std::string(std::to_string(this->addr) + " ").c_str());
                        }

                        /* Mnemonic */
                        if (include_mnemonic) {

                              std::string mnemonic = luramas::il::disassembler::mnemonic_string(this->op);
                              if (case_mnemonic) {
                                    std::ranges::transform(mnemonic, mnemonic.begin(), [](const char c) { return std::toupper(c); });
                              }

                              luramas_color_print(luramas_color_fontcolor_brightblue, luramas_color_background_black, std::string(mnemonic + " ").c_str());
                        }

                        /* Operands */
                        if (include_operands) {
                              const auto op_count = this->operands.size();
                              for (auto i = 0u; i < op_count; ++i) {
                                    luramas_color_print(luramas_color_fontcolor_yellow, luramas_color_background_black, luramas::il::disassembler::operand_string(this->operands[i]).c_str());
                                    luramas_color_print(luramas_color_fontcolor_nothing, luramas_color_background_nothing, ((i + 1u) == op_count) ? " " : ", ");
                              }
                        }

                        /* Comment */
                        if (comment != NULL) {
                              luramas_color_print(luramas_color_fontcolor_magenta, luramas_color_background_black, std::string(std::string("; ") + comment).c_str());
                        }

                        /* Hint */
                        if (include_hint) {
                              luramas_color_print(luramas_color_fontcolor_magenta, luramas_color_background_black, std::string(((comment == NULL) ? std::string("; ") : std::string("  ")) + luramas::il::disassembler::mnemonic_hint_string(this->op)).c_str());
                        }

                        /* Line end */
                        if (include_address || include_mnemonic || include_operands || include_hint || comment != NULL) {
                              luramas_color_print(luramas_color_fontcolor_nothing, luramas_color_background_nothing, "\n");
                        }

                        return;
                  }

#endif
            };

            /* IL class */
            class ilang {

                public:
                  struct kvalue {

                        luramas::il::arch::kval_kinds type = luramas::il::arch::kval_kinds::none;

                        struct boolean {
                              bool boolean = false;
                        } boolean;

                        struct integer {
                              double integer = 0.0;
                              std::string str = ""; /* Formatted */
                        } integer;

                        struct string {
                              std::string str = "";
                        } string;

                        struct closure {
                              std::size_t id = 0u;
                              std::shared_ptr<luramas::il::ilang> closure;
                        } closure;

                        struct table {
                              std::size_t node_size = 0u;
                              std::size_t array_size = 0u;
                        } table;

                        struct userdata {
                              std::string str = "";
                        } userdata;

                        struct function {
                              std::string str = "";
                        } function;

                        struct thread {
                              std::string str = "";
                        } thread;

                        struct upvalue {
                              std::string str = "";
                        } upvalue;

                        /* Kvalue string */
                        std::string str() {

                              std::string retn = "";

                              switch (this->type) {

                                    case luramas::il::arch::kval_kinds::none: {
                                          retn = "none";
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::boolean: {
                                          retn = (this->boolean.boolean) ? "true" : "false";
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::integer: {
                                          retn = this->integer.str;
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::string: {
                                          retn = this->string.str;
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::userdata: {
                                          retn = this->userdata.str;
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::table: {
                                          retn = "node_" + std::to_string(this->table.node_size) + ", array_" + std::to_string(this->table.array_size);
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::function: {
                                          retn = this->function.str;
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::closure: {
                                          retn = "closure_" + std::to_string(this->closure.id);
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::thread: {
                                          retn = this->thread.str;
                                          break;
                                    }
                                    case luramas::il::arch::kval_kinds::upvalue: {
                                          retn = this->upvalue.str;
                                          break;
                                    }
                                    default: {
                                          throw std::runtime_error("Unkown IL kvalue type.");
                                    }
                              }

                              return retn;
                        }
                  };

                  std::vector<std::shared_ptr<disassembly>> dis;             /* IL disassembly */
                  std::vector<std::shared_ptr<kvalue>> kval;                 /* IL Kvalues */
                  std::vector<std::shared_ptr<luramas::il::ilang>> closures; /* IL Closures */

                  /* Format: kcode_members(unsigned 64-Bit) | kcode(Kind, Data) | bitcode_instructions(unsigned 64-Bit) | bitcode(Opcode, Operands) */
                  void *kcode = nullptr;                     /* Kind, Data */
                  void *bitcode = nullptr;                   /* Opcode, Operands */
                  std::size_t bitcode_size = 0u;             /* Size of bitcode data. */
                  std::size_t kcode_size = 0u;               /* Size of kcode data. */
                  std::uint64_t kcode_members = 0ull;        /* Amount of kcode members. */
                  std::uint64_t bitcode_instructions = 0ull; /* Amount of bitcode instructions. */

                  ~ilang() {
                        std::free(kcode);
                        std::free(bitcode);
                        return;
                  }

                  /* Returns assembled bytecode for interpreter. */
                  void *bytecode(std::size_t &size) const {

                        const auto retn = std::malloc(sizeof(std::uint64_t) + kcode_size + sizeof(std::uint64_t) + bitcode_size);
                        auto ptr = static_cast<std::int8_t *>(retn);

                        /* Kcode */
                        *std::bit_cast<std::uint64_t *>(ptr) = this->kcode_members;
                        std::memcpy(ptr += sizeof(std::uint64_t), this->kcode, this->kcode_size);

                        /* Bitcode */
                        *std::bit_cast<std::uint64_t *>(ptr += this->kcode_size) = this->bitcode_instructions;
                        std::memcpy(ptr += sizeof(std::uint64_t), this->bitcode, this->bitcode_size);

                        size = ptr - static_cast<std::int8_t *>(retn);

                        return retn;
                  }

                  /* Appends disassembly to a private vector. */
                  void append_dis(const std::shared_ptr<disassembly> &d) {
                        this->p_dis.emplace_back(d);
                        return;
                  }

                  /* Inserts disassembly to an index on a private vector. (Fills to index if it does not exist) */
                  void append_dis(const std::shared_ptr<disassembly> &d, const std::size_t idx) {
                        if (this->p_dis.size() != idx) {
                              for (auto i = 0u; i < idx; ++i)
                                    this->p_dis.emplace_back(nullptr);
                        }
                        this->p_dis.insert(this->p_dis.begin() + idx, d);
                        return;
                  }

                  /* Validates, corrects, and then commits uncommited disassembly. */
                  void commit_dis(const bool resolve = true /* Resolve?? */) {

                        /* Remove any nullptrs that may be in vector. */
                        if (std::find(this->p_dis.begin(), this->p_dis.end(), nullptr) != this->p_dis.end()) {
                              this->p_dis.erase(std::remove(this->p_dis.begin(), this->p_dis.end(), nullptr), this->p_dis.end());
                        }

                        /* Validate */
                        this->validate(this->validate_operands());

                        /* Only resolve if it is not the first commit, nothing really to resolve before that. */
                        if (resolve) {
                              this->resolve_addresses();
                              this->resolve_jumps();
                        }

                        /* Commit */
                        this->dis = this->p_dis;

                        return;
                  }

                  /* Returns private disassembly if needed to be mutated. */
                  std::vector<std::shared_ptr<disassembly>> mutate_dis() const {
                        return this->p_dis;
                  }

                  /* Remove disassembly from a private vector. */
                  void remove_dis(const std::shared_ptr<disassembly> &dis) {
                        this->p_dis.erase(std::remove(this->p_dis.begin(), this->p_dis.end(), dis), this->p_dis.end());
                        return;
                  }

                  /* Remove disassembly by index from a private vector. */
                  void remove_dis(const std::size_t idx) {
                        this->p_dis.erase(this->p_dis.begin() + idx);
                        return;
                  }

#pragma region metadata

                  struct metadata {

                  } metadata;

#pragma endregion

#pragma region visitors

                  /* Visits addr of IL disassembly. */
                  std::shared_ptr<disassembly> visit_addr(const std::uintptr_t addr) const {

                        for (const auto &i : this->dis)
                              if (i->addr == addr) {
                                    return i;
                              }

                        throw std::runtime_error("Could not find address for IL visit.");
                  }

                  /* Visits instructions that refrences addr. */
                  std::vector<std::shared_ptr<disassembly>> visit_ref(const std::uintptr_t addr) const {

                        std::vector<std::shared_ptr<disassembly>> result;

                        for (const auto &i : this->dis)
                              if (i->ref != nullptr && i->ref->addr == addr) {
                                    result.emplace_back(i);
                              }

                        return result;
                  }

#pragma endregion

                  /* Validate IL makes sures there is no error in IL disassembly, returns error if any. */
                  const char *const validate() {

                        auto valid = this->validate_operands();
                        if (valid != NULL) {
                              return valid;
                        }

                        if ((valid = this->validate_instructions()) != NULL) {
                              return valid;
                        }

                        return NULL;
                  }

                private:
                  std::vector<std::shared_ptr<disassembly>> p_dis;

#pragma region validate

                  /* Throws error if IL is not valid. */
                  void validate(const char *const valid);

                  /* Validates operands for IL. */
                  const char *const validate_operands();

                  /* Validates instructions for IL. */
                  const char *const validate_instructions();

#pragma endregion Internal validation for IL does not need a manager.

#pragma region resolver

                  /* Resolves mutated or missing addresses. */
                  void resolve_addresses();

                  /* Resolves jump location and refrence. */
                  void resolve_jumps();

#pragma endregion Internal resolver for IL does not need a manager.
            };

            /* Dissassemble IL bytecode to a buffer. */
            void disassemble_il_bytecode(void *il_bytecode, std::shared_ptr<luramas::il::ilang> &buffer);

      } // namespace il

} // namespace luramas

#endif