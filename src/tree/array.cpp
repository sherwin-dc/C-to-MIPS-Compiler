#include "cparserDerivedVisitor.h"

using namespace cparser;


antlrcpp::Any cparserDerivedVisitor::visitArray_declaration(cparserParser::Array_declarationContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->direct_declarator()), __PRETTY_FUNCTION__);

    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->integer_constant()), __PRETTY_FUNCTION__);

    string new_var = e1->result.first;
    //size of array
    int array_size;
    try {
      array_size = stoi(e2->result.first.substr(0, string::npos));
    } catch (std::invalid_argument) {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Cannot create array with size " << e2->result.first << endl;
    }

    //doesnt know type of array yet
    if (array_bindings.count(new_var)) {
      array_bindings[new_var] = to_string(array_size) + "-" + array_bindings[new_var];
    } else {
      pair<string, string> uninitialised_array = {new_var, to_string(array_size)};
    
      array_bindings.insert(uninitialised_array);
    }
    
    //not possible to be both a variable and an array
    // variable_bindings.erase(new_var);

    
    // combine the structs to get a new dynamically allocated one. e1 first, then e2 second
    // std::unique_ptr<tree_to_3AC> new_struct = combine_structs(e1, e2, new_var, e2->result.second);

    return return_3AC_struct(e1);
}

