#pragma once
#include "../../../../../../il/il/il.hpp"
#include "../../../../../../ir/data/data.hpp"
#include "../langkeywords.hpp"
#include <iostream>

/*

    Data

*/
#include "statement/arith.hpp"
#include "statement/assignment.hpp"
#include "statement/comment.hpp"

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

#include "../support.hpp"