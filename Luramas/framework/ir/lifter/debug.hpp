#pragma once

#define DEBUG_IR_LIFTER_OPERANDS false /* Needs to be false to allow comment data. */
#if DEBUG_IR_LIFTER_OPERANDS
#define DEBUG_IR_LIFTER_OPERANDS_PRINT_OVERRIDE false /* Allows you too override print in debug and just comment it. True for comment False for print */
#define DEBUG_IR_LIFTER_OPERANDS_PRINT_IL true        /* This must be true too disassemble IR. */
#endif

#define DEBUG_IR_LIFTER_WARNINGS false
