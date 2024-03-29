# IL Lifting Documentation

## Table of Contents

* [Description](#Description)
* [Examples](#Examples)
* [Support](#Support)
* [Parser Managers Description](#Parser-Managers-Description)
* [Parser Managers Init](#Parser-Managers-Init)
* [Parser Managers Usage](#Parser-Managers-Usage)
* [Resolver Managers Description](#Resolver-Managers-Description)
* [Resolver Managers Usage](#Resolver-Managers-Usage)
* [Lifting Template](../template/lifter)


## Description

The IL lifter lifts the targeted original bytecode and translates or lifts it to the IL. 
The IL serves as a middle man between a vast variety of different interpreted languages.
Lifting the IL is usally the same throughout a few interpreted languages thought it may be different depending on the language.
Some languages like C# require extra steps compared to languages like lua or python which do not have as much metadata as C#.
This is just an example and all interpreted languages are different or simular to one another but the IL is supposed to generalize and support a lot of the features
interpreted languages may have one way or another. 

## Examples

Examples of different lifters can be found at [framework/il/lifter/](../../framework/il/lifter/)

## Support

It is very difficult to add support for every interpreted languege directly which is why the IL exists.
The IL does not support everything in a language directly instead translating what it is to something simular in the IL so the the IL can understand it better.
An example of a indirect support of something would be pushing to the top of the stack. Some interpreted languages have instructions that push registers to the top of the stack.
There is currently no instruction that pushes a value to the top of the stack. Instead there are load instructions that load to registers or slots that acts like the stack.
Loading a value to a slot that has not already been used and is technically on top of the stack allows you to push values on top of the stack but it requires you to know the whole stack frame. 

## Parser Managers

### Parser Managers Description

The parser manager manages different parser functions reducing the hassle to commit and resolve after every parse.

### Parser Managers Init

Parser inits does not add new or remove any instructions not relative to the original disassembly.
Everything must be mapped correctly addresses, translations, size, etc to make it easier to lift.
The init typically does not resolve any addresses, jumps, etc but it can be specified if needed.

### Parser Managers Usage

Every parse from the parser manager can change and remove instructions making the lifted code more accurate to the targeted language.

## Resolver managers

### Resolver Managers Description

The resolver manager manages different resolver functions reducing the hassle to commit and resolve after every parse.

### Resolver Managers Usage

Every resolver from the resolver manager changes operands or mutate instructions to make a more accurate lift.