antlrcpp::Any cparserDerivedVisitor::visitArray_indexing(cparserParser::Array_indexingContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);

    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->expression()), __PRETTY_FUNCTION__);

    

    string e2_var = e2->result.first;
    string e2_var_type = e2->result.second;
    string e1_var = e1->result.first;
    string e1_var_type = e1->result.second;

    // check which is the array/pointer
    string id, other_id, type, other_type;
    if ((e2_var_type[e2_var_type.size()-1]=='*') || (char_is_digit(e2_var_type[e2_var_type.size()-1]))) {
      type = e2_var_type; id = e2_var; other_type = e1_var_type; other_id = e1_var;
    } else if ((e1_var_type[e1_var_type.size()-1]=='*') || (char_is_digit(e1_var_type[e1_var_type.size()-1]))) {
      type = e1_var_type; id = e1_var; other_type = e2_var_type; other_id = e2_var;
    }

    // get index of first non digit from the back
    int idx=type.size()-1;
    for ( ; idx>=0; idx--) {
      if (!char_is_digit(type[idx])) {
        break;
      }
    }
    // remove any '-' if present
    if (type[idx]=='-') {idx--;};

    // remove any '*' if present, provided that type was not an array in the first place
    if ((type[idx]=='*') && !char_is_digit(type[type.size()-1])) {idx--;};

    string base_type = type.substr(0, idx+1);

    std::unique_ptr<tree_to_3AC> combined = combine_structs(e1, e2, id, type);
    string n_id1 = new_id();
    string n_id2 = new_id();
    // if (char_is_digit(base_type[base_type.size()-1])) {
    //   combined->lines_3AC.emplace_back(1,n_id2, base_type, id, type, "+", e2_var, e2_var_type);
    // } else {
      combined->lines_3AC.emplace_back(1,n_id1, base_type+"*", id, type, "+", other_id, other_type);
      combined->lines_3AC.emplace_back(2, n_id2, base_type, "?", "?", "der", n_id1, base_type+"*");
      
    // }
    combined->result = make_pair(n_id2, base_type);

    return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitArray_initialise_values(cparserParser::Array_initialise_valuesContext *ctx) {
  //? Get the size of the array
  auto d = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);
  string type = d->result.second;
  // check if array or struct
  if (char_is_digit(type[type.size()-1])) {

    string base_type = type.substr(0,1);
    int data_size=get_array_base_size(type);
    int byte_size=get_array_whole_size(type);
    
    int array_size = byte_size / data_size;
    if (array_size < 1) {
      cerr << "Error in " << __PRETTY_FUNCTION__ << " Array size less than 1. byte size: " << byte_size << " data_size: " << data_size << endl;
    }


      auto vec = ctx->initialiser();
      int no_of_initialisers = vec.size();
      vector<pair<string, string>> initialiser_vars;
      std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
      initialiser_vars.push_back(e1->result);
      std::unique_ptr<tree_to_3AC> tmp;

      for (int i=1; i<vec.size() ; i++) {
          std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
          initialiser_vars.push_back(e2->result);
          tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
          std::swap(tmp, e1);
      }

      auto combined = combine_structs(e1, d, "", "");
      string first_element = new_id();
      combined->lines_3AC.emplace_back(2, first_element, base_type, "?", "?", "cp", initialiser_vars[0].first, initialiser_vars[0].second);
      for (int i=1; i<initialiser_vars.size(); i++) {
        combined->lines_3AC.emplace_back(2, new_id(), base_type, "?", "?", "cp", initialiser_vars[i].first, initialiser_vars[i].second);
      }
      // initialise any empty elements
      for (int i=0; i<array_size-no_of_initialisers; i++) {
        combined->lines_3AC.emplace_back(2, new_id(), base_type, "?", "?", "ld", "0", base_type);
      }
      combined->lines_3AC.emplace_back(2, d->result.first, type, "?", "?", "adr", first_element, base_type);
      combined->result = d->result;
      return return_3AC_struct(combined);
      
  } else if (type[type.size()-1]=='$') {
    string str_name = type.substr(1, type.size()-2);
    int byte_size = struct_bindings[str_name].struct_size;

      auto vec = ctx->initialiser();

      vector<pair<string, string>> initialiser_vars;
      std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
      initialiser_vars.push_back(e1->result);
      std::unique_ptr<tree_to_3AC> tmp;

      for (int i=1; i<vec.size() ; i++) {
          std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
          initialiser_vars.push_back(e2->result);
          tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
          std::swap(tmp, e1);
      }

      int byte_offset = 0;
      auto combined = combine_structs(e1, d, "", "");
      // place empty double to force double-aligned
      combined->lines_3AC.emplace_back(2, new_id(), "d", "?", "?", "ld", "0","d");
      string first_element = new_id();
      combined->lines_3AC.emplace_back(2, first_element, initialiser_vars[0].second, "?", "?", "cp", initialiser_vars[0].first, initialiser_vars[0].second);
      byte_offset += get_array_base_size(initialiser_vars[0].second);

      for (int i=1; i<initialiser_vars.size(); i++) {
        int base_type_size = get_array_base_size(initialiser_vars[i].second);
        while (byte_offset % base_type_size) {
          combined->lines_3AC.emplace_back(2, new_id(), "c", "?", "?", "ld", "0", "c");
          byte_offset++;
        }
        combined->lines_3AC.emplace_back(2, new_id(), initialiser_vars[i].second, "?", "?", "cp", initialiser_vars[i].first, initialiser_vars[i].second);
        byte_offset += get_array_base_size(initialiser_vars[i].second);
      }

      while (byte_offset < byte_size) {
        combined->lines_3AC.emplace_back(2, new_id(), "c", "?", "?", "ld", "0", "c");
        byte_offset++;
      }

      // string char_id = new_id();
      // combined->lines_3AC.emplace_back(2, char_id, initialiser_vars[0].second + "*", "?", "?", "adr", first_element, initialiser_vars[0].second);
      combined->lines_3AC.emplace_back(2, d->result.first, type, "?", "?", "asc", first_element, initialiser_vars[0].second);
      return return_3AC_struct(combined);
  }
  return 0;
}

