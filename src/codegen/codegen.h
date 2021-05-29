#include "../tree/return_struct.h"

#pragma once

extern ofstream aout; // output to file
extern vector<string> data_section; // data section of whole program
namespace fngen {
  extern vector<streampos> stack_returns;
  extern map<pair<string, string>, int> stack_var;
  // map of global vars to data label
  extern map<pair<string, string>, string> global_var;
  extern int next_stack_pos;

  extern int no_of_params;
  extern int max_no_of_params;
  extern bool is_first_integral;
  extern int param_byte_offset;

  extern map<pair<string, string>, pair<string, string>> var_to_pointer;
  extern bool global_mode;
}

void codegen_main(string asmfile, const tree_to_3AC &tree_struct);
void function_gen(const statement_3AC &line);
void unknown_op(const statement_3AC &line);
void asm_start();

void function_start(const statement_3AC &line);
void function_end();
void place_param(const statement_3AC &line);
void place_return(const statement_3AC &line);
void function_call(const statement_3AC &line);

void three_op_code(const statement_3AC &line);
void two_op_code(const statement_3AC &line);

// 3 operand functions
void op_plus(const statement_3AC &line);
void op_minus(const statement_3AC &line);
void op_slt(const statement_3AC &line);
void op_mult(const statement_3AC &line);
void op_divide(const statement_3AC &line);
void op_mod(const statement_3AC &line);
void op_and(const statement_3AC &line);
void op_xor(const statement_3AC &line);
void op_or(const statement_3AC &line);
void op_nor(const statement_3AC &line);
void op_left_shift(const statement_3AC &line);
void op_right_shift(const statement_3AC &line);
//void op_right_shift_arithmentic(const statement_3AC &line);

// 2 op functions
void op_unary_minus(const statement_3AC &line);
void op_load(const statement_3AC &line);
void op_dereference(const statement_3AC &line);
void op_address_of(const statement_3AC &line);
void op_copy(const statement_3AC &line);
void op_associate(const statement_3AC &line);

// labels and branches
void place_label(const statement_3AC &line);
void branch_eq(const statement_3AC &line);
void branch_ne(const statement_3AC &line);

// utils
void store_var_in_memory(string id, string type, string src_reg, int offset=0);
void load_var_from_memory(string id, string type, string src_reg, int offset=0);
string new_data_label(string append = "" );