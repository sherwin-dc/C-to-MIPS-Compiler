grammar cparser;

@parser::postinclude { 
  #include <unordered_map>
  #include <string>
  #include <iostream>
  extern std::unordered_map<std::string, std::string> typedef_bindings; 
}
@parser::members {

  bool is_typedefed_id() {
    if (typedef_bindings.count(_input->LT(1)->getText())) { 
      // std::cerr << _input->LT(1)->getText() << " has been typedefed" << std::endl;
      return true; 
      
    } else { 
      // std::cerr << _input->LT(1)->getText() << " has not been typedefed" << std::endl;
      return false; 
    }
  }

}

prog : translation_unit EOF;


/* Most of these names and definitions copied from ISO standard for C, ISO 9899 Annex A */

translation_unit
  : external_declaration+
  ;

block_item
  : statement #statement_top 
  | declaration #declaration_top 
  ;

external_declaration
  : function_definition #function_definition_top
  | declaration #declaration_label
  ;

function_definition
  : parameter_declaration compound_statement //took out declaration_list, because seems like K&R style of specifying functions
  ; // since parameter_declaration = declaration_specifiers + declarator anyway

declaration_specifiers
  : type_specifier+ #type_specifier_top//e.g. int
  //to add in typedef stuff later (storage_class_specifier)
  ;

compound_statement
  : OPENBRACE (block_item_list)? CLOSEBRACE
  ;

block_item_list
  : block_item+
  ;



declaration_list
  : declaration+
  ;

/* note: expression different from expressions */

statement
  : selection_statement
  | expression_statement
  | compound_statement
  | iteration_statement
  | jump_statement
  | labelled_statement
  ;

expression_statement
  : expression? SEMICOLON
  ;

expression
  : assignment_expression
  ;

postfix_expression
  : primary_expression #primary_expression_top
  | postfix_expression LBRACKET (argument_expression_list)? RBRACKET #fn_call
  | postfix_expression PLUS_PLUS #post_increment
  | postfix_expression MINUS_MINUS #post_decrement
  | postfix_expression LSQBRACKET expression RSQBRACKET #array_indexing
  | postfix_expression DOT identifier #struct_member_variables
  | postfix_expression ARROW identifier #pointer_struct_member_variables
  ;

argument_expression_list
  : assignment_expression (COMMA assignment_expression)*
  // | argument_expression_list COMMA assignment_expression
  // | assignment_expression
  ;

primary_expression
  : constant #constant_top
  | enumeration_constant #enumeration_constant_top
  | identifier #identifier_expr
  | string_literal #string_literal_top
  | LBRACKET expression RBRACKET #expr_in_brackets
  ;

constant
  : integer_constant #integer_constant_top
  | floating_constant #floating_constant_top
  | character_constant #character_constant_top
  ;

iteration_statement
  : WHILE_KEYWORD LBRACKET expression RBRACKET statement # while_statement
  | DO_KEYWORD statement WHILE_KEYWORD LBRACKET expression RBRACKET SEMICOLON #do_while_statement
  | FOR_KEYWORD LBRACKET exp1=expression? SEMICOLON exp2=expression? SEMICOLON exp3=expression? RBRACKET statement # for_statement 
  ; // note that c90 can only do for(i=0;i<a;i++). Cannot declare int i inside.

jump_statement
  : CONTINUE_KEYWORD SEMICOLON // jump to end of loop inside: while, do_while, for
  | BREAK_KEYWORD SEMICOLON // while, do_while, for
  | RETURN_KETWORD expression? SEMICOLON
  ;

labelled_statement
  : CASE_KEYWORD constant_expression COLON statement #case_statement
  | DEFAULT_KEYWORD COLON statement #default_statement
  ;

selection_statement
  : IF_KEYWORD LBRACKET expression RBRACKET statement # if_statement
  | IF_KEYWORD LBRACKET expression RBRACKET statement ELSE_KEYWORD statement # if_else_statement
  | SWITCH_KEYWORD LBRACKET expression RBRACKET statement # switch_statement
  ;

constant_expression
  : conditional_expression
  ;


/* Note: Lexer rules start with uppercase letters, and parser rules with lowercase */
// arith_expression
//   : logical_or_expression
//   ;

