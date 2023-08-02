#pragma once
#include "../../il/il.hpp"
#include <functional>

namespace luramas {

      namespace il {

            namespace lifter {

                  namespace resolver {

                        template <typename datastruct_t /* Original datastruct pointer i.e. Python not Python* for template. */>
                        class resolver_manager {

                            public:
                              /* Resolver callback function follows format (resolver_manager&) returns void. */
                              using resolver_callback = void (*)(resolver_manager<datastruct_t> &pm);

                              /* Set buffer and ptr */
                              resolver_manager(std::shared_ptr<luramas::il::ilang> &buffer, datastruct_t *ptr) {
                                    this->il = buffer;
                                    this->ptr = ptr;
                                    return;
                              }

                              /* Add callback */
                              void add(const resolver_callback &callback) {
                                    this->resolver_callbacks.emplace_back(callback);
                                    return;
                              }

                              /* Remove callback */
                              void remove(const resolver_callback &callback) {
                                    const auto it = std::find_if(this->resolver_callbacks.begin(), this->resolver_callbacks.end(), [&](const auto &cb) {
                                          return cb == callback;
                                    });
                                    if (it != this->resolver_callbacks.end()) {
                                          this->resolver_callbacks.erase(it);
                                    }
                                    return;
                              }

                              /* Append disassembly with index for resolving disassembly. */
                              void append(const std::size_t idx, const std::shared_ptr<luramas::il::disassembly> &disassembly) {
                                    this->resolver_dism.emplace_back(std::make_pair(idx + this->sync_idx++, disassembly));
                                    return;
                              }

                              /* Call callbacks */
                              template <bool clear = true /* Clears callback vector after use.*/>
                              void run() {

                                    /* Call operand callbacks */
                                    for (const auto &callback : this->resolver_callbacks)
                                          callback(*this);

                                    /* Append */
                                    for (const auto &i : this->resolver_dism)
                                          this->il->append_dis(i.second, i.first);

                                    /* Clear operand callbacks */
                                    if (clear) {
                                          this->resolver_callbacks.clear();
                                    }

                                    this->il->commit_dis();
                                    this->sync_idx = 1u;
                                    this->resolver_dism.clear();
                                    return;
                              }

                              std::shared_ptr<luramas::il::ilang> il = nullptr;
                              datastruct_t *ptr;

                            private:
                              std::size_t sync_idx = 1u;                                                                                                  /* Syncs index for parser call backs. */
                              std::vector<resolver_callback> resolver_callbacks;                                                                          /* Resolver callbacks */
                              std::vector<std::pair<std::size_t /* Index */, std::shared_ptr<luramas::il::disassembly> /* disassembly */>> resolver_dism; /* disassembly from callbacks to be emblaced in IL buffer. */
                        };

                  } // namespace resolver

            } // namespace lifter

      } // namespace il

} // namespace luramas