#include "cparserDerivedVisitor.h"

using namespace cparser;

antlrcpp::Any cparserDerivedVisitor::visitIdentifier_base(cparserParser::Identifier_baseContext *ctx) {
    // create a ptr to the struct
  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

  //std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->identifier()), __PRETTY_FUNCTION__);

  string identifier_name = ctx->IDENTIFIER()->getText();

  new_struct->result.first = identifier_name;
  //type will be determined when variable is initialised

  //"0" to indicate that type is not determined yet
  pair<string, string> empty_str = {identifier_name,"0"};
  if(variable_bindings.find(identifier_name)==variable_bindings.end()){
    variable_bindings.insert(empty_str);
  } else {
    new_struct->result.second = variable_bindings[identifier_name]; // otherwise put the type in
  }

  return return_3AC_struct(new_struct); 
}

antlrcpp::Any cparserDerivedVisitor::visitSuffix_as_id(cparserParser::Suffix_as_idContext *ctx) {

  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();
  
  antlr4::tree::TerminalNode * TOKEN;
  if (ctx->UNSIGNED_SUFFIX()) {
    TOKEN=ctx->UNSIGNED_SUFFIX();
  } else if (ctx->LONG_LONG_SUFFIX()) {
    TOKEN=ctx->LONG_LONG_SUFFIX();
  } else if (ctx->LONG_SUFFIX()) {
    TOKEN=ctx->LONG_SUFFIX();
  } else if (ctx->FLOATING_SUFFIX()) {
    TOKEN=ctx->FLOATING_SUFFIX();
  }
  string identifier_name = TOKEN->getText();

  new_struct->result.first = identifier_name;

  pair<string, string> empty_str = {identifier_name,"0"};
  if(variable_bindings.find(identifier_name)==variable_bindings.end()){
    variable_bindings.insert(empty_str);
  }

  return return_3AC_struct(new_struct); 
}

antlrcpp::Any cparserDerivedVisitor::visitSuffix_as_id_td(cparserParser::Suffix_as_id_tdContext *ctx) {

  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();
  
  antlr4::tree::TerminalNode * TOKEN;
  if (ctx->UNSIGNED_SUFFIX()) {
    TOKEN=ctx->UNSIGNED_SUFFIX();
  } else if (ctx->LONG_LONG_SUFFIX()) {
    TOKEN=ctx->LONG_LONG_SUFFIX();
  } else if (ctx->LONG_SUFFIX()) {
    TOKEN=ctx->LONG_SUFFIX();
  } else if (ctx->FLOATING_SUFFIX()) {
    TOKEN=ctx->FLOATING_SUFFIX();
  } else if (ctx->IDENTIFIER()) {
    TOKEN=ctx->IDENTIFIER();
  }
  string identifier_name = TOKEN->getText();

  new_struct->result.first = identifier_name;

  pair<string, string> empty_str = {identifier_name,"0"};
  if(variable_bindings.find(identifier_name)==variable_bindings.end()){
    variable_bindings.insert(empty_str);
  }

  return return_3AC_struct(new_struct); 
}