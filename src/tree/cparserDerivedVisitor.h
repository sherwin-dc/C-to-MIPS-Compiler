#include "cparserBaseVisitor.h"
#include <string>
#include <cstdlib>
#pragma once

#include "return_struct.h"

extern std::ostream& dst;

namespace cparser {

class cparserDerivedVisitor : public cparserBaseVisitor {
public:
  antlrcpp::Any visitProg(cparserParser::ProgContext *ctx) override;
  antlrcpp::Any visitTranslation_unit(cparserParser::Translation_unitContext *ctx) override;
  antlrcpp::Any visitFunction_definition(cparserParser::Function_definitionContext *ctx) override;
  antlrcpp::Any visitDeclaration_label(cparserParser::Declaration_labelContext *ctx) override;
  //antlrcpp::Any visitFunction_definition_top(cparserParser::Function_definition_topContext *ctx) override;
  //antlrcpp::Any visitDeclaration_label(cparserParser::Declaration_labelContext *ctx) override;
  antlrcpp::Any visitBlock_item_list(cparserParser::Block_item_listContext *ctx) override;
  antlrcpp::Any visitExpression_statement(cparserParser::Expression_statementContext *ctx) override;
  antlrcpp::Any visitCompound_statement(cparserParser::Compound_statementContext *ctx) override;
  // antlrcpp::Any visitDeclaration(cparserParser::DeclarationContext *ctx) override;
  antlrcpp::Any visitDeclaration_list(cparserParser::Declaration_listContext *ctx) override;

  //struct
  antlrcpp::Any visitStruct_specifier(cparserParser::Struct_specifierContext *ctx) override;
  // antlrcpp::Any visitStruct_declaration_list(cparserParser::Struct_declaration_listContext *ctx) override;
  // antlrcpp::Any visitStruct_declarator_list(cparserParser::Struct_declarator_listContext *ctx) override;
  antlrcpp::Any visitStruct_member_variables(cparserParser::Struct_member_variablesContext *ctx) override;
  antlrcpp::Any visitPointer_struct_member_variables(cparserParser::Pointer_struct_member_variablesContext *ctx) override;

  //? Functions
  antlrcpp::Any visitFn_declarator_params(cparserParser::Fn_declarator_paramsContext *ctx) override;
  antlrcpp::Any visitFn_call(cparserParser::Fn_callContext *ctx) override;
  antlrcpp::Any visitFunction_params(cparserParser::Function_paramsContext *ctx) override;
  antlrcpp::Any visitParameter_list(cparserParser::Parameter_listContext *ctx) override;
  antlrcpp::Any visitArgument_expression_list(cparserParser::Argument_expression_listContext *ctx) override;

  //? Flow statements
  antlrcpp::Any visitSwitch_statement(cparserParser::Switch_statementContext *ctx) override;
  antlrcpp::Any visitCase_statement(cparserParser::Case_statementContext *ctx) override;
  antlrcpp::Any visitDefault_statement(cparserParser::Default_statementContext *ctx) override;
  antlrcpp::Any visitJump_statement(cparserParser::Jump_statementContext *ctx) override;
  antlrcpp::Any visitWhile_statement(cparserParser::While_statementContext *ctx) override;
  antlrcpp::Any visitDo_while_statement(cparserParser::Do_while_statementContext *ctx) override;
  antlrcpp::Any visitFor_statement(cparserParser::For_statementContext *ctx) override;
  antlrcpp::Any visitIf_statement(cparserParser::If_statementContext *ctx) override;
  antlrcpp::Any visitIf_else_statement(cparserParser::If_else_statementContext *ctx) override;
  antlrcpp::Any visitExpr_in_brackets(cparserParser::Expr_in_bracketsContext *ctx) override;
  antlrcpp::Any visitDeclarator_in_brackets(cparserParser::Declarator_in_bracketsContext *ctx) override;

  antlrcpp::Any visitDecimal_constant(cparserParser::Decimal_constantContext *ctx) override;
  antlrcpp::Any visitOctal_constant(cparserParser::Octal_constantContext *ctx) override;
  antlrcpp::Any visitHex_constant(cparserParser::Hex_constantContext *ctx) override;
  antlrcpp::Any visitCharacter_constant(cparserParser::Character_constantContext *ctx) override;
  antlrcpp::Any visitFloating_constant(cparserParser::Floating_constantContext *ctx) override;
  // antlrcpp::Any visitFrac_constant2(cparserParser::Frac_constant2Context *ctx) override;
  antlrcpp::Any visitConstant_top(cparserParser::Constant_topContext *ctx) override;
  antlrcpp::Any visitString_literal(cparserParser::String_literalContext *ctx) override;
  antlrcpp::Any visitEnumerator(cparserParser::EnumeratorContext *ctx) override;
  antlrcpp::Any visitEnum_specifier(cparserParser::Enum_specifierContext *ctx) override;
  antlrcpp::Any visitEnumeration_constant_top(cparserParser::Enumeration_constant_topContext *ctx) override;

