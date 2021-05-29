#include "cparserDerivedVisitor.h"

using namespace cparser;

antlrcpp::Any cparserDerivedVisitor::visitProg(cparserParser::ProgContext *ctx) {
  return visit(ctx->translation_unit());
}

antlrcpp::Any cparserDerivedVisitor::visitTranslation_unit(cparserParser::Translation_unitContext *ctx) {
  //get vector of block_item_context *
  auto vec = ctx->external_declaration();
  if (vec.size()==1) {
    return visitChildren(ctx);
  }

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
  std::unique_ptr<tree_to_3AC> tmp;

  for (int i=1; i<vec.size() ; i++) {
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
    tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
    std::swap(tmp, e1);
  }

  return return_3AC_struct(e1);
}

// antlrcpp::Any cparserDerivedVisitor::visitFunction_definition_top(cparserParser::Function_definition_topContext *ctx) {
//   return visit(ctx->function_definition());
// }

// antlrcpp::Any cparserDerivedVisitor::visitDeclaration_label(cparserParser::Declaration_labelContext *ctx) {
//   return visit(ctx->declaration());
// }

// antlrcpp::Any cparserDerivedVisitor::visitFunction_definition(cparserParser::Function_definitionContext *ctx) {
//   if (!ctx->declaration_list()) {
//     std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->declaration_specifiers()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->declarator()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e3 = get_struct(visit(ctx->compound_statement()), __PRETTY_FUNCTION__);
//     return return_3AC_struct(e3);
//   }else{
//     std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->declaration_specifiers()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->declarator()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e3 = get_struct(visit(ctx->declaration_list()), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> e4 = get_struct(visit(ctx->compound_statement()), __PRETTY_FUNCTION__);
//     return return_3AC_struct(e4);
//   }
// }

antlrcpp::Any cparserDerivedVisitor::visitBlock_item_list(cparserParser::Block_item_listContext *ctx) {

  //get vector of block_item_context *
  auto vec = ctx->block_item();
  if (vec.size()==1) {
    return visitChildren(ctx);
  }

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
  std::unique_ptr<tree_to_3AC> tmp;

  for (int i=1; i<vec.size() ; i++) {
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
    tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
    std::swap(tmp, e1);
  }

  return return_3AC_struct(e1);

}

antlrcpp::Any cparserDerivedVisitor::visitExpression_statement(cparserParser::Expression_statementContext *ctx) {
  if (ctx->expression()) {
    return visit(ctx->expression());
  } else {
    // return an empty struct
    std::unique_ptr<tree_to_3AC> empty = create_new_struct();
    return return_3AC_struct(empty);
  }
}


antlrcpp::Any cparserDerivedVisitor::visitDeclaration_list(cparserParser::Declaration_listContext *ctx) {
  //get vector of block_item_context *
  auto vec = ctx->declaration();
  if (vec.size()==1) {
    return visitChildren(ctx);
  }

  std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
  std::unique_ptr<tree_to_3AC> tmp;

  for (int i=1; i<vec.size() ; i++) {
    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
    tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
    std::swap(tmp, e1);
  }

  return return_3AC_struct(e1);

}

antlrcpp::Any cparserDerivedVisitor::visitCompound_statement(cparserParser::Compound_statementContext *ctx) {
  if (ctx->block_item_list()) {
    return visit(ctx->block_item_list());
  } else {
    // return an empty struct
    std::unique_ptr<tree_to_3AC> empty = create_new_struct();
    return return_3AC_struct(empty);
  }
}
// This is for global declarations
antlrcpp::Any cparserDerivedVisitor::visitDeclaration_label(cparserParser::Declaration_labelContext *ctx) {
  auto e = get_struct(visit(ctx->declaration()), __PRETTY_FUNCTION__);
  e->lines_3AC.emplace_front(7, "#Global");

  return return_3AC_struct(e);
}