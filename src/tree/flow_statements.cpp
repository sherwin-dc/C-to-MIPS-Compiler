#include "cparserDerivedVisitor.h"

using namespace cparser;

antlrcpp::Any cparserDerivedVisitor::visitIf_statement(cparserParser::If_statementContext *ctx) {
  /**
   * if (expression) then statement
   * 
   * res = expression
   * `@endif = res:t eq #0:t`
   * statement
   * `@endif`
   */

  auto exp = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
  string label=new_label("endif");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label, "?", exp->result.first, exp->result.second, "eq", "#0", exp->result.second};
  exp->lines_3AC.push_back(branch);

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  statement_3AC endif = {3, label};
  stm->lines_3AC.push_back(endif);

  auto combined = combine_structs(exp, stm, "", "");

  return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitIf_else_statement(cparserParser::If_else_statementContext *ctx) {
  
  /**
   * if (expression) statement else statement
   * 
   * res = expression
   * `@else = res:t eq0`
   * 
   * statement
   * `@endif = nop`
   * `@else`
   * 
   * statement
   * `@endif`
   */

  auto exp = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
  string label_else=new_label("else");
  //{4, @ELSE4, ?, #3, f, +, #4, f}
  statement_3AC branch = {4, label_else, "?", exp->result.first, exp->result.second, "eq", "#0", exp->result.second};
  exp->lines_3AC.push_back(branch);

  auto stm0 = get_struct(visit(ctx->statement(0)), __PRETTY_FUNCTION__);
  string label_endif = new_label("endif");
  statement_3AC endif = {4, label_endif, "?", "#0", "u", "eq", "#0", "u"};
  stm0->lines_3AC.push_back(endif);
  statement_3AC else_label = {3, label_else};
  stm0->lines_3AC.push_back(else_label);

  auto combined = combine_structs(exp, stm0, "", "");
  auto stm1 = get_struct(visit(ctx->statement(1)), __PRETTY_FUNCTION__);
  statement_3AC endif_label = {3, label_endif};
  stm1->lines_3AC.push_back(endif_label);

  auto combined2 = combine_structs(combined, stm1, "", "");
  return return_3AC_struct(combined2);
}

