/*
* framework/ast/debug.hpp
* 
* AST Debugging
* 10/7/2023
* 
* AST debugging macros.
*/
#pragma once

#ifndef luramas_ast_debug
#define luramas_ast_debug

#include "../debug.hpp"

#pragma region ast_debug

#define DEBUG_AST true      /* Must be true for DEBUG_AST_DISPLAY_DATA too work. */
#define DEBUG_AST_NODE true /* Must be true for DEBUG_IR_LIFTER_OPERANDS or debug_functions or "DEBUG_AST_DISPLAY_DATA" too work. */

#pragma endregion

#pragma region display_debug

#define DEBUG_AST_DISPLAY_PASS_THROUGH true        /* Prints pass throughs for ast. */
#define DEBUG_AST_DISPLAY_PASS_THROUGH_BLOCKS true /* Prints pass throughs for blocks. */
#define DEBUG_AST_DISPLAY_WARNINGS false           /* Prints warnings. */

#if DEBUG_AST_NODE
#define DEBUG_AST_DISPLAY_DATA true /* Prints tree and closure data after ast. */
#endif

#pragma endregion

#pragma region ast_func

#define DEBUG_AST_LOCVAR_REGISTERS false /* Bases ALL locvar detection on register stack(can be inaccurate). */

#pragma endregion

#endif