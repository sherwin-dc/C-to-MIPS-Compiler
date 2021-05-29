#include "cparserDerivedVisitor.h"

using namespace cparser;

// antlrcpp::Any cparserDerivedVisitor::visitInt_keyword_expr(cparserParser::Int_keyword_exprContext *ctx) {
//      // create a ptr to the struct
//   std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

//   string type_name = ctx->INT_KEYWORD()->getText();

//   new_struct->result.second = 's';
  
//   return return_3AC_struct(new_struct); 
// }

// antlrcpp::Any cparserDerivedVisitor::visitUnsigned_keyword_expr(cparserParser::Unsigned_keyword_exprContext *ctx) {
//          // create a ptr to the struct
//   std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

//   string type_name = ctx->UNSIGNED_INT_KEYWORD()->getText();

//   new_struct->result.second = 'u';
  
//   return return_3AC_struct(new_struct); 
// }

antlrcpp::Any cparserDerivedVisitor::visitType_name(cparserParser::Type_nameContext *ctx) {
  // visitChildren(ctx);
  // get type specifiers
  string type = "";
  auto vec = ctx->type_specifier();
  for (auto it : vec) {
    if (it->CHAR_KEYWORD()) {
      type = "c";
    } else if (it->DOUBLE_KEYWORD()) {
      type = "d";
    } else if (it->FLOAT_KEYWORD()) {
      type = "f";
    } else if (it->INT_KEYWORD()) {
      type = "s";
    } else if (it->VOID_KEYWORD()) {
      type = "u";
    } else if (it->enum_specifier()) {
      visit(it->enum_specifier());
      type = "s"; // enum
    } else if (it->struct_specifier()) {
      auto str = get_struct(visit(it->struct_specifier()), __PRETTY_FUNCTION__);
      type = str->result.second;
    } else if (it->typedefed_identifier()) {
      auto i = get_struct(visit(it->typedefed_identifier()), __PRETTY_FUNCTION__);
      type = typedef_bindings[i->result.first];
    } /* else if (it->identifier()) {
      cerr << "Warning in " << __PRETTY_FUNCTION__ << " : identifier parsed as type" << endl;
      redo_parsing_typedef = true;
      type = "t";
    } */
  }

  for (auto it : vec) {
    if (it->SIGNED_KEYWORD()) {
      type = "s";
    } else if (it->UNSIGNED_KEYWORD()) {
      type = "u";
    }
  }
  auto pointers = ctx->STAR();
  for (int i=0; i<pointers.size(); i++) {
    type += "*";
  }

  auto s = create_new_struct();
  s->result.second = type;

  return return_3AC_struct(s);

}

/* declaration_specifers */
antlrcpp::Any cparserDerivedVisitor::visitType_specifier_top(cparserParser::Type_specifier_topContext *ctx) {
  visitChildren(ctx);
  string type = "";
  auto vec = ctx->type_specifier();
  for (auto it : vec) {
    if (it->CHAR_KEYWORD()) {
      type = "c";
    } else if (it->DOUBLE_KEYWORD()) {
      type = "d";
    } else if (it->FLOAT_KEYWORD()) {
      type = "f";
    } else if (it->INT_KEYWORD()) {
      type = "s";
    } else if (it->VOID_KEYWORD()) {
      type = "u";
    } else if (it->enum_specifier()) {
      visit(it->enum_specifier());
      type = "s"; // enum
    } else if (it->struct_specifier()) {
      auto str = get_struct(visit(it->struct_specifier()), __PRETTY_FUNCTION__);
      type = str->result.second;
    } else if (it->typedefed_identifier()) {
      auto i = get_struct(visit(it->typedefed_identifier()), __PRETTY_FUNCTION__);
      type = typedef_bindings[i->result.first];
    } /* else if (it->identifier()) {
      cerr << "Warning in " << __PRETTY_FUNCTION__ << " : identifier parsed as type" << endl;
      redo_parsing_typedef = true;
      type = "t";
    } */
  }

  

  for (auto it : vec) {
    if (it->SIGNED_KEYWORD()) {
      type = "s";
    } else if (it->UNSIGNED_KEYWORD()) {
      type = "u";
    }
  }

  for (auto it : vec) {
    if (it->TYPEDEF_KEYWORD()) {
      type = "T" + type;
    } 
  }

  auto s = create_new_struct();
  s->result.second = type;

  return return_3AC_struct(s);
}