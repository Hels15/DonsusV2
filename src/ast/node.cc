#include "node.h"

donsus_ast::donsus_node_type::donsus_node_type(underlying type) : type(type) {}

std::string
donsus_ast::de_get_from_donsus_node_type(donsus_ast::donsus_node_type type) {
  switch (type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
    return "DONSUS_VARIABLE_DEFINITION";
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL:
    return "DONSUS_FUNCTION_DECLARATION";
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF:
    return "DONSUS_FUNCTION_DEFINITION";
  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT:
    return "DONSUS_ASSIGNMENT";
  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER:
    return "DONSUS_IDENTIFIER";
  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT:
    return "DONSUS_IF_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION:
    return "DONSUS_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT:
    return "DONSUS_ELSE_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL:
    return "DONSUS_FUNCTION_CALL";
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT:
    return "DONSUS_RETURN_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION:
    return "DONSUS_STRING_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION:
    return "DONSUS_UNARY_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION:
    return "DONSUS_BOOL_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION:
    return "DONSUS_PRINT_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION:
    return "DONSUS_ARRAY_DECLARATION";
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION:
    return "DONSUS_ARRAY_DEFINITION";
  case donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION:
    return "DONSUS_FLOAT_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS:
    return "DONSUS_ARRAY_ACCESS";
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG:
    return "DONSUS_FUNCTION_ARG";
  case donsus_ast::donsus_node_type::DONSUS_WHILE_LOOP:
    return "DONSUS_WHILE_LOOP";
  case donsus_ast::donsus_node_type::DONSUS_FOR_LOOP:
    return "DONSUS_FOR_LOOP";
  case donsus_ast::donsus_node_type::DONSUS_RANGE_EXPRESSION:
    return "DONSUS_RANGE_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_INTEGER_EXPRESSION:
    return "DONSUS_INTEGER_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_BINARY_EXPRESSION:
    return "DONSUS_BINARY_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_OCTAL_EXPRESSION:
    return "DONSUS_OCTAL_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_HEX_EXPRESSION:
    return "DONSUS_HEX_EXPRESSION";
  case donsus_ast::donsus_node_type::DONSUS_CONTINUE_STATEMENT:
    return "DONSUS_CONTINUE_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_BREAK_STATEMENT:
    return "DONSUS_BREAK_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_FALLTHROUGH_STATEMENT:
    return "DONSUS_FALLTHROUGH_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_CASE:
    return "DONSUS_CASE";
  case donsus_ast::donsus_node_type::DONSUS_DO:
    return "DONSUS_DO";
  case donsus_ast::donsus_node_type::DONSUS_OF:
    return "DONSUS_OF";
  case donsus_ast::donsus_node_type::DONSUS_OTHERWISE_STATEMENT:
    return "DONSUS_OTHERWISE_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_AS_STATEMENT:
    return "DONSUS_AS_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_TYPE_STATEMENT:
    return "DONSUS_TYPE_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_ALIAS_STATEMENT:
    return "DONSUS_ALIAS_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_TYPECLASS:
    return "DONSUS_TYPECLASS";
  case donsus_ast::donsus_node_type::DONSUS_INSTANCE:
    return "DONSUS_INSTANCE";
  case donsus_ast::donsus_node_type::DONSUS_CLASS:
    return "DONSUS_CLASS";
  case donsus_ast::donsus_node_type::DONSUS_FINAL:
    return "DONSUS_FINAL";
  case donsus_ast::donsus_node_type::DONSUS_ABSTRACT_STATEMENT:
    return "DONSUS_ABSTRACT_STATEMENT";
  case donsus_ast::donsus_node_type::DONSUS_BITSHIFT:
    return "DONSUS_BITSHIFT";
  case donsus_ast::donsus_node_type::DONSUS_BITWISE:
    return "DONSUS_BITWISE";
  default: {
    return "";
  }
  }
}