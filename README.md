![Build and Test](https://github.com/sherwin-dc/c-to-mips-compiler/workflows/Build%20and%20Test/badge.svg)     ![Coverage](https://github.com/sherwin-dc/c-to-mips-compiler/blob/badges/coverage-badge.svg)

# C90 to MIPS Compiler

## Overview

This is a compiler created to meet coursework requirements for a [Language Processing Module](https://github.com/LangProc/langproc-2020-cw). This compiler was made using ANTLR4 for lexing/parsing and C++ for processing syntax trees and code generation.  

This compiler takes in preprocessed C90 (also known as C89 or ANSI C) and outputs MIPS1 assembly. The list of all features tested is in the [deliverable](https://github.com/sherwin-dc/c-to-mips-compiler/blob/master/c_compiler.md) and also shown below. For reference, a copy of the ISO C89 specification can be found [here](http://port70.net/~nsz/c/c89/c89-draft.html).  

<details><summary>Full list of tested features</summary>

Basic Features:  

* a file containing just a single function with no arguments
* variables of `int` type
* local variables
* arithmetic and logical expressions
* if-then-else statements
* while loops

Intermediate Features:  

* files containing multiple functions that call each other
* functions that take up to four parameters
* for loops
* arrays declared globally (i.e. outside of any function in your file)
* arrays declared locally (i.e. inside a function)
* reading and writing elements of an array
* recursive function calls
* the `enum` keyword
* `switch` statements
* the `break` and `continue` keywords

Advanced Features:  

* variables of `double`, `float`, `char`, `unsigned`, structs, and pointer types
* calling externally-defined functions (i.e. the file being compiled declares a function, but its definition is provided in a different file that is linked in later on)
* functions that take more than four parameters
* mutually recursive function calls
* locally scoped variable declarations (e.g. a variable that is declared inside the body of a while loop, such as `while(...) { int x = ...; ... }`.
* the `typedef` keyword
* the `sizeof(...)` function (which takes either a type or a variable)
* taking the address of a variable using the `&` operator
* dereferencing a pointer-variable using the `*` operator
* pointer arithmetic
* character literals, including escape sequences like `\n`
* strings (as NULL-terminated character arrays)
* declaration and use of structs

</details>

This compiler addresses **everything** listed except for variable shadowing.

## Example Tests

Along with the specification, 87 sample tests were provided to help test for compiler correctness. These tests, along with a different set of hidden tests, were run to assess the functionality of the compiler. Additional custom tests were also added. These tests can be found in the `compiler_tests/` directory.  

Each test is accompanied by a driver which is compiled with GCC to check the correctness of the compiler. This also means that the compiler needs to conform to the same ABI where applicable, for example when calling functions that take in arguments or the ordering of struct and array elements in memory.  

```
            This Compiler
test.c ---------------------> test.s ---------
                                              \  GNU Assembler
                                                --------------> executable
                  GCC                         /
test_driver.c --------------> test_driver.s --
```

A few of the custom tests created are shown below, all of which this compiler correctly handles. Although these tests could potentially be more advanced than those used to assess the compiler, they serve to demonstrate the features this compiler supports.  

### Duff's Device

A `do...while` loop included inside of and wrapped around switch `case` statements which was used to perform loop unrolling. More information on Duff's Device can be found [here](https://en.wikipedia.org/wiki/Duff%27s_device).
<details><summary>View Example</summary>

The following is a simplified version of Duff's Device.

```C
int f(int count)
{
    int to = 0;
    int n = (count+7) / 8;
    
    switch(count%8)
    {
       case 0: do { to++;
       case 7:      to++;
       case 6:      to++; 
       case 5:      to++;
       case 4:      to++; 
       case 3:      to++; 
       case 2:      to++;
       case 1:      to++;
        } while (--n>0);
    }

    return to;
}
```

This test can be found in `compiler_tests/custom/duffs_device.c`
</details>

### Point Me In The Right Direction

Pointer to pointer types.
<details><summary>View Example</summary>

Note that the C specification states that compilers need to support a minimum of 12 levels of pointers.

```C
float f(float ************a12, float **b2)
{
    /* In this test the main function creates these pointer types
    and ensures thet they were dereferenced correctly through a
    floating point addition */
    return ************a12+**b2;
}
```

This test can be found in `compiler_tests/custom/pointer_to_pointer.c`
</details>

### Rows of Squares

Use of multidimentional arrays.
<details><summary>View Example</summary>

Just as with pointers, in the C specification compilers need to support at least 12-D arrays. Note the variation in indexing the arrays, ie using `3[x]...` instead of `x[3]...`.

```C
/* Function takes in 
pointer to 6-D array */
int f(int (*ptr)[2][1][2][1][1][2]) {

  /* Declare and set the value of
  an element in a 12-D array */
  int x[4][2][1][2][1][4][2][2][1][4][2][3];
   3[x][1][0][1][0][2][0][1][0][2][1][2] = 20;

  /* Retrieve values from the array declared 
  locally and the array initialised by GCC */
  return 1[*ptr][0][1][0][0][1] + x[3][1][0][1][0][2][0][1][0][2][1][2];
}
```

This test can be found in `compiler_tests/custom/multi_array.c`
</details>

### Struct Chain

Use of nested structs, struct initialisation and linked-list.
<details><summary>View Example</summary>

```C
struct x {
  double a;
  char b;
  int c;

  /* Unnamed nested struct */
  struct {
    int d;
    double e;
    struct x * ptr; /* For linked-list */
  } ys;
};

/* Globally initialise struct */
struct x x1 = {2.5, 'Z', 10};

/* Function takes in start of linked-list */
int f(struct x * x3) {

  struct x x2 = {3.5, 'd' /* 100 */, 50};

  /* Retrieve values from the global and local struct */
  int a = x1.c;
  int b = x2.b;

  /* Retrieve value at end of linked-list */
  struct x * p = x3;
  while (p->ys.ptr) {
    p = p->ys.ptr;
  }
  
  int c = p->ys.d;
  return a + b + c;
}
```

This test can be found in `compiler_tests/custom/nested_struct.c`
</details>

### Array-Struct Blitz

Multidimensional arrays of structs, with each struct element containing multidimensional arrays of nested structs.
<details><summary>View Example</summary>

Part of this test is to retrieve a value through a pointer to a 2D array of `struct x`, passed through the main function compiled by GCC. This ensures that this compiler has handled struct packing, padding and alignment correctly, and that it conforms to the ABI.

```C
struct x {
  double a;
  char b;
  int c[3][4];

  /* 2D Array of nested structs */
  struct y {
    int d;
    double e;
  } ys[2][5];
};

/* Global 3D Array of Structs */
struct x x1[2][3][4];

/* Function takes in pointer to 2D array of structs */
int f(struct x (* ptr)[3][2]) { 
  
  
  /* Set the value of an element of a multidimensional
  array wihtin the struct, and then retrieve it*/
  x1[1][0][3].c[2][1] = 3;
  int a = x1[1][0][3].c[2][1];

  /* Retrieve the value of an element of a multidimensional array
  of structs, nested inside of another struct, after being given 
  a pointer to a multidimensional array of such structs that have 
  been initialized by GCC */
  int b = (*ptr)[2][1].ys[1][4].d;

  return a+b;
}
```

This test can be found in `compiler_tests/custom/array_struct_pointer.c`
</details>

## Installing and Running

### Dependencies

The attached Dockerfile and Vagrantfile already contain all the dependencies required.

### Build

The compiler can be built by running the following command.

```
make bin/c_compiler
```

### Compiling Files

The following command can be used to invoke the compiler and produce MIPS assembly given a C source file.

```
bin/c_compiler -S [source-file.c] -o [dest-file.s]
```

## Contributors

This compiler was created by:

[Sherwin da Cruz](https://github.com/sherwin-dc)  
[Xin Yue Foo](https://github.com/xyf19)  