  //arithmetic expressions
  antlrcpp::Any visitAddition_expression(cparserParser::Addition_expressionContext *ctx) override;
  antlrcpp::Any visitSubtraction_expression(cparserParser::Subtraction_expressionContext *ctx) override;
  antlrcpp::Any visitMultiplication_expression(cparserParser::Multiplication_expressionContext *ctx) override;
  antlrcpp::Any visitDivision_expression(cparserParser::Division_expressionContext *ctx) override;
  antlrcpp::Any visitMod_expression(cparserParser::Mod_expressionContext *ctx) override;
  antlrcpp::Any visitUnary_minus(cparserParser::Unary_minusContext *ctx) override;

  antlrcpp::Any visitPost_increment(cparserParser::Post_incrementContext *ctx) override;
  antlrcpp::Any visitPost_decrement(cparserParser::Post_decrementContext *ctx) override;
  antlrcpp::Any visitPre_increment(cparserParser::Pre_incrementContext *ctx) override;
  antlrcpp::Any visitPre_decrement(cparserParser::Pre_decrementContext *ctx) override;

  antlrcpp::Any visitUnary_dereference(cparserParser::Unary_dereferenceContext *ctx) override;
  antlrcpp::Any visitUnary_address_of(cparserParser::Unary_address_ofContext *ctx) override;
  antlrcpp::Any visitSizeof_expr(cparserParser::Sizeof_exprContext *ctx) override;
  antlrcpp::Any visitSizeof_type(cparserParser::Sizeof_typeContext *ctx) override;

  antlrcpp::Any visitArray_indexing(cparserParser::Array_indexingContext *ctx) override;
  antlrcpp::Any visitArray_declaration(cparserParser::Array_declarationContext *ctx) override;
  antlrcpp::Any visitArray_initialise_values(cparserParser::Array_initialise_valuesContext *ctx) override;
  antlrcpp::Any visitArray_initialise_values_global(cparserParser::Array_initialise_values_globalContext *ctx) override;

  //logic expressions
  antlrcpp::Any visitLogical_or(cparserParser::Logical_orContext *ctx) override;
  antlrcpp::Any visitLogical_and(cparserParser::Logical_andContext *ctx) override;
  antlrcpp::Any visitOr_expr(cparserParser::Or_exprContext *ctx) override;
  antlrcpp::Any visitXor_expr(cparserParser::Xor_exprContext *ctx) override;
  antlrcpp::Any visitAnd_expr(cparserParser::And_exprContext *ctx) override;
  antlrcpp::Any visitEquality(cparserParser::EqualityContext *ctx) override;
  antlrcpp::Any visitNonequality(cparserParser::NonequalityContext *ctx) override;
  antlrcpp::Any visitLogical_not_expression(cparserParser::Logical_not_expressionContext *ctx) override;
  antlrcpp::Any visitNot_expression(cparserParser::Not_expressionContext *ctx) override;
  antlrcpp::Any visitTernary_conditional_expr(cparserParser::Ternary_conditional_exprContext *ctx) override;
  //relational expressions
  antlrcpp::Any visitLess_than_expr(cparserParser::Less_than_exprContext *ctx) override;
  antlrcpp::Any visitMore_than_expr(cparserParser::More_than_exprContext *ctx) override;
  antlrcpp::Any visitLess_than_equal_expr(cparserParser::Less_than_equal_exprContext *ctx) override;
  antlrcpp::Any visitMore_than_equal_expr(cparserParser::More_than_equal_exprContext *ctx) override;
  //shift expressions
  antlrcpp::Any visitLeft_shift(cparserParser::Left_shiftContext *ctx) override;
  antlrcpp::Any visitRight_shift(cparserParser::Right_shiftContext *ctx) override;

  antlrcpp::Any visitIdentifier_base(cparserParser::Identifier_baseContext *ctx) override;
  antlrcpp::Any visitSuffix_as_id(cparserParser::Suffix_as_idContext *ctx) override;
  antlrcpp::Any visitSuffix_as_id_td(cparserParser::Suffix_as_id_tdContext *ctx) override;
  antlrcpp::Any visitAssign_value_expr(cparserParser::Assign_value_exprContext *ctx) override;
  antlrcpp::Any visitAssign_value_constant(cparserParser::Assign_value_constantContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_eq(cparserParser::Assignment_expr_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_plus_eq(cparserParser::Assignment_expr_plus_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_minus_eq(cparserParser::Assignment_expr_minus_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_times_eq(cparserParser::Assignment_expr_times_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_div_eq(cparserParser::Assignment_expr_div_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_mod_eq(cparserParser::Assignment_expr_mod_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_and_eq(cparserParser::Assignment_expr_and_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_xor_eq(cparserParser::Assignment_expr_xor_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_or_eq(cparserParser::Assignment_expr_or_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_rightshift_eq(cparserParser::Assignment_expr_rightshift_eqContext *ctx) override;
  antlrcpp::Any visitAssignment_expr_leftshift_eq(cparserParser::Assignment_expr_leftshift_eqContext *ctx) override;

  antlrcpp::Any visitType_name(cparserParser::Type_nameContext *ctx) override;
  antlrcpp::Any visitType_specifier_top(cparserParser::Type_specifier_topContext *ctx) override;
  antlrcpp::Any visitDirect_declarator_top(cparserParser::Direct_declarator_topContext *ctx) override;
  antlrcpp::Any visitDeclarator_top(cparserParser::Declarator_topContext *ctx) override;
  antlrcpp::Any visitLone_declaration_specifier(cparserParser::Lone_declaration_specifierContext *ctx) override;
};

}