#include "print_ast.h"
#include <iostream>

std::string lexeme_value(token token_, const std::string &source) {
  return source.substr(token_.offset, token_.length);
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

inline void print_array_body(donsus_ast::array array_def, int indent_level,
                             std::string &source) {
  print_with_newline("Array body items:", indent_level);
  for (auto n : array_def.items) {
    print_ast_node(n, indent_level, source);
  }
}
inline void print_indices_expression(donsus_ast::indices indices,
                                     int indent_level, std::string &source) {
  print_with_newline("name: ", indent_level + 1);
  print_ast_node(indices.name, indent_level + 2, source);

  print_with_newline("index: ", indent_level + 1);
  print_ast_node(indices.index, indent_level + 2, source);
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
}

inline void print_type(donsus_ast::donsus_node_type type, int indent_level) {
  print_with_newline("type: " + std::string(type.type_name()), indent_level);
}

inline void print_function_def(donsus_ast::function_def &f_def,
                               int indent_level, std::string &source) {
  print_with_newline("return_types: ", indent_level);
  print_ast_node(f_def.return_type, indent_level + 1, source);
  print_with_newline("Actual type: ", indent_level);
  if (f_def.function_type == donsus_ast::FunctionType::Unknown) {
    print_with_newline("Unknown", indent_level + 1);
  } else if (f_def.function_type == donsus_ast::FunctionType::DECL) {
    print_with_newline("DECL", indent_level + 1);
  } else if (f_def.function_type == donsus_ast::FunctionType::DEF) {
    print_with_newline("DEF", indent_level + 1);
  } else {
    print_with_newline("Unrecognised", indent_level + 1);
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
  print_with_newline("specifiers: " +
                         std::string(donsus_ast::specifiers_utils::type_name(
                             f_def.specifiers)),
                     indent_level);
}

inline void print_if_statement(donsus_ast::if_statement &statement,
                               int indent_level, std::string &source) {
  print_with_newline("If Var Decls: ", indent_level);
  for (auto node : statement.if_var_decls) {
    print_ast_node(node, indent_level + 1, source);
  }
  if (statement.condition) {
    print_with_newline("condition: ", indent_level);
  }
  print_ast_node(statement.condition, indent_level, source);
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
}
inline void print_typeclass(donsus_ast::typeclass &typeclass, int indent_level,
                            std::string &source) {
  print_with_newline("identifier name: ", indent_level);
  print_ast_node(typeclass.identifier_name, indent_level + 1, source);

  print_ast_node(typeclass.template_decl, indent_level + 1, source);

  print_with_newline("body: ", indent_level);
  for (auto item : typeclass.body) {
    print_ast_node(item, indent_level + 1, source);
  }
}

inline void print_template_decl(donsus_ast::template_decl &template_decl,
                                int indent_level, std::string &source) {
  print_with_newline("template decl: ", indent_level);
  print_with_newline("types: ", indent_level);
  for (auto node : template_decl.types) {
    print_ast_node(node, indent_level + 1, source);
  }
}
inline void print_loop(donsus_ast::for_loop &loop, int indent_level,
                       std::string &source) {
  print_with_newline("loop type:", indent_level);
  switch (loop.type_of_loop) {
  case donsus_ast::loop_type::TRADITIONAL: {
    print_with_newline("init_statement: ", indent_level);
    print_ast_node(loop.init_statement, indent_level, source);
    print_with_newline("condition: ", indent_level);
    print_ast_node(loop.condition, indent_level, source);
    print_with_newline("increment_expr: ", indent_level);
    print_ast_node(loop.increment_expr, indent_level, source);
    for (auto node : loop.body) {
      print_ast_node(node, indent_level + 1, source);
    }
    break;
  }
  case donsus_ast::loop_type::RANGE_BASED: {
    print_with_newline("loop_variable: " + loop.loop_variable, indent_level);
    print_with_newline("iterator: ", indent_level);
    print_ast_node(loop.iterator, indent_level + 1, source);
    print_with_newline("body: ", indent_level);
    for (auto node : loop.body) {
      print_ast_node(node, indent_level + 1, source);
    }
    break;
  }
  case donsus_ast::loop_type::Unknown: {
    print_with_newline("UNKNOWN", indent_level);
  }
  }
}
inline void print_tuple(donsus_ast::tuple &tuple, int indent_level,
                        std::string &source) {
  print_with_newline("Items:", indent_level);
  for (auto item : tuple.items) {
    print_ast_node(item, indent_level + 1, source);
    print_with_newline("\n", indent_level);
  }
}
inline void print_instance_statement(donsus_ast::instance &instance,
                                     int indent_level, std::string &source) {
  print_with_newline("identifier_name: ", indent_level + 1);
  print_ast_node(instance.identifier_name, indent_level, source);
  print_with_newline("type: ", indent_level + 1);
  print_ast_node(instance.type, indent_level, source);
  print_with_newline("body: ", indent_level + 1);
  for (auto node : instance.body) {
    print_ast_node(node, indent_level + 1, source);
  }
}
inline void
print_language_extension(donsus_ast::language_extension &language_extension,
                         int indent_level, std::string &source) {
  print_with_newline("Extensions:", indent_level + 1);
  print_ast_node(language_extension.extensions, indent_level, source);
}
inline void print_else_if_statement(donsus_ast::else_if_statement &statement,
                                    int indent_level, std::string &source) {
  if (statement.condition) {
    print_with_newline("condition: ", indent_level);
  }
  print_with_newline("If Var Decls: ", indent_level);
  for (auto node : statement.if_var_decls) {
    print_ast_node(node, indent_level + 1, source);
  }
  print_ast_node(statement.condition, indent_level, source);
  print_with_newline("body: ", indent_level);
  for (auto node : statement.body) {
    print_ast_node(node, indent_level + 1, source);
  }
}
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

inline void print_array_def(donsus_ast::array_def &def, int indent_level,
                            std::string &source) {
  print_with_newline(
      "identifier_type: " +
          def.identifier_type->get<donsus_ast::identifier>().identifier_name,
      indent_level);
  print_with_newline(
      "identifier_name: " +
          def.identifier_name->get<donsus_ast::identifier>().identifier_name,
      indent_level);

  print_with_newline(
      "indices: " +
          (def.indices
               ? lexeme_value(
                     def.indices->get<donsus_ast::integer_expr>().value, source)
               : "not defined"),
      indent_level);

  std::string array_type_str;
  switch (def.array_type) {
  case donsus_ast::ArrayType::FIXED:
    array_type_str = "FIXED";
    break;
  case donsus_ast::ArrayType::STATIC:
    array_type_str = "STATIC";
    break;
  case donsus_ast::ArrayType::DYNAMIC:
    array_type_str = "DYNAMIC";
    break;
  default:
    array_type_str = "UNKNOWN";
    break;
  }
  print_with_newline("type of array: " + array_type_str, indent_level);

  print_with_newline("body: ", indent_level);
  print_ast_node(def.body, indent_level + 1, source);
  print_with_newline(" ", indent_level);
}

inline void print_class_def(donsus_ast::class_def &def, int indent_level,
                            std::string &source) {
  print_with_newline("NAME:", indent_level + 1);
  print_ast_node(def.name, indent_level + 2, source);
  print_with_newline(
      "specifiers: " +
          std::string(
              donsus_ast::specifiers_class_utils::type_name(def.specifiers)),
      indent_level);
  print_with_newline("BODY:", indent_level + 1);
  for (auto child : def.body) {
    print_ast_node(child, indent_level + 2, source);
  }
  print_with_newline("INHERITS: ", indent_level + 1);
  for (auto child : def.inherits) {
    print_ast_node(child, indent_level + 2, source);
  }
}
inline void print_var_def(donsus_ast::variable_def &def, int indent_level,
                          std::string &source) {
  print_with_newline("identifier_type: ", indent_level);
  print_ast_node(def.identifier_type, indent_level + 1, source);
  print_with_newline(
      "identifier_name: " +
          def.identifier_name->get<donsus_ast::identifier>().identifier_name,
      indent_level);
  print_with_newline(
      "specifiers: " +
          std::string(donsus_ast::specifiers_utils::type_name(def.specifiers)),
      indent_level);
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
    print_var_def(ast_node->get<donsus_ast::variable_def>(), indent_level,
                  source); // reuse variable decl
    for (auto children_expr : ast_node->children) {
      print_with_newline("children: ", indent_level);
      print_ast_node(children_expr, indent_level + 2, source);
    }

    break;
  }
  case type::ARRAY: {
    print_type(ast_node->type, indent_level);
    print_array_body(ast_node->get<donsus_ast::array>(), indent_level, source);
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
    print_var_def(ast_node->get<donsus_ast::variable_def>(), indent_level,
                  source);
    break;
  }
  case type::INSTANCE: {
    print_type(ast_node->type, indent_level);
    print_instance_statement(ast_node->get<donsus_ast::instance>(),
                             indent_level, source);
    break;
  }
  case type::PRINT_EXPRESSION: {
    print_type(ast_node->type, indent_level);
    indent_level++;
    print_with_newline("expression: ", indent_level);
    print_ast_node(ast_node->children[0], indent_level + 1, source);
    break;
  }
  case type::LANGUAGE_EXTENSION: {
    print_type(ast_node->type, indent_level);
    print_language_extension(ast_node->get<donsus_ast::language_extension>(),
                             indent_level, source);
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
  case type::TUPLE: {
    print_type(ast_node->type, indent_level);
    print_tuple(ast_node->get<donsus_ast::tuple>(), indent_level, source);
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
  case type::INDICES: {
    print_type(ast_node->type, indent_level);
    print_indices_expression(ast_node->get<donsus_ast::indices>(), indent_level,
                             source);
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

  case type::PARAM_DECL: {
    print_ast_node(ast_node->get<donsus_ast::param_decl>().identifier_name,
                   indent_level, source);
    print_ast_node(ast_node->get<donsus_ast::param_decl>().identifier_type,
                   indent_level, source);
    print_with_newline(
        "specifiers: " +
            std::string(donsus_ast::specifiers_utils::type_name(
                ast_node->get<donsus_ast::param_decl>().specifiers)),
        indent_level);
    break;
  }
  case type::IDENTIFIER: {
    print_type(ast_node->type, indent_level);
    print_identifier(ast_node->get<donsus_ast::identifier>(), indent_level);
    break;
  }

    /*  case type::ARRAY_DECL: {
        print_type(ast_node->type, indent_level);
        print_array_decl(ast_node->get<donsus_ast::array_decl>(), indent_level);
        break;
      }*/

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
    print_if_statement(ast_node->get<donsus_ast::if_statement>(),
                       indent_level + 1, source);
    break;
  }
  case type::ELSE_IF_STATEMENT: {
    print_type(ast_node->type, indent_level);
    print_else_if_statement(ast_node->get<donsus_ast::else_if_statement>(),
                            indent_level + 1, source);
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
    print_type(ast_node->type, indent_level + 1);
    print_loop(ast_node->get<donsus_ast::for_loop>(), indent_level, source);
    break;
  }

  case type::POINTER: {
    print_type(ast_node->type, indent_level);
    print_with_newline("POINTEE:", indent_level);
    print_ast_node(ast_node->get<donsus_ast::pointer>().pointee,
                   indent_level + 1, source);
    break;
  }
  case type::REFERENCE: {
    print_type(ast_node->type, indent_level);
    print_with_newline("REFERENT:", indent_level);
    print_ast_node(ast_node->get<donsus_ast::reference>().referent,
                   indent_level + 1, source);
    break;
  }
  case type::PATTERN: {
    print_type(ast_node->type, indent_level);
    print_with_newline("TYPE: ", indent_level + 1);

    print_with_newline("GUARD: ", indent_level + 1);
    print_ast_node(ast_node->get<donsus_ast::pattern>().guard, indent_level + 2,
                   source);

    print_with_newline("RESULT EXPRESSION: ", indent_level + 1);
    print_ast_node(ast_node->get<donsus_ast::pattern>().result_expression,
                   indent_level + 2, source);
    break;
  }
  case type::CLASS: {
    print_type(ast_node->type, indent_level);
    print_class_def(ast_node->get<donsus_ast::class_def>(), indent_level + 1,
                    source);
    break;
  }
  case type::CASE: {
    print_type(ast_node->type, indent_level);
    print_with_newline("SCRUTINEE:", indent_level + 1);
    print_ast_node(ast_node->get<donsus_ast::case_expr>().scrutinee,
                   indent_level + 1, source);
    print_with_newline("PATTERNS: ", indent_level + 1);
    for (auto pattern : ast_node->get<donsus_ast::case_expr>().patterns) {
      print_ast_node(pattern, indent_level + 1, source);
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
  case type::TYPECLASS: {
    print_type(ast_node->type, indent_level);
    print_typeclass(ast_node->get<donsus_ast::typeclass>(), indent_level,
                    source);
    break;
  }
  case type::TEMPLATE_DECL: {
    print_type(ast_node->type, indent_level);
    print_template_decl(ast_node->get<donsus_ast::template_decl>(),
                        indent_level, source);
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
    if (!ast_node->get<donsus_ast::return_statement>().body) {
      print_with_newline("body: {}", indent_level);
    } else {
      print_with_newline("body: ", indent_level);
      print_ast_node(ast_node->get<donsus_ast::return_statement>().body,
                     indent_level + 1, source);
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