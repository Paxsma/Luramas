#include "../framework/target/target.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#pragma region version

#if luramas_target == luramas_target_luau_v3

#include "luau/V3/example.hpp"

#endif

#if luramas_target == luramas_target_lua_53_6

#include "lua/Lua_5.3.6/example.hpp"

#endif

#pragma endregion

/* Throw any string as argument to compile */
std::string decompile(const std::string &code) {

      /* lifter config */
      auto config = std::make_shared<luramas::ir::lifter::lifter_config>();                /* Config */
      auto format = std::make_shared<luramas::ir::data::format::format>();                 /* Format */
      auto optimizations = std::make_shared<luramas::ir::data::optimizations::optimize>(); /* Optimizations */

      return luramas::decompile(code, config, format, optimizations); /* Return decompilation. */
}

std::int32_t main() {

      /* Read from compile_luau.lua */
      std::stringstream code;
      std::ifstream file("C:\\Users\\Mellon\\Documents\\Projects\\Luramas\\Luramas\\examples\\luau\\compile_luau.lua");

      if (file.is_open()) {
            std::string line = "";
            while (std::getline(file, line)) {
                  code << line << std::endl;
            }
            file.close();
      }

      /* Compile. */
      std::cout << decompile(code.str()) << std::endl;
      std::cin.get();
      return 0;
}