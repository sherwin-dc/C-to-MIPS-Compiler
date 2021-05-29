#include "codegen.h"

/* Fn_name a1:t a2:t ... an:t / #Global
   {7, MyFn, ?, ?, ?, ?, ?, ?} then .args = args */
void function_start(const statement_3AC &line) {
  if (line.a=="#Global") {
    fngen::global_mode = true;
    return;
  }

  //? Comment to indicate beginning of function
  aout << "# Start of function " << line.a << endl;
  aout << ".globl " << line.a << endl;
  aout << line.a << ":" << endl;
  // Get position of cursor in file, then print empty space that will be replaced
  aout << "move $t0, $s0" << endl;
  aout << "move $t7, $sp" << endl;
  aout << "move $s0, $sp" << endl;
  fngen::stack_returns.emplace_back(aout.tellp());
  aout << /*  ADDIU $s0, $s0, X   \n   ADDIU $sp, $sp, Y */ "#                                                                               " << endl;
  aout << "move $fp, $sp" << endl;
  // save value of $s0 as this will become our fixed frame pointer
  store_var_in_memory("#sp", "u", "$t7");
  store_var_in_memory("#s0", "u", "$t0");
  store_var_in_memory("#ra", "u", "$ra");

  // store zero in memory (2 words)
  fngen::stack_var[make_pair("#0", "f")] = fngen::next_stack_pos;
  fngen::stack_var[make_pair("#0", "c")] = fngen::next_stack_pos;
  fngen::stack_var[make_pair("#0", "d")] = fngen::next_stack_pos;
  store_var_in_memory("#0", "u", "$zero");
  store_var_in_memory("#0", "s", "$zero");

  

  vector<pair<string, string>> arguments = line.args;
  int byte_offset = 0;

  //? if first 2 arguments are doubles or floats
  int no_args = arguments.size();
  bool flag_first_integral = false;
  if (no_args>0) {
    //! First argument
    if (arguments[0].second=="d") {
      byte_offset += 8;
      // take var from float register and put on stack
      store_var_in_memory(arguments[0].first, arguments[0].second, "$f12");
    } else if (arguments[0].second=="f") {
      byte_offset += 4;
      store_var_in_memory(arguments[0].first, arguments[0].second, "$f12");
    // } else if (arguments[0].second=="c") {
    //   byte_offset += 1;
    //   flag_first_integral = true;
    //   aout << "SRA $t0, $a0, 24" << endl;
    //   store_var_in_memory(arguments[0].first, arguments[0].second, "$t0");
    } else {
      byte_offset += 4;
      flag_first_integral = true;
      store_var_in_memory(arguments[0].first, arguments[0].second, "$a0");
    }
    //! Second argument
    if (no_args>1) {
      if (arguments[1].second=="d") {
        if ((byte_offset % 8)) {byte_offset += 8 - (byte_offset % 8);} 
        byte_offset += 8;
        if (flag_first_integral) {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$a2");
        } else {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$f14");
        }
        
      } else if (arguments[1].second=="f") {
        if ((byte_offset % 4)) {byte_offset += 4 - (byte_offset % 4);} 
        
        if (flag_first_integral) {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$a1");
        } else {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$f14");
        }
        byte_offset += 4;
      // } else if (arguments[1].second=="c") {
      //   if (!(byte_offset % 4)) {cerr << "Passing packed chars as args not supported." << endl;}
      //   if (byte_offset==4) {
      //     store_var_in_memory(arguments[1].first, arguments[1].second, "$a1");
      //   } else if (byte_offset==8) {
      //     store_var_in_memory(arguments[1].first, arguments[1].second, "$a2");
      //   } else {
      //     // need to shift right and then store char
      //     aout << "SRA $t0, $a0, " << 32-((1+(byte_offset%4))*8) << endl;
      //     store_var_in_memory(arguments[1].first, arguments[1].second, "$t0");
      //   }
        
      //   byte_offset += 1;
      } else {
        if ((byte_offset % 4)) {byte_offset += 4 - (byte_offset % 4);} 
        if (byte_offset==4) {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$a1");
        } else {
          store_var_in_memory(arguments[1].first, arguments[1].second, "$a2");
        }
        byte_offset += 4;
      }
    } // no_args >1
  } // no_args >0


  //? Copy all arg registers onto prev function's stack
  aout << "SW $a0, 0($t7)" << endl;
  aout << "SW $a1, 4($t7)" << endl;
  aout << "SW $a2, 8($t7)" << endl;
  aout << "SW $a3, 12($t7)" << endl;

  //! Subsequent arguments (taken from stack)
  if (no_args>2) {
    for (int i=2; i<arguments.size(); i++) {
      string id = arguments[i].first;
      string type = arguments[i].second;
      if (type=="d") {
        if ((byte_offset % 8)) {byte_offset += 8 - (byte_offset % 8);}
        aout << "LW $t0, " << byte_offset << "($t7)" << endl;
        aout << "LW $t1, " << byte_offset+4 << "($t7)" << endl;
        store_var_in_memory(id, type, "$t0");
        byte_offset+=8;
      // } else if (type=="c") {
      //   aout << "LB $t0, " << byte_offset << "($s0)" << endl; // char is sign extended and fills whole reg
      //   store_var_in_memory(id, type, "$t0");
      //   byte_offset+=1;
      } else {
        if ((byte_offset % 4)) {byte_offset += 4 - (byte_offset % 4);}
        aout << "LW $t0, " << byte_offset << "($t7)" << endl;
        store_var_in_memory(id, type, "$t0");
        byte_offset+=4;
      }
    }
  } // no_args>2
  

}

