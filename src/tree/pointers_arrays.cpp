#include "cparserDerivedVisitor.h"

using namespace cparser;


antlrcpp::Any cparserDerivedVisitor::visitUnary_dereference(cparserParser::Unary_dereferenceContext *ctx) {
  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string id = new_id();
  string type = e->result.second.substr(0, e->result.second.size()-1);

  e->lines_3AC.emplace_back(2, id, type, "?", "?", "der", e->result.first, e->result.second);
  e->result.first = id;
  e->result.second = type;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitUnary_address_of(cparserParser::Unary_address_ofContext *ctx) {
  auto e = get_struct(visit(ctx->unary_expression()), __PRETTY_FUNCTION__);
  string id = new_id();
  string type = e->result.second + "*";
  e->lines_3AC.emplace_back(2, id, type, "?", "?", "adr", e->result.first, e->result.second);
  e->result.first = id;
  e->result.second = type;

  return return_3AC_struct(e);

}