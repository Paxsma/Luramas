#pragma once
#include <iterator>
#include <string>
#include <vector>

namespace luramas {

      namespace ir {

            namespace data {

                  /* 
      
                     Basically std::string wrapper.
                     Can be used too mutate a compiled IR string without mutating actual IR.
                     IR mutations will be avaliable thought if needed this just adds an extra layer of safety if needed. 

                  */
                  class refrence_string {

                      public:
                        /* Append marked string. */
                        refrence_string append_mark(const std::string &str) {
                              this->data.emplace_back(std::make_pair(str, true));
                              return *this;
                        }

                        void operator=(const refrence_string rs) {
                              this->data = rs.data;
                              return;
                        }

                        void operator=(const char *const str) {

                              this->data.clear();
                              this->data.emplace_back(std::make_pair(std::string(str), false));

                              return;
                        }

                        void operator=(const std::string &str) {

                              this->data.clear();
                              this->data.emplace_back(std::make_pair(str, false));

                              return;
                        }

                        void operator+=(const refrence_string rs) {

                              if (!rs.data.empty()) {
                                    std::copy(rs.data.begin(), rs.data.end(), std::back_inserter(this->data));
                              }

                              return;
                        }

                        void operator+=(const char *const str) {

                              this->data.emplace_back(std::make_pair(std::string(str), false));
                              return;
                        }

                        void operator+=(const std::string &str) {

                              this->data.emplace_back(std::make_pair(str, false));
                              return;
                        }

                        void push_front(const std::string &str) {

                              this->data.insert(this->data.begin(), std::make_pair(str, false));
                              return;
                        }

                        void push_back(const refrence_string rs) {

                              if (!rs.data.empty()) {
                                    this->data.insert(this->data.end(), rs.data.begin(), rs.data.end());
                              }
                              return;
                        }

                        void push_back(const char *const str) {

                              this->data.emplace_back(std::make_pair(std::string(str), false));
                              return;
                        }

                        void operator()(const std::string &str = {}) {

                              if (!str.empty()) {
                                    this->data.emplace_back(std::make_pair(str, false));
                              }

                              return;
                        }

                        void operator()(const char *const str = NULL) {

                              if (str != NULL) {
                                    this->data.emplace_back(std::make_pair(std::string(str), false));
                              }

                              return;
                        }

                        refrence_string operator+(const refrence_string r) {

                              refrence_string retn("");

                              if (!this->data.empty()) {
                                    std::copy(this->data.begin(), this->data.end(), std::back_inserter(retn.data));
                              }

                              if (!r.data.empty()) {
                                    std::copy(r.data.begin(), r.data.end(), std::back_inserter(retn.data));
                              }

                              return retn;
                        }

                        refrence_string(const std::string &str = "", const bool varg = false) {

                              if (!str.empty()) {
                                    this->data.emplace_back(std::make_pair(str, varg));
                              }

                              return;
                        }

                        refrence_string(const char *const str = NULL, const bool varg = false) {

                              if (str != NULL) {
                                    this->data.emplace_back(std::make_pair(std::string(str), varg));
                              }

                              return;
                        }

                        bool empty() {
                              return this->str().empty();
                        }

                        void clear() {
                              this->data.clear();
                              return;
                        }

                        std::string str() {

                              std::string retn = "";

                              for (const auto &i : this->data)
                                    retn += i.first;

                              return retn;
                        }

                        struct compiled_str {

                              std::string data = "";
                              std::vector<std::pair<std::string /* Name */, std::size_t /* Pos */>> positions;
                        };

                        compiled_str compile() {

                              compiled_str retn;

                              for (const auto &i : this->data) {

                                    if (i.second) {
                                          retn.positions.emplace_back(std::make_pair(i.first, retn.data.length()));
                                    }

                                    retn.data += i.first;
                              }

                              return retn;
                        }

                        refrence_string clone() {

                              refrence_string retn("");

                              for (const auto &i : this->data)
                                    retn.data.emplace_back(std::make_pair(i.first, i.second));

                              return retn;
                        }

                        std::vector<std::pair<std::string /* Data */, bool /*  variable, argument, upvalue?? */>> data;
                  };

            } // namespace data

      } // namespace ir

} // namespace luramas