void place_param(const statement_3AC &line) {
  string type=line.at;
  string id = line.a;
  fngen::no_of_params++;

  

  if (fngen::no_of_params==1) {

    if (type=="d") {
      fngen::param_byte_offset += 8;

      load_var_from_memory(id, type, "$f12");
    } else if (type=="f") {
      fngen::param_byte_offset += 4;
      load_var_from_memory(id, type, "$f12");
    // } else if (type=="c") {
      
    //   fngen::is_first_integral = true;
    //   load_var_from_memory(id, type, "$t0");
    //   aout << "SLL $a0, $t0, 24" << endl;
    //   aout << "SW $a0, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
    //   fngen::param_byte_offset += 1;
    } else {
      
      fngen::is_first_integral = true;
      load_var_from_memory(id, type, "$a0");
      aout << "SW $a0, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
      fngen::param_byte_offset += 4;
    }


  } // no_of_params==1

  if (fngen::no_of_params==2) {
    if (type=="d") {
      if ((fngen::param_byte_offset % 8)) {fngen::param_byte_offset += 8 - (fngen::param_byte_offset % 8);} 
      
      if (fngen::is_first_integral) {
        load_var_from_memory(id, type, "$a2");
        aout << "SW $a2, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
      } else {
        load_var_from_memory(id, type, "$f14");
      }
      fngen::param_byte_offset += 8;

    } else if (type=="f") {
      if ((fngen::param_byte_offset % 4)) {fngen::param_byte_offset += 4 - (fngen::param_byte_offset % 4);} 
      
      if (fngen::is_first_integral) {
        load_var_from_memory(id, type, "$a1");
        aout << "SW $a1, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
      } else {
        load_var_from_memory(id, type, "$f14");
      }
      fngen::param_byte_offset += 4;
    // } else if (type=="c") {

    //   if (fngen::param_byte_offset==4) {
    //     load_var_from_memory(id, type, "$t0");
    //     aout << "SLL $a1, $t0, 24" << endl;
    //     aout << "SW $a1, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
    //   } else if (fngen::param_byte_offset==8) {
    //     load_var_from_memory(id, type, "$t0");
    //     aout << "SLL $a2, $t0, 24" << endl;
    //     aout << "SW $a2, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
    //   } else {
    //     // need to shift left and then store char
    //     load_var_from_memory(id, type, "$t0");
    //     aout << "SLL $t0, $t0, 24" << endl;
    //     aout << "SRL $t0, $t0, " << ((fngen::param_byte_offset%4)*8) << endl;
    //     aout << "OR $a0, $a0, $t0" << endl;
    //     aout << "SW $a0, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
    //   }
      
    //   fngen::param_byte_offset += 1;
    } else {
      if ((fngen::param_byte_offset % 4)) {fngen::param_byte_offset += 4 - (fngen::param_byte_offset % 4);} 
      if (fngen::param_byte_offset==4) {
        load_var_from_memory(id, type, "$a1");
        aout << "SW $a1, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
      } else {
        load_var_from_memory(id, type, "$a2");
        aout << "SW $a2, " << fngen::param_byte_offset << "($sp)" << endl; //also put on stack
      }
      fngen::param_byte_offset += 4;
    }
  } // no_of_params==2

  if (fngen::no_of_params>2) {
    if (type=="d") {
        if ((fngen::param_byte_offset % 8)) {fngen::param_byte_offset += 8 - (fngen::param_byte_offset % 8);}
        load_var_from_memory(id, type, "$t0");
        aout << "SW $t0, " << fngen::param_byte_offset << "($sp)" << endl;
        aout << "SW $t1, " << fngen::param_byte_offset+4 << "($sp)" << endl;

        fngen::param_byte_offset+=8;
      // } else if (type=="c") {

      //   if (!(fngen::param_byte_offset%4)) {
      //     load_var_from_memory(id, type, "$t0");
      //     aout << "SLL $t0, $t0, 24" << endl;
      //     aout << "SRL $t0, $t0, 24" << endl;
      //     aout << "SW $t0, " << fngen::param_byte_offset << "($sp)" << endl;
      //   } else {
      //     // need to shift left and then store char
      //     load_var_from_memory(id, type, "$t0");
      //     aout << "LW $t1, " << fngen::param_byte_offset-(fngen::param_byte_offset%4) << "($sp)" << endl;
      //     aout << "SLL $t0, $t0, 24" << endl;
      //     aout << "SRL $t0, $t0, " << ((fngen::param_byte_offset%4)*8) << endl;
      //     aout << "OR $t1, $t1, $t0" << endl;
      //     aout << "SW $t1, " << fngen::param_byte_offset << "($sp)" << endl;
      //   }
      //   fngen::param_byte_offset+=1;

      } else {
        if ((fngen::param_byte_offset % 4)) {fngen::param_byte_offset += 4 - (fngen::param_byte_offset % 4);}
        load_var_from_memory(id, type, "$t0");
        aout << "SW $t0, " << fngen::param_byte_offset << "($sp)" << endl;
        
        fngen::param_byte_offset+=4;
      }
  } // no_of_params>2
}

