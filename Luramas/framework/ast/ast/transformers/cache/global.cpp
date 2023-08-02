#include "../transformers.hpp"

std::unordered_map<std::shared_ptr<luramas::ast::node> /* Branch */, std::shared_ptr<luramas::ast::node> /* End */> luramas::ast::transformers::global_cache::scopes::cached_ends;

void luramas::ast::transformers::global_cache::clear() {
      luramas::ast::transformers::global_cache::scopes::cached_ends.clear();
      return;
}