//logical_or_expression will already become assignment-expression (all the way down below)
/*precedence of logical operators */
assignment_expression
  : unary_expression EQUAL assignment_expression #assignment_expr_eq
  | unary_expression PLUS_EQUAL assignment_expression #assignment_expr_plus_eq
  | unary_expression MINUS_EQUAL assignment_expression #assignment_expr_minus_eq
  | unary_expression TIMES_EQUAL assignment_expression #assignment_expr_times_eq
  | unary_expression DIVIDE_EQUAL assignment_expression #assignment_expr_div_eq
  | unary_expression MOD_EQUAL assignment_expression #assignment_expr_mod_eq
  | unary_expression AND_EQUAL assignment_expression #assignment_expr_and_eq
  | unary_expression XOR_EQUAL assignment_expression #assignment_expr_xor_eq
  | unary_expression OR_EQUAL assignment_expression #assignment_expr_or_eq
  | unary_expression '>>=' assignment_expression #assignment_expr_rightshift_eq
  | unary_expression '<<=' assignment_expression #assignment_expr_leftshift_eq
  | conditional_expression #conditional_expression_top
  ;

conditional_expression
  : logical_or_expression '?' expression ':' conditional_expression #ternary_conditional_expr
  | logical_or_expression #logical_or_top
  ;

logical_or_expression
  : logical_or_expression LOG_OR logical_and_expression #logical_or
  | logical_and_expression #logical_and_top
  ;

logical_and_expression
  : logical_and_expression LOG_AND or_expression #logical_and
  | or_expression #or_top
  ;

or_expression
  : or_expression OR xor_expression #or_expr
  | xor_expression #xor_top
  ;

xor_expression
  : xor_expression XOR and_expression #xor_expr
  | and_expression #and_top
  ;

and_expression
  : and_expression AND equality_expression #and_expr
  | equality_expression #equality_top
  ;

equality_expression
  : equality_expression EQUALS_TO relational_expression #equality
  | equality_expression NOT_EQUALS_TO relational_expression #nonequality
  | relational_expression #additive_top
  ;

relational_expression
  : relational_expression '>=' shift_expression #more_than_equal_expr
  | relational_expression '<=' shift_expression #less_than_equal_expr
  | relational_expression '>' shift_expression #more_than_expr
  | relational_expression '<' shift_expression #less_than_expr
  | shift_expression #shift_expression_top
  ;

shift_expression
  : shift_expression '<<' additive_expression #left_shift
  | shift_expression '>>' additive_expression #right_shift
  | additive_expression #additive_expression_top
  ;

/*ARITHMETIC */
additive_expression
  : additive_expression PLUS multiplicative_expression #addition_expression
  | additive_expression MINUS multiplicative_expression #subtraction_expression
  | multiplicative_expression #multiplication_expression_top
  ;

multiplicative_expression
  : multiplicative_expression STAR unary_expression #multiplication_expression
  | unary_expression DIVIDE multiplicative_expression #division_expression
  | multiplicative_expression MOD unary_expression # mod_expression
  | unary_expression #un_statement
  ;

unary_expression
  //: UNARY_OPERATOR unary_expression #un_expression
  : NEGATE unary_expression #logical_not_expression
  | NOT unary_expression #not_expression
  | postfix_expression #postfix_label
  | STAR unary_expression #unary_dereference
  | AND unary_expression #unary_address_of
  | MINUS unary_expression #unary_minus
  | SIZEOF_KEYWORD unary_expression #sizeof_expr
  | SIZEOF_KEYWORD LBRACKET type_name RBRACKET #sizeof_type
  | PLUS_PLUS unary_expression #pre_increment
  | MINUS_MINUS unary_expression #pre_decrement
  ;

type_name
  : type_specifier+ STAR*
  ;

type_specifier //from specifier_qualifier_list in annex
  : INT_KEYWORD
  | UNSIGNED_KEYWORD
  | SIGNED_KEYWORD
  | VOID_KEYWORD
  | CHAR_KEYWORD
  | FLOAT_KEYWORD
  | DOUBLE_KEYWORD
  | enum_specifier
  | struct_specifier
  | TYPEDEF_KEYWORD
  | typedefed_identifier // this is for typedefed keywords
  ;

enum_specifier
  : ENUM_KEYWORD identifier? OPENBRACE enumerator (COMMA enumerator)* COMMA? CLOSEBRACE
  | ENUM_KEYWORD identifier
  ;

enumerator
  : enumeration_constant (EQUAL constant)?
  ;

struct_specifier
  : STRUCT_KEYWORD identifier? (OPENBRACE (parameter_declaration SEMICOLON)+ CLOSEBRACE)
  | STRUCT_KEYWORD identifier
  ;

// struct_declaration_list
//   : type_specifier+ struct_declarator_list SEMICOLON
//   ;