void place_return(const statement_3AC &line) {
  // put return value in reg
  if ((line.at=="f") || (line.at=="d")) {
    load_var_from_memory(line.a, line.at, "$f0");
  } else {
    load_var_from_memory(line.a, line.at, "$v0");
  }

  // restore $ra and $s0
  load_var_from_memory("#ra", "u", "$ra");
  load_var_from_memory("#sp", "u", "$sp");
  load_var_from_memory("#s0", "u", "$s0");
  
  aout << "jr $ra" << endl;
  aout << "move $fp, $sp" << endl; // delay slot
}

void function_call(const statement_3AC &line) {

  // reset some of the counters to keep track of params
  if (fngen::no_of_params > fngen::max_no_of_params) {
    fngen::max_no_of_params = fngen::no_of_params;
  }
  fngen::no_of_params=0;
  fngen::is_first_integral=false;
  fngen::param_byte_offset=0;

  // copy last 4 stack values into arg registers
  aout << "LW $a0, 0($sp)" << endl;
  aout << "LW $a1, 4($sp)" << endl;
  aout << "LW $a2, 8($sp)" << endl;
  aout << "LW $a3, 12($sp)" << endl;

  // jump to function
  aout << "JAL " << line.b << endl;
  aout << "nop" << endl; //branch delay slot

  // get the return value
  if ((line.at=="f") || (line.at=="d")) {
    store_var_in_memory(line.a, line.at, "$f0");
  } else {
    store_var_in_memory(line.a, line.at, "$v0");
  }


}


void function_end() {

  if (fngen::global_mode) {
    fngen::global_mode=false;
    return;
  }
  //? Now that function is over, write the stack increments/drecrements
  streampos original_pos = aout.tellp();
  aout.seekp(fngen::stack_returns[0]);
  if (!fngen::max_no_of_params) {fngen::max_no_of_params=1;}
  int bytes_for_stack_vars = fngen::next_stack_pos;
  if ((bytes_for_stack_vars % 8)) {bytes_for_stack_vars += 8 - (bytes_for_stack_vars % 8);} 
  int stack_size = bytes_for_stack_vars + (8*fngen::max_no_of_params);
  string replace = "ADDIU $s0, $s0, " + to_string(-bytes_for_stack_vars) + "\nADDIU $sp, $sp, " + to_string(-stack_size);
  aout.write(replace.c_str(), replace.size());

  // string inc_sp = "ADDIU $sp, $sp, " + to_string(no_of_bytes_in_stack);
  // for (int i=1; i<fngen::stack_returns.size(); i++) {
  //   aout.seekp(fngen::stack_returns[i]);
  //   aout.write(inc_sp.c_str(), inc_sp.size());
  // }
  aout.seekp(original_pos);

  // clear some other vars
  fngen::stack_returns.clear();
  fngen::stack_var.clear();
  fngen::next_stack_pos=8;
  fngen::var_to_pointer.clear();
}