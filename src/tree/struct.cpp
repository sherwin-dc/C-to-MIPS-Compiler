#include "cparserDerivedVisitor.h"

using namespace cparser;

antlrcpp::Any cparserDerivedVisitor::visitPointer_struct_member_variables(cparserParser::Pointer_struct_member_variablesContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);

    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->identifier()), __PRETTY_FUNCTION__);

    string str_name = e1->result.second.substr(1, e1->result.second.size()-3);
    struct_info s = struct_bindings[str_name];

    pair<int, string> member_var = s.struct_members[e2->result.first];


    string id = new_id();

 
    //to be changed to what is stored in bindings
    string type = member_var.second;
    std::unique_ptr<tree_to_3AC> combined = combine_structs(e1, e2, id, type);

    string id_straddr_offset = new_id();
    string id_offset = new_id();
    // set id to be the member variable
    // get address of struct
    // combined->lines_3AC.emplace_back(2, id_straddr, e1->result.second + "*", "?", "?", "adr", e1->result.first, e1->result.second);
    combined->lines_3AC.emplace_back(2, id_offset, "s", "?", "?", "ld", to_string(member_var.first), "s");
    // add byte offset
    combined->lines_3AC.emplace_back(1, id_straddr_offset, e1->result.second, e1->result.first, e1->result.second, "+", id_offset,"s");
    // dereference
    combined->lines_3AC.emplace_back(2, id, type, "?", "?", "der", id_straddr_offset, e1->result.second);

    return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitStruct_member_variables(cparserParser::Struct_member_variablesContext *ctx) {
    std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(ctx->postfix_expression()), __PRETTY_FUNCTION__);

    std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(ctx->identifier()), __PRETTY_FUNCTION__);

    string str_name = e1->result.second.substr(1, e1->result.second.size()-2);
    struct_info s = struct_bindings[str_name];

    pair<int, string> member_var = s.struct_members[e2->result.first];


    string id = new_id();

 
    //to be changed to what is stored in bindings
    string type = member_var.second;
    std::unique_ptr<tree_to_3AC> combined = combine_structs(e1, e2, id, type);
    string id_straddr = new_id();
    string id_straddr_offset = new_id();
    string id_offset = new_id();
    // set id to be the member variable
    // get address of struct
    combined->lines_3AC.emplace_back(2, id_straddr, e1->result.second + "*", "?", "?", "adr", e1->result.first, e1->result.second);
    combined->lines_3AC.emplace_back(2, id_offset, "s", "?", "?", "ld", to_string(member_var.first), "s");
    // add byte offset
    combined->lines_3AC.emplace_back(1, id_straddr_offset, e1->result.second + "*", id_straddr, e1->result.second + "*", "+", id_offset,"s");
    // dereference
    combined->lines_3AC.emplace_back(2, id, type, "?", "?", "der", id_straddr_offset, e1->result.second + "*");

    return return_3AC_struct(combined);
}

antlrcpp::Any cparserDerivedVisitor::visitStruct_specifier(cparserParser::Struct_specifierContext *ctx)
{
    unique_ptr<tree_to_3AC> id;
    string str_name;
    // check if identifier is present
    if (ctx->identifier()) {
      id = get_struct(visit(ctx->identifier()), __PRETTY_FUNCTION__);
      str_name = id->result.first; 
        if(variable_bindings.find(id->result.first)!=variable_bindings.end()) {
          // as this is the type of struct, not a veriable, erase this from var bindings
          variable_bindings.erase(id->result.first);
        }
    } else {
      str_name = new_id(); // create a name for the struct
    }
    auto new_struct = create_new_struct();
    new_struct->result.second = "r" + str_name + "$";

    if (ctx->OPENBRACE()) {

        
        
        auto vec = ctx->parameter_declaration();
        struct_info s;
        s.struct_size = 0; // keep track of byte offset
        s.alignment=1;
        for (auto it : vec) {
          auto member = get_struct(visit(it), __PRETTY_FUNCTION__);
          string member_id = member->result.first;
          string member_type = member->result.second;

          if (member_type=="d") {
            if (s.struct_size % 8) {s.struct_size += 8 - abs(s.struct_size % 8);}
            if (s.alignment < 8) {s.alignment=8;}
            s.struct_members[member_id] = make_pair(s.struct_size, member_type);
            s.struct_size += 8;

          } else if (member_type[member_type.size()-1]=='$')  {
            // nested struct
            string nested_struct_name = member_type.substr(1, member_type.size()-2);
            struct_info nested_struct = struct_bindings[nested_struct_name];
            if (s.struct_size % nested_struct.alignment) {s.struct_size += nested_struct.alignment - abs(s.struct_size % nested_struct.alignment);}
            if (s.alignment < nested_struct.alignment) {s.alignment=nested_struct.alignment;}
            s.struct_members[member_id] = make_pair(s.struct_size, member_type);
            s.struct_size += nested_struct.struct_size;

          } else if ((member_type=="f") || (member_type=="u") || (member_type=="s") || (member_type[member_type.size()-1] =='*') ) {
            if (s.struct_size % 4) {s.struct_size += 4 - abs(s.struct_size % 4);}
            if (s.alignment < 4) {s.alignment=4;}
            s.struct_members[member_id] = make_pair(s.struct_size, member_type);
            s.struct_size += 4;
          
          } else if (char_is_digit(member_type[member_type.size()-1])) { // array
            int array_base_size = get_array_alignment(member_type); // might be incorrect if multidimentional array
            int array_whole_size = get_array_whole_size(member_type);

            if (s.struct_size % array_base_size) {s.struct_size += array_base_size - abs(s.struct_size % array_base_size);}
            if (s.alignment < array_base_size) {s.alignment=array_base_size;}
            s.struct_members[member_id] = make_pair(s.struct_size, member_type);
            s.struct_size += array_whole_size;

          } else if (member_type=="c") {
            s.struct_members[member_id] = make_pair(s.struct_size, member_type);
            s.struct_size += 1;

          } else {
            cerr << "Error in " << __PRETTY_FUNCTION__ << " : unknown member_type " << member_type << endl;
          }



        }
        // make struct multiple of alignment
        if (s.struct_size % s.alignment) {s.struct_size += s.alignment - abs(s.struct_size % s.alignment);}

        // insert struct into bindings
        struct_bindings[str_name] = s;

        

    }

    return return_3AC_struct(new_struct);
}