antlrcpp::Any cparserDerivedVisitor::visitSwitch_statement(cparserParser::Switch_statementContext *ctx) {
  /**
   * switch (expression) statement
   * 
   * #res = expression
   * @case_expr = #0 eq #0                       |
   * @switch                                     | -> repeat for each case statement present
   * @case_statement = #case_res eq #res         |
   * 
   * @default = #0 eq #0
   * @end_switch =  #0 eq #0
   * 
   * statement
   * 
   * @break                                      | -> repeat for all break statements
   * @end_switch
   */

  auto expr = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  pair<string, string> res = expr->result;

  for (int i=0; i<stm->cases.size(); i++) {
    expr->lines_3AC.emplace_back(4, stm->cases[i].case_expr_label, "?", "#0", "u", "eq", "#0", "u");
    expr->lines_3AC.emplace_back(3, stm->cases[i].switch_label);
    expr->lines_3AC.emplace_back(4, stm->cases[i].case_statement_label, "?", stm->cases[i].expr_id, stm->cases[i].expr_type, "eq", res.first, res.second);
    expr->lines_3AC.emplace_back(0);
  }
  stm->cases.clear(); // case statements consumed

  if (stm->default_label != "") {
    expr->lines_3AC.emplace_back(4, stm->default_label, "?", "#0", "u", "eq", "#0", "u");
    stm->default_label = ""; // default consumed
  }

  string end_switch = new_label("end_switch");
  expr->lines_3AC.emplace_back(4, end_switch, "?", "#0", "u", "eq", "#0", "u");
  expr->lines_3AC.emplace_back(0);

  for (int i=0; i<stm->breaks.size(); i++) {
    stm->lines_3AC.emplace_back(3, stm->breaks[i]);
  }
  stm->breaks.clear(); // consume breaks
  stm->lines_3AC.emplace_back(3, end_switch);
  stm->lines_3AC.emplace_back(0);

  auto combined = combine_structs(expr, stm, "", "");

  return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitCase_statement(cparserParser::Case_statementContext *ctx) {
  /**
   * case expr : statement
   * 
   * @case_statement = #0 eq #0
   * @case_expr
   * #1 = expr
   * @switch = #0 eq #0
   * @case_statement
   * statement
   */

  auto expr = get_struct(visit(ctx->constant_expression()), __PRETTY_FUNCTION__);
  string case_statement_label = new_label("case_statement");
  string case_expr_label = new_label("case_expr");
  string switch_label = new_label("switch");
  statement_3AC case_statement_jump = {4, case_statement_label, "?", "#0", "u", "eq", "#0", "u"};
  statement_3AC case_expr_statement = {3, case_expr_label};
  expr->lines_3AC.push_front(case_expr_statement);
  expr->lines_3AC.push_front(case_statement_jump);
  expr->lines_3AC.emplace_front(0, "// case:");
  statement_3AC switch_jump = {4, switch_label, "?", "#0", "u", "eq", "#0", "u"};
  statement_3AC case_statement = {3, case_statement_label};
  expr->lines_3AC.push_back(switch_jump);
  expr->lines_3AC.push_back(case_statement);

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  // stm->lines_3AC.emplace_back(0, "", "// end case");
  auto combined = combine_structs(expr, stm, "", "");
  case_expr e = {case_statement_label, case_expr_label, switch_label, expr->result.first, expr->result.second};
  combined->cases.push_back(e);

  return return_3AC_struct(combined);

}

antlrcpp::Any cparserDerivedVisitor::visitDefault_statement(cparserParser::Default_statementContext *ctx) {
  /**
   * default: statement
   * 
   * @default
   * statement
   */

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  string label_default = new_label("default");
  stm->lines_3AC.emplace_front(3, label_default);
  stm->lines_3AC.emplace_front(0, "// default:");
  // stm->lines_3AC.emplace_back(0, "","// end default");
  stm->default_label = label_default;

  return return_3AC_struct(stm);
}

antlrcpp::Any cparserDerivedVisitor::visitJump_statement(cparserParser::Jump_statementContext *ctx) {

  // check whether its a break; continue; or return expr;
  auto str = create_new_struct();
  if (ctx->BREAK_KEYWORD()) {
    string break_label = new_label("break");
    str->lines_3AC.emplace_back(4, break_label, "?", "#0", "u", "eq", "#0", "u");
    str->breaks.push_back(break_label);

  } else if (ctx->CONTINUE_KEYWORD()) {
    string continue_label = new_label("continue");
    str->lines_3AC.emplace_back(4, continue_label, "?", "#0", "u", "eq", "#0", "u");
    str->continues.push_back(continue_label);

  } else if (ctx->expression()) {
    auto expr = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
    expr->lines_3AC.emplace_back(5, expr->result.first, expr->result.second, "?", "?", "return");
    expr->result = make_pair("","");
    return return_3AC_struct(expr);

  } else {
    // should not be tested on void functions
    str->lines_3AC.emplace_back(5, "#0", "u", "?", "?", "return");
  }

  return return_3AC_struct(str);
}

antlrcpp::Any cparserDerivedVisitor::visitWhile_statement(cparserParser::While_statementContext *ctx) {
  /**
   * while ( expression ) statement
   * 
   * @start_while
   * res = expression
   * @end_while = res eq #0
   * statement
   * @continues
   * @start_while = #0 eq #0
   * @breaks
   * @end_while
   */

  auto expr = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
  string start_while = new_label("start_while");
  string end_while = new_label("end_while");
  expr->lines_3AC.emplace_front(3, start_while);
  expr->lines_3AC.emplace_front(0, "// while:");
  expr->lines_3AC.emplace_back(4, end_while, "?", expr->result.first, expr->result.second, "eq", "#0", expr->result.second);

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  for (int i=0; i<stm->continues.size(); i++) {
    stm->lines_3AC.emplace_back(3, stm->continues[i]);
  }
  stm->continues.clear(); // consume continues
  stm->lines_3AC.emplace_back(4, start_while, "?", "#0", "u", "eq", "#0", "u");
  
  for (int i=0; i<stm->breaks.size(); i++) {
    stm->lines_3AC.emplace_back(3, stm->breaks[i]);
  }
  stm->breaks.clear(); // consume breaks
  stm->lines_3AC.emplace_back(3, end_while);
  auto combined = combine_structs(expr, stm, "", "");

  return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitDo_while_statement(cparserParser::Do_while_statementContext *ctx) {
  /**
   * do statement while ( expression ) ;
   * 
   * @start_while
   * statement
   * @continues
   * res = expression
   * @start_while = res ne #0
   * @breaks
   */

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  auto expr = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);
  string start_while = new_label("start_while");

  stm->lines_3AC.emplace_front(3, start_while);
  stm->lines_3AC.emplace_front(0, "// do while:");
  for (int i=0; i<stm->continues.size(); i++) {
    stm->lines_3AC.emplace_back(3, stm->continues[i]);
  }
  stm->continues.clear(); // consume continues

  expr->lines_3AC.emplace_back(4, start_while, "?", expr->result.first, expr->result.second, "ne", "#0", expr->result.second);
  for (int i=0; i<stm->breaks.size(); i++) {
    expr->lines_3AC.emplace_back(3, stm->breaks[i]);
  }
  stm->breaks.clear(); // consume breaks
  auto combined = combine_structs(stm, expr, "", "");

  return return_3AC_struct(combined);

}

antlrcpp::Any cparserDerivedVisitor::visitFor_statement(cparserParser::For_statementContext *ctx) {
  /**
   * for ( exp1? ; exp2? ; exp3? ) statement
   * 
   * exp1
   * @for_loop
   * r2 = exp2
   * @end_for = r2 eq #0
   * statement
   * @continues
   * exp3
   * @for_loop = #0 eq #0
   * @breaks
   * @end_for
   * 
   */
  auto exp1 = create_new_struct();
  if (ctx->exp1) {
    exp1 = get_struct(visit(ctx->exp1), __PRETTY_FUNCTION__);
  }
  string for_loop = new_label("for_loop");
  string end_for = new_label("end_for");
  exp1->lines_3AC.emplace_back(3, for_loop);
  exp1->lines_3AC.emplace_front(0, "// for loop:");

  auto exp2 = create_new_struct();
  if (ctx->exp2) {
    exp2 = get_struct(visit(ctx->exp2), __PRETTY_FUNCTION__);
  } else {
    // if exp2 is not provided, have to replace with an integer constant
    string new_var = new_id();
    exp2->lines_3AC.emplace_back(2, new_var, "u", "?", "?", "ld", "1", "u");
    exp2->result = make_pair(new_var, "u");
  }
  exp2->lines_3AC.emplace_back(4, end_for, "?", exp2->result.first, exp2->result.second, "eq", "#0", exp2->result.second);

  auto stm = get_struct(visit(ctx->statement()), __PRETTY_FUNCTION__);
  for (int i=0; i<stm->continues.size(); i++) {
    stm->lines_3AC.emplace_back(3, stm->continues[i]);
  }
  stm->continues.clear(); // consume continues

  auto exp3 = create_new_struct();
  if (ctx->exp3) {
    exp3 = get_struct(visit(ctx->exp3), __PRETTY_FUNCTION__);
  }
  exp3->lines_3AC.emplace_back(4, for_loop, "?", "#0", "u", "eq", "#0", "u");

  for (int i=0; i<stm->breaks.size(); i++) {
    exp3->lines_3AC.emplace_back(3, stm->breaks[i]);
  }
  stm->breaks.clear(); // consume breaks
  exp3->lines_3AC.emplace_back(3, end_for);

  auto comb1 = combine_structs(exp1, exp2, "", "");
  auto comb2 = combine_structs(stm, exp3, "", "");
  auto combined = combine_structs(comb1, comb2, "", "");


  return return_3AC_struct(combined);

}

antlrcpp::Any cparserDerivedVisitor::visitExpr_in_brackets(cparserParser::Expr_in_bracketsContext *ctx) {
  return visit(ctx->expression());
}

antlrcpp::Any cparserDerivedVisitor::visitDeclarator_in_brackets(cparserParser::Declarator_in_bracketsContext *ctx) {
  return visit(ctx->declarator());
}