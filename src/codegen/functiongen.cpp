#include "codegen.h"


void function_gen(const statement_3AC &line) {

    switch (line.flag)
    {

      case 0:
        if ((line.a != "") && (line.a != "?")) {aout << endl << "# " << line.a << endl;}
        else if ((line.at != "") && (line.at != "?")) {aout << "# " << line.at << endl << endl;}
        else {aout << endl;}
        break;
      case 1:
        three_op_code(line);
        break;
      case 2:
        two_op_code(line);
        break;
      case 3:
        place_label(line);
        break;
      case 4:
        if (line.op=="eq") {
          branch_eq(line);
        } else if (line.op == "ne") {
          branch_ne(line);
        } else {
          unknown_op(line);
        }
        break;
      case 5:
        if (line.op=="param") {
          place_param(line);
        } else if (line.op == "return") {
          place_return(line);
        } else {
          unknown_op(line);
        }
        break;
      case 6:
        function_call(line);
        break;
      case 7:
        function_start(line);
        break;
      default:
        unknown_op(line);
        break;

 


  }

  
}


void unknown_op(const statement_3AC &line) {
    cerr << "Unknown 3AC code: ";
    cerr << line.a << ":" << line.at << " = " << line.b << ":" << line.bt << " " << line.op << line.c << ":" << line.ct;
    cerr << " And args ";
    for (int i=0; i< line.args.size(); i++) {
      cerr << " " << line.args[i].first << ":" << line.args[i].second;
    }
    cerr << endl;
}