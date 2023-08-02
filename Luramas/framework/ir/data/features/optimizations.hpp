#pragma once
#include <cstdint>
#include <vector>

namespace luramas {

      namespace ir {

            namespace data {

                  namespace optimizations {

                        /* 
      
                 Raw output without optimizations is too reflect how the code actually looks in the original script format. Using any optimizations 
                 can and will lead too inaccuracys on the script side. This can result in the final code being buggy or non functioning. Use everything
                 at your own risk!!!
           
            */
                        struct optimize {

                              bool disabled = false; /* All optimizations are disabled. */

                              struct var {

                                    std::uint32_t inline_var = 3u; /* If inlining is possible on a variable and variable gets used more or equal too this amount it will get inlined. (0 for nothing). **CAN CAUSE UNEXPECTED PROBLEMS FOR EXECUTED OUTPUT CODE** */

                              } variable;

                              struct comment {

                                    bool comment = true; /* Uses non expandable comment when not nessary. this->indent.collapse.comment.disabled must be false */

                              } comment;

                              struct semicolon {

                                    bool disable = false; /* Removes all semicolons. */

                                    struct constant {

                                          bool semicolon_until = true;   /* Semicolon after until e.g. until(??); */
                                          bool semicolon_repeat = false; /* Semicolon after repeat e.g. repeat; */
                                          bool semicolon_end = false;    /* Semicolon after end e.g. end; */
                                          bool semicolon_break = true;   /* Semicolon after break e.g. break; */
                                          bool semicolon_return = true;  /* Semicolon after return e.g. return; */

                                    } constant;

                              } semicolon;

                              struct assignment {

                                    bool disable_arith_assignment = false; /* Disables arith assignment e.g. a1 += a2; -> a1 = a1 + a2; */

                              } assignment;

                              struct naming {

                                    bool name_assignment = true; /* Variables can be renamed based on initial assignment. (See "naming.cpp" for schemantics) */

                                    /*
                          
                            *  Arguments/Variables will be renamed based on control flow. 
                               - Not perfect, can be inaccurate.
                               - Takes a logical assumption based on numerous hueristics about name.
                               - Will ignore any varibles that got assigned by initial assignment.

                        */
                                    bool name_controlflow = true;
                              };

                              struct compare {

                                    /*
                      
                        Includes parenthesis for compare e.g. (foo == bar) | foo == bar 
                      
                       */
                                    bool parenthesis = true;

                              } compare;

                              struct return_ {

                                    bool useless_return = true; /* Doesn't emit useless return if it is not needed. */

                              } return_;

                              struct variables {

                                    /* 
                  
                             **MAY CAUSE CERTAIN ISSUES WITH SCRIPT IF USED INAPPROPRIATELY**
                  
                             * Inlining is based on rules so you can have certain rules on how you want something specific too be inlined. 
                             * If it gets set in one scope then used outside of it without getting overridden it will not get inlined. 
                             
                             Note. Inlining will be ignore if the target variable is an upvalue, as of right now there is no way too change this. 

                       */
                                    struct inlining {

                                          /* Can be set with specific refrences count for each rule. */
                                          struct inline_rule {

                                                /* At least one of these must be a value 0 means ignore. If there both zero this rule will be ignore. **If you want any just use 1 for refrence count** */
                                                std::size_t refrence_count = 0u;       /* Amount of times it needs too be refrenced (or more) too be inlined, ">=" */
                                                std::size_t refrence_exact_count = 0u; /* Exact number of times it needs too be refrenced too be inlined, "=="  (Overides "refrence_count" if not 0) */

                                                bool ignore_call = false;      /* If the variable is just a call it will not get inlined. */
                                                bool ignore_any_call = false;  /* If the variable is contains a call it will not get inlined. */
                                                bool inline_constants = false; /* Inline constants. */
                                          };

                                          std::vector<inline_rule> inlining_rules;

                                    } inlining;

                                    /* 
                            0: Disabled
                            1: Destroys all dead variables. 
                            2: Destroys all dead variables expect if it is a call its return will get ignore. 
                        */
                                    std::uint8_t clean_dead_variables = 0u;

                              } variables;
                        };

                  } // namespace optimizations

            } // namespace data

      } // namespace ir

} // namespace luramas