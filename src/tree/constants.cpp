#include "cparserDerivedVisitor.h"

using namespace cparser;

antlrcpp::Any cparserDerivedVisitor::visitDecimal_constant(cparserParser::Decimal_constantContext *ctx) {

  // create a ptr to the struct
  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

  // covert from string to base 10 number and back to string
  string number_str = ctx->DECIMAL_DIGITS()->getText();
  int number = std::strtol(number_str.c_str(), nullptr, 10);

  // check if signed or unsigned, and load into a variable
  string new_var_type;
  if (ctx->integer_suffix()) {
    if(ctx->integer_suffix()->UNSIGNED_SUFFIX()) {
      // number is unsigned
      new_var_type = "u";
    } else {
    // number is signed (default)
    new_var_type = "s";
    }
  } else {
    // number is signed (default)
    new_var_type = "s";
  }


  //? Create new 3AC statement
  // string new_var = new_id();
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  // statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", to_string(number), new_var_type};
  // new_struct->lines_3AC.push_back(new_3AC);
  new_struct->result = make_pair(to_string(number), new_var_type);
  

  // return pointer to dynamically allocated struct
  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitOctal_constant(cparserParser::Octal_constantContext *ctx) {

  // create a ptr to the struct
  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

  // covert from string to base 10 number and back to string
  string number_str = ctx->OCTAL_DIGITS()->getText();
  int number = std::strtol(number_str.c_str(), nullptr, 8);

  // check if signed or unsigned, and load into a variable
  string new_var_type;
  if (ctx->integer_suffix()) {
    if(ctx->integer_suffix()->UNSIGNED_SUFFIX()) {
      // number is unsigned
      new_var_type = "u";
    } else {
    // number is signed (default)
    new_var_type = "s";
    }
  } else {
    // number is signed (default)
    new_var_type = "s";
  }


  //? Create new 3AC statement
  // string new_var = new_id();
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  // statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", to_string(number), new_var_type};
  // new_struct->lines_3AC.push_back(new_3AC);
  new_struct->result = make_pair(to_string(number), new_var_type);
  
  // return pointer to dynamically allocated struct
  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitHex_constant(cparserParser::Hex_constantContext *ctx) {
  // create a ptr to the struct
  std::unique_ptr<tree_to_3AC> new_struct = create_new_struct();

  // covert from string to base 10 number and back to string
  string number_str = ctx->HEX_NUMBER()->getText();
  int number = std::strtol(number_str.c_str(), nullptr, 16);

  // check if signed or unsigned, and load into a variable
  string new_var_type;
  if (ctx->integer_suffix()) {
    if(ctx->integer_suffix()->UNSIGNED_SUFFIX()) {
      // number is unsigned
      new_var_type = "u";
    } else {
    // number is signed (default)
    new_var_type = "s";
    }
  } else {
    // number is signed (default)
    new_var_type = "s";
  }


  //? Create new 3AC statement
  // string new_var = new_id();
  //a:t = ld b:t
  //{2, #1, f, ?, ?, -, #4, f}
  // statement_3AC new_3AC = {2, new_var, new_var_type, "?", "?" , "ld", to_string(number), new_var_type};
  // new_struct->lines_3AC.push_back(new_3AC);
  new_struct->result = make_pair(to_string(number), new_var_type);
  
  // return pointer to dynamically allocated struct
  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitFloating_constant(cparserParser::Floating_constantContext *ctx) {
  auto s = create_new_struct();
  // string id = new_id();
  string text = ctx->FLOATING_CONSTANT()->getText();
  string type;
  string num;
  if ((text[text.size()-1]=='f') || (text[text.size()-1]=='F') /*|| (text[text.size()-1]=='L') || (text[text.size()-1]=='l') */) {
    type = "f";
    num = text.substr(0, text.size()-1);
  } else {
    type = "d";
    if ((text[text.size()-1]=='L') || (text[text.size()-1]=='l')) {
      num = text.substr(0, text.size()-1);
    } else {
      num = text;
    }
    
  }
  
  s->result = make_pair(num, type);
  // s->lines_3AC.emplace_back(2, id, type, "?", "?" , "ld", num , type);

  return return_3AC_struct(s);
}

// antlrcpp::Any cparserDerivedVisitor::visitFrac_constant2(cparserParser::Frac_constant2Context *ctx) {
//   auto s = create_new_struct();
//   // string id = new_id();
//   string type;
//   string num;
//   if (ctx->FLOATING_SUFFIX()) {
//     type = "f";
//   } else {
//     type = "d";
//   }
  

//     num = (ctx->DIGIT_SEQUENCE()->getText());
 
//   s->result = make_pair(num, type);
//   // s->lines_3AC.emplace_back(2, id, type, "?", "?" , "ld", num , type);

//   return return_3AC_struct(s);

// }

antlrcpp::Any cparserDerivedVisitor::visitCharacter_constant(cparserParser::Character_constantContext *ctx) {
  string char_with_quotes = ctx->CHARACTER_CONSTANT()->getText();
  string char_only = char_with_quotes.substr(1, char_with_quotes.size() - 2);
  signed char c;
  if (char_only.size()==2) {
    switch (char_only[1])
    {
    case '\'':
      c='\'';
      break;
    case '\?':
      c='\?';
      break;
    case '\"':
      c='\"';
      break;
    case '\\':
      c='\\';
      break;
    case '\a':
      c='\a';
      break;
    case '\b':
      c='\b';
      break;
    case '\f':
      c='\f';
      break;
    case '\n':
      c='\n';
      break;
    case '\r':
      c='\r';
      break;
    case '\t':
      c='\t';
      break;
    case '\v':
      c='\v';
      break;
    }
  } else {
    c=char_only[0];
  }

  auto new_struct = create_new_struct();
  // string id = new_id();
  int ch = c;
  // new_struct->lines_3AC.emplace_back(2, id, "c", "?", "?" , "ld", to_string(ch), "c");
  new_struct->result = make_pair(to_string(ch), "c");

  return return_3AC_struct(new_struct);
  
}

antlrcpp::Any cparserDerivedVisitor::visitConstant_top(cparserParser::Constant_topContext *ctx) {
  auto e = get_struct(visit(ctx->constant()), __PRETTY_FUNCTION__);
  string id = new_id();
  e->lines_3AC.emplace_back(2, id, e->result.second, "?", "?" , "ld", e->result.first, e->result.second);
  e->result.first = id;

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitString_literal(cparserParser::String_literalContext *ctx) {
  string text_with_quotes = ctx->STRING_LITERAL()->getText();
  string str_id = new_id();
  auto new_struct = create_new_struct();
  new_struct->lines_3AC.emplace_back(2, str_id, "g", "?", "?", "ld", text_with_quotes, "g");
  string id = new_id();
  new_struct->lines_3AC.emplace_back(2, id, "c*", "?", "?", "adr", str_id, "g");
  new_struct->result = make_pair(id, "c*");

  return return_3AC_struct(new_struct);
}

antlrcpp::Any cparserDerivedVisitor::visitEnumerator(cparserParser::EnumeratorContext *ctx) {
  auto e = get_struct(visit(ctx->enumeration_constant()), __PRETTY_FUNCTION__);
  string enum_name = e->result.first;
  e->result.second = "?";

  if (variable_bindings.count(enum_name)) {
    variable_bindings.erase(enum_name);
  }

  if (ctx->constant()) {
    auto c = get_struct(visit(ctx->constant()), __PRETTY_FUNCTION__);
    e->result.second = c->result.first; // put the constant value in the pair
  }

  return return_3AC_struct(e);
}

antlrcpp::Any cparserDerivedVisitor::visitEnum_specifier(cparserParser::Enum_specifierContext *ctx) {
  auto vec = ctx->enumerator();

  int enum_val=0;
  for (auto it : vec) {
    auto e = get_struct(visit(it), __PRETTY_FUNCTION__);
    if (e->result.second != "?") {
      enum_val = stoi(e->result.second);
    } 

    enum_bindings[e->result.first] = enum_val;
    enum_val++;
    
  }

  return 0; // doesnt return anything
}

antlrcpp::Any cparserDerivedVisitor::visitEnumeration_constant_top(cparserParser::Enumeration_constant_topContext *ctx) {
  auto e = get_struct(visit(ctx->enumeration_constant()), __PRETTY_FUNCTION__);
  string enum_name = e->result.first;

  if (enum_bindings.count(enum_name)) {
    int enum_val = enum_bindings[enum_name];
    string id = new_id();
    e->result = make_pair(id, "s");
    e->lines_3AC.emplace_back(2, id, "s", "?", "?", "ld", to_string(enum_val), "s");
  } else {
      // this is a normal identifier


        //"0" to indicate that type is not determined yet
        pair<string, string> empty_str = {enum_name,"0"};
        if(variable_bindings.find(enum_name)==variable_bindings.end()){
          variable_bindings.insert(empty_str);
        } else {
          e->result.second = variable_bindings[enum_name]; // otherwise put the type in
        }

  }

  return return_3AC_struct(e);
}