// struct_declarator_list
//   : struct_declarator (COMMA struct_declarator)*
//   ;

// struct_declarator
//   // : (declarator)? COLON constant_expression
//   : declarator
//   ;

/*Variable Declaration */
declaration
  : parameter_declaration EQUAL constant SEMICOLON #assign_value_constant//e.g int a = 10 , needed for global variables
  | parameter_declaration EQUAL initialiser SEMICOLON #assign_value_expr//e.g int a = b+c
  | parameter_declaration EQUAL OPENBRACE constant (COMMA constant)* CLOSEBRACE SEMICOLON #array_initialise_values_global
  | parameter_declaration EQUAL OPENBRACE initialiser (COMMA initialiser)* CLOSEBRACE SEMICOLON #array_initialise_values
  | parameter_declaration SEMICOLON #declarator_top//e.g int** a; int b[5];
  | declaration_specifiers SEMICOLON #lone_declaration_specifier // not sure if this will appear eg. int; double; //more for struct declaration e.g. struct{...};
  ;

/* NOTE: Changed the below to the above (should be equivalent) */

// declaration
//   : declaration_specifiers (init_declarator_list)? SEMICOLON
//   ;

// init_declarator_list
//   : init_declarator #init_declarator_top
//   ;

// init_declarator
//   : declarator EQUAL initialiser #assign_value_expr//e.g a = 10
//   | declarator #declarator_top//e.g a
//   ;

declarator
  : STAR* direct_declarator #direct_declarator_top
  ;

direct_declarator
  : direct_declarator LSQBRACKET integer_constant RSQBRACKET  #array_declaration
  | direct_declarator LBRACKET (parameter_type_list)? RBRACKET #fn_declarator_params//e.g. function parameters
  //| direct_declarator LBRACKET (identifier_list)? RBRACKET #fn_declarator //this is more of K&R type fns
  | identifier #fn_identifier
  | LBRACKET declarator RBRACKET #declarator_in_brackets
  ;

parameter_type_list
  : parameter_list #parameter_list_top
  ;

parameter_list
  : parameter_declaration (COMMA parameter_declaration)*
  // | parameter_list COMMA parameter_declaration
  // | parameter_declaration
  ;

parameter_declaration
  : declaration_specifiers declarator #function_params//e,g, int a; int a(...); int a[...]
  ;

identifier_list
  : identifier
  ;

initialiser
  : assignment_expression #assignment_expression_top
  //to add in stuff like initialiser for arrays - {...}
  ;

// assignment_operator
//   : EQUAL
//   | PLUS_EQUAL
//   | MINUS_EQUAL
//   | TIMES_EQUAL
//   | DIVIDE_EQUAL
//   | MOD_EQUAL
//   | AND_EQUAL
//   | XOR_EQUAL
//   | OR_EQUAL
//   | '>>='
//   | '<<='
//   ;

enumeration_constant
  : identifier
  ;

typedefed_identifier
  : { is_typedefed_id() }? IDENTIFIER #suffix_as_id_td
  //| UNIVERSAL_CHAR_NAME
  | { is_typedefed_id() }? UNSIGNED_SUFFIX #suffix_as_id_td
  | { is_typedefed_id() }? LONG_SUFFIX #suffix_as_id_td
  | { is_typedefed_id() }? LONG_LONG_SUFFIX #suffix_as_id_td
  | { is_typedefed_id() }? FLOATING_SUFFIX #suffix_as_id_td
  ;

/*Identifiers */
identifier
  : IDENTIFIER #identifier_base
  //| UNIVERSAL_CHAR_NAME
  | UNSIGNED_SUFFIX #suffix_as_id
  | LONG_SUFFIX #suffix_as_id
  | LONG_LONG_SUFFIX #suffix_as_id
  | FLOATING_SUFFIX #suffix_as_id
  ;

floating_constant
  : FLOATING_CONSTANT
  ;

FLOATING_CONSTANT
  : (DIGIT_SEQUENCE)? DOT DIGIT_SEQUENCE (FLOATING_SUFFIX)?
  | DIGIT_SEQUENCE DOT (FLOATING_SUFFIX)?
  ;

character_constant
  : CHARACTER_CONSTANT
  ;

string_literal
  : STRING_LITERAL
  ;

CHARACTER_CONSTANT
  : SINGLE_QUOTE (SIMPLE_ESCAPE_SEQUENCE | .) SINGLE_QUOTE
  ;

STRING_LITERAL
  : DOUBLE_QUOTE (SIMPLE_ESCAPE_SEQUENCE | ~('"' | '\\') )* DOUBLE_QUOTE
  ;

