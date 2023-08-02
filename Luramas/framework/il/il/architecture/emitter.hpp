/*
* framework/il/il/architecture/emitter.hpp
* 
* IL Emitter
* 10/7/2023
* 
* Provides tools to emit IL data.
*/
#pragma once

#ifndef luramas_il_emitter
#define luramas_il_emitter

#include "../il.hpp"

namespace luramas {

      namespace il {

            namespace emitter {

                  namespace kval {

                        /* Emits kvalue to IL kcode. */
                        __inline void emit_kvalue(std::shared_ptr<luramas::il::ilang> &il, const luramas::il::arch::kval_kinds kind, const luramas::il::ilang::kvalue kvalue) {

                              ++il->kcode_members;

                              il->kcode = (il->kcode == nullptr) ? std::malloc(sizeof(std::uint8_t)) : std::realloc(il->kcode, il->kcode_size + sizeof(std::uint8_t));
                              *std::bit_cast<std::uint8_t *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = std::int32_t(kind);
                              ++il->kcode_size;

                              switch (kind) {

                                    /* Emit boolean */
                                    case luramas::il::arch::kval_kinds::boolean: {
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + sizeof(std::uint8_t));
                                          *std::bit_cast<std::uint8_t *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = kvalue.boolean.boolean;
                                          ++il->kcode_size;
                                          break;
                                    }

                                    /* Emit closure ID */
                                    case luramas::il::arch::kval_kinds::closure: {
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + sizeof(std::uint32_t));
                                          *std::bit_cast<std::uint32_t *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = kvalue.closure.id;
                                          il->kcode_size += sizeof(std::uint32_t);
                                          break;
                                    }

                                    case luramas::il::arch::kval_kinds::function: {

                                          /* Emit size */
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + sizeof(std::uint32_t));
                                          *std::bit_cast<std::uint32_t *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = kvalue.function.str.length();
                                          il->kcode_size += sizeof(std::uint32_t);

                                          /* Emit string */
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + kvalue.function.str.length());
                                          std::memcpy(static_cast<std::int8_t *>(il->kcode) + il->kcode_size, kvalue.function.str.c_str(), kvalue.function.str.length());
                                          il->kcode_size += kvalue.function.str.length();

                                          break;
                                    }

                                    /* Emit double */
                                    case luramas::il::arch::kval_kinds::integer: {
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + sizeof(double));
                                          *std::bit_cast<double *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = kvalue.integer.integer;
                                          il->kcode_size += sizeof(double);
                                          break;
                                    }

