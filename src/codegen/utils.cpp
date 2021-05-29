#include "codegen.h"

/**
 * Checks if a var already has a set position in the stack
 * If not allot it a new position, otherwise store it where it needs to go.
 * Format of src_reg is $XN, where X is a single letter and N is a number
 */
void store_global_var(string id, string type, string src_reg);
void load_global_var(string id, string type, string dst_reg);

void store_var_in_memory(string id, string type, string src_reg, int offset /* =0 */) {
  
  pair<string, string> id_type = make_pair(id, type);

  // if global var
  if (fngen::global_var.count(id_type)) {
    store_global_var(id, type, src_reg);
    return;
  }

  // check if var already has a place on the stack
  int stack_pos=0;
  
  if (fngen::stack_var.count(id_type)) {
    stack_pos = fngen::stack_var[id_type];
  } else {
    // assign it a position on the stack


    if (type=="d") {
      if (fngen::next_stack_pos % 8) {fngen::next_stack_pos += 8 - abs(fngen::next_stack_pos % 8);}
      stack_pos=fngen::next_stack_pos;
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      fngen::next_stack_pos += 8;

    } else if ((type=="f") || (type=="u") || (type=="s") || (type.size() > 1) /* pointers and arrays and structs */ ) {
      if (fngen::next_stack_pos % 4) {fngen::next_stack_pos += 4 - abs(fngen::next_stack_pos % 4);}
      stack_pos=fngen::next_stack_pos;
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      // cerr << " variable and type " << id << ":" << type << " stored at " << fngen::next_stack_pos << endl;
      fngen::next_stack_pos += 4;
      
    } else if (type=="c") {
      stack_pos=fngen::next_stack_pos;
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      // cerr << " variable and type " << id << ":" << type << " stored at " << fngen::next_stack_pos << endl;
      fngen::next_stack_pos += 1;
    } else {
      cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << endl;
    }
  }

  int reg_no;
  try {
    reg_no = stoi(src_reg.substr(2, src_reg.size()-2));
  } catch (std::invalid_argument) {
    if (type=="d") {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Non numberred register passed with type double" << endl;
    }
  }

  if (type=="d") {
    if (src_reg[1]=='f') {
      // floating point reg
      aout << "SWC1 " << src_reg << ", " << stack_pos << "($s0)" << endl;
      aout << "SWC1 " << src_reg.substr(0, 2) << ++reg_no << ", " << stack_pos+4 << "($s0)" << " # store " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "SW " << src_reg << ", " << stack_pos << "($s0)" << endl;
      aout << "SW " << src_reg.substr(0, 2) << ++reg_no << ", " << stack_pos+4 << "($s0)" << " # store " << id << ":" << type << endl;
    }
  } else if (type=="f") {
    if (src_reg[1]=='f') {
      // floating point reg
      aout << "SWC1 " << src_reg << ", " << stack_pos << "($s0)" << " # store " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "SW " << src_reg << ", " << stack_pos << "($s0)" << " # store " << id << ":" << type << endl;
    }
  } else if ((type=="u") || (type=="s") || (type.size() > 1)) {
    aout << "SW " << src_reg << ", " << stack_pos << "($s0)" << " # store " << id << ":" << type << endl;
  } else if (type=="c") {
    aout << "SB " << src_reg << ", " << stack_pos+offset << "($s0)" << " # store " << id << ":" << type << endl;
  } else {
    cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << endl;
  }

  // check if this was obtained from a pointer
  if (fngen::var_to_pointer.count(make_pair(id, type))) {
    string pointer_id = fngen::var_to_pointer[make_pair(id, type)].first;
    string pointer_type = fngen::var_to_pointer[make_pair(id, type)].second;
    load_var_from_memory(pointer_id, pointer_type, "$t9");
    aout << "SW " << src_reg << ", " << "0($t9)" << " # store dereference " << id << ":" << type << endl;
  }


}


