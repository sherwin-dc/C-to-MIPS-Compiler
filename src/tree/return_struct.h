#include <unordered_map>
#include <string>
#include <vector>
#include <deque>
#include <memory> // unique_ptr (C++11) and make_unique (C++14)

#include "antlr4-runtime.h"
#pragma once
using namespace std;



struct statement_3AC {
  int flag;
  string a, at, /* = */ b, bt, op, c, ct;
  vector<pair<string, string>> args;
  /** 
   * line break (for readability)
   * {0, comment_before_something}
   * {0, "", comment_after_something}
   * 
   * a:t = b:t op c:t
   * {1, #1, f, #3, f, +, #4, f}
   * 
   * a:t = op c:t
   * {2, #1, f, ?, ?, -, #4, f}
   * 
   * @label
   * {3, @ELSE4, ?, ?, ?, ?, ?, ?}
   * 
   * @label = a:t br b:t
   * {4, @ELSE4, ?, #3, f, eq, #4, f}
   * 
   * param a:t / return a:t
   * {5, #1, u, ?, ?, param, ?, ?}
   * 
   * a:t = call Fn_name no_of_params
   * {6, #1, f, !MyFn, ?, call, 3, ?}
   * 
   * Fn_name a1:t a2:t ... an:t / #Global
   * {7, MyFn, ?, ?, ?, ?, ?, ?} then .args = args
   * 
  */
 statement_3AC(int flag, string a="?", string at="?", /* = */ string b="?", string bt="?", string op="?", string c="?", string ct="?")
  : flag(flag), a(a), at(at), b(b), bt(bt), op(op), c(c), ct(ct)
 {
   args = {};
 }
};

struct case_expr {
  string case_statement_label, case_expr_label, switch_label, expr_id, expr_type;
};
  // 
struct tree_to_3AC {
  // identifier/id that contains the result of whole expression so far <id, type>
  pair<string, string> result;

  //string value;
  // hash map of identifiers (key) to type (value)
  unordered_map<string, string> id_bindings;

  //string id_name;

  // vector of 3AC codes
  deque<statement_3AC> lines_3AC;
  //? case statements
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
  vector<case_expr> cases;
  string default_label;
  /**
   * vector of break statements
   * when break is encountered, it creates a label and an unconditional branch to that label. This label is stored in the vector
   */
  vector<string> breaks;
  /**
   * vector of continue statements
   * when continue is encountered, it creates a label and an unconditional branch to that label. This label is stored in the vector
   */
  vector<string> continues;
  /**
   * vector of params for function parameters ie f(int a, float**b)
   * only parameter_list and argument_expression_list will insert into this vector
   */
  vector<pair<string, string>> params;

  // tree_to_3AC() {
  //   result = make_pair("","");
  //   id_bindings = {};
  //   lines_3AC = {};
  //   cases = {};
  //   default_label = "";
  //   breaks=  {};
  //   continues = {};
  //   params = {};
  // }
};

string new_id();

string new_label(string append="");

struct struct_info {
  
  // map of id within struct to byte offset and type ie. map['a''] = (8, d)
  unordered_map<string, pair<int, string>> struct_members;
  // size of struct, in bytes
  int struct_size;
  int alignment;
};

// map of names of structs to its info
extern unordered_map<string, struct_info> struct_bindings; 

// (local?) variables
// hash map of identifier_name in source code (key) to type - <id,type>
extern unordered_map<string, string> variable_bindings;

//functions
// hash map of fn name in source code (key) to return type - <id, return type>
extern unordered_map<string, string> function_bindings;

//hash map of fn name in source code (key) to return type - <id, <return type, size of array>>
extern unordered_map<string, string> array_bindings;
extern unordered_map<string, int> enum_bindings;

// map of typedef names to actual types
extern unordered_map<string, string> typedef_bindings;
extern bool redo_parsing_typedef;

void print_return_struct(string outfile, const tree_to_3AC &tree_struct);

unique_ptr<tree_to_3AC> combine_structs(const unique_ptr<tree_to_3AC> &first, const unique_ptr<tree_to_3AC> &second, const string &new_result, const string &new_result_type);

unique_ptr<tree_to_3AC> combine_structs_one(const unique_ptr<tree_to_3AC> &first, const string &new_result, const string &new_result_type);

vector<string> argument_type (const pair<string, string> e1_result, const pair<string, string> e2_result);

unique_ptr<tree_to_3AC> get_struct(antlrcpp::Any obj, const char* fn_name);

tree_to_3AC* return_3AC_struct(unique_ptr<tree_to_3AC> &ptr);

unique_ptr<tree_to_3AC> create_new_struct();

bool char_is_digit(const char &c);
int get_array_base_size(const string &type);
int get_array_whole_size(const string &type);
int get_array_alignment(const string &type);

void clear_bindings();

