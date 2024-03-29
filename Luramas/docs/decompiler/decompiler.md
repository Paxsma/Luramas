# Decompiler Documentation

## Decompiler Config

Configs for the decompiler can be found for the following:
* **Defaults**: **luramas::lifter::lifter_config**
* **Formatting**: **luramas::ir::data::format::format**
* **Optimizations**: **luramas::ir::data::optimizations::optimize**

## Decompiler Flow

For the decompiler it generates various levels of abstraction to make decompilations easier and decent, as the order: 
* **Bytecode**
* **Disassembly**
* **Lifter**(too an Intermidiante Language(IL)) 
* (OPTIONAL) **Disassembly**
* **Lexer**
* Abstract Syntax Tree(**AST**)
* **Lifter**(too an Intermidiante Representation(IR)) 
* **Optimizations**
* **Formatting**
* **Code Generation**
* (OPTIONAL)**Loss** (repeats Bytecode -> AST)

## Decompiler Test Code

File to put source code in to test decompiler can be found in [test/](../../test)

### Decompile Target Examples

Decompilation of different targets can be found in [examples/](Luramas/examples/)

### Decompile Target Source Code

To decompile source to different langueges there files can be found here:
* **Lua**: [examples/lua/compile_lua.lua](examples/lua/compile_lua.lua)
* **LuaJit**: [examples/luajit/compile_luajit.lua](examples/luajit/compile_luajit.lua)
* **LuaU**: [examples/luau/compile_luau.lua](examples/luau/compile_lua.lua)
* **Python**: [examples/python/compile_py.py](examples/python/compile_py.py)
**ALL FILES MUST BE UTF-8**



