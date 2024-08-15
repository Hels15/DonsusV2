#ifndef NODE_H
#define NODE_H

#include "../Include/Donsus/tomi.h"
#include "../Include/Internal/type.h"
#include "../utility/handle.h"
#include "../utility/property.h"

#include <string>

namespace donsus_ast {

#define UNDERLYING_NODE_TYPES                                                  \
  X(VARIABLE_DEFINITION, "VARIABLE_DEFINITION")                                \
  X(FUNCTION_DECL, "FUNCTION_DECL")                                            \
  X(FUNCTION_DEF, "FUNCTION_DEF")                                              \
  X(IF_STATEMENT, "IF_STATEMENT")                                              \
  X(ASSIGNMENT, "ASSIGNMENT")                                                  \
  X(IDENTIFIER, "IDENTIFIER")                                                  \
  X(INTEGER_EXPRESSION, "INTEGER_EXPRESSION")                                  \
  X(FLOAT_EXPRESSION, "FLOAT_EXPRESSION")                                      \
  X(BINARY_EXPRESSION, "BINARY_EXPRESSION")                                    \
  X(OCTAL_EXPRESSION, "OCTAL_EXPRESSION")                                      \
  X(HEX_EXPRESSION, "HEX_EXPRESSION")                                          \
  X(EXPRESSION, "EXPRESSION")                                                  \
  X(FUNCTION_CALL, "FUNCTION_CALL")                                            \
  X(ELSE_STATEMENT, "ELSE_STATEMENT")                                          \
  X(RETURN_STATEMENT, "RETURN_STATEMENT")                                      \
  X(CONTINUE_STATEMENT, "CONTINUE_STATEMENT")                                  \
  X(BREAK_STATEMENT, "BREAK_STATEMENT")                                        \
  X(FALLTHROUGH_STATEMENT, "FALLTHROUGH_STATEMENT")                            \
  X(CASE, "CASE")                                                              \
  X(DO, "DO")                                                                  \
  X(OF, "OF")                                                                  \
  X(OTHERWISE_STATEMENT, "OTHERWISE_STATEMENT")                                \
  X(AS_STATEMENT, "AS_STATEMENT")                                              \
  X(TYPE_STATEMENT, "TYPE_STATEMENT")                                          \
  X(ALIAS_STATEMENT, "ALIAS_STATEMENT")                                        \
  X(TYPECLASS, "TYPECLASS")                                                    \
  X(INSTANCE, "INSTANCE")                                                      \
  X(CLASS, "CLASS")                                                            \
  X(FINAL, "FINAL")                                                            \
  X(ABSTRACT_STATEMENT, "ABSTRACT_STATEMENT")                                  \
  X(BITSHIFT, "BITSHIFT")                                                      \
  X(BITWISE, "BITWISE")                                                        \
  X(ARRAY_DEFINITION, "ARRAY_DEFINITION")                                      \
  X(ARRAY_DECLARATION, "ARRAY_DECLARATION")                                    \
  X(STRING_EXPRESSION, "STRING_EXPRESSION")                                    \
  X(BOOL_EXPRESSION, "BOOL_EXPRESSION")                                        \
  X(UNARY_EXPRESSION, "UNARY_EXPRESSION")                                      \
  X(PRINT_EXPRESSION, "PRINT_EXPRESSION")                                      \
  X(ARRAY_ACCESS, "ARRAY_ACCESS")                                              \
  X(FUNCTION_ARG, "FUNCTION_ARG")                                              \
  X(WHILE_LOOP, "WHILE_LOOP")                                                  \
  X(FOR_LOOP, "FOR_LOOP")                                                      \
  X(RANGE_EXPRESSION, "RANGE_EXPRESSION")

struct donsus_node_type {
  enum underlying : int {
#define X(name, text) name,
    UNDERLYING_NODE_TYPES
#undef X
  };
  donsus_node_type() = default;
  donsus_node_type(underlying type);

  const char *type_name() const;
  const char *type_name(underlying) const;

  underlying type;
};

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