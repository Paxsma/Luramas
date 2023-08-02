#pragma once
#include "target/target.hpp"

#if luramas_target == luramas_target_lua_53_6

#include "disassembler/lua/Lua_5.3.6/3rdparty/imports.hpp" /* Third party imports */
#include "disassembler/lua/Lua_5.3.6/disassembler.hpp"     /* disassembler */
#include "il/lifter/langs/lua/Lua_5.3.6/Lua_5.3.6.hpp"     /* Intermidiante Language Lifter */

#endif

#if luramas_target == luramas_target_luau_v3

#include "disassembler/luau/V3/3rdparty/imports.hpp" /* Third party imports */
#include "disassembler/luau/V3/disassembler.hpp"     /* disassembler */
#include "il/lifter/langs/luau/V3/Luau-V3.hpp"       /* Intermidiante Language Lifter */

#endif