// antlrcpp::Any cparserDerivedVisitor::visitStruct_declaration_list(cparserParser::Struct_declaration_listContext *ctx)
// {
//     visitChildren(ctx);
//     // get type specifiers
//     string type = "";
//     auto vec = ctx->type_specifier();

//     for (auto it : vec)
//     {
//         if (it->CHAR_KEYWORD())
//         {
//             type = "c";
//         }
//         else if (it->DOUBLE_KEYWORD())
//         {
//             type = "d";
//         }
//         else if (it->FLOAT_KEYWORD())
//         {
//             type = "f";
//         }
//         else if (it->INT_KEYWORD())
//         {
//             type = "s";
//         }
//         else
//         {
//             type = "s"; // enum
//         }
//     }

//     for (auto it : vec)
//     {
//         if (it->SIGNED_KEYWORD())
//         {
//             type = "s";
//         }
//         else if (it->UNSIGNED_KEYWORD())
//         {
//             type = "u";
//         }
//     }

//     // auto s = create_new_struct();
//     // s->result.second = type;

//     auto e2 = get_struct(visit(ctx->struct_declarator_list()), __PRETTY_FUNCTION__);

//     // auto s = create_new_struct();
//     // s->result.first = e2->result.first;
//     // s->result.second = type;
//     std::unique_ptr<tree_to_3AC> new_struct = combine_structs_one(e2, e2->result.first, type);

//     return return_3AC_struct(new_struct);
// }

// antlrcpp::Any cparserDerivedVisitor::visitStruct_declarator_list(cparserParser::Struct_declarator_listContext *ctx)
// {
//     //get vector of param_declaration *
//     auto vec = ctx->struct_declarator();
//     if (vec.size() == 1)
//     {
//         auto s = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
//         return return_3AC_struct(s);
//     }

//     std::unique_ptr<tree_to_3AC> e1 = get_struct(visit(vec[0]), __PRETTY_FUNCTION__);
//     std::unique_ptr<tree_to_3AC> tmp;

//     for (int i = 1; i < vec.size(); i++)
//     {
//         std::unique_ptr<tree_to_3AC> e2 = get_struct(visit(vec[i]), __PRETTY_FUNCTION__);
//         tmp = combine_structs(e1, e2, "", ""); // at this stage there are no return types
//         std::swap(tmp, e1);
//     }

//     return return_3AC_struct(e1);
// }

// antlrcpp::Any cparserDerivedVisitor::visitStruct_declarator(cparserParser::Struct_declaratorContext *ctx)
// {
//     auto e = get_struct(visit(ctx->declarator()), __PRETTY_FUNCTION__);
//     string enum_name = e->result.first;
//     e->result.second = "?";

//     if (variable_bindings.count(enum_name))
//     {
//         variable_bindings.erase(enum_name);
//     }

//     if (ctx->constant())
//     {
//         auto c = get_struct(visit(ctx->constant()), __PRETTY_FUNCTION__);
//         e->result.second = c->result.first; // put the constant value in the pair
//     }

//     return return_3AC_struct(e);
// }
