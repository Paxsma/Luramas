#pragma once
#include "../../il/il.hpp"
#include <functional>
#include <optional>

namespace luramas {

      namespace il {

            namespace lifter {

                  namespace parser {

                        /* Remove or add pending disassembly. */
                        enum class action : std::uint8_t {
                              remove,
                              add
                        };

                        /* 
                            Parse data from original bytecode to IL. 
                            Before running parse you need to use the append or remove functions to add or remove any IL, this ensure saftey for the IL.
                        */
                        template <typename dism_t /* Original disassembly vector i.e. disassembly. */>
                        class parser_manager {

                            public:
                              /* Parse callback function follows format (parser_manager&) returns void. */
                              using parse_callback = void (*)(parser_manager<dism_t> &pm);

                              /* Sets il, dism, and calls init commits instructions. */
                              parser_manager(std::shared_ptr<luramas::il::ilang> &buffer, const std::vector<std::shared_ptr<dism_t>> &dism, const parse_callback &init, const bool resolve = true) {
                                    this->il = buffer;
                                    this->dism = dism;
                                    if (init != nullptr) {
                                          init(*this);
                                          buffer->commit_dis(resolve);
                                    }
                                    return;
                              }

                              /* Add parser callback */
                              void add(const parse_callback &callback) {
                                    this->parse_callbacks.emplace_back(callback);
                                    return;
                              }

                              /* Remove parser callback */
                              void remove(const parse_callback &callback) {
                                    const auto it = std::find_if(this->parse_callbacks.begin(), this->parse_callbacks.end(), [&](const auto &cb) {
                                          return cb == callback;
                                    });
                                    if (it != this->parse_callbacks.end()) {
                                          this->parse_callbacks.erase(it);
                                    }
                                    return;
                              }

                              /* Call callbacks */
                              template <bool clear = true /* Clears callback vector after use.*/>
                              void run() {

                                    /* Call callbacks */
                                    for (const auto &callback : this->parse_callbacks)
                                          callback(*this);

                                    /* Append */
                                    for (const auto &i : this->parsing_dism)
                                          if (std::get<action>(i) == action::add) {
                                                this->il->append_dis(std::get<std::shared_ptr<luramas::il::disassembly>>(i), std::get<std::size_t>(i));
                                          } else {
                                                if (std::get<std::shared_ptr<luramas::il::disassembly>>(i) == nullptr) {
                                                      this->il->remove_dis(std::get<std::size_t>(i));
                                                } else {
                                                      this->il->remove_dis(std::get<std::shared_ptr<luramas::il::disassembly>>(i));
                                                }
                                          }

                                    /* Commit */
                                    this->il->commit_dis();

                                    /* Clear */
                                    if (clear) {
                                          this->parse_callbacks.clear();
                                    }

                                    /* Clear for next run */
                                    this->sync_idx = 1u;
                                    this->parsing_dism.clear();
                                    return;
                              }

                              /* Append disassembly with index for parsing disassembly. */
                              void append(const std::size_t idx, const std::shared_ptr<luramas::il::disassembly> &disassembly) {
                                    this->parsing_dism.emplace_back(std::make_tuple(action::add, idx + this->sync_idx++, disassembly));
                                    return;
                              }

                              /* Remove disassembly for parsing disassembly. */
                              void remove(const std::shared_ptr<luramas::il::disassembly> &disassembly) {
                                    --this->sync_idx;
                                    this->parsing_dism.emplace_back(std::make_tuple(action::remove, 0u, disassembly));
                                    return;
                              }

                              /* Remove disassembly with index for parsing disassembly. */
                              void remove(const std::size_t idx) {
                                    this->parsing_dism.emplace_back(std::make_tuple(action::remove, idx + --this->sync_idx, nullptr));
                                    return;
                              }

                              std::vector<std::shared_ptr<dism_t>> dism;        /* Original disassembly vector */
                              std::shared_ptr<luramas::il::ilang> il = nullptr; /* IL buffer */

                            private:
                              std::size_t sync_idx = 1u;                                                                                                                       /* Syncs index for parser call backs. */
                              std::vector<parse_callback> parse_callbacks;                                                                                                     /* Parsing callbacks */
                              std::vector<std::tuple<action /* Action */, std::size_t /* Index */, std::shared_ptr<luramas::il::disassembly> /* disassembly */>> parsing_dism; /* disassembly from callbacks to be emblaced in IL buffer. */
                        };

                  } // namespace parser

            } // namespace lifter

      } // namespace il

} // namespace luramas