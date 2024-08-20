#ifndef NODE_H
#define NODE_H

#include "../../Include/token.h"
#include "../Include/Donsus/tomi.h"
#include "../Include/Internal/type.h"
#include "../utility/handle.h"
#include "../utility/property.h"

#include <string>
#include <unordered_map>

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
  X(RETURN_STATEMENT, "RETURN_STATEMENT")                                      \
  X(CONTINUE_STATEMENT, "CONTINUE_STATEMENT")                                  \
  X(BREAK_STATEMENT, "BREAK_STATEMENT")                                        \
  X(FALLTHROUGH_STATEMENT, "FALLTHROUGH_STATEMENT")                            \
  X(CASE, "CASE")                                                              \
  X(DO, "DO")                                                                  \
  X(OF, "OF")                                                                  \
  X(OTHERWISE_STATEMENT, "OTHERWISE_STATEMENT")                                \
  X(AS_STATEMENT, "AS_STATEMENT")                                              \
  X(TYPE_CONSTRUCTOR, "TYPE_CONSTRUCTOR")                                      \
  X(ALIAS_STATEMENT, "ALIAS_STATEMENT")                                        \
  X(TYPECLASS, "TYPECLASS")                                                    \
  X(INSTANCE, "INSTANCE")                                                      \
  X(CLASS, "CLASS")                                                            \
  X(FINAL, "FINAL")                                                            \
  X(ABSTRACT_STATEMENT, "ABSTRACT_STATEMENT")                                  \
  X(BITSHIFT, "BITSHIFT")                                                      \
  X(BITWISE, "BITWISE")                                                        \
  X(ARRAY_DEF, "ARRAY_DEF")                                                    \
  X(ARRAY_DECL, "ARRAY_DECL")                                                  \
  X(STRING_EXPRESSION, "STRING_EXPRESSION")                                    \
  X(BOOL_EXPRESSION, "BOOL_EXPRESSION")                                        \
  X(UNARY_EXPRESSION, "UNARY_EXPRESSION")                                      \
  X(PRINT_EXPRESSION, "PRINT_EXPRESSION")                                      \
  X(ARRAY_ACCESS, "ARRAY_ACCESS")                                              \
  X(FUNCTION_ARG, "FUNCTION_ARG")                                              \
  X(WHILE_LOOP, "WHILE_LOOP")                                                  \
  X(FOR_LOOP, "FOR_LOOP")                                                      \
  X(RANGE_EXPRESSION, "RANGE_EXPRESSION")                                      \
  X(LANGUAGE_EXTENSION, "LANGUAGE_EXTENSION")                                  \
  X(GENERICS_DECL, "GENERICS_DECL")                                            \
  X(ARG_DECL, "ARG_DECL")                                                      \
  X(TUPLE, "TUPLE")                                                            \
  X(POINTER, "POINTER")                                                        \
  X(REFERENCE, "REFERENCE")                                                    \
  X(TEMPLATE_DECL, "TEMPLATE_DECL")                                            \
  X(ELSE_IF_STATEMENT, "ELSE_IF_STATEMENT")                                    \
  X(ELSE_STATEMENT, "ELSE_STATEMENT")                                          \
  X(DATA_CONSTRUCTOR, "DATA_CONSTRUCTOR")                                      \
  X(PATTERN, "PATTERN")                                                        \
  X(CONSTRAINT, "CONSTRAINT")

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

#define SPECIFIERS_LIST                                                        \
  X(none, 0)                                                                   \
  X(comptime, 1 << 0)                                                          \
  X(static_, 1 << 1)                                                           \
  X(thread_local_, 1 << 2)                                                     \
  X(mut, 1 << 3)

enum specifiers_ {
#define X(name, value) name = value,
  SPECIFIERS_LIST
#undef X
};

struct specifiers_utils {
  static const char *type_name(specifiers_ type) {
    switch (type) {
#define X(name, value)                                                         \
  case specifiers_::name:                                                      \
    return #name;
      SPECIFIERS_LIST
#undef X
    default:
      return "Unknown";
    }
  }
};

struct node;
struct variable_def {
  specifiers_ specifiers{};
  utility::handle<donsus_ast::node> identifier_type;
  utility::handle<donsus_ast::node> identifier_name;
  // value stored in children
};
struct range_expr {
  // start..end
  utility::handle<donsus_ast::node> start;
  utility::handle<donsus_ast::node> end;
};

