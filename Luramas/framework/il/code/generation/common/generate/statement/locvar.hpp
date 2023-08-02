/*
* framework/il/code/generation/common/generate/statement/assignment.hpp
* 
* IL Code Generation Local Variable Statements
* 10/7/2023
* 
* IL code generation local variable statements.
*/
#pragma once

#ifndef luramas_code_generation_common_statement_locvar
#define luramas_code_generation_common_statement_locvar

#include "../generate.hpp"
#include "../supported.hpp"

/* Syntax specific does not matter here therefore it is not used. */

namespace luramas {

      namespace emitter_ir {

            namespace common {

                  namespace locvar {

                        /* Makes new locvar alphabetical name string. */
                        __inline std::string make_locvar_alphabetical_name(const std::string &prefix, const std::size_t suffix, const bool suffix_chars) {

                              const auto suffix_str = std::to_string(suffix);

                              std::string dest = prefix;

                              if (suffix_chars) {

                                    for (const auto c : suffix_str)
                                          switch (c) {

                                                case '0': {
                                                      dest += 'a';
                                                      break;
                                                }

                                                case '1': {
                                                      dest += 'b';
                                                      break;
                                                }

                                                case '2': {
                                                      dest += 'c';
                                                      break;
                                                }

                                                case '3': {
                                                      dest += 'd';
                                                      break;
                                                }

                                                case '4': {
                                                      dest += 'e';
                                                      break;
                                                }

                                                case '5': {
                                                      dest += 'f';
                                                      break;
                                                }
                                                case '6': {
                                                      dest += 'g';
                                                      break;
                                                }
                                                case '7': {
                                                      dest += 'h';
                                                      break;
                                                }

                                                case '8': {
                                                      dest += 'i';
                                                      break;
                                                }

                                                case '9': {
                                                      dest += 'n';
                                                      break;
                                                }

                                                default: {
                                                      throw std::runtime_error("Something went wrong with locvar name emitter suffix.");
                                                }
                                          }

                              } else {
                                    dest += suffix_str;
                              }

                              return dest;
                        }

                        /* Emits new locvar alphabetical name string. */
                        __inline void emit_locvar_alphabetical_name(std::string &dest, const std::string &prefix, const std::size_t suffix, const bool suffix_chars) {

                              const auto suffix_str = std::to_string(suffix);

                              dest = prefix;

                              if (suffix_chars) {

                                    for (const auto c : suffix_str)
                                          switch (c) {

                                                case '0': {
                                                      dest += 'a';
                                                      break;
                                                }

                                                case '1': {
                                                      dest += 'b';
                                                      break;
                                                }

                                                case '2': {
                                                      dest += 'c';
                                                      break;
                                                }

                                                case '3': {
                                                      dest += 'd';
                                                      break;
                                                }

                                                case '4': {
                                                      dest += 'e';
                                                      break;
                                                }

                                                case '5': {
                                                      dest += 'f';
                                                      break;
                                                }

                                                case '6': {
                                                      dest += 'g';
                                                      break;
                                                }
                                                case '7': {
                                                      dest += 'h';
                                                      break;
                                                }

                                                case '8': {
                                                      dest += 'i';
                                                      break;
                                                }

                                                case '9': {
                                                      dest += 'n';
                                                      break;
                                                }

                                                default: {
                                                      throw std::runtime_error("Something went wrong with locvar name emitter suffix.");
                                                }
                                          }

                              } else {
                                    dest += suffix_str;
                              }

                              return;
                        }

                  } // namespace locvar

            } // namespace common

      } // namespace emitter_ir

} // namespace luramas

#endif