#include "cparserDerivedVisitor.h"

using namespace cparser;

// antlrcpp::Any cparserDerivedVisitor::visitDeclaration(cparserParser::DeclarationContext *ctx) {
//     //need to store type that function returns here - because program may call fn before it is defined (where it gets its type from compound_statement result)
//   if (!ctx->init_declarator_list()) {
//     return visit(ctx->declaration_specifiers());
//   }else{
//     std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->declaration_specifiers()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->init_declarator_list()), __PRETTY_FUNCTION__);
//     string new_var = e2->result.first;
//     string new_var_type = e1->result.second;
//     //store fn's type to bindings
//     function_bindings[e2->result.first].second = new_var_type;
//     std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, new_var_type);

//     return return_3AC_struct(e2);
//   }
// }

antlrcpp::Any cparserDerivedVisitor::visitFn_declarator_params(cparserParser::Fn_declarator_paramsContext *ctx) {
    //stores number of params corr. to fn identifier in function_bindings; remove fn identifier from variable bindings
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->direct_declarator()), __PRETTY_FUNCTION__);
    //detects that identifier is for a function - maps its variable bindings to function bindings
    //check whether function has been declared previously

    //? Just override function bindings
    function_bindings[e1->result.first /*fn name*/] = "0"; // dont yet know return type
    
    if (ctx->parameter_type_list()){
        std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->parameter_type_list()), __PRETTY_FUNCTION__);
        e1 = combine_structs(e1, e2, e1->result.first, e1->result.second); // parameters automatically transferred to e1
    }
    
    
    return return_3AC_struct(e1);
  

}

antlrcpp::Any cparserDerivedVisitor::visitFunction_definition(cparserParser::Function_definitionContext *ctx) {
    
    // function already added to bindings in parameter_declaraion
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->compound_statement()), __PRETTY_FUNCTION__);


    std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, "", ""); // at this point no need for result
    statement_3AC stm = {7, e1->result.first};
    stm.args = e1->params;
    new_struct->lines_3AC.push_front(stm);
    new_struct->lines_3AC.emplace_back(5, "#0", "u", "?", "?", "return"); // void functions dont need return, so we will add one
    new_struct->params.clear(); // doest matter whether cleared or not
    return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitFn_call(cparserParser::Fn_callContext *ctx) {
    //fn identifier is read as variable identifier and stored in variable bindings
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);

    string fn_name = e1->result.first;
    //type would have been established - because fn would have been declared previously
    string fn_return_var = new_id();
    string fn_return_var_type = function_bindings[fn_name];

    if (ctx->argument_expression_list()) {
      std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->argument_expression_list()), __PRETTY_FUNCTION__);
      auto combined = combine_structs(e1, e2, fn_return_var, fn_return_var_type);
      for (auto it : e2->params) {
        combined->lines_3AC.emplace_back(5, it.first, it.second, "?", "?", "param");
      }
      combined->lines_3AC.emplace_back(6, fn_return_var, fn_return_var_type, fn_name, "?", "call", to_string(e2->params.size()));

      return return_3AC_struct(combined);
    } 
    
    e1->lines_3AC.emplace_back(6, fn_return_var, fn_return_var_type, fn_name, "?", "call", "0");
    e1->result.first = fn_return_var;
    e1->result.second = fn_return_var_type;
    
    return return_3AC_struct(e1);
}

/* parameter declaration eg int** a */
antlrcpp::Any cparserDerivedVisitor::visitFunction_params(cparserParser::Function_paramsContext *ctx) {
  auto declaration_specifiers = get_struct(visit(ctx->declaration_specifiers()), __PRETTY_FUNCTION__);
  auto declarator = get_struct(visit(ctx->declarator()), __PRETTY_FUNCTION__);

  // get identifier
  string id = declarator->result.first;
  string type = declaration_specifiers->result.second;

  

  if (array_bindings.count(id)){
 
    type += array_bindings[id];
  }
  // add pointer type to declaration_specifier type
  string dec_type = declarator->result.second;
  for (int i=0; i < dec_type.size(); i++) {
    if (dec_type[i]=='*') {
      type += "*";
    }
  }
  
// Check for typedefine
  if (type[0] == 'T') {
    type = type.substr(1, string::npos);

    // if not already typedefed 
    if (!typedef_bindings.count(id)) {
      typedef_bindings[id] = type;
      redo_parsing_typedef = true;
    }
    
  }


// cerr << "param decl " << id << ":" << type << endl;
  //? Check whether this is an array, function or variable
  if (array_bindings.count(id)) {
    array_bindings.clear(); // no need anymore
    variable_bindings[id] = type;
  } else if (function_bindings.count(id)) {
    // we finally know the return type of the function
    function_bindings[id] = type;
  } else {
    // add to bindings of local variables (override if already inside)
    variable_bindings[id] = type;
  }
  
  // add to return of struct
  auto s = combine_structs(declaration_specifiers, declarator, id, type);

  return return_3AC_struct(s);
}

antlrcpp::Any cparserDerivedVisitor::visitParameter_list(cparserParser::Parameter_listContext *ctx) {
  // here we put the return values from the parameter declaration into the params vector
  //get vector of param_declaration *
  auto vec = ctx->parameter_declaration();
  if (vec.size()==1) {
    auto s = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
    s->params.emplace_back(s->result.first, s->result.second);
    return return_3AC_struct(s);
  }

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
  e1->params.emplace_back(e1->result.first, e1->result.second);
  std::unique_ptr<tree_to_3AC> tmp;

  for (int i=1; i<vec.size() ; i++) {
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
    e2->params.emplace_back(e2->result.first, e2->result.second);
    tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
    std::swap(tmp, e1);
  }

  return return_3AC_struct(e1);
}

antlrcpp::Any cparserDerivedVisitor::visitArgument_expression_list(cparserParser::Argument_expression_listContext *ctx) {
  // here we put the return values from the parameter declaration into the params vector

  auto vec = ctx->assignment_expression();
  if (vec.size()==1) {
    auto s = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
    s->params.emplace_back(s->result.first, s->result.second);
    return return_3AC_struct(s);
  }

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
  e1->params.emplace_back(e1->result.first, e1->result.second);
  std::unique_ptr<tree_to_3AC> tmp;

  for (int i=1; i<vec.size() ; i++) {
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
    e2->params.emplace_back(e2->result.first, e2->result.second);
    tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
    std::swap(tmp, e1);
  }

  return return_3AC_struct(e1);

}


antlrcpp::Any cparserDerivedVisitor::visitDirect_declarator_top(cparserParser::Direct_declarator_topContext *ctx) {
  auto ex = get_struct(visit(ctx->direct_declarator()), __PRETTY_FUNCTION__);
  auto pointer_vec = ctx->STAR();
  for (int i=0; i<pointer_vec.size(); i++) {
    ex->result.second += "*";
  }
  return return_3AC_struct(ex);
}
