# IR to Code Generation Documentation

IR can generate code respecting various syntaxes of different langeuges.

## Syntax Support

Every datatype, keyword, etc will be avaliable through [common](../../framework/il/code/generation/common) in emitter_ir. 
If the target language does not support a datatype, object, etc provided by the AST it will throw an error.
Each language will have its own emmiter in emmiter_ir but the common is a wrapper for all of them.

## Custom Support

Adding your own syntax can be a bit tricky but understanding the framework can help. 
The common in generation is a wrapper for all of the support syntaxes. 
Thought the output langaege is never garunteed to function even less if you decompile a binary with the original language being different from the target, there can be items missing that the target does not support but the original does.
Each function in the emitter has its own switch to guide what syntax to generate code from.
Adding your own syntax enum in the syntax enum and your own switch for each type will let you add your own target syntax.
Thought each datatype, object, etc is limited to the IL it is still very much possible to support a wide range of different syntaxes for your target.