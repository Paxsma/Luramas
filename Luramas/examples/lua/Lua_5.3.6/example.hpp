#pragma once
#include <memory>
#include <string>

#if luramas_target == luramas_target_lua_53_6

#pragma region framework_imports

#include "../../../framework/ast/ast/ast.hpp"                                 /* Abstract Syntax Tree */
#include "../../../framework/il/code/generation/common/generation_syntax.hpp" /* Syntax Emission */
#include "../../../framework/il/il/il.hpp"                                    /* Intermidiante Language */
#include "../../../framework/ir/features/formatting/smart_formatting.hpp"     /* Smart Formatting */
#include "../../../framework/ir/ir.hpp"                                       /* Intermidiant Representation */
#include "../../../framework/ir/lifter/lifter/lifter.hpp"                     /* Intermidiant Representation Lifter */
#include "../../../framework/loss/loss.hpp"                                   /* Loss Function */
#include "../../../framework/target_imports.hpp"                              /* Imports for target */

#pragma endregion

namespace luramas {

      std::string decompile(const std::string &code, const std::shared_ptr<ir::lifter::lifter_config> &config, std::shared_ptr<luramas::ir::data::format::format> &format, std::shared_ptr<luramas::ir::data::optimizations::optimize> &optimizations);

}

#endif