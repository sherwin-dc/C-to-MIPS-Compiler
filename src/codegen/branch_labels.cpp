#include "codegen.h"
/* @label
  {3, @ELSE4, ?, ?, ?, ?, ?, ?}

  @label = a:t br b:t
  {4, @ELSE4, ?, #3, f, eq, #4, f} */

  // Note: @ changed to _

void place_label(const statement_3AC &line) {
  aout << line.a << ":" << endl;
}
void branch_eq(const statement_3AC &line) {
  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "C.eq.d $f4, $f6" << endl;
    aout << "BC1T " << line.a << endl;
    aout << "nop" << endl;


  } else if (line.at=="f") { // floating point compare ensures that +0 equal to -0
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "C.eq.s $f4, $f6" << endl;
    aout << "BC1T " << line.a << endl;
    aout << "nop" << endl;

  } else { // signed, unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "BEQ $t0, $t1, " << line.a << endl;
    aout << "nop" << endl;

  }

}
void branch_ne(const statement_3AC &line) {

  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "C.eq.d $f4, $f6" << endl;
    aout << "BC1F " << line.a << endl;
    aout << "nop" << endl;


  } else if (line.at=="f") { // floating point compare ensures that +0 equal to -0
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "C.eq.s $f4, $f6" << endl;
    aout << "BC1F " << line.a << endl;
    aout << "nop" << endl;

  } else { // signed, unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "BNE $t0, $t1, " << line.a << endl;
    aout << "nop" << endl;

  }
  
}