/*
* framework/debug.hpp
* 
* Debugging
* 10/7/2023
* 
* Provides fast and simple debugability if needed to functions.
* Allows users to debug control flow and fix issues if needed.
*/
#pragma once

#ifndef luramas_debug
#define luramas_debug

#include "color.hpp"

#define debug_functions true /* Enable debug functions */

/* Used mostly in highly voilatile regions. */
#define universal_debug true /* Enables universal debug good for debugging. */
#if universal_debug

/* 1st arg can be str, va_list has too be cstr. */
#if defined(_WIN32) || defined(_WIN64)

/* Generic */
#define debug_line(str, ...)                                                                                                                                                                                 \
      {                                                                                                                                                                                                      \
            std::printf(luramas_color_fontcolor_brightcyan luramas_color_background_black "[" debug_name " %d](%s) " luramas_color_fontcolor_yellow luramas_color_background_black, __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                                                                                                                                                   \
            std::printf(luramas_color_fontcolor_white luramas_color_background_black "\n");                                                                                                                  \
      };

/* Undesirable result */
#define debug_warning(str, ...)                                                                                                                                                                           \
      {                                                                                                                                                                                                   \
            std::printf(luramas_color_fontcolor_brightcyan luramas_color_background_black "[" debug_name " %d](%s) " luramas_color_fontcolor_red luramas_color_background_black, __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                                                                                                                                                \
            std::printf(luramas_color_fontcolor_white luramas_color_background_black "\n");                                                                                                               \
      };

/* Desirable result */
#define debug_success(str, ...)                                                                                                                                                                             \
      {                                                                                                                                                                                                     \
            std::printf(luramas_color_fontcolor_brightcyan luramas_color_background_black "[" debug_name " %d](%s) " luramas_color_fontcolor_green luramas_color_background_black, __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                                                                                                                                                  \
            std::printf(luramas_color_fontcolor_white luramas_color_background_black "\n");                                                                                                                 \
      };

/* Important change */
#define debug_result(str, ...)                                                                                                                                                                                \
      {                                                                                                                                                                                                       \
            std::printf(luramas_color_fontcolor_brightcyan luramas_color_background_black "[" debug_name " %d](%s) " luramas_color_fontcolor_magenta luramas_color_background_black, __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                                                                                                                                                    \
            std::printf(luramas_color_fontcolor_white luramas_color_background_black "\n");                                                                                                                   \
      };

#define debug_init(str) std::printf(luramas_color_fontcolor_green luramas_color_background_black "[" debug_name "] START" luramas_color_fontcolor_white luramas_color_background_black "\n")
#define debug_close(str) std::printf(luramas_color_fontcolor_green luramas_color_background_black "[" debug_name "] CLOSE" luramas_color_fontcolor_white luramas_color_background_black "\n")

#else

#define debug_line(str, ...)                                                 \
      {                                                                      \
            std::printf("[" debug_name " %d](%s) ", __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                   \
            std::printf("\n");                                               \
      };

#define debug_warning(str, ...)                                              \
      {                                                                      \
            std::printf("[" debug_name " %d](%s) ", __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                   \
            std::printf("\n");                                               \
      };

#define debug_success(str, ...)                                              \
      {                                                                      \
            std::printf("[" debug_name " %d](%s) ", __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                   \
            std::printf("\n");                                               \
      };

#define debug_result(str, ...)                                               \
      {                                                                      \
            std::printf("[" debug_name " %d](%s) ", __LINE__, __FUNCTION__); \
            std::printf(str, __VA_ARGS__);                                   \
            std::printf("\n");                                               \
      };

#define debug_init(str) std::printf("[" debug_name "] START\n")
#define debug_close(str) std::printf("[" debug_name "] CLOSE\n")

#endif

#else

#define debug_init(str, ...)
#define debug_line(str, ...)
#define debug_warning(str, ...)
#define debug_success(str, ...)
#define debug_result(str, ...)
#define debug_close(str, ...)

#endif

#endif