enum class loop_type { TRADITIONAL, RANGE_BASED };
struct for_loop {
  loop_type type_of_loop;
  // RANGE_BASED
  // if loop_variable not set explicitly, thus its a nullptr
  // loop_variable will be created explicitly
  utility::handle<donsus_ast::node> loop_variable;
  // RANGE_BASED
  utility::handle<donsus_ast::node> iterator;
  // TRADITIONAL || RANGE_BASED
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
  // TRADITIONAL
  utility::handle<donsus_ast::node> init_statement;
  // TRADITIONAL
  utility::handle<donsus_ast::node> increment_expr;
  // TRADITIONAL
  utility::handle<donsus_ast::node> condition;
};
enum class while_type { TRADITIONAL, DO_BASED };

struct while_loop {
  while_type type_of_loop;
  utility::handle<donsus_ast::node> condition;
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};
struct integer_expr {
  token value;
};
struct float_expr {
  token value;
};

struct tuple {
  Tomi::Vector<utility::handle<donsus_ast::node>> items;
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
  Tomi::Vector<token> qualifiers;
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
  specifiers_ specifiers{};
  utility::handle<donsus_ast::node> return_type;

  // function signature
  Tomi::Vector<utility::handle<donsus_ast::node>>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function
};

struct function_def {
  specifiers_ specifiers;
  utility::handle<donsus_ast::node>
      return_type; // the return type of the function

  // function signature
  Tomi::Vector<utility::handle<donsus_ast::node>>
      parameters; // parameters inside the function signature

  std::string func_name; // name of the function

  Tomi::Vector<utility::handle<donsus_ast::node>> body;
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
  utility::handle<donsus_ast::node> condition;
  Tomi::Vector<utility::handle<donsus_ast::node>> if_var_decls;
  Tomi::Vector<utility::handle<donsus_ast::node>>
      alternate; // this represents the block to be executed if the condition
                 // evaluates as false(else or elif statement)*/
};
struct else_if_statement {
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
  utility::handle<donsus_ast::node> condition;
  Tomi::Vector<utility::handle<donsus_ast::node>> if_var_decls;
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
  std::string identifier_name;
};

struct string_expr {
  token value;
};
struct expression {
  token value;
};

struct print_expr {};

struct case_expr {
  utility::handle<donsus_ast::node> scrutinee;
  Tomi::Vector<utility::handle<donsus_ast::node>> patterns;
};

struct pattern {
  // if the scrutinee satisfies the guard
  // the value of the expression is the "result_expression"
  utility::handle<donsus_ast::node> guard;
  utility::handle<donsus_ast::node> result_expression;
};

struct arg_decl {
  Tomi::Vector<token> qualifiers;
  utility::handle<donsus_ast::node> identifier_type;
  utility::handle<donsus_ast::node> identifier_name;
  bool is_pointer;
  bool is_reference;
};

struct do_expr {
  Tomi::Vector<utility::handle<donsus_ast::node>> body;
};

/*struct otherwise_statement {};*/

struct as_statement {
  utility::handle<donsus_ast::node> left;
  utility::handle<donsus_ast::node> right;
};

struct type_constructor {
  utility::handle<donsus_ast::node> name;
  Tomi::Vector<utility::handle<donsus_ast::node>> arguments;
  // This is a data constructor
  utility::handle<donsus_ast::node> body;
};

struct data_constructor {};

struct alias {
  utility::handle<donsus_ast::node> existing_type;
  utility::handle<donsus_ast::node> synonym;
};
struct typeclass {
  utility::handle<donsus_ast::node> identifier_name;
  // Todo: this should be optional
  utility::handle<donsus_ast::node> template_decl;
};
struct final {};

struct abstract_statement {};

/*
 * typeclass "<T>" -> "<T>"
 * */
struct template_decl {
  Tomi::Vector<utility::handle<donsus_ast::node>> types;
};

struct bitshift {};
struct class_decl {
  Tomi::Vector<token> qualifiers;
};
struct pointer {
  utility::handle<donsus_ast::node> pointee;
};
struct reference {
  utility::handle<donsus_ast::node> referent;
};

struct language_extension {};
struct instance {
  utility::handle<donsus_ast::node> identifier;
  utility::handle<donsus_ast::node> type;
};
// func :: Num X => X
// decl only contains parameters, it does exclude the last return
// value unlike haskell.
enum class generics_type {
  PARAMETRIC, // unconstrained
  AD_HOC
};

struct generics_decl {
  generics_type type_of_decl;
  utility::handle<donsus_ast::node> name;
  Tomi::Vector<utility::handle<donsus_ast::node>> constraints;
  Tomi::Vector<utility::handle<donsus_ast::node>> params;
};
// Num X
struct constraint {
  // Num
  utility::handle<donsus_ast::node> class_constraint;
  // X
  Tomi::Vector<utility::handle<donsus_ast::node>> type_variables;
};

struct node : utility::property<> {
  Tomi::Vector<utility::handle<donsus_ast::node>> children;
  donsus_node_type type;
  token first_token_in_ast;
  TYPE real_type{};
};
} // namespace donsus_ast

#endif