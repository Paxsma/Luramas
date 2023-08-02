#pragma once
#include "../../ir.hpp"

namespace luramas {

      namespace smart_formatting {

            /* Smart format */
            void smart_format(const luramas::emitter_ir::syntax::emitter_syntax syn, luramas::ir::lines &buffer, std::shared_ptr<luramas::ir::data::format::format> &format);

      } // namespace smart_formatting

} // namespace luramas