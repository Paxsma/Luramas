/*
* framework/color.hpp
* 
* Debugging Color
* 10/7/2023
* 
* Adds color to debugging to make it easier to decide what went wrong or not.
*/
#pragma once

#ifndef luramas_debug_color
#define luramas_debug_color

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#pragma region font_color

#define luramas_color_fontcolor_red "\033[31"
#define luramas_color_fontcolor_green "\033[32"
#define luramas_color_fontcolor_yellow "\033[33"
#define luramas_color_fontcolor_brightblue "\033[94"
#define luramas_color_fontcolor_brightcyan "\033[96"
#define luramas_color_fontcolor_magenta "\033[95"
#define luramas_color_fontcolor_white "\033[0"
#define luramas_color_fontcolor_nothing ""

#pragma endregion Font color

#pragma region bg_color

#define luramas_color_background_black "m"
#define luramas_color_background_nothing ""

#pragma endregion Background color

#define luramas_color_print(font_color, background_color, cstr) std::printf(font_color background_color "%s" luramas_color_fontcolor_white luramas_color_background_black, cstr)

#endif

#endif