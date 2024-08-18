#include "print_ast.h"
#include <iostream>

std::string lexeme_value(token token_, std::string &source) {
  return "to be determined";
}

inline void print_ast_node(utility::handle<donsus_ast::node> ast_node,
                           int indent_level, std::string &source);

inline std::string generate_indentation(int level) {
  return std::string(2 * level, ' ');
}

inline void print_with_newline(const std::string &s, int indent_level) {
  std::cout << generate_indentation(indent_level) << s << '\n';
}

inline void print_function_call(donsus_ast::function_call &f_call,
                                int indent_level, std::string &source) {
  print_with_newline("func_name: " + f_call.func_name, indent_level);
  print_with_newline("arguments: ", indent_level);
  for (auto p : f_call.arguments) {
    print_ast_node(p, indent_level + 1, source);
    print_with_newline(" ", indent_level);
  }
}

inline void print_range(donsus_ast::range_expr &range, int indent_level,
                        std::string &source) {
  print_with_newline("start: ", indent_level);
  print_ast_node(range.start, indent_level + 1, source);
  print_with_newline("end: ", indent_level);
  print_ast_node(range.end, indent_level + 1, source);
}

inline void print_print_expression(utility::handle<donsus_ast::node> &ast_node,
                                   int indent_level, std::string &source) {
  print_ast_node(ast_node, indent_level + 1, source);
}

inline void print_integer_expression(donsus_ast::integer_expr &num_expr,
                                     int indent_level, std::string &source) {
  print_with_newline("kind: " + std::string(num_expr.value.type_name()),
                     indent_level);
  print_with_newline("value: " + lexeme_value(num_expr.value, source),
                     indent_level);
  print_with_newline("length: " + std::to_string(num_expr.value.length),
                     indent_level);
  print_with_newline("line: " + std::to_string(num_expr.value.line),
                     indent_level);
  print_with_newline("precedence: " + std::to_string(num_expr.value.precedence),
                     indent_level);
}

inline void print_float_expression(donsus_ast::float_expr &float_expr,
                                   int indent_level, std::string &source) {
  print_with_newline("kind: " + std::string(float_expr.value.type_name()),
                     indent_level);
  print_with_newline("value: " + lexeme_value(float_expr.value, source),
                     indent_level);
  print_with_newline("length: " + std::to_string(float_expr.value.length),
                     indent_level);
  print_with_newline("line: " + std::to_string(float_expr.value.line),
                     indent_level);
  print_with_newline("precedence: " +
                         std::to_string(float_expr.value.precedence),
                     indent_level);
}

inline void print_string_expression(donsus_ast::string_expr &string_expr,
                                    int indent_level, std::string &source) {
  print_with_newline("kind: " + std::string(string_expr.value.type_name()),
                     indent_level);
  print_with_newline("value: " + lexeme_value(string_expr.value, source),
                     indent_level);
  print_with_newline("length: " + std::to_string(string_expr.value.length),
                     indent_level);
  print_with_newline("line: " + std::to_string(string_expr.value.line),
                     indent_level);
  print_with_newline("precedence: " +
                         std::to_string(string_expr.value.precedence),
                     indent_level);
}

inline void print_bool_expression(donsus_ast::bool_expr &bool_expr,
                                  int indent_level, std::string &source) {
  print_with_newline("kind: " + std::string(bool_expr.value.type_name()),
                     indent_level);
  print_with_newline("value: " + lexeme_value(bool_expr.value, source),
                     indent_level);
  print_with_newline("length: " + std::to_string(bool_expr.value.length),
                     indent_level);
  print_with_newline("line: " + std::to_string(bool_expr.value.line),
                     indent_level);
  print_with_newline("precedence: " +
                         std::to_string(bool_expr.value.precedence),
                     indent_level);
}

inline void print_identifier(donsus_ast::identifier &identifier,
                             int indent_level) {
  print_with_newline("identifier_name: " + identifier.identifier_name,
                     indent_level);
}