integer_constant
  : DECIMAL_DIGITS (integer_suffix)? # decimal_constant
  | OCTAL_DIGITS (integer_suffix)? # octal_constant
  | HEX_NUMBER (integer_suffix)? # hex_constant
  ; 



integer_suffix
  : UNSIGNED_SUFFIX (LONG_SUFFIX)?
  | UNSIGNED_SUFFIX LONG_LONG_SUFFIX
  | LONG_SUFFIX (UNSIGNED_SUFFIX)?
  | LONG_LONG_SUFFIX (UNSIGNED_SUFFIX)?
  ;
  
UNSIGNED_SUFFIX:                ('u'|'U');
LONG_SUFFIX:                    ('l'|'L');
LONG_LONG_SUFFIX:               ('ll'|'LL');
HEXADECIMAL_PREFIX:             '0' ('x'|'X');
FLOATING_SUFFIX:                ('f'|'l'|'F'|'L');

/* digits */

OCTAL_DIGITS:             '0'[0-7]*;
DECIMAL_DIGITS:           [1-9][0-9]*;
HEX_NUMBER:               HEXADECIMAL_PREFIX [0-9a-fA-F]+;
fragment DIGIT_SEQUENCE:           [0-9]+;
fragment SIMPLE_ESCAPE_SEQUENCE:   ( '\\\'' | '\\"' | '\\?' | '\\\\' | '\\a' | '\\b' | '\\f' | '\\n' | '\\r' | '\\t' | '\\v' );

IF_KEYWORD: 'if';
ELSE_KEYWORD: 'else';
SWITCH_KEYWORD: 'switch';
DO_KEYWORD: 'do';
WHILE_KEYWORD: 'while';
FOR_KEYWORD: 'for';
CONTINUE_KEYWORD: 'continue';
BREAK_KEYWORD: 'break';
RETURN_KETWORD: 'return';
CASE_KEYWORD: 'case';
DEFAULT_KEYWORD: 'default';

/*arithmetic operators*/
STAR	 :	'*';
DIVIDE	 :	'/';
MOD		 :	'%';
ARROW	 :	'->';
MINUS_MINUS	 :	'--';
PLUS_PLUS	 :	'++';
PLUS	 :	'+';
MINUS	 :	'-';
LBRACKET :	'(';
RBRACKET :	')';
LSQBRACKET: '[';
RSQBRACKET: ']';
OPENBRACE : '{';
CLOSEBRACE : '}';
EQUAL	 :  '=';
PLUS_EQUAL : '+=';
MINUS_EQUAL : '-=';
TIMES_EQUAL : '*=';
DIVIDE_EQUAL : '/=';
MOD_EQUAL	 : '%=';
AND   : '&';
OR    : '|';
XOR   : '^';
LOG_AND : '&&';
LOG_OR  : '||';
EQUALS_TO : '==';
NOT_EQUALS_TO : '!=';
AND_EQUAL     : '&=';
XOR_EQUAL   : '^=';
OR_EQUAL    : '|=';
LESS_THAN   : '<';
COMMA     : ',';
SEMICOLON : ';';
NEGATE : '!';
NOT : '~';
DOT: '.';
COLON : ':';
SINGLE_QUOTE: '\'';
DOUBLE_QUOTE: '"';


//UNARY_OPERATOR : [+-]; // to add in &* later on
 
SIZEOF_KEYWORD: 'sizeof';

INT_KEYWORD	: 'int';
UNSIGNED_KEYWORD  : 'unsigned';
SIGNED_KEYWORD  : 'signed';
VOID_KEYWORD : 'void';
CHAR_KEYWORD : 'char';
FLOAT_KEYWORD : 'float';
DOUBLE_KEYWORD : 'double';
ENUM_KEYWORD : 'enum';
STRUCT_KEYWORD : 'struct';
TYPEDEF_KEYWORD : 'typedef';
IDENTIFIER	: [a-zA-Z_][a-zA-Z0-9_]*; //variable name

//UNIVERSAL_CHAR_NAME : [('\'u)('\'U)]HEX_DIGITS;

LINE_COMMENT: '//' .*? NEWLINE_CHARS -> skip;
MULTI_COMMENT: '/*' .*? '*/' ->skip;
NEWLINE: NEWLINE_CHARS->skip;
fragment NEWLINE_CHARS: ('\n' '\r' | '\r' | '\n')+ ;
WHITESPACE: ' '+ -> skip;
TAB: '\t'+ -> skip;


