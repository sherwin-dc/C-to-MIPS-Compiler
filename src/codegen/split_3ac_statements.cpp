#include "codegen.h"

/* a:t = b:t op c:t
   {1, #1, f, #3, f, +, #4, f} */
void three_op_code(const statement_3AC &line) {
  string operand = line.op;
  if(operand=="+") {
    op_plus(line);
  } else if (operand=="-") {
    op_minus(line);
  } else if (operand=="<") {
    op_slt(line);
  } else if (operand=="*") {
    op_mult(line);
  } else if (operand=="/") {
    op_divide(line);
  } else if (operand=="%") {
    op_mod(line);
  } else if (operand=="&") {
    op_and(line);
  } else if (operand=="^") {
    op_xor(line);
  } else if (operand=="|") {
    op_or(line);
  } else if (operand=="nor") {
    op_nor(line);
  } else if (operand=="<<l") {
    op_left_shift(line);
  } else if (operand==">>l") {
    op_right_shift(line);
  // } else if (operand==">>r") {
  //   op_right_shift_arithmentic(line);
  } else {
    unknown_op(line);
  }

}

void two_op_code(const statement_3AC &line) {
  string operand = line.op;
  if(operand=="-") {
    op_unary_minus(line);
  } else if (operand=="ld") {
    op_load(line);
  } else if (operand=="der") {
    op_dereference(line);
  } else if (operand=="adr") {
    op_address_of(line);
  } else if (operand=="cp") {
    op_copy(line);
  } else if (operand=="asc") {
    op_associate(line);
  } else {
    unknown_op(line);
  }
}