/*
* framework/loss/loss.hpp
* 
* Loss
* 10/7/2023
* 
* Calculates loss of IL against original.
*/
#pragma once

#ifndef luramas_loss
#define luramas_loss

#include "../ast/ast/ast.hpp"

namespace luramas {

      namespace loss {

            /* Attempts too calculate loss against decompiled compiled assembly and original assembly (Not 100% accurate compiler changes can alter it **Assumes compilers are the same**) )*/
            double loss(const std::shared_ptr<luramas::ast::ast> &main_ast, const std::shared_ptr<luramas::ast::ast> &decompiled);

      } // namespace loss

} // namespace luramas

#endif