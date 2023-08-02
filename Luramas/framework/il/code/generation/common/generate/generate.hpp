/*
* framework/il/code/generation/common/generate/generate.hpp
* 
* IL Code Generation Headers
* 10/7/2023
* 
* IL code generation header files.
*/
#pragma once

#ifndef luramas_code_generation_common_generate
#define luramas_code_generation_common_generate

#include "../../../../../il/il/il.hpp"
#include "../../../../../ir/data/data.hpp"
#include "../generation_syntax.hpp"
#include <iostream>

/*

    Data

*/
#include "statement/arith.hpp"
#include "statement/assignment.hpp"
#include "statement/comment.hpp"
#include "statement/locvar.hpp"

#include "line/line.hpp"

#include "expression/arith.hpp"
#include "expression/call.hpp"
#include "expression/concat.hpp"
#include "expression/datatype.hpp"
#include "expression/logical.hpp"
#include "expression/selfcall.hpp"
#include "expression/table.hpp"
#include "expression/unary.hpp"

#include "constant/branch.hpp"
#include "constant/for.hpp"
#include "constant/function.hpp"
#include "constant/loop.hpp"
#include "constant/return.hpp"

#include "langkeywords.hpp"
#include "support.hpp"

#endif