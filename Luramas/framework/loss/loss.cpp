#include "loss.hpp"
#include "../il/il/il.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

double luramas::loss::loss(const std::shared_ptr<luramas::ast::ast> &main_ast, const std::shared_ptr<luramas::ast::ast> &decompiled) {
      return ((std::fmod(main_ast->total, 100)) - (std::fmod(decompiled->total, 100)));
}
