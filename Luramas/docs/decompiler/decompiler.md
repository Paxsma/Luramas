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

### Decompile Lua

To decompile lua code just too [compile_me.lua](Luramas/compile_me.lua) **MUST BE UTF-8** where code in it will get compiled and decompiled and you can compare results.

### Decompile Bytecode

If you have some bytecode that has already been compiled just create a char point for it and pass it through luau_load get proto and make ast from it with config. Just refer to main.cpp.

### Decompile Target Examples

Decompilation of different targets can be found in [examples/](Luramas/examples/)