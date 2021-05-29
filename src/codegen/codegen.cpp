#include "codegen.h"
ofstream aout;
vector<string> data_section { ".data"};

namespace fngen {
  vector<streampos> stack_returns;
  // ids to position on stack, starting from end of previous frame pointer, ie -4,
  map<pair<string, string>, int> stack_var;
  // map of global vars to data label
  map<pair<string, string>, string> global_var;
  int next_stack_pos=8;

  int no_of_params=0;
  int max_no_of_params=0;
  bool is_first_integral=false;
  int param_byte_offset=0;

  bool global_mode=false;

  // map of variables to pointers eg. map[a:f] = b:f*
  map<pair<string, string>, pair<string, string>> var_to_pointer;
}

void codegen_main(string asmfile, const tree_to_3AC &tree_struct) {
  //? Open asmfile
  aout.open(asmfile, ios_base::trunc);

  if(!aout.is_open()) {
    cerr << "Cannot open output assembly file. Exiting" << endl;
    exit(1);
  }

  asm_start();
  // find where the first function starts
  int first_fn;
  for (int i=0; i<tree_struct.lines_3AC.size(); i++) {
    if (tree_struct.lines_3AC[i].flag == 7) {
      first_fn = i;
      break;
    }
  }
  
  for (int i=0; i<tree_struct.lines_3AC.size(); i++) {
    //? Check for start of functions
    if ((tree_struct.lines_3AC[i].flag == 7) && (i>first_fn)) {
      function_end();
    }
    function_gen(tree_struct.lines_3AC[i]);
  }
  function_end();

  for (int i=0; i<data_section.size(); i++) {
    aout << data_section[i] << endl;
  }

  aout.close();
  
}

void asm_start() {
  aout << ".set noreorder" << endl;
}