antlrcpp::Any cparserDerivedVisitor::visitArray_initialise_values_global(cparserParser::Array_initialise_values_globalContext *ctx) {
  //? Get the size of the array
  auto d = get_struct(visit(ctx->parameter_declaration()), __PRETTY_FUNCTION__);
  string type = d->result.second;
  if (char_is_digit(type[type.size()-1])) {
    string base_type = type.substr(0,1);
    int data_size=get_array_base_size(type);
    int byte_size=get_array_whole_size(type);
    
    int array_size = byte_size / data_size;
    if (array_size < 1) {
      cerr << "Error in " << __PRETTY_FUNCTION__ << " Array size less than 1. byte size: " << byte_size << " data_size: " << data_size << endl;
    }



      auto vec = ctx->constant();
      int no_of_initialisers = vec.size();
      vector<pair<string, string>> initialiser_vars;
      std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
      initialiser_vars.push_back(e1->result);
      std::unique_ptr<tree_to_3AC> tmp;

      for (int i=1; i<vec.size() ; i++) {
          std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
          initialiser_vars.push_back(e2->result);
          tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
          std::swap(tmp, e1);
      }

      auto combined = combine_structs(e1, d, "", "");
      string first_element = new_id();
      combined->lines_3AC.emplace_back(2, first_element, base_type, "?", "?", "ld", initialiser_vars[0].first, initialiser_vars[0].second);
      for (int i=1; i<initialiser_vars.size(); i++) {
        combined->lines_3AC.emplace_back(2, new_id(), base_type, "?", "?", "ld", initialiser_vars[i].first, initialiser_vars[i].second);
      }
      // initialise any empty elements
      for (int i=0; i<array_size-no_of_initialisers; i++) {
        combined->lines_3AC.emplace_back(2, new_id(), base_type, "?", "?", "ld", "0", base_type);
      }
      combined->lines_3AC.emplace_back(2, d->result.first, type, "?", "?", "adr", first_element, base_type);
      combined->result = d->result;
      return return_3AC_struct(combined);


  } else if (type[type.size()-1]=='$') {
    
    string str_name = type.substr(1, type.size()-2);
    int byte_size = struct_bindings[str_name].struct_size;

      auto vec = ctx->constant();

      vector<pair<string, string>> initialiser_vars;
      std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
      initialiser_vars.push_back(e1->result);
      std::unique_ptr<tree_to_3AC> tmp;

      for (int i=1; i<vec.size() ; i++) {
          std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
          initialiser_vars.push_back(e2->result);
          tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
          std::swap(tmp, e1);
      }

      int byte_offset = 0;
      auto combined = combine_structs(e1, d, "", "");
      // place empty double to force double-aligned
      combined->lines_3AC.emplace_back(2, new_id(), "d", "?", "?", "ld", "0","d");
      string first_element = new_id();
      combined->lines_3AC.emplace_back(2, first_element, initialiser_vars[0].second, "?", "?", "ld", initialiser_vars[0].first, initialiser_vars[0].second);
      byte_offset += get_array_base_size(initialiser_vars[0].second);

      for (int i=1; i<initialiser_vars.size(); i++) {
        int base_type_size = get_array_base_size(initialiser_vars[i].second);
        while (byte_offset % base_type_size) {
          combined->lines_3AC.emplace_back(2, new_id(), "c", "?", "?", "ld", "0", "c");
          byte_offset++;
        }
        combined->lines_3AC.emplace_back(2, new_id(), initialiser_vars[i].second, "?", "?", "ld", initialiser_vars[i].first, initialiser_vars[i].second);
        byte_offset += get_array_base_size(initialiser_vars[i].second);
      }

      while (byte_offset < byte_size) {
        combined->lines_3AC.emplace_back(2, new_id(), "c", "?", "?", "ld", "0", "c");
        byte_offset++;
      }

      // string char_id = new_id();
      // combined->lines_3AC.emplace_back(2, char_id, initialiser_vars[0].second + "*", "?", "?", "adr", first_element, initialiser_vars[0].second);
      combined->lines_3AC.emplace_back(2, d->result.first, type, "?", "?", "asc", first_element, initialiser_vars[0].second);
      return return_3AC_struct(combined);
  }
  return 0;
}