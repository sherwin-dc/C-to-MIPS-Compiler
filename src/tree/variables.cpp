#include "cparserDerivedVisitor.h"

using namespace cparser;

// antlrcpp::Any cparserDerivedVisitor::visitVariable_declaration(cparserParser::Variable_declarationContext *ctx) {
//   //int a = 10;
//   //int a;
  
//   std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->declaration_specifiers()), __PRETTY_FUNCTION__);

//   std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->init_declarator_list()), __PRETTY_FUNCTION__);

//   // new variable to store result of this expression
//   string new_var = new_id();
//   // type of new var will be type of the first operand - follows type of declared variable
//   string e1_type = e1->result.second;
//   string e2_type = e2->result.second;
//   string new_var_type = e1_type;

//   // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
//   std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

//     // new variable to store result of this expression
//     string new_var;
//     string e2_type;
//     pair<string,string> empty_str = {"0","0"};
//     if(variable_bindings[e2->id_name]!=empty_str){
//         new_var = variable_bindings[e2->id_name].first;
//         e2_type = variable_bindings[e2->id_name].second;
//     }else{
//         new_var = new_id(); //e2->result.first;
//         e2_type = e1->result.second;
//     }
    
//   string e2_type = e2->result.second;
//   string new_var_type = e1_type;

//   std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);




//     //create new 3ac line
//     //a:t = ld b:t
//   //{2, #1, f, ?, ?, -, #4, f}
//   statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", e2->result.first, e2_type};
//   new_struct->lines_3AC.push_back(new_3AC);
//   variable_bindings[e2->id_name] = make_pair(new_var, new_var_type);

//   return return_3AC_struct(new_struct);
    
    
    
    
//     // dst << "VARIABLE DECLARATION" << std::endl;


//     // return 0;
// }

antlrcpp::Any cparserDerivedVisitor::visitAssign_value_expr(cparserParser::Assign_value_exprContext *ctx) {
    //a = 10;
    // we already got the type of this var in parameter_declaration
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->initialiser()), __PRETTY_FUNCTION__);
    
    // // new variable to store result of this expression
    // string new_var = e1->result.first;
    // string e1_type;
    // if( (variable_bindings[e1->result.first] )[0]!='0'){
    //     e1_type = variable_bindings[e1->result.first];
    // }else{
    //     variable_bindings[e1->result.first]=e2->result.second; 
    //     //identifier doesnt store its type before its initialised - takes type of e2
    //     e1_type = e2->result.second;
    // }
  string new_var = e1->result.first;
  string new_var_type = e1->result.second;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);


  //create new 3ac line
    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "cp", e2->result.first, e2->result.second};
  new_struct->lines_3AC.push_back(new_3AC);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);



}