void load_var_from_memory(string id, string type, string dst_reg, int offset /*=0*/) {
  
  pair<string, string> id_type = make_pair(id, type);

  // if global var
  if (fngen::global_var.count(id_type)) {
    load_global_var(id, type, dst_reg);
    return;
  }

  int stack_pos=0;
  if (fngen::stack_var.count(id_type)) {
    stack_pos = fngen::stack_var[id_type];
  } else {
    cerr << "Warning: In " << __PRETTY_FUNCTION__ << " : id and type " << id << ":" << type << " not in stack (will occur for switch statements). Will assign new posiiton" << endl;
    // assign new posiiton
     if (type=="d") {
      if (fngen::next_stack_pos % 8) {fngen::next_stack_pos += 8 - abs(fngen::next_stack_pos % 8);}
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      fngen::next_stack_pos += 8;

    } else if ((type=="f") || (type=="u") || (type=="s") || (type.size() > 1) /* pointers and arrays and structs */ ) {
      if (fngen::next_stack_pos % 4) {fngen::next_stack_pos += 4 - abs(fngen::next_stack_pos % 4);}
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      fngen::next_stack_pos += 4;
    } else if (type=="c") {
      fngen::stack_var[id_type] = fngen::next_stack_pos;
      fngen::next_stack_pos += 1;
    } else {
      cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << endl;
    }

    stack_pos = fngen::stack_var[id_type];
  }

  int reg_no;
  try {
    reg_no = stoi(dst_reg.substr(2, dst_reg.size()-2));
  } catch (std::invalid_argument) {
    if (type=="d") {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Non numberred register passed with type double" << endl;
    }
  }
  // check if type is float/double or not
  if (type=="d") {
    if (dst_reg[1]=='f') {
      // floating point reg
      aout << "LWC1 " << dst_reg << ", " << stack_pos << "($s0)" << " # load " << id << ":" << type << endl;
      aout << "LWC1 " << dst_reg.substr(0, 2) << ++reg_no << ", " << stack_pos+4 << "($s0)" << " # load " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "LW " << dst_reg << ", " << stack_pos << "($s0)" << " # load " << id << ":" << type << endl;
      aout << "LW " << dst_reg.substr(0, 2) << ++reg_no << ", " << stack_pos+4 << "($s0)" << " # load " << id << ":" << type << endl;
    }
  } else if (type=="f") {
    if (dst_reg[1]=='f') {
      // floating point reg
      aout << "LWC1 " << dst_reg << ", " << stack_pos << "($s0)" << " # load " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "LW " << dst_reg << ", " << stack_pos << "($s0)" << " # load " << id << ":" << type << endl;
    }
  } else if ((type=="u") || (type=="s") || (type.size() > 1)) {
    aout << "LW " << dst_reg << ", " << stack_pos << "($s0)" << " # load " << id << ":" << type << endl;
  } else if (type=="c") {
    aout << "LB " << dst_reg << ", " << stack_pos+offset << "($s0)" << " # load " << id << ":" << type << endl; // char is sign extended and fills whole reg
  } else {
    cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << " # load " << id << ":" << type << endl;
  }
}

string new_data_label(string append /* = "" */ ) {
  static size_t no = 0;
  no++;
  return "_data" + append + to_string(no);
}

void store_global_var(string id, string type, string src_reg) {
  string label = fngen::global_var[make_pair(id, type)];

  int reg_no;
  try {
    reg_no = stoi(src_reg.substr(2, src_reg.size()-2));
  } catch (std::invalid_argument) {
    if (type=="d") {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Non numberred register passed with type double" << endl;
    }
  }

  // put address of global var in $t8
  aout << "la $t8, " << label << endl;

  if (type=="d") {
    if (src_reg[1]=='f') {
      // floating point reg
      aout << "SWC1 " << src_reg << ", 0($t8)" << " # store global " << id << ":" << type << endl;
      aout << "SWC1 " << src_reg.substr(0, 2) << ++reg_no << ", 4($t8)" << " # store " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "SW " << src_reg << ", 0($t8)" << " # store global " << id << ":" << type << endl;
      aout << "SW " << src_reg.substr(0, 2) << ++reg_no << ", 4($t8)" << " # store global " << id << ":" << type << endl;
    }
  } else if (type=="f") {
    if (src_reg[1]=='f') {
      // floating point reg
      aout << "SWC1 " << src_reg << ", " << label << " # store global " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "SW " << src_reg << ", " << label << " # store global " << id << ":" << type << endl;
    }
  } else if ((type=="u") || (type=="s") || (type.size() > 1)) {
    aout << "SW " << src_reg << ", " << label << " # store global " << id << ":" << type << endl;
  } else if (type=="c") {
    aout << "SB " << src_reg << ", " << label << " # store global " << id << ":" << type << endl;
  } else {
    cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << endl;
  }
}

void load_global_var(string id, string type, string dst_reg) {

  string label = fngen::global_var[make_pair(id, type)];

  int reg_no;
  try {
    reg_no = stoi(dst_reg.substr(2, dst_reg.size()-2));
  } catch (std::invalid_argument) {
    if (type=="d") {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Non numberred register passed with type double" << endl;
    }
  }
  // put address of global var in $t8
  aout << "la $t8, " << label << endl;

  // check if type is float/double or not
  if (type=="d") {
    if (dst_reg[1]=='f') {
      // floating point reg
      aout << "LWC1 " << dst_reg << ", 0($t8)" << " # load global " << id << ":" << type << endl;
      aout << "LWC1 " << dst_reg.substr(0, 2) << ++reg_no << ", 4($t8)" << " # load global " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "LW " << dst_reg << ", 0($t8)" << " # load global " << id << ":" << type << endl;
      aout << "LW " << dst_reg.substr(0, 2) << ++reg_no << ", 4($t8)" << " # load global " << id << ":" << type << endl;
    }
  } else if (type=="f") {
    if (dst_reg[1]=='f') {
      // floating point reg
      aout << "LWC1 " << dst_reg << ", " << label << " # load global " << id << ":" << type << endl;
    } else {
      // GPR to mem
      aout << "LW " << dst_reg << ", " << label << " # load global " << id << ":" << type << endl;
    }
  } else if ((type=="u") || (type=="s") || (type.size() > 1)) {
    aout << "LW " << dst_reg << ", " << label << " # load global " << id << ":" << type << endl;
  } else if (type=="c") {
    aout << "LB " << dst_reg << ", " << label << " # load global " << id << ":" << type << endl; // char is sign extended and fills whole reg
  } else {
    cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown type " << type << endl;
  }
}