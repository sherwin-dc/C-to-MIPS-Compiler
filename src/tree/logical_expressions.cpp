#include "cparserDerivedVisitor.h"

using namespace cparser;


antlrcpp::Any cparserDerivedVisitor::visitLogical_or(cparserParser::Logical_orContext *ctx) {
  //|| operator: returns 1 if first term is non-zero; else evaluates second term - if second term is non-zero, returns 1; returns 0 otherwise
//   if(a){
//       t = 1;
//   }else{
//       if (b){
//           t=1;
//       }else{
//           t=0;
//       }
//   }
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->logical_or_expression()), __PRETTY_FUNCTION__);
  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->logical_and_expression()), __PRETTY_FUNCTION__);
  
    // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = "s";

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);
  
  
  //string e1_type = e1->result.second;
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", e1->result.first, e1_type, "eq", "#0", e1_type};
  new_struct->lines_3AC.push_back(branch);

  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //std::unique_ptr<tree_to_3AC> stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if part - inner loop 
  string label_else_2=new_label("else");
  //string e2_type = e2->result.second;
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch_2 = {4, label_else_2, "?", e2->result.first, e2_type, "eq", "#0", e2_type};
  new_struct->lines_3AC.push_back(branch_2);

  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);

  //std::unique_ptr<tree_to_3AC> stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif_inner = new_label("endif");
  statement_3AC endif_inner = {4, label_endif_inner, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif_inner);
  statement_3AC else_label_2 = {3, label_else_2};
  new_struct->lines_3AC.push_back(else_label_2);

    //inner loop else
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_3 = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_3);
  
  statement_3AC endif_label_inner = {3, label_endif_inner};
  new_struct->lines_3AC.push_back(endif_label_inner);

  //outer loop - endif - end of else statement
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitLogical_and(cparserParser::Logical_andContext *ctx) {
    // if (a){
    //     if (b){
    //         t = 1;
    //     }else{
    //         t = 0;
    //     }
    // }else{
    //     t = 0;
    // }
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->logical_and_expression()), __PRETTY_FUNCTION__);
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->or_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = "s";

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);
  

  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", e1->result.first, e1_type, "eq", "#0", e1_type};
  new_struct->lines_3AC.push_back(branch);



    //outer loop - if part
    //if part - inner loop 
  string label_else_inner=new_label("else");
  //string e2_type = e2->result.second;
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch_2 = {4, label_else_inner, "?", e2->result.first, e2_type, "eq", "#0", e2_type};
  new_struct->lines_3AC.push_back(branch_2);

  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);

  //std::unique_ptr<tree_to_3AC> stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif_inner = new_label("endif");
  statement_3AC endif_inner = {4, label_endif_inner, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif_inner);
  statement_3AC else_label_2 = {3, label_else_inner};
  new_struct->lines_3AC.push_back(else_label_2);

    //inner loop else
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_3 = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_3);
  
  statement_3AC endif_label_inner = {3, label_endif_inner};
  new_struct->lines_3AC.push_back(endif_label_inner);

    //outer loop - else
  //std::unique_ptr<tree_to_3AC> stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //outer loop - endif - end of else statement
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitOr_expr(cparserParser::Or_exprContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->or_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->xor_expression()), __PRETTY_FUNCTION__);

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
  statement_3AC new_3AC = {1, new_var, new_var_type, e1->result.first, e1_type , "|", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitXor_expr(cparserParser::Xor_exprContext *ctx) {
 std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->xor_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->and_expression()), __PRETTY_FUNCTION__);

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
  statement_3AC new_3AC = {1, new_var, new_var_type, e1->result.first, e1_type , "^", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAnd_expr(cparserParser::And_exprContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->and_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->equality_expression()), __PRETTY_FUNCTION__);

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
  statement_3AC new_3AC = {1, new_var, new_var_type, e1->result.first, e1_type , "&", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitEquality(cparserParser::EqualityContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->equality_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = "s";

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

    //branch to else label if e1==e2
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", e1->result.first, e1_type, "eq", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(branch);

    //branch not taken -> e1 and e1 are not equal - return 0
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //branch taken -> e1 and e2 are equal - return 1
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitNonequality(cparserParser::NonequalityContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->equality_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = "s";

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

    //branch to else label if e1==e2
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", e1->result.first, e1_type, "eq", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(branch);

    //branch not taken -> e1 and e1 are not equal - return 1
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //branch taken -> e1 and e2 are equal - return 0
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitLogical_not_expression(cparserParser::Logical_not_expressionContext *ctx) {
    //! operator: returns 0 if expression is non-zero; returns 1 otherwise
    std::unique_ptr<tree_to_3AC> exp = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
    
    pair<string, string> result = exp->result;
    vector<string> arg_type = argument_type(result, {"0","0"});
    string exp_type = arg_type[0];

  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", exp->result.first, exp_type, "eq", "#0", exp_type};
  exp->lines_3AC.push_back(branch);



  string new_var_type;
  //!a returns type int
    new_var_type = "s";
    //? Create new 3AC statement
  string new_var = new_id();
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", "0", new_var_type};
  exp->lines_3AC.push_back(new_3AC);
  //exp->result = make_pair(new_var, new_var_type);



  //std::unique_ptr<tree_to_3AC> stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  exp->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  exp->lines_3AC.push_back(else_label);


  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "ld", "1", new_var_type};
  exp->lines_3AC.push_back(new_3AC_2);
  
  statement_3AC endif_label = {3, label_endif};
  exp->lines_3AC.push_back(endif_label);

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs_one(exp, new_var, new_var_type);
  new_struct->lines_3AC = exp->lines_3AC;

  return return_3AC_struct(new_struct);
}


antlrcpp::Any cparserDerivedVisitor::visitNot_expression(cparserParser::Not_expressionContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  //std::unique_ptr<tree_to_3AC> e2 = 

  // new variable to store result of this expression
  string new_var = new_id();

      pair<string, string> result = e1->result;
    vector<string> arg_type = argument_type(result, {"0","0"});
    string e1_type = arg_type[0];
    string new_var_type = e1_type;

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1->result.first, e1_type , "nor", e1->result.first, e1_type};
  e1->lines_3AC.push_back(new_3AC);

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs_one(e1, new_var, new_var_type);
  new_struct->lines_3AC = e1->lines_3AC;
  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitLess_than_expr(cparserParser::Less_than_exprContext *ctx) {
  //auto exp = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);
  //get the slt 3ac line first
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  //getting the type according to whether it is a constant or variable
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  string logic_type = new_id();
  string logic_var_type = "s";
  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, logic_type, logic_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "<", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //if-else logic that returns 1 (int type) when slt is true
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", new_var, new_var_type, "eq", "#0", new_var_type};
  new_struct->lines_3AC.push_back(branch);

  //if branch not taken - return 1
  statement_3AC new_3AC_br_not_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "1", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_not_taken);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if branch is taken - return 0
  statement_3AC new_3AC_br_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "0", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_taken);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitMore_than_expr(cparserParser::More_than_exprContext *ctx) {
  //auto exp = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);
  //get the slt 3ac line first
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  //getting the type according to whether it is a constant or variable
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  string logic_type = new_id();
  string logic_var_type = "s";
  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, logic_type, logic_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e2_var, e2_type , "<", e1_var, e1_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //if-else logic that returns 1 (int type) when slt is true
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", new_var, new_var_type, "eq", "#0", new_var_type};
  new_struct->lines_3AC.push_back(branch);

  //if branch not taken - return 1
  statement_3AC new_3AC_br_not_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "1", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_not_taken);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if branch is taken - return 0
  statement_3AC new_3AC_br_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "0", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_taken);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitLess_than_equal_expr(cparserParser::Less_than_equal_exprContext *ctx) {
  //auto exp = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);
  //get the slt 3ac line first
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  //getting the type according to whether it is a constant or variable
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  string logic_type = new_id();
  string logic_var_type = "s";
  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, logic_type, logic_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e2_var, e2_type , "<", e1_var, e1_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //if-else logic that returns 1 (int type) when slt is true
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", new_var, new_var_type, "eq", "#0", new_var_type};
  new_struct->lines_3AC.push_back(branch);

  //if branch not taken - return 1
  statement_3AC new_3AC_br_not_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "0", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_not_taken);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if branch is taken - return 0
  statement_3AC new_3AC_br_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "1", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_taken);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitMore_than_equal_expr(cparserParser::More_than_equal_exprContext *ctx) {
  //auto exp = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);
  //get the slt 3ac line first
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->relational_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  // new variable to store result of this expression
  //getting the type according to whether it is a constant or variable
  string new_var = new_id();
  string e1_var = e1->result.first;
  string e2_var = e2->result.first;
  pair<string, string> result = e1->result;
  pair<string, string> result_2 = e2->result;
  vector<string> arg_type = argument_type(result, result_2);
  string e1_type = arg_type[0];
  string e2_type = arg_type[1];

  string new_var_type = e1_type;

  string logic_type = new_id();
  string logic_var_type = "s";
  // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, logic_type, logic_var_type);

  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "<", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  //if-else logic that returns 1 (int type) when slt is true
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", new_var, new_var_type, "eq", "#0", new_var_type};
  new_struct->lines_3AC.push_back(branch);

  //if branch not taken - return 1
  statement_3AC new_3AC_br_not_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "0", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_not_taken);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if branch is taken - return 0
  statement_3AC new_3AC_br_taken = {2, logic_type, logic_var_type, "?", "?" , "ld", "1", logic_var_type};
  new_struct->lines_3AC.push_back(new_3AC_br_taken);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitLeft_shift(cparserParser::Left_shiftContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->additive_expression()), __PRETTY_FUNCTION__);

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
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , "<<l", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitRight_shift(cparserParser::Right_shiftContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->shift_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->additive_expression()), __PRETTY_FUNCTION__);

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
  statement_3AC new_3AC = {1, new_var, new_var_type, e1_var, e1_type , ">>l", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitTernary_conditional_expr(cparserParser::Ternary_conditional_exprContext *ctx) {
  //get the slt 3ac line first
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->logical_or_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e3 = get_struct(visit(ctx->conditional_expression()), __PRETTY_FUNCTION__);

  string e1_type;
  string e2_type;
  string e3_type;
  //checking if e1 is a variable
  if (variable_bindings.find(e1->result.first)!=variable_bindings.end()){
    e1_type = variable_bindings[e1->result.first];
  }else{
    e1_type = e1->result.second;
  }
  //checking if e2 is a variable
  if (variable_bindings.find(e2->result.first)!=variable_bindings.end()){
    e2_type = variable_bindings[e2->result.first];
  }else{
    e2_type = e2->result.second;
  }
  //checking if e3 is a variable
  if (variable_bindings.find(e3->result.first)!=variable_bindings.end()){
    e3_type = variable_bindings[e3->result.first];
  }else{
    e3_type = e3->result.second;
  }

  string new_var_type = e1_type;

  string new_var = new_id();
  //string new_var_type = "s";

  std::unique_ptr<tree_to_3AC> interm_struct = combine_structs(e2, e3, e2->result.first, e2_type);
  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, interm_struct, new_var, new_var_type);

  //if-else logic that returns expression when logical_or_expression is true
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", e1->result.first, e1_type, "eq", "#0", e1_type};
  new_struct->lines_3AC.push_back(branch);

  //if branch not taken - return expression
  statement_3AC new_3AC_br_not_taken = {2, new_var, new_var_type, "?", "?" , "ld", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(new_3AC_br_not_taken);

  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  new_struct->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  new_struct->lines_3AC.push_back(else_label);

  //if branch is taken - return 0
  statement_3AC new_3AC_br_taken = {2, new_var, new_var_type, "?", "?" , "ld", e3->result.first, e3_type};
  new_struct->lines_3AC.push_back(new_3AC_br_taken);
  
  statement_3AC endif_label = {3, label_endif};
  new_struct->lines_3AC.push_back(endif_label);

  return return_3AC_struct(new_struct);
}