inline void print_assignment(donsus_ast::assignment &assignment,
                             int indent_level, std::string &source) {
  print_with_newline("lvalue: ", indent_level);
  print_ast_node(assignment.lvalue, indent_level + 1, source);
  print_with_newline("rvalue: ", indent_level);
  print_ast_node(assignment.rvalue, indent_level + 1, source);
  print_with_newline("identifier_op: " + std::string(assignment.op.type_name()),
                     indent_level);
  print_with_newline("identifier_name: " + assignment.identifier_name,
                     indent_level);
}

inline void print_type(donsus_ast::donsus_node_type type, int indent_level) {
  print_with_newline("type: " + std::string(type.type_name()), indent_level);
}

inline void print_function_decl(donsus_ast::function_decl &f_decl,
                                int indent_level, std::string &source) {
  print_with_newline("return_types: ", indent_level);
  // Todo: Fix this
  /*    print_with_newline("return_type: " +
                             std::string(token::type_name(f_decl.return_type.type)),
                         indent_level + 1);*/

  print_with_newline("parameters: ", indent_level);
  for (auto p : f_decl.parameters) {
    print_ast_node(p, indent_level + 1, source);
  }
  print_with_newline("func_name: " + f_decl.func_name, indent_level);
}

inline void print_function_def(donsus_ast::function_def &f_def,
                               int indent_level, std::string &source) {
  print_with_newline("return_types: ", indent_level);
  for (auto r : f_def.return_type) {
    print_with_newline("return_type: " +
                           std::string(token::type_name(r.to_parse(r.type_un))),
                       indent_level + 1);
  }

  print_with_newline("parameters: ", indent_level);
  for (auto p : f_def.parameters) {
    print_ast_node(p, indent_level + 1, source);
  }
  print_with_newline("func_name: " + f_def.func_name, indent_level);
  print_with_newline("body: ", indent_level);
  for (auto node : f_def.body) {
    print_ast_node(node, indent_level + 1, source);
  }
}

inline void print_statement(donsus_ast::if_statement &statement,
                            int indent_level, std::string &source) {
  print_with_newline("body: ", indent_level);
  for (auto node : statement.body) {
    print_ast_node(node, indent_level + 1, source);
  }
  if (statement.alternate.empty()) {
    print_with_newline("alternate: {}", indent_level);
  } else {
    for (auto node : statement.alternate) {
      print_with_newline("alternate: ", indent_level);
      print_ast_node(node, indent_level + 1, source);
    }
  }
};

inline void print_else_statement(donsus_ast::else_statement &statement,
                                 int indent_level, std::string &source) {
  print_with_newline("body: ", indent_level);
  for (auto node : statement.body) {
    print_ast_node(node, indent_level + 1, source);
  }
}

inline void print_expression(donsus_ast::expression &expression,
                             int indent_level, std::string &source) {
  print_with_newline("kind: " + std::string(expression.value.type_name()),
                     indent_level);
  print_with_newline("value: " + lexeme_value(expression.value, source),
                     indent_level);
  print_with_newline("length: " + std::to_string(expression.value.length),
                     indent_level);
  print_with_newline("line: " + std::to_string(expression.value.line),
                     indent_level);
  print_with_newline("precedence: " +
                         std::to_string(expression.value.precedence),
                     indent_level);
}

inline void print_array_decl(donsus_ast::array_decl &decl, int indent_level) {
  print_with_newline("identifier_type: " +
                         std::string(token::type_name(decl.type)),
                     indent_level);
  print_with_newline("identifier_name: " + decl.identifier_name, indent_level);
  print_with_newline("size: " + std::to_string(decl.size), indent_level);
  print_with_newline("number_of_elements: " +
                         std::to_string(decl.number_of_elements),
                     indent_level);
}

