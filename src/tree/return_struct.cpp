#include <unordered_map>
#include <string>
#include "return_struct.h"
#include <iostream>
#include <fstream>
using namespace std;

/* Returns a pointer to a dynamically allocated struct */
unique_ptr<tree_to_3AC> combine_structs(const unique_ptr<tree_to_3AC> &first, const unique_ptr<tree_to_3AC> &second, const string &new_result, const string &new_result_type) {

  unique_ptr<tree_to_3AC> new_struct = make_unique<tree_to_3AC>(tree_to_3AC());
  new_struct->lines_3AC.clear();
  // copy the second vector behind the first
  new_struct->lines_3AC = first->lines_3AC;
  for (int i=0; i<second->lines_3AC.size(); i++) {
    statement_3AC stm = second->lines_3AC[i];
    new_struct->lines_3AC.push_back(stm);
  }
  //new_struct->id_name = first->id_name;
  // // copy over values - to be overwritten in class accordingly
  // new_struct->values = first->values;

  // copy over cases
  new_struct->cases = first->cases;
  for (int i=0; i<second->cases.size(); i++) {
    new_struct->cases.push_back(second->cases[i]);
  }

  // copy default
  if (first->default_label != "") {
    new_struct->default_label = first->default_label;
  } else if (second->default_label != "") {
    new_struct->default_label = second->default_label;
  }

  // copy over breaks
  new_struct->breaks = first->breaks;
  for (int i=0; i<second->breaks.size(); i++) {
    new_struct->breaks.push_back(second->breaks[i]);
  }

  // copy over continues
  new_struct->continues = first->continues;
  for (int i=0; i<second->continues.size(); i++) {
    new_struct->continues.push_back(second->continues[i]);
  }

  // copy over bindings, second will override first
  new_struct->id_bindings = second->id_bindings;
  new_struct->id_bindings.insert(first->id_bindings.begin(), first->id_bindings.end());

  // copy over params
  new_struct->params = first->params;
  for (int i=0; i<second->params.size(); i++) {
    new_struct->params.push_back(second->params[i]);
  }

  // put in new result and result type
  new_struct->result.first = new_result;
  new_struct->result.second = new_result_type;
  new_struct->id_bindings[new_result] = new_result_type;

  return move(new_struct);
}

unique_ptr<tree_to_3AC> combine_structs_one(const unique_ptr<tree_to_3AC> &first, const string &new_result, const string &new_result_type) {
  unique_ptr<tree_to_3AC> new_struct = make_unique<tree_to_3AC>(tree_to_3AC());

  return combine_structs(first, new_struct, new_result, new_result_type);

}

vector<string> argument_type (const pair<string, string> e1_result, const pair<string, string> e2_result){
  vector<string> result;
  
  string e1_type;
  string e2_type;
  //checking if e1 is a variable
  if (variable_bindings.find(e1_result.first)!=variable_bindings.end()){
    e1_type = variable_bindings[e1_result.first];
  }else{
    e1_type = e1_result.second;
  }
  //checking if e2 is a variable
  if (variable_bindings.find(e2_result.first)!=variable_bindings.end()){
    e2_type = variable_bindings[e2_result.first];
  }else{
    e2_type = e2_result.second;
  }

  result.push_back(e1_type);
  result.push_back(e2_type);
  return result;
}


string new_id() {
  static size_t no = 0;
  no++;
  // '#' to prevent clashing with real identifiers in program
  return "#" + to_string(no);
}

string new_label(string append /* = "" */ ) {
  static size_t no = 0;
  no++;
  return "_" + append + to_string(no);
}

unordered_map<string, struct_info> struct_bindings; 
// local(?) variables
unordered_map<string, string> variable_bindings;

//functions
unordered_map<string, string> function_bindings;

unordered_map<string, string> array_bindings;
unordered_map<string, int> enum_bindings;
unordered_map<string, string> typedef_bindings;
bool redo_parsing_typedef = false;

void clear_bindings() {
  struct_bindings.clear();
  variable_bindings.clear();
  array_bindings.clear();
  enum_bindings.clear();
}


void print_3AC_line(ofstream &fout, const statement_3AC &line) {
  switch (line.flag)
  {
  case 0:
    if ((line.a != "") && (line.a != "?")) {fout << endl << line.a << endl;}
    else if ((line.at != "") && (line.at != "?")) {fout << line.at << endl << endl;}
    else {fout << endl;}
    break;
  case 1:
    fout << line.a << ":" << line.at << " = " << line.b << ":" << line.bt << " " << line.op << " " << line.c << ":" << line.ct << endl;
    break;
  case 2:
    fout << line.a << ":" << line.at << " = "  << line.op << " " << line.c << ":" << line.ct << endl;
    break;
  case 3:
    fout << line.a << endl;
    break;
  case 4:
    fout << line.a << " = " << line.b << ":" << line.bt << " " << line.op << " " << line.c << ":" << line.ct << endl;
    break;
  case 5:
    fout << line.op << " " << line.a << ":" << line.at << endl;
    break;
  case 6:
    fout << line.a << ":" << line.at << " = "  << line.op << " " << line.b << " " << line.c << endl;
    break;
  case 7:
    fout << line.a;
    for (int i=0; i< line.args.size(); i++) {
      fout << " " << line.args[i].first << ":" << line.args[i].second;
    }
    fout << endl;
    break;
  // case 8:
  //   fout << line.a << ":" << line.at << " = "  << line.op << endl;
  //   break;

  default:
    cerr << "Unknown 3AC code: ";
    cerr << line.a << ":" << line.at << " = " << line.b << ":" << line.bt << " " << line.op << line.c << ":" << line.ct;
    cerr << " And args ";
    for (int i=0; i< line.args.size(); i++) {
      cerr << " " << line.args[i].first << ":" << line.args[i].second;
    }
    cerr << endl;
    break;
  }
}

