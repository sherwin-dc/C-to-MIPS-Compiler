#include "codegen.h"
/* a:t = op c:t
  {2, #1, f, ?, ?, -, #4, f} */
void op_unary_minus(const statement_3AC &line) {

}
void op_load(const statement_3AC &line) {
  // if (line.at=="d") {
  //   aout << "li.d $f4, " << line.c << endl;
  //   store_var_in_memory(line.a, line.at, "$f4");

  // } else if (line.at=="f") {
  //   aout << "li.s $f4, " << line.c << endl;
  //   store_var_in_memory(line.a, line.at, "$f4"); 

  // } else { // unsigned; and signed int, char
  //   aout << "li $t0, " << line.c << endl;
  //   store_var_in_memory(line.a, line.at, "$t0");
  // }
  string data_label;
  if (line.ct=="d") {
    data_label = new_data_label("double");
    data_section.emplace_back(data_label+": .double "+line.c);
    if (!fngen::global_mode) {
      aout << "l.d $f4, " << data_label << endl;
      store_var_in_memory(line.a, line.at, "$f4");
    }

  } else if (line.ct=="f") {
    data_label = new_data_label("float");
    data_section.emplace_back(data_label+": .float "+line.c);
    if (!fngen::global_mode) {
      aout << "l.s $f4, " << data_label << endl;
      store_var_in_memory(line.a, line.at, "$f4"); 
    }

  } else if (line.ct=="c") {
    data_label = new_data_label("char");
    data_section.emplace_back(data_label+": .byte "+line.c);
    if (!fngen::global_mode) {
      aout << "LB $t0, " << data_label << endl;
      store_var_in_memory(line.a, line.at, "$t0"); 
    }

  } else if (line.ct=="g") {
    data_label = new_data_label("str");
    data_section.emplace_back(data_label+": .asciiz "+line.c);
    if (!fngen::global_mode) {
      fngen::global_var[make_pair(line.a, line.at)] = data_label; // store as global
    }
  } else { // unsigned; and signed int, char
    data_label = new_data_label("word");
    data_section.emplace_back(data_label+": .word "+line.c);
    if (!fngen::global_mode) {
      aout << "LW $t0, " << data_label << endl;
      store_var_in_memory(line.a, line.at, "$t0"); 
    }
  }

  if (fngen::global_mode) {
    fngen::global_var[make_pair(line.a, line.at)] = data_label;
  }

}
void op_dereference(const statement_3AC &line) {
  
  // if dereferencing a pointer to array, instead just associate
  if ( /*(char_is_digit(line.at[line.at.size()-1]))  && */ (line.ct[line.ct.size()-1]=='*') &&  (char_is_digit(line.ct[line.ct.size()-2])) ) {
    op_associate(line);
    if (fngen::var_to_pointer.count(make_pair(line.c, line.ct))) {
      fngen::var_to_pointer[make_pair(line.a, line.at)] = fngen::var_to_pointer[make_pair(line.c, line.ct)];
    }
    return;

  // Also if dereferencing anything to get an array, also associate
  } 
  else if (char_is_digit(line.at[line.at.size()-1])) {
    op_associate(line);
    if (fngen::var_to_pointer.count(make_pair(line.c, line.ct))) {
      fngen::var_to_pointer[make_pair(line.a, line.at)] = fngen::var_to_pointer[make_pair(line.c, line.ct)];
    }
    return;

  }

  //load address into register
  load_var_from_memory(line.c, line.ct, "$t0");
  // get value at address
  if (line.at=="c") {
    aout << "LB $t1, 0($t0)" << endl;
  } else if (line.at=="d") {
    aout << "LW $t1, 0($t0)" << endl;
    aout << "LW $t2, 4($t0)" << endl;
  } else {
    aout << "LW $t1, 0($t0)" << endl;
  }
  
  // store this value on stack
  store_var_in_memory(line.a, line.at, "$t1");
  // store the new var in the map
  fngen::var_to_pointer[make_pair(line.a, line.at)] = make_pair(line.c, line.ct);


}
void op_address_of(const statement_3AC &line) {

  // if taking address of an array, instead just associate
  if (char_is_digit(line.ct[line.ct.size()-1])) {
    op_associate(line);
    return;
  }

  // if we want the address of something that was dereferenced from an address, just give the address
  if (fngen::var_to_pointer.count(make_pair(line.c, line.ct))) {
    pair<string, string> address_var = fngen::var_to_pointer[make_pair(line.c, line.ct)];
    statement_3AC s = {2, line.a, line.at, "?", "?", "cp", address_var.first, address_var.second};
    op_copy(s);
    return;
    
  }

  if (fngen::global_mode) {
    string c_label = fngen::global_var[make_pair(line.c, line.ct)];
    string a_label = new_data_label("Array");
    data_section.emplace_back(a_label + ": .word " + c_label);
    fngen::global_var[make_pair(line.a, line.at)] = a_label;
    return;
  }
  if (fngen::global_var.count(make_pair(line.c, line.ct))) {
    string label = fngen::global_var[make_pair(line.c, line.ct)];
    aout << "la $t0, " << label << endl;
  } else {
  // get the position of c:t on the stack
  int stack_pos = fngen::stack_var[make_pair(line.c, line.ct)];
  aout << "move $t0, $s0" << endl;
  aout << "ADDIU $t0, $t0, " << stack_pos << endl;
  }
  store_var_in_memory(line.a, line.at, "$t0");

}
void op_copy(const statement_3AC &line) {
  string type = line.at;
  if ((type.size()==1) || type[type.size()-1]=='*') { // all types excluding structs and arrays
    load_var_from_memory(line.c, line.ct, "$t0");
    store_var_in_memory(line.a, line.at, "$t0");
  } else {
    // TODO
    cerr << "In function " << __PRETTY_FUNCTION__ << " types not implemented: " << line.a << ":" << line.at << endl;
  }
  
}

void op_associate(const statement_3AC &line) {
  if (fngen::global_mode) {
    fngen::global_var[make_pair(line.a, line.at)] = fngen::global_var[make_pair(line.c, line.ct)];
  } else {
    fngen::stack_var[make_pair(line.a, line.at)] = fngen::stack_var[make_pair(line.c, line.ct)];
    

  }
}