inline void print_array_def(donsus_ast::array_def &def, int indent_level,
                            std::string &source) {
  print_with_newline("identifier_type: " +
                         std::string(token::type_name(def.type)),
                     indent_level);
  print_with_newline("identifier_name: " + def.identifier_name, indent_level);
  print_with_newline("size: " + std::to_string(def.size), indent_level);
  print_with_newline("number_of_elements: " +
                         std::to_string(def.number_of_elements),
                     indent_level);
  print_with_newline("elements: ", indent_level);
  for (auto e : def.elements) {
    print_ast_node(e, indent_level + 1, source);
    print_with_newline(" ", indent_level);
  }
}

inline void print_var_def(donsus_ast::variable_def &def, int indent_level) {
  // Todo: fix this
  /*print_with_newline("identifier_type: " +
                         std::string(token::type_name(def.identifier_type)),
                     indent_level);*/
  print_with_newline("identifier_name: " + def.identifier_name, indent_level);
}

inline void print_ast_node(utility::handle<donsus_ast::node> ast_node,
                           int indent_level, std::string &source) {
  if (!ast_node)
    return;

  using type = donsus_ast::donsus_node_type;

  switch (ast_node->type.type) {
  case type::VARIABLE_DEFINITION: {
    print_type(ast_node->type, indent_level);
    indent_level++;
    print_var_def(ast_node->get<donsus_ast::variable_def>(),
                  indent_level); // reuse variable decl
    for (auto children_expr : ast_node->children) {
      print_with_newline("children: ", indent_level);
      print_ast_node(children_expr, indent_level + 2, source);
    }

    break;
  }

  case type::STRING_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_string_expression(ast_node->get<donsus_ast::string_expr>(),
                            indent_level, source);
    break;
  }

  case type::RANGE_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_range(ast_node->get<donsus_ast::range_expr>(), indent_level, source);
    break;
  }

  case type::BOOL_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_bool_expression(ast_node->get<donsus_ast::bool_expr>(), indent_level,
                          source);
    break;
  }

  case type::UNARY_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_with_newline("expression: ", indent_level);
    print_ast_node(ast_node->children[0], indent_level + 1, source);
    break;
  }

  case type::FUNCTION_ARG: {
    print_type(ast_node->type, indent_level);
    print_var_def(ast_node->get<donsus_ast::variable_def>(), indent_level);
    break;
  }

  case type::PRINT_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    indent_level++;
    print_with_newline("expression: ", indent_level);
    print_ast_node(ast_node->children[0], indent_level + 1, source);
    break;
  }

  case type::ASSIGNMENT: {
    print_type(ast_node->type, indent_level);
    indent_level++;
    print_assignment(ast_node->get<donsus_ast::assignment>(), indent_level,
                     source);

    for (auto children_expr : ast_node->children) {
      print_ast_node(children_expr, indent_level + 2, source);
    }
    break;
  }
  case type::INTEGER_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_integer_expression(ast_node->get<donsus_ast::integer_expr>(),
                             indent_level, source);

    if (ast_node->children.empty()) {
      print_with_newline("children: {}", indent_level);
    } else {
      print_with_newline("children: ", indent_level);
      for (auto children : ast_node->children) {
        print_ast_node(children, indent_level + 1, source);
        print_with_newline(" ", indent_level);
      }
    }
    break;
  }

  case type::FLOAT_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_float_expression(ast_node->get<donsus_ast::float_expr>(),
                           indent_level, source);

    if (ast_node->children.empty()) {
      print_with_newline("children: {}", indent_level);
    } else {
      print_with_newline("children: ", indent_level);
      for (auto children : ast_node->children) {
        print_ast_node(children, indent_level + 1, source);
        print_with_newline(" ", indent_level);
      }
    }
    break;
  }

  case type::IDENTIFIER: {
    print_type(ast_node->type, indent_level);
    print_identifier(ast_node->get<donsus_ast::identifier>(), indent_level);
    break;
  }
  case type::FUNCTION_DECL: {
    print_type(ast_node->type, indent_level);
    print_function_decl(ast_node->get<donsus_ast::function_decl>(),
                        indent_level + 1, source);
    break;
  }

  case type::ARRAY_DECL: {
    print_type(ast_node->type, indent_level);
    print_array_decl(ast_node->get<donsus_ast::array_decl>(), indent_level);
    break;
  }

  case type::ARRAY_DEF: {
    print_type(ast_node->type, indent_level);
    print_array_def(ast_node->get<donsus_ast::array_def>(), indent_level,
                    source);
    break;
  }

  case type::FUNCTION_DEF: {
    print_type(ast_node->type, indent_level);
    print_function_def(ast_node->get<donsus_ast::function_def>(),
                       indent_level + 1, source);
    break;
  }

  case type::IF_STATEMENT: {
    print_type(ast_node->type, indent_level);
    if (ast_node->children.empty()) {
      print_with_newline("condition: {}", indent_level);
    } else {
      print_with_newline("condition: ", indent_level);
      for (auto children : ast_node->children) {
        print_ast_node(children, indent_level + 1, source);
        print_with_newline(" ", indent_level);
      }
    }

    print_statement(ast_node->get<donsus_ast::if_statement>(), indent_level,
                    source);

    break;
  }

  case type::FUNCTION_CALL: {
    print_type(ast_node->type, indent_level);
    print_function_call(ast_node->get<donsus_ast::function_call>(),
                        indent_level, source);
    break;
  }

  case type::WHILE_LOOP: {
    print_type(ast_node->type, indent_level);
    print_with_newline("condition: ", indent_level);
    print_ast_node(ast_node->children[0], indent_level + 1, source);
    print_with_newline("body: ", indent_level);
    for (auto node : ast_node->get<donsus_ast::while_loop>().body) {
      print_ast_node(node, indent_level + 1, source);
    }
    break;
  }

  case type::ELSE_STATEMENT: {
    print_type(ast_node->type, indent_level);
    print_else_statement(ast_node->get<donsus_ast::else_statement>(),
                         indent_level, source);
    break;
  }

  case type::FOR_LOOP: {
    print_type(ast_node->type, indent_level);
    print_with_newline("loop_variable: " +
                           ast_node->get<donsus_ast::for_loop>().loop_variable,
                       indent_level);
    print_with_newline("iterator: ", indent_level);
    print_ast_node(ast_node->get<donsus_ast::for_loop>().iterator,
                   indent_level + 1, source);
    print_with_newline("body: ", indent_level);
    for (auto node : ast_node->get<donsus_ast::for_loop>().body) {
      print_ast_node(node, indent_level + 1, source);
    }
    break;
  }

  case type::EXPRESSION: {
    print_type(ast_node->type, indent_level);
    print_expression(ast_node->get<donsus_ast::expression>(), indent_level,
                     source);
    if (ast_node->children.empty()) {
      print_with_newline("children: {}", indent_level);
    } else {
      print_with_newline("children: ", indent_level);
      for (auto children : ast_node->children) {
        print_ast_node(children, indent_level + 1, source);
        print_with_newline(" ", indent_level);
      }
    }

    break;
  }

  case type::ARRAY_ACCESS: {
    print_type(ast_node->type, indent_level);
    print_with_newline(
        "identifier_name: " +
            ast_node->get<donsus_ast::array_access>().identifier_name,
        indent_level);
    print_ast_node(ast_node->get<donsus_ast::array_access>().index,
                   indent_level, source);
    break;
  }

  case type::RETURN_STATEMENT: {
    print_type(ast_node->type, indent_level);
    if (ast_node->children.empty()) {
      print_with_newline("children: {}", indent_level);
    } else {
      print_with_newline("children: ", indent_level);
      for (auto children : ast_node->children) {
        print_ast_node(children, indent_level + 1, source);
        print_with_newline(" ", indent_level);
      }
    }
    break;
  }

  default:
    break;
  }
}

void print_ast(utility::handle<donsus_ast::tree> tree, std::string &source) {
  int indent_level = 0;

  for (auto n : tree->get_nodes()) {
    print_ast_node(n, indent_level, source);
  }
}