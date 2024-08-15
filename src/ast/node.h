#ifndef NODE_H
#define NODE_H

#include "../Include/Donsus/tomi.h"
#include "../Include/Internal/type.h"
#include "../utility/handle.h"
#include "../utility/property.h"

#include <string>

namespace donsus_ast {
// Todo: use smart enum based on:
// https://github.com/carbon-language/carbon-lang/blob/trunk/common/enum_base.h
// Use it like this:
// https://github.com/carbon-language/carbon-lang/blob/trunk/toolchain/lex/token_kind.h#L23-L109

struct donsus_node_type {
  enum underlying : int {
    DONSUS_VARIABLE_DEFINITION, // just the type of the node
    DONSUS_FUNCTION_DECL,       // just the type of the node
    DONSUS_FUNCTION_DEF,        // just the type of the node
    DONSUS_IF_STATEMENT,        // just the type of the node
    DONSUS_ASSIGNMENT,          // just the type of the node
    DONSUS_IDENTIFIER,          // just the type of the node
    DONSUS_INTEGER_EXPRESSION,  // just the type of the node
    DONSUS_FLOAT_EXPRESSION,    // just the type of the node
    DONSUS_BINARY_EXPRESSION,
    DONSUS_OCTAL_EXPRESSION,
    DONSUS_HEX_EXPRESSION,
    DONSUS_EXPRESSION,            // just the type of the node
    DONSUS_FUNCTION_CALL,         // just the type of the node
    DONSUS_ELSE_STATEMENT,        // just the type of the node
    DONSUS_RETURN_STATEMENT,      // just the type of the node
    DONSUS_CONTINUE_STATEMENT,    // just the type of the node
    DONSUS_BREAK_STATEMENT,       // just the type of the node
    DONSUS_FALLTHROUGH_STATEMENT, // just the type of the node
    DONSUS_CASE,                  // just the type of the node
    DONSUS_DO,                    // just the type of the node
    DONSUS_OF,                    // just the type of the node
    DONSUS_OTHERWISE_STATEMENT,   // just the type of the node
    DONSUS_AS_STATEMENT,          // just the type of the node
    DONSUS_TYPE_STATEMENT,        // just the type of the node
    DONSUS_ALIAS_STATEMENT,       // just the type of the node
    DONSUS_TYPECLASS,             // just the type of the node
    DONSUS_INSTANCE,              // just the type of the node
    DONSUS_CLASS,                 // just the type of the node
    DONSUS_FINAL,                 // just the type of the node
    DONSUS_ABSTRACT_STATEMENT,    // just the type of the node
    DONSUS_BITSHIFT,              // just the type of the node
    DONSUS_BITWISE,               // just the type of the node
    DONSUS_ARRAY_DEFINITION,      // just the type of the node
    DONSUS_ARRAY_DECLARATION,     // just the type of the node

    DONSUS_STRING_EXPRESSION, // just the type of the node
    DONSUS_BOOL_EXPRESSION,   // just the type of the node
    DONSUS_UNARY_EXPRESSION,  // just the type of the node
    DONSUS_PRINT_EXPRESSION,  // just the type of the node
    DONSUS_ARRAY_ACCESS,      // just the type of the node
    DONSUS_FUNCTION_ARG,      // just the type of the node
    DONSUS_WHILE_LOOP,        // just the type of the node
    DONSUS_FOR_LOOP,          // just the type of the node
    DONSUS_RANGE_EXPRESSION,  // just the type of the node
  };
  donsus_node_type() = default;
  donsus_node_type(underlying type);

  underlying type;
};
std::string de_get_from_donsus_node_type(donsus_node_type type);

struct node;
struct variable_def {
  donsus_token_kind identifier_type;
  std::string identifier_name;
  void *identifier_value;
};
struct range_expr {
  utility::handle<donsus_ast::node> start;
  utility::handle<donsus_ast::node> end;
};
struct for_loop {
  std::string loop_variable;
  utility::handle<donsus_ast::node> iterator;
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};
struct while_loop {
  // condition is stored in the children
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};
struct integer_expr {
  token value;
};
struct float_expr {
  token value;
};

enum ArrayType { FIXED, STATIC, DYNAMIC };

struct array_def {
  ArrayType array_type;
  std::string identifier_name;
  donsus_token_kind type;
  Tomi::Vector<utility::handle<donsus_ast::node>> elements;
  /*If the array_type is dynamic the size will be zero and ignored
 If the array_type is static or fixed then the size must be provided
*/
  int number_of_elements;
  int size; // Represents the number between the square brackets a:int[3]
            // [1,2,3] here it is 3.
};

struct array_decl {
  ArrayType array_type;
  std::string identifier_name;
  donsus_token_kind type;
  /*If the array_type is dynamic the size will be zero and ignored
    If the array_type is static then the size must be provided
  */
  int number_of_elements;
  int size; // Represents the number between the square brackets a:int[3] =
            // [1,2,3]; here it is 3
};

struct array_access {
  std::string identifier_name;
  utility::handle<donsus_ast::node> index;
};
struct bool_expr {
  token value;
};
struct unary_expr {
  token op;
};
struct function_decl {
  Tomi::Vector<TYPE> return_type;

  // function signature
  Tomi::Vector<utility::handle<donsus_ast::node>>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function

  Tomi::Vector<utility::handle<donsus_ast::node>>
      body; // This might be refactored in the future
};

struct function_def {
  Tomi::Vector<TYPE> return_type; // the return type of the function

  // function signature
  Tomi::Vector<utility::handle<donsus_ast::node>>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function

  Tomi::Vector<utility::handle<donsus_ast::node>>
      body; // This might be refactored in the future
};

struct function_call {
  // test(1,2,3,4,5,6)
  // test ->func_name
  // (1,2,3,4,5,6) ->arguments
  std::string func_name;
  Tomi::Vector<utility::handle<donsus_ast::node>> arguments;
};

struct if_statement {
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
  // we will include something for the init_statement_condition
  // the children represents the condition
  // the body represents the block when the condition evaluates to true
  Tomi::Vector<utility::handle<donsus_ast::node>>
      alternate; // this represents the block to be executed if the condition
                 // evaluates as false(else or elif statement)
};

struct else_statement {
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};

struct return_kw {
  Tomi::Vector<TYPE> types; // only if the main type is not long enough
};
struct assignment {
  utility::handle<donsus_ast::node> lvalue;
  utility::handle<donsus_ast::node> rvalue;
  std::string identifier_name;
  token op; // operator
};

struct identifier {
  std::string identifier_name; //  name of lvalue
};

struct string_expr {
  token value;
};
struct expression {
  token value;
};
/*
struct print_expr {};

struct case_expr {};
*/

struct do_expr {
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};

/*struct otherwise_statement {};*/

struct as_statement {
  utility::handle<donsus_ast::node> left;
  utility::handle<donsus_ast::node> right;
};

/*
struct type_statement {};

struct alias_statement {};
struct typeclass {};
struct final {};

struct abstract_statement {};

struct bitshift {};
*/

struct node : utility::property<> {
  Tomi::Vector<utility::handle<donsus_ast::node>> children;
  donsus_node_type type;

  TYPE real_type{};
};
} // namespace donsus_ast

#endif