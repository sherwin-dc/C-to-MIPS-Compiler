#include "codegen.h"
/* a:t = b:t op c:t
   {1, #1, f, #3, f, +, #4, f} */
bool is_integral_type(const string type);
int is_pointer_array_type(const string type);

void op_plus(const statement_3AC &line) {

  // check for and perform pointer arithmetic
  int b_sz = is_pointer_array_type(line.bt);
  int c_sz = is_pointer_array_type(line.ct);
  // if either are pointers
  if ( (b_sz && !c_sz) || (!b_sz && c_sz) ) {
    if (b_sz) { //? b is the pointer
      if (is_integral_type(line.ct)) {
        aout << "li $t1, " << b_sz << endl; // load byte value into reg
        load_var_from_memory(line.b, line.bt, "$t4"); //load pointer
        load_var_from_memory(line.c, line.ct, "$t0"); // load integral into register

        if (line.ct=="u") {
          aout << "MULTU $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "ADDU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        } else {
          aout << "MULT $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "ADDU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        }

        store_var_in_memory(line.a, line.at, "$t5");

      } else {
        cerr << "Error in " << __PRETTY_FUNCTION__ << " cannot do " << line.a << ":" << line.at << " = "  \
        << line.b << ":" << line.bt << " + " << line.c << ":" << line.ct << endl;
      }
    } else if (c_sz) { //? c is the pointer
      if (is_integral_type(line.bt)) {
        aout << "li $t1, " << c_sz << endl; // load byte value into reg
        load_var_from_memory(line.c, line.ct, "$t4"); //load pointer
        load_var_from_memory(line.b, line.bt, "$t0"); // load integral into register

        if (line.bt=="u") {
          aout << "MULTU $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "ADDU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        } else {
          aout << "MULT $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "ADDU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        }

        store_var_in_memory(line.a, line.at, "$t5");

      } else {
        cerr << "Error in " << __PRETTY_FUNCTION__ << " cannot do " << line.a << ":" << line.at << " = "  \
        << line.b << ":" << line.bt << " + " << line.c << ":" << line.ct << endl;
      }
    }

    return;
  }

  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "ADD.d $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if (line.at=="f") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6
    aout << "ADD.s $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "ADD $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "ADDU $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
  }

}
void op_minus(const statement_3AC &line) {

  // check for and perform pointer arithmetic
  int b_sz = is_pointer_array_type(line.bt);
  int c_sz = is_pointer_array_type(line.ct);
  // if either are pointers
  if ( (b_sz && !c_sz) || (!b_sz && c_sz) ) {
    if (b_sz) { //? b is the pointer
      if (is_integral_type(line.ct)) {
        aout << "li $t1, " << b_sz << endl; // load byte value into reg
        load_var_from_memory(line.b, line.bt, "$t4"); //load pointer
        load_var_from_memory(line.c, line.ct, "$t0"); // load integral into register

        if (line.ct=="u") {
          aout << "MULTU $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "SUBU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        } else {
          aout << "MULT $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "SUBU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        }

        store_var_in_memory(line.a, line.at, "$t5");

      } else {
        cerr << "Error in " << __PRETTY_FUNCTION__ << " cannot do " << line.a << ":" << line.at << " = "  \
        << line.b << ":" << line.bt << " + " << line.c << ":" << line.ct << endl;
      }
    } else if (c_sz) { //? c is the pointer
      if (is_integral_type(line.bt)) {
        aout << "li $t1, " << c_sz << endl; // load byte value into reg
        load_var_from_memory(line.c, line.ct, "$t4"); //load pointer
        load_var_from_memory(line.b, line.bt, "$t0"); // load integral into register

        if (line.bt=="u") {
          aout << "MULTU $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "SUBU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        } else {
          aout << "MULT $t0, $t1" << endl;
          aout << "MFHI $t2" << endl;
          aout << "MFLO $t3" << endl;
          aout << "SUBU $t5, $t4, $t3 # Perform pointer arithmetic" << endl;
        }

        store_var_in_memory(line.a, line.at, "$t5");

      } else {
        cerr << "Error in " << __PRETTY_FUNCTION__ << " cannot do " << line.a << ":" << line.at << " = "  \
        << line.b << ":" << line.bt << " + " << line.c << ":" << line.ct << endl;
      }
    }

    return;
  } else if ((b_sz) && (b_sz==c_sz)) { // if both are pointers of the same size
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "SUBU $t2, $t0, $t1" << endl;
    aout << "li $t3, " << c_sz << endl; // load byte value into reg
    aout << "DIVU $t2, $t3" << endl;
    aout << "MFLO $t4" << endl;
    store_var_in_memory(line.a, line.at, "$t4");

    return;
  }


  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "SUB.d $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if (line.at=="f") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6
    aout << "SUB.s $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "SUB $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "SUBU $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
  }
}
void op_slt(const statement_3AC &line) {
  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "c.Lt.d $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if (line.at=="f") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6
    aout << "c.Lt.s $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "SLT $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "SLTU $t2, $t0, $t1" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
  }
}
void op_mult(const statement_3AC &line) {
  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "MUL.d $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if (line.at=="f") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6
    aout << "MUL.s $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "MULT $t0, $t1" << endl;
    aout << "MFHI $t2" << endl;
    aout << "MFLO $t3" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
    store_var_in_memory(line.a, line.at, "$t3");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "MULTU $t0, $t1" << endl;
    aout << "MFHI $t2" << endl;
    aout << "MFLO $t3" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
    store_var_in_memory(line.a, line.at, "$t3");
  }
}
void op_divide(const statement_3AC &line) {
  if (line.at=="d") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4 and $f5
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6 and $f7
    aout << "DIV.d $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if (line.at=="f") {
    load_var_from_memory(line.b, line.bt, "$f4"); // will load into $f4
    load_var_from_memory(line.c, line.ct, "$f6"); // will load into $f6
    aout << "DIV.s $f8, $f4, $f6" << endl;
    store_var_in_memory(line.a, line.at, "$f8");

  } else if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "DIV $t0, $t1" << endl;
    aout << "MFLO $t2" << endl;
    store_var_in_memory(line.a, line.at, "$t2");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "DIVU $t0, $t1" << endl;
    aout << "MFLO $t2" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
  }
}
void op_mod(const statement_3AC &line) {
  //modulo operator cannot be used with floating point numbers
  if ((line.at=="s") || (line.at=="c")) {
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "DIV $t0, $t1" << endl;
    aout << "MFHI $t2" << endl;
    store_var_in_memory(line.a, line.at, "$t2");

  } else { // unsigned, pointer, array and struct
    load_var_from_memory(line.b, line.bt, "$t0");
    load_var_from_memory(line.c, line.ct, "$t1");
    aout << "DIVU $t0, $t1" << endl;
    aout << "MFHI $t2" << endl;
    store_var_in_memory(line.a, line.at, "$t2");
  }
}


bool is_integral_type(const string type) {
  if ((type=="s") || (type=="u") || (type=="c")) {
    return true;
  }
  return false;
}

/* Determines if this is a pointer or array,
returns 0 if not or the size in bytes of the type pointed to */
int is_pointer_array_type(const string type) {
  if (type[type.size()-1]=='*') {

    if ((type[type.size()-2]=='*') || (type[type.size()-2]=='s') || (type[type.size()-2]=='u') || (type[type.size()-2]=='f')) {
      return 4;
    } else if ((type[type.size()-2]=='d')) {
      return 8;
    } else if ((type[type.size()-2]=='c')) {
      return 1;
    } else if (type[type.size()-2] == '$') { // pointer to struct
      return 0;
    } else if (char_is_digit(type[type.size()-2])) { // pointer to whole array
      return get_array_whole_size(type.substr(0, type.size()-1));
    }

  } else if (char_is_digit(type[type.size()-1])) { // array
    return get_array_base_size(type);
  }
  return 0;
}