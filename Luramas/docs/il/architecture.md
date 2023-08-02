# IL Architecture Documentation

## Table of Contents

* [Description](#Description)
* [Registers](#Registers)
* [Calls](#Calls)
* [Call Arguements](#Call-Arguements)
* [Returns](#Returns)
* [Operands](#Operands)
* [Loop Initiators](#Loop-Initiators)
* [Stack based instructions](#Stack-Based-Instructions)
* [Kvalues](#Kvalues)
* [Multret](#Multret)
* [Optable](op_table.md) in op_table.md
* [Virtual Functions](virtual_functions.md) in virtual_functions.md


## Description

The IL architecture is the backbone to the IL. It describes the behavoir and many things about the IL.

## IL Structure

### Registers

Registers are representeded by current stack. Stack structure can be represented as:

```
> Stack
[-----------------------------]
|--General Purpose Registers--] ?? (at least >= 0)
|---------Arguements----------] > 0
|------------Flags------------] < 0 
[-----------------------------]
```

When a register is refrenced it means its slot on the stack.
They can be used interchangably and are used interchangably in the framework but registers are more common.
They mean the same thing in the context of the IL.

### Calls

There is only one call instruction in the IL and that calls a register. 

### Call Arguements

The call instruction takes (?? > arguements) from the stack and adds it too the stack when the function is runned.
The register the instruction is calling always comes before when arguements are placed on the stack.
Example:
```
	0 INITVARARGS ; Initiates varargs.
	1 GETTABUPVALUE r2, bar ; Adds bar too 2nd index in the stack or register 2.
	3 GETTABUPVALUE r1, foo ; Adds foo too 1st index in the stack or register 1.
	4 MOVE r2, r0 ; Moves r0 too r2 too get used in called.
	5 CALL r1, 1, 0  ; Calls r1 (foo) with one arguement (bar).
	6 RETURN r0, 0 ; Exits, returns nothing.
```
(Please note. The concept of using the stack for indexing multiple values at a time also applys too different instructions in the IL, it is not just refrenced here **but** the concept is **relatively the same** for each instruction but **they are not the same format**.)

### Returns

When a 

### Operands

All IL instructions can only have either **one** memory refrence or a jump for one of its operand.
Each instruction can only have one destination operand.
	 
### Loop Initiators

Loop initiators is an instruction that initiates the loop for the first iteration. 
They are **optional** and are **not garunteed too exist for every or any loop in the input code**.

### Stack Based Instructions

Stack based instructions exist in the IL. 
Some of them include call, and concat. 
They both use the stack too index multiple different values in the stack. 
There formats are different and is noted in the IL.

## IL Datatypes

### Kvalues

Kvalues are a lua keyword for constant values. 
When the IL generates its kvalue list whose members cannot be changed by anything, they will remain constant.
**Lua** example:
```lua
   print ("value"); -- "value" would be a kvalue because it is a string that can never change.
```

**Python** example:
```python
   print ("kvalue"); # "kvalue" would be a kvalue because it is a string that can never change.
```

### Upvalues

Upvalues are a lua keyword too describe values up from the code.
These values are simular too captured variables but they can pass multiple different states other then just the register.
**Lua** Example:
```lua
   local upvalue_variable = foo();
   local function bar () 
		print (upvalue_variable); -- upvalue_variable will be an upvalue because it is refrence outside of scope whilst also being inited.
   end
   bar();
```

**Python** example:
```python
   upvalue_variable = foo();
   def bar():
		print(upvalue_variable); # upvalue_variable will be an upvalue because it is refrence outside of scope whilst also being inited.
   bar();
```

### None

"None" is an null datatype. This is different from null because it **does not equal zero**. 
Instead it is its own datatype that is not zero. 
This can be thought of like nullptr in c++ or None in python.

### Multret

Multiple returns(multret) is used to describe something that has multiple returns or assignments. For example:
**Lua** Example:
```lua
   local function bar () 
		return a, b, c, d, e, f, g; -- Returns multiple values.
   end
   print(bar);
   a, b, c = 10, 20, 30; -- Sets multiple values.
```

**Python** example:
```python
   def bar():
    	return a, b, c, d, e, f, g; # Returns multiple values.
   print(bar);
   a, b, c = 10, 20, 30; # Sets multiple values.
```