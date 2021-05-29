# 3AC representation

## Overview

Each 3AC line would be converted into MIPS assembly
Every (non-global) variable would be assigned a memory location on the stack relative to sp, which the later part of the compiler would keep track of
During every operation, the required variables would be loaded from memory into registers, operation performed on registers, and any value changed would be put back in memory



### 3 operands

format `a:t = b:t op c:t`

### 2 operand

format `a:t = op b:t`

### Types

:t is `:s` signed, `:u` unsigned, `:f` float, `:c` char or `:d` double
pointer is `:s*`, `:u*`, `:f*`, `:d*` or `:s**`, `:f*******` etc
array is `:c10`, `:d3` etc. This will be treated in the same way as a pointer. The number is the no of elements in the array
multidimentional array `:d3-4-2` for an array declared as `double x[2][4][3]`
struct is `:rSTRUCT_NAME$`

string is `:g` and is only used for placing string literals in the data section, nothing else. A string returns `:c*` in the parser

```3AC
#1 = ld 8:s
#2:d* = x:r24 + #1:s
#3:d = der #2:d*
```

For eg. indexing `x[5]`, where x is declared as `double x[10]` in c would become

```3AC
#1:s = ld 5:s
#2:s = ld 8:s // sizeof double
#3:s = #1:s * #2:s
#4:c* = x:c10 + #3:s
#5:c = der #4:c*

```


### Identifiers

`a`, `b`, `c` can start with #, eg `#5`, without #, eg `count`. `#0` specially means zero

### Globals

All variables initialised at global scope will be preceeded by a function `#Global`

Eg

```3AC
/* Compiler takes note of names of any variables written to here in case they are used in other parts of the program */
#Global
global_int:s = ld 50:s
#5:u = ld 4:u

```

### Branching

label `@label`

conditional branch `@label = a:t br b:t` where jump to `@label` if `a:t` br `b:t`. For example `@else1 = a:d lt b:d` or `@else1 = a:d ne0`

### Function calls

Param: `param a:t`
Function: `!Fn_name a1:t a2:t ... an:t` ... `return a:t` See below example
Function call: `a:t = call !Fn_name params`

Eg

```3AC
MyFn Myint:s Anotherint:u Afloat:f
a:u = Anotherint:u
b:u = ld 10:u
a:u = b:u + b:u

return a:u

f #1:s #2:u #3:f
param #1:s
param #2:u
param #3:f
b:u = call MyFn 3
return b:u
```

## Supported `op`

### 2AC

- `-`
- `ld` (load const - eg `a:s = ld 5:s`)
- `der` (dereference - eg `a:f = der b:f*`)
- `adr` (address of - eg `a:f* = adr b:f`)
- `cp` (copy)
- `asc` (associate - both variables will have the same place on the stack - used for structs)

### 3AC

- `+`, `-`, `<`
- `*`, `/`, `%`
- `&`, `^`, `|`, `nor`
- `<<l`, `>>l`, `>>r`

## Supported `br`

- `eq`, `ne`
unconditional branch: `@label = #0 eq #0`
