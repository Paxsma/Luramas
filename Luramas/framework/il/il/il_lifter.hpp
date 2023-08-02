#pragma once
#include "../../target/target.hpp"

#if luramas_target == luramas_target_luau_v3
#include "../lifter/luau/V3/Luau-V3.hpp"
#endif

#if luramas_target == luramas_target_lua_53_6
#include "../lifter/lua/Lua_5.3.6/Lua_5.3.6.hpp"
#endif