void print_return_struct(string outfile, const tree_to_3AC &tree_struct) {
  ofstream fout;
  fout.open(outfile);
  if (fout.is_open()) {
    for (int i=0; i<tree_struct.lines_3AC.size(); i++) {
      print_3AC_line(fout, tree_struct.lines_3AC[i]);
    }
  } else {
    cerr << "Cannot open 3AC output file " << outfile << endl;
  }
  fout.close();

}

unique_ptr<tree_to_3AC> get_struct(antlrcpp::Any obj, const char * fn_name) {
  if (obj.is<tree_to_3AC*>()) {
    tree_to_3AC* raw_ptr = obj.as<tree_to_3AC*>();
    return move(unique_ptr<tree_to_3AC>(raw_ptr));
  } else {
    std::cerr << "Bad_cast() error in function: " << fn_name << ". Returning new struct instead" << std::endl;
    return move(make_unique<tree_to_3AC>(tree_to_3AC()));
  }
}

unique_ptr<tree_to_3AC> create_new_struct() {
  unique_ptr<tree_to_3AC> new_struct = make_unique<tree_to_3AC>(tree_to_3AC());
  new_struct->lines_3AC.clear();
  return move(new_struct);
}

tree_to_3AC* return_3AC_struct(unique_ptr<tree_to_3AC> &ptr) {
  return ptr.release();
}

bool char_is_digit(const char &c) {
  if ((c>='0') && (c<='9')) {
    return true;
  }
  return false;
}

int get_array_base_size(const string &type) {

      // get index of first non digit from the back
      int idx=type.size()-1;
      for ( ; idx>=0; idx--) {
        if (!char_is_digit(type[idx])) {
          break;
        }
      }
      // remove any '-' if present
      if (type[idx]=='-') {idx--;};

      return get_array_whole_size(type.substr(0, idx+1));
      
}
int get_array_whole_size(const string &type) {

  // check if this is a single character
  if ((type=="s") || (type=="u") || (type=="f")) {
    return 4;
  } else if ((type=="d")) {
    return 8;
  } else if ((type=="c")) {
    return 1;
  } else if (type[type.size()-1]=='*') {
    return 4;
  } else if (type.size() > 1) {
    // check if this is an array
    if (char_is_digit(type[type.size()-1])) {

      // get index of first non digit from the back
      int idx=type.size()-1;
      for ( ; idx>=0; idx--) {
        if (!char_is_digit(type[idx])) {
          break;
        }
      }
      
      int no_array_elements;
      try {
        no_array_elements = stoi(type.substr(idx+1, string::npos));
      } catch (std::invalid_argument) {
        cerr << "Error in " << __PRETTY_FUNCTION__ << ": Cannot get size in bytes of the array type: " << type << endl;
      }

      // remove any '-' if present
      if (type[idx]=='-') {idx--;};
      return get_array_whole_size(type.substr(0, idx+1)) * no_array_elements;

    } else if (type[type.size()-1] == '$') { // struct
      string str_name = type.substr(1, type.size()-2);
      struct_info struct_information = struct_bindings[str_name];
      return struct_information.struct_size;

    } else {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Unknown type/subtype " << type << endl;
    }

  }
  
  return 0;
}

int get_array_alignment(const string &type) {
  // check if this is a single character
  if ((type=="s") || (type=="u") || (type=="f")) {
    return 4;
  } else if ((type=="d")) {
    return 8;
  } else if ((type=="c")) {
    return 1;
  } else if (type[type.size()-1]=='*') {
    return 4;
  } else if (type.size() > 1) {
    // check if this is an array
    if (char_is_digit(type[type.size()-1])) {

      // get index of first non digit from the back
      int idx=type.size()-1;
      for ( ; idx>=0; idx--) {
        if (!char_is_digit(type[idx])) {
          break;
        }
      }
      
      // remove any '-' if present
      if (type[idx]=='-') {idx--;};
      return get_array_alignment(type.substr(0, idx+1));

    } else if (type[type.size()-1] == '$') { // struct
      string str_name = type.substr(1, type.size()-2);
      struct_info struct_information = struct_bindings[str_name];
      return struct_information.alignment;

    } else {
      cerr << "Error in " << __PRETTY_FUNCTION__ << ": Unknown type/subtype " << type << endl;
    }

  }

  return 0;
}