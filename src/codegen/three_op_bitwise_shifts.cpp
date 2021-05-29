#include "codegen.h"
/* a:t = b:t op c:t
   {1, #1, f, #3, f, +, #4, f} */
void op_and(const statement_3AC &line) {
   //bitwise operations only work on integer operands
   load_var_from_memory(line.b, line.bt, "$t0");
   load_var_from_memory(line.c, line.ct, "$t1");
   aout << "AND $t2, $t0, $t1" << endl;
   store_var_in_memory(line.a, line.at, "$t2");
}
void op_xor(const statement_3AC &line) {
   load_var_from_memory(line.b, line.bt, "$t0");
   load_var_from_memory(line.c, line.ct, "$t1");
   aout << "XOR $t2, $t0, $t1" << endl;
   store_var_in_memory(line.a, line.at, "$t2");
}
void op_or(const statement_3AC &line) {
   load_var_from_memory(line.b, line.bt, "$t0");
   load_var_from_memory(line.c, line.ct, "$t1");
   aout << "OR $t2, $t0, $t1" << endl;
   store_var_in_memory(line.a, line.at, "$t2");
}
void op_nor(const statement_3AC &line) {
   load_var_from_memory(line.b, line.bt, "$t0");
   load_var_from_memory(line.c, line.ct, "$t1");
   aout << "NOR $t2, $t0, $t1" << endl;
   store_var_in_memory(line.a, line.at, "$t2");
}
void op_left_shift(const statement_3AC &line) {
   //shifts only work on integer opeands
   load_var_from_memory(line.b, line.bt, "$t0");
   load_var_from_memory(line.c, line.ct, "$t1");
   aout << "SLL $t2, $t0, $t1" << endl;
   store_var_in_memory(line.a, line.at, "$t2");
}

void op_right_shift(const statement_3AC &line) {
   if ((line.at=="s") || (line.at=="c")) {
      load_var_from_memory(line.b, line.bt, "$t0");
      load_var_from_memory(line.c, line.ct, "$t1");
      aout << "SRA $t2, $t0, $t1" << endl;
      store_var_in_memory(line.a, line.at, "$t2");
   } else { //unsigned int
      load_var_from_memory(line.b, line.bt, "$t0");
      load_var_from_memory(line.c, line.ct, "$t1");
      aout << "SRL $t2, $t0, $t1" << endl;
      store_var_in_memory(line.a, line.at, "$t2");
   }
   
}