antlrcpp::Any cparserDerivedVisitor::visitAssign_value_constant(cparserParser::Assign_value_constantContext *ctx) {
  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->constant()), __PRETTY_FUNCTION__);

  string new_var = e1->result.first;
  string new_var_type = e1->result.second;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);
  new_struct->lines_3AC.emplace_back(2, new_var, new_var_type, "?", "?" , "ld", e2->result.first, e2->result.second);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_eq(cparserParser::Assignment_expr_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);


  //create new 3ac line
    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "cp", e2->result.first, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_plus_eq(cparserParser::Assignment_expr_plus_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "+", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_minus_eq(cparserParser::Assignment_expr_minus_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "-", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_times_eq(cparserParser::Assignment_expr_times_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "*", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_div_eq(cparserParser::Assignment_expr_div_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "/", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_mod_eq(cparserParser::Assignment_expr_mod_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "%", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_and_eq(cparserParser::Assignment_expr_and_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "&", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_xor_eq(cparserParser::Assignment_expr_xor_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "^", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_or_eq(cparserParser::Assignment_expr_or_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "|", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_rightshift_eq(cparserParser::Assignment_expr_rightshift_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , ">>l", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitAssignment_expr_leftshift_eq(cparserParser::Assignment_expr_leftshift_eqContext *ctx) {
    //a = 10;
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);

  std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->assignment_expression()), __PRETTY_FUNCTION__);
    
    // new variable to store result of this expression
    string new_var = e1->result.first;
    string e1_type;

    if((variable_bindings[e1->result.first]!="0") && (variable_bindings[e1->result.first]!="")){
        e1_type = variable_bindings[e1->result.first];
    }else{
        variable_bindings[e1->result.first]=e2->result.second; 
        //identifier doesnt store its type before its initialised - takes type of e2
        e1_type = e2->result.second;
    }
    
  string e2_var = e2->result.first;
  string e2_type = e2->result.second;
  string new_var_type = e1_type;

  std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

  string interm_var = new_id();
  string interm_var_type = new_var_type;
  // create new 3AC operation for this code
  // a:t = b:t op c:t
  statement_3AC new_3AC = {1, interm_var, interm_var_type, new_var, new_var_type , "<<l", e2_var, e2_type};
  new_struct->lines_3AC.push_back(new_3AC);


    //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  statement_3AC new_3AC_2 = {2, new_var, new_var_type, "?", "?" , "cp", interm_var, interm_var_type};
  new_struct->lines_3AC.push_back(new_3AC_2);
  //variable_bindings[e1->result.first] = make_pair(new_var, new_var_type);

  //new_struct->variable_bindings[e1->id_name] = make_pair(new_var, new_var_type);

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitDeclarator_top(cparserParser::Declarator_topContext *ctx) {
  // initialise variable to 0 if this is a variable
  auto s = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);
  string id = s->result.first;
  if (variable_bindings.count(id)) {
    // check whether this is an array
    string type = s->result.second;
    if (char_is_digit(type[type.size()-1])) {
        //? Get the size of the array
        int byte_size=get_array_whole_size(type);
        
        // place empty double to force double-aligned
        s->lines_3AC.emplace_back(2, new_id(), "d", "?", "?", "ld", "0","d");
        // initialise values to zero
        string first_element = new_id();
        s->lines_3AC.emplace_back(2, first_element, "c", "?", "?", "ld", "0", "c");
        for (int i=1; i<byte_size; i++) {
          s->lines_3AC.emplace_back(2, new_id(),"c", "?", "?", "ld", "0", "c");
        }
        s->lines_3AC.emplace_back(2, s->result.first, type, "?", "?", "adr", first_element, "c");

    } else if (type[type.size()-1]=='$') { // struct
      string str_name = type.substr(1, type.size()-2);
      int str_size = struct_bindings[str_name].struct_size;

      // place empty double to force double-aligned
      s->lines_3AC.emplace_back(2, new_id(), "d", "?", "?", "ld", "0","d");
      // initialise values to zero
      string first_element = new_id();
      s->lines_3AC.emplace_back(2, first_element, "c", "?", "?", "ld", "0", "c");
      for (int i=1; i<str_size; i++) {
        s->lines_3AC.emplace_back(2, new_id(),"c", "?", "?", "ld", "0", "c");
      }
      // string char_id = new_id();
      s->lines_3AC.emplace_back(2, s->result.first, type, "?", "?", "asc", first_element, "c");
      // s->lines_3AC.emplace_back(2, s->result.first, type, "?", "?", "der", char_id, "c*");

    } else {
      s->lines_3AC.emplace_back(2, s->result.first, s->result.second, "?", "?", "ld", "0", s->result.second);
    }
    
  }
  
  return return_3AC_struct(s);
}

antlrcpp::Any cparserDerivedVisitor::visitLone_declaration_specifier(cparserParser::Lone_declaration_specifierContext *ctx) {
  return visit(ctx->declaration_specifiers());
}

antlrcpp::Any cparserDerivedVisitor::visitSizeof_expr(cparserParser::Sizeof_exprContext *ctx) {

  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string id = new_id();
  string type = "u";
  string expr_type = e->result.second;
  int size=0;
  if (expr_type[expr_type.size()-1]=='*') {
    size = 4;
  } else if (expr_type=="c") {
    size = 1;
  } else if ((expr_type=="u") || (expr_type=="s") || (expr_type=="f")) {
    size = 4;
  } else if (expr_type=="d") {
    size = 8;
  } else if (char_is_digit(expr_type[expr_type.size()-1])) {
    size = get_array_whole_size(expr_type);
  } else if (expr_type[expr_type.size()-1]=='$') {
    string str_name = expr_type.substr(1, expr_type.size()-2);
    size = struct_bindings[str_name].struct_size;
  }
    
  e->lines_3AC.emplace_back(2, id,type, "?", "?", "ld", to_string(size), type);
  e->result.first = id;
  e->result.second = type;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitSizeof_type(cparserParser::Sizeof_typeContext *ctx) {
  auto e = get_struct(visit(ctx->type_name()), __PRETTY_FUNCTION__);
  string id = new_id();
  string type = "u";
  string expr_type = e->result.second;
  int size=0;
  if (expr_type[expr_type.size()-1]=='*') {
    size = 4;
  } else if (expr_type=="c") {
    size = 1;
  } else if ((expr_type=="u") || (expr_type=="s") || (expr_type=="f")) {
    size = 4;
  } else if (expr_type=="d") {
    size = 8;
  } else if (char_is_digit(expr_type[expr_type.size()-1])) {
    size = get_array_whole_size(expr_type);
  } else if (expr_type[expr_type.size()-1]=='$') {
    string str_name = expr_type.substr(1, expr_type.size()-2);
    size = struct_bindings[str_name].struct_size;
  }
  e->lines_3AC.emplace_back(2, id,type, "?", "?", "ld", to_string(size), type);
  e->result.first = id;
  e->result.second = type;

  return return_3AC_struct(e);
}