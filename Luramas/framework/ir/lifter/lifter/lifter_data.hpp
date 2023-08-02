#pragma once
#include <cstdint>
#include <string>
#include <unordered_set>

/* Global lifter data */
namespace luramas {

      namespace ir {

            namespace lifter {

                  /* lifter config */
                  struct lifter_config {

                        /* Prefixs. (DEFAULT) */
                        std::string iterator_prefix = "i_";
                        std::string variable_prefix = "var_";
                        std::string loop_variable_prefix = "k_";
                        std::string loop_variable_prefix_2 = "v_";
                        std::string arg_prefix = "arg_";
                        std::string upvalue_prefix = "up_";
                        std::string function_prefix = "func_";
                        std::string vararg_prefix = "vararg_";

                        /* Suffix */

                        /* Turns suffix digits into english characters. **Not applicable to smart names.** */
                        bool upvalue_suffix_char = true;   /* Emits character suffix too upvalue.  */
                        bool arg_suffix_char = true;       /* Emits character suffix too arguement.  */
                        bool function_suffix_char = true;  /* Emits character suffix too function name.  */
                        bool iteration_suffix_char = true; /* Emits character suffix too iteration variable.  */
                        bool var_suffix_char = true;       /* Emits character suffix too variable.  */
                        bool vararg_suffix_char = true;    /* Emits character suffix too variable.  */

                        /* Comment */
                        struct comment {

                              bool header = false; /* Includes header per closure and main along with extra stuff like time and full bytecode. */

                              /* Original disassembly. */
                              struct original {

                                    bool address = false;          /* Includes address perline. */
                                    bool bytecode = false;         /* Includes bytecode perline. */
                                    bool disassembly = false;      /* Includes disassembly perline. */
                                    bool disassembly_hint = false; /* Includes disassembly hint perline. */
                                    std::string seperator = " | "; /* Seperates comment data for address, bytecode, disassembly, disassembly_hint. */

                              } original;

                              /* Intermidiant language disassembly. */
                              struct IL {

                                    bool address = false;          /* Includes address perline. */
                                    bool bytecode = false;         /* Includes bytecode perline. */
                                    bool disassembly = false;      /* Includes disassembly perline. */
                                    bool disassembly_hint = false; /* Includes disassembly hint perline. */
                                    std::string seperator = " | "; /* Seperates comment data for address, bytecode, disassembly, disassembly_hint. */

                              } il;

                        } comment;

                        /* Misc */
                        bool smart_variable = false;      /* "Smart" variable names. **Ignores prefix(unless unkown) but keeps suffix as integer** (Won't do anything if suffixs chars config are enabled) */
                        bool smart_variable_name = false; /* Same as smart variable but both must be true for suffix names. */

                        /* Calculates loss of original and current decompilation. **Ignores "disable_comments" in format.** */
                        bool post_loss = false;

                        bool unique() {
                              std::unordered_set<std::string> unique_prefixes;
                              unique_prefixes.insert(this->iterator_prefix);
                              unique_prefixes.insert(this->variable_prefix);
                              unique_prefixes.insert(this->loop_variable_prefix);
                              unique_prefixes.insert(this->loop_variable_prefix_2);
                              unique_prefixes.insert(this->arg_prefix);
                              unique_prefixes.insert(this->upvalue_prefix);
                              unique_prefixes.insert(this->function_prefix);
                              return unique_prefixes.size() == 7u /* Amount of prefixes. */;
                        }
                  };

            } // namespace lifter

      } // namespace ir

} // namespace luramas