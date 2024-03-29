# IL Documentation

## Table of Contents

* [Description](#Description)
* [Examples](../../examples)
* [Architecture](architecture.md)
* [Optable](op_table.md)
* [Lifting Code](lifting.md)
* [Structure](../../framework/il/il/il.hpp)
* [Visual representation](#Visual-Representation)
* [Changing Targets](#Changing-Targets)
* [Virtual Functions](virtual_functions.md)
* [Symbols](symbols.md)


## Description

Intermidiant Language (**IL**) tanslates (**lifts**) various interpreted languages bytecode to a higher level bytecode language that can be understood easier.
The IL Bytecode format has its own opcodes, operands, kinds, etc.
The IL Bytecode format is a hybrid from vinella **Lua** and **PythonVM** but most of the keywords are focused on **Lua**.
While the architecture is focused mostly on **PythonVM** and **Lua** that does not mean that other languages can not be translate they mostly can but depending on how
different the language is from **PythonVM** or **Lua** some stuff may not be able to translate cleanly thought a lot of support like metadata is still there and can
still be done.

## Visual Representation

Too disassemble IL too a visual representation of this use this code:
```cpp
    /* Iterates through il code, disassembles it, and prints it. */
    for (const auto &code : il->il) {
          std::cout << code->disassemble<true /* The template is false by default but it includes instruction hint if it is true. */>() << std::endl; 
    }
```

## Changing Targets

The IL is target oriented which means you can only lift one target at a time per compilation. 
It is set

