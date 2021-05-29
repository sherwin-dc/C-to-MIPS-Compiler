#include "cparserDerivedVisitor.h"

using namespace cparser;


antlrcpp::Any cparserDerivedVisitor::visitAddition_expression(cparserParser::Addition_expressionContext *ctx) {

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->additive_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->multiplicative_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "+", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitSubtraction_expression(cparserParser::Subtraction_expressionContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->additive_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->multiplicative_expression()), __PRETTY_FUNCTION__);

    // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];
  string new_var_type = e1_type;

  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "-", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitMultiplication_expression(cparserParser::Multiplication_expressionContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->multiplicative_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

    // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "*", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitDivision_expression(cparserParser::Division_expressionContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->multiplicative_expression()), __PRETTY_FUNCTION__);

    // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "/", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitMod_expression(cparserParser::Mod_expressionContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->multiplicative_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

    // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  //mod operation in mips: division, then taking value from HI reg

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "%", e2_var, e2_type};
  //statement_3AC new_3AC_2 = {8, new_var, new_var_type, "?", "?", "mfhi", "?", "?"};
  new_struct->lines_3AC.push_back(new_3AC);
  //new_struct->lines_3AC.push_back(new_3AC_2);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitUnary_minus(cparserParser::Unary_minusContext *ctx) {
  // do zero - x
  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string id = new_id();
  e->lines_3AC.emplace_back(1, id, e->result.second, "#0", e->result.second, "-", e->result.first, e->result.second);
  e->result.first = id;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitPost_increment(cparserParser::Post_incrementContext *ctx) {
  auto e = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);
  string type = e->result.second;
  string id_one = new_id();
  string id = new_id();
  string copy = new_id();
  string incr_type = type;
  if (type.size()>1) {
    incr_type = "s";
  }
  e->lines_3AC.emplace_back(2, id_one, incr_type, "?", "?", "ld", "1", incr_type);
  e->lines_3AC.emplace_back(1, id, type, e->result.first, type, "+", id_one, incr_type);
  e->lines_3AC.emplace_back(2, copy, type, "?", "?", "cp", e->result.first, type);
  e->lines_3AC.emplace_back(2, e->result.first, type, "?", "?", "cp", id, type);
  e->result.first = copy;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitPost_decrement(cparserParser::Post_decrementContext *ctx) {
  auto e = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);
  string type = e->result.second;
  string id_one = new_id();
  string id = new_id();
  string copy = new_id();
  string incr_type = type;
  if (type.size()>1) {
    incr_type = "s";
  }
  e->lines_3AC.emplace_back(2, id_one, incr_type, "?", "?", "ld", "1", incr_type);
  e->lines_3AC.emplace_back(1, id, type, e->result.first, type, "-", id_one, incr_type);
  e->lines_3AC.emplace_back(2, copy, type, "?", "?", "cp", e->result.first, type);
  e->lines_3AC.emplace_back(2, e->result.first, type, "?", "?", "cp", id, type);
  e->result.first = copy;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitPre_increment(cparserParser::Pre_incrementContext *ctx) {
  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string type = e->result.second;
  string id_one = new_id();
  string id = new_id();
  string incr_type = type;
  if (type.size()>1) {
    incr_type = "s";
  }
  e->lines_3AC.emplace_back(2, id_one, incr_type, "?", "?", "ld", "1", incr_type);
  e->lines_3AC.emplace_back(1, id, type, e->result.first, type, "+", id_one, incr_type);
  e->lines_3AC.emplace_back(2, e->result.first, type, "?", "?", "cp", id, type);

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitPre_decrement(cparserParser::Pre_decrementContext *ctx) {
  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string type = e->result.second;
  string id_one = new_id();
  string id = new_id();
  string incr_type = type;
  if (type.size()>1) {
    incr_type = "u";
  }
  e->lines_3AC.emplace_back(2, id_one, incr_type, "?", "?", "ld", "1", incr_type);
  e->lines_3AC.emplace_back(1, id, type, e->result.first, type, "-", id_one, incr_type);
  e->lines_3AC.emplace_back(2, e->result.first, type, "?", "?", "cp", id, type);

  return return_3AC_struct(e);

}