                                    /* Emit string */
                                    case luramas::il::arch::kval_kinds::string: {
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + kvalue.string.str.length());
                                          std::memcpy(static_cast<std::int8_t *>(il->kcode) + il->kcode_size, kvalue.string.str.c_str(), kvalue.function.str.length());
                                          il->kcode_size += kvalue.string.str.length();
                                          break;
                                    }

                                    case luramas::il::arch::kval_kinds::table: {

                                          /* Emit node size */
                                          il->kcode = std::realloc(il->kcode, il->kcode_size + sizeof(std::uint64_t));
                                          *std::bit_cast<std::uint64_t *>(static_cast<std::int8_t *>(il->kcode) + il->kcode_size) = kvalue.table.node_size;
                                          il->kcode_size += sizeof(std::uint64_t);

                                          break;
                                    }

                                    /* None/thread/upvalue emits nothing. */
                                    case luramas::il::arch::kval_kinds::none:
                                    case luramas::il::arch::kval_kinds::thread:
                                    case luramas::il::arch::kval_kinds::upvalue: {
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Unkown kvalue emission kind.");
                                    }
                              }

                              return;
                        }

                  } // namespace kval

                  namespace bitcode {

                        /* Emits operand to IL bitcode. */
                        __inline void emit_encoding(std::shared_ptr<luramas::il::ilang> &il, const luramas::il::arch::operand::operand_encoding enc, const std::int64_t U) {

                              switch (enc) {

                                    case luramas::il::arch::operand::operand_encoding::boolean: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::uint8_t));
                                          *std::bit_cast<std::uint8_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::uint8_t(U);
                                          ++il->bitcode_size;
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::enumeral: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::uint8_t));
                                          *std::bit_cast<std::uint8_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::uint8_t(U);
                                          ++il->bitcode_size;
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::idx: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::uint32_t));
                                          *std::bit_cast<std::uint32_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::uint32_t(U);
                                          il->bitcode_size += sizeof(std::uint32_t);
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::integer: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::int64_t));
                                          *std::bit_cast<double *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = double(U);
                                          il->bitcode_size += sizeof(std::int64_t);
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::jmp: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::int64_t));
                                          *std::bit_cast<std::int64_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::int64_t(U);
                                          il->bitcode_size += sizeof(std::int64_t);
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::reg: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::int16_t));
                                          *std::bit_cast<std::uint16_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::uint16_t(U);
                                          il->bitcode_size += sizeof(std::int16_t);
                                          break;
                                    }

                                    case luramas::il::arch::operand::operand_encoding::value: {
                                          il->bitcode = std::realloc(il->bitcode, il->bitcode_size + sizeof(std::int32_t));
                                          *std::bit_cast<std::int32_t *>(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size) = std::int32_t(U);
                                          il->bitcode_size += sizeof(std::int32_t);
                                          break;
                                    }

                                    default: {
                                          throw std::runtime_error("Unkown operand encoding type.");
                                    }
                              }
                              return;
                        }

                        /* Emits opcode to IL bitcode. */
                        template <luramas::il::arch::opcodes op>
                        void emit_opcode(std::shared_ptr<luramas::il::ilang> &il, const std::int64_t A = 0ll, const std::int64_t B = 0ll, const std::int64_t C = 0ll, const std::int64_t D = 0ll) {

                              /* Emit opcode */
                              il->bitcode = (il->bitcode == nullptr) ? std::malloc(++il->bitcode_size) : std::realloc(il->bitcode, il->bitcode_size++);
                              std::memset(static_cast<std::int8_t *>(il->bitcode) + il->bitcode_size, op, sizeof(std::uint8_t));

                              std::size_t idx = 0u;
                              const std::vector<std::int64_t> enc_stack = {A, B, C, D};

                              const auto opdata = luramas::il::arch::opencodings[std::uint8_t(op)];
                              for (const auto enc : opdata.encodings)
                                    emit_encoding(il, enc, enc_stack[idx++]);

                              ++il->bitcode_instructions;
                              return;
                        }

                  } // namespace bitcode

                  /* Emits operand kind to IL disassembly. */
                  __inline void emit_operand_kind(std::shared_ptr<luramas::il::ilang> &il, const luramas::il::arch::operand::operand_kind kind, const std::uintptr_t addr, const std::shared_ptr<luramas::il::arch::operand::operand> &operand, const std::int64_t U) {

                        operand->type = kind;

                        switch (kind) {

                              case luramas::il::arch::operand::operand_kind::value: {
                                    operand->dis.val = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::reg: {
                                    operand->dis.reg = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::upvalue_kind: {
                                    operand->dis.upvalue_kind = static_cast<luramas::il::arch::operand::upvalue_kind>(U);
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::table_size: {
                                    operand->dis.table_size = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::kval: {
                                    operand->dis.kvalue_idx = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::kval_embeded: {
                                    operand->dis.kvalue_idx = U;
                                    operand->k_value = il->kval[operand->dis.kvalue_idx]->str();
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::closure_index: {
                                    operand->dis.closure_idx = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::upvalue: {
                                    operand->dis.upvalue_idx = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::closure: {
                                    operand->dis.closure_id = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::jmp: {
                                    operand->dis.jmp = U;
                                    operand->ref_addr = addr + U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::boolean: {
                                    operand->dis.boolean = U;
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::integer: {

                                    operand->dis.integer = U;

                                    operand->integer_str = std::to_string(operand->dis.integer);
                                    operand->integer_str.erase(operand->integer_str.find_last_not_of('0') + 1, std::string::npos);
                                    operand->integer_str.erase(operand->integer_str.find_last_not_of('.') + 1, std::string::npos);
                                    break;
                              }

                              case luramas::il::arch::operand::operand_kind::none: {
                                    break;
                              }

                              default: {
                                    throw std::runtime_error("Unkown kind to emit.");
                              }
                        }

                        return;
                  }

                  /* Emits opcode to IL buffer. */
                  template <luramas::il::arch::opcodes op>
                  void emit_opcode(std::shared_ptr<luramas::il::ilang> &il, const std::uintptr_t addr, std::shared_ptr<luramas::il::disassembly> &buffer, const std::int64_t A = 0ll, const std::int64_t B = 0ll, const std::int64_t C = 0ll, const std::int64_t D = 0ll) {

                        std::size_t idx = 0u;
                        const std::vector<std::int64_t> enc_stack = {A, B, C, D};

                        const auto opdata = luramas::il::arch::opkinds[std::uint8_t(op)];

                        for (const auto enc : opdata.kinds) {
                              auto ptr = std::make_shared<luramas::il::arch::operand::operand>();
                              emit_operand_kind(il, enc, addr, ptr, enc_stack[idx++]);
                              buffer->operands.emplace_back(ptr);
                        }

                        buffer->op = opdata.op;
                        buffer->addr = addr;
                        buffer->len = 1u;
                        buffer->total = 0.75f;

                        return;
                  }

            } // namespace emitter

      } // namespace il

} // namespace luramas

#endif