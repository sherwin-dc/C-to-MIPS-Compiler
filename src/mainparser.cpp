#include <iostream>

#include "antlr4-runtime.h"
#include "cparserLexer.h"
#include "cparserParser.h"
#include "tree/cparserDerivedVisitor.h"
#include "tree/return_struct.h"
#include "codegen/codegen.h"

// using namespace std;
// using namespace antlr4;

// global output stream
std::ostream& dst = std::cout;

int main(int argc, const char** argv) {

    //? Input file
  
    if (argc < 2) {
      std::cerr << "No input or output file provided. Exiting" << std::endl;
      exit(1);
    }
    std::ifstream stream;
    std::string input_filename = argv[1];
    stream.open(input_filename);

  std::unique_ptr<tree_to_3AC> rtn_struct;
  do {
    stream.clear();
    stream.seekg(0);
    clear_bindings();
    redo_parsing_typedef = false;
    //? Lex and Parse
    std::cerr << "Begin Parsing and Lexing" << std::endl;
    antlr4::ANTLRInputStream input(stream);
    cparser::cparserLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    cparser::cparserParser parser(&tokens);   
    cparser::cparserParser::ProgContext* tree = parser.prog();
     
    std::cerr << "Done Parsing and Lexing" << std::endl;

    //? Transverse parse tree
    
    // if( parser.getNumberOfSyntaxErrors() ) {
    //   continue;
    // }

    cparser::cparserDerivedVisitor visitor;

    antlrcpp::Any obj = visitor.visitProg(tree);

    rtn_struct = get_struct(obj, __PRETTY_FUNCTION__);

    // lexer.reset();
    // parser.reset();
    // input.reset();

  } while (redo_parsing_typedef);

    std::cerr << "Done creating 3AC code" << std::endl;
    
    
    //? Print out 3AC code
    if (argc >=3) {
      print_return_struct(argv[3], *rtn_struct);
    } else {
      std::cerr << "No 3AC file provided" << std::endl;
    }

    //? Codegen
    codegen_main(argv[2], *rtn_struct);
    
    return 0;
}