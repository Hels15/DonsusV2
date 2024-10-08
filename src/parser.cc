#include "../Include/parser.h"
#include "../Include/Donsus/tomi.h"
#include "../Include/ast/nodes/node.h"
#include "../src/utility/parser/helpers.h"
#include "../src/utility/parser/print_ast.h"

// Todo: add unary
// Todo: Replace it when switching to multi-threading
Tomi::Vector<AstFile *> global_files;

Parser::Parser(donsus_lexer &lexer, AstFile &file)
    : lexer(lexer), allocator(1024), file(file) {
  cur_token = donsus_lexer_next(*this);
  tree = allocator.r_alloc<donsus_ast::tree>();
  file.parser = this;
  global_files.push_back(&file);
}

token Parser::parser_next() {
  cur_token = donsus_lexer_next(*this);
  // Todo: think about this
  if (cur_token.kind == donsus_token_kind::NEWLINE)
    parser_next();
  return cur_token;
}

auto Parser::get_error_stream() -> std::string {
  return error.output_stream.toString();
}
void Parser::print_token() {
  while (cur_token.kind != donsus_token_kind::END) {
    std::cout << "Kind: " << cur_token.type_name() << "\n";
    std::cout << "Length: " << cur_token.length << "\n";
    std::cout << "Line: " << cur_token.line << "\n";
    std::cout << "Column: " << cur_token.column << "\n";
    std::cout << "Offset: " << cur_token.offset << "\n";
    std::cout << "Value: " << lexeme_value(cur_token, file.source) << "\n";
    std::cout << "--------------------------------- \n";
    parser_next();
  }
}

token Parser::peek(int loop) {
  donsus_lexer save = lexer;
  token result_main;

  // skipping through newline
  int token_count = 0;
  while (token_count < loop) {
    result_main = donsus_lexer_next(*this);

    if (result_main.kind != donsus_token_kind::NEWLINE) {
      token_count++;
    }
  }

  lexer = save;
  return result_main;
}

auto Parser::parse() -> end_result {
#if DEBUG
  std::cout << "LEXER: "
            << "\n";
  Parser save = *this;
  print_token();
  *this = save;
#endif
  // statement:  declarationstatement
  //           | expressionstatement
  //           | compoundstatement
  //           | selectionstatement
  //           | iterationstatement
  //           | classdeclstatement;

  while (cur_token.kind != donsus_token_kind::END) {
    // start/declaration statement
    if (cur_token.kind == donsus_token_kind::LET_KW &&
        is_specifier(peek().kind)) {

      parse_result result = variable_def();
      // multiple definitions
      if (peek().kind == donsus_token_kind::COMM) {
        parser_except(donsus_token_kind::COMM);
        parser_next();
        Tomi::Vector<parse_result> vars = variable_multi_def_multi_value(
            result->get<donsus_ast::variable_def>());
        tree->add_node(result);
        for (auto var : vars) {
          tree->add_node(var);
        }
        parser_except(donsus_token_kind::SEMICOLON);
      } else {
        tree->add_node(result);
        parser_except(donsus_token_kind::SEMICOLON);
      }
    }

    else if (cur_token.kind == donsus_token_kind::INSTANCE_KW) {
      parse_result result = instance();
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::AT) {
      parse_result result = language_extension();
      parser_except(donsus_token_kind::SEMICOLON);
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::TYPECLASS_KW) {
      parse_result result = typeclass();
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::ALIAS_KW) {
      parse_result result = alias();
      parser_except(donsus_token_kind::SEMICOLON);
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::DOUBLE_COLON) {
      parse_result result = generics_decl();
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::COLON &&
               peek(3).kind != donsus_token_kind::LSQB) {
      parse_result result = variable_def();
      if (peek().kind == donsus_token_kind::COMM) {
        parser_except(donsus_token_kind::COMM);
        parser_next();
        Tomi::Vector<parse_result> vars = variable_multi_def_multi_value(
            result->get<donsus_ast::variable_def>());
        tree->add_node(result);
        for (auto var : vars) {
          tree->add_node(var);
        }
        parser_except(donsus_token_kind::SEMICOLON);
      } else {
        tree->add_node(result);
        parser_except(donsus_token_kind::SEMICOLON);
      }
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               is_assignment(peek().kind)) {
      parse_result result = assignments();
      parser_except(donsus_token_kind::SEMICOLON);
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::COMM &&
               peek(2).kind == donsus_token_kind::IDENTIFIER) {
      parse_result ab = variable_multi_def();
      token previous = cur_token;
      parser_next();
      if (cur_token.kind != donsus_token_kind::SEMICOLON)
        parser_except_current_token(ab, donsus_token_kind::SEMICOLON, previous);

      for (auto node : ab->get<donsus_ast::multi_var_def>().vars) {
        tree->add_node(node);
      }
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek(3).kind == donsus_token_kind::LSQB) {
      parse_result result = array_def();
      parser_except(donsus_token_kind::SEMICOLON);
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::LSQB) {
      parse_result result = indices();
      parser_except(donsus_token_kind::SEMICOLON);
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::FUNCTION_DEFINITION_KW) {
      parse_result result = function_def();

      tree->add_node(result);
    }
    // end/declaration statement
    // start/expressions statement
    // func-call
    // start/selection statement
    else if (cur_token.kind == donsus_token_kind::IF_KW) {
      parse_result result = if_statement();
      if (result->children.empty()) {
        // throw some error here
      }
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::CASE_KW) {
      parse_result result = case_expr();
      parser_except(donsus_token_kind::SEMICOLON);
      // reset context
      tree->add_node(result);
    }
    // end/ selection statement
    //  start/iteration statement
    else if (cur_token.kind == donsus_token_kind::FOR_KW) {
      parse_result result = for_loop();
      tree->add_node(result);
    } else if (cur_token.kind == donsus_token_kind::WHILE_KW) {
      parse_result result = while_loop();
      tree->add_node(result);
    }
    // end/iteration statement
    // start/classdecl statement
    else if (cur_token.kind == donsus_token_kind::CLASS_KW) {
      parse_result result = class_def();
      tree->add_node(result);
    } else {
      if (cur_token.kind != donsus_token_kind::NEWLINE) {
        parse_result result = expr();
        parser_except(donsus_token_kind::SEMICOLON);
        tree->add_node(result);
      }
    }

    // end/classdecl statement
    parser_next();
  }
#if DEBUG
  if (!file.error_count) {
    std::cout << "AST: "
              << "\n";
    print_ast(tree, file.source);
  }
#endif

  return tree;
  // return smt here
}

// Handles all the return possibilities from a function
// e.g (int, int)
auto Parser::return_from_func() -> parse_result {
  if (cur_token.kind == donsus_token_kind::LPAR) {
    return tuple();
  } else if (cur_token.kind == donsus_token_kind::IDENTIFIER) {
    return expr();
  } else if (cur_token.kind == donsus_token_kind::IDENTIFIER ||
             peek(2).kind == donsus_token_kind::LSQB) {
    return array_def();
  }
  syntax_error(tree->get_current_node(), cur_token,
               "Return type: " + std::string(cur_token.type_name()) +
                   " is not valid");
  parse_result empty;
  return empty;
}
// the caller should call this when cur_token = LPAR
auto Parser::tuple() -> parse_result {
  parse_result tuple = create_tuple();
  tuple->first_token_in_ast = cur_token;
  auto &body = tuple->get<donsus_ast::tuple>();

  parser_next();
  while (cur_token.kind != donsus_token_kind::RPAR) {
    body.items.push_back(expr());
    if (peek().kind == donsus_token_kind::COMM) {
      parser_except(donsus_token_kind::COMM);
      parser_next();
      continue;
    }
    parser_next();
  }
  return tuple;
}
auto Parser::create_tuple() -> parse_result {
  return tree->create_node<donsus_ast::tuple>(
      donsus_ast::donsus_node_type::TUPLE);
}

auto Parser::create_variable_def() -> parse_result {
  return tree->create_node<donsus_ast::variable_def>(
      donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
}
auto Parser::variable_def() -> parse_result {
  donsus_ast::specifiers_ s{};
  parse_result definition = create_variable_def();

  definition->first_token_in_ast = cur_token;
  if (cur_token.kind == donsus_token_kind::LET_KW)
    parser_next();
  // move to the specifier
  while (is_specifier(cur_token.kind)) {
    auto value = lexeme_value(cur_token, file.source);
    if (value == "comptime") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::comptime);
    }
    if (value == "static") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::static_);
    }
    if (value == "thread_local") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::thread_local_);
    }
    if (value == "mut") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::mut);
    }
    if (value == "private") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::private_);
    }
    parser_next();
  }

  auto &body = definition->get<donsus_ast::variable_def>();
  body.identifier_name = identifier();
  body.specifiers = s;

  parser_except(donsus_token_kind::COLON);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::UNKNOWN: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) + "is not a valid type!");
    parse_result empty;
    return empty;
  }

  case donsus_token_kind::EQUAL: {
    syntax_error(definition, cur_token,
                 "Type must be specified before equal sign");
    parse_result empty;
    return empty;
  }
  case donsus_token_kind::STAR: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for pointer ");
    parse_result empty;
    return empty;
  }

  case donsus_token_kind::AMPERSAND: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for reference");
    parse_result empty;
    return empty;
  }
  default:
    break;
  }
  parse_result concrete_type = type();
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::STAR:
    body.identifier_type = pointer(concrete_type);
    parser_next();
    break;

  case donsus_token_kind::AMPERSAND:
    body.identifier_type = reference(concrete_type);
    parser_next();
    break;

  default:
    body.identifier_type = concrete_type;
    break;
  }

  parser_except_current(definition, donsus_token_kind::EQUAL);
  parser_next();
  parse_result expression = expr();
  definition->children.push_back(expression);
  return definition;
}

auto Parser::create_param_decl() -> parse_result {
  return tree->create_node<donsus_ast::param_decl>(
      donsus_ast::donsus_node_type::PARAM_DECL);
}
auto Parser::param_decl() -> parse_result {
  parse_result declaration = create_param_decl();
  declaration->first_token_in_ast = cur_token;
  auto &body = declaration->get<donsus_ast::param_decl>();
  body.identifier_name = identifier();
  parser_except(donsus_token_kind::COLON);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::UNKNOWN:
    syntax_error(declaration, cur_token,
                 lexeme_value(cur_token, file.source) + "is not a valid type!");
    break;

  case donsus_token_kind::EQUAL:
    syntax_error(declaration, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified before equal sign");
    break;

  case donsus_token_kind::STAR:
    syntax_error(declaration, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for pointer ");
    break;

  case donsus_token_kind::AMPERSAND:
    syntax_error(declaration, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for reference");
    break;

  default:
    break;
  }
  parse_result concrete_type = type();
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::STAR:
    body.identifier_type = pointer(concrete_type);
    parser_next();
    break;

  case donsus_token_kind::AMPERSAND:
    body.identifier_type = reference(concrete_type);
    parser_next();
    break;

  default:
    body.identifier_type = concrete_type;
    break;
  }

  return declaration;
}

auto Parser::expr() -> parse_result { return bool_or_expr(); }
auto Parser::bool_or_expr() -> parse_result {
  parse_result node = bool_and_expr();

  while (peek().kind == donsus_token_kind::DOUBLE_PIPE) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = bool_and_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);
    node = new_node;
  }
  return node;
}
auto Parser::bool_and_expr() -> parse_result {
  parse_result node = compare_expr();

  while (peek().kind == donsus_token_kind::DOUBLE_AMPERSAND) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = compare_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);
    node = new_node;
  }

  return node;
}
auto Parser::compare_expr() -> parse_result {
  parse_result node = arithmetic_expr();

  while (is_compare_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = arithmetic_expr();
    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::arithmetic_expr() -> parse_result {
  parse_result node = bitwise_expr();

  while (is_addition_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();

    parse_result right = bitwise_expr();
    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::bitwise_expr() -> parse_result {
  parse_result node = addition_expr();

  while (is_multi_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = addition_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::addition_expr() -> parse_result {
  parse_result node = multiply_expr();

  while (is_bitwise_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = addition_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::multiply_expr() -> parse_result {
  parse_result node = bitshift_expr();

  while (is_bitshift_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();
    parse_result right = bitshift_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::bitshift_expr() -> parse_result {
  parse_result node = as_expr();

  while (is_as_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();

    parse_result right = as_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}

auto Parser::as_expr() -> parse_result {
  parse_result node = prefix_expr();

  while (is_prefix_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();

    parse_result right = prefix_expr();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}
auto Parser::prefix_expr() -> parse_result {
  parse_result node = expr_val();

  while (is_member_access_op(peek().kind)) {
    parser_next();
    token op = cur_token;
    parser_next();

    parse_result right = expr_val();

    parse_result new_node = create_expr_w_value(op);
    new_node->children.push_back(node);
    new_node->children.push_back(right);

    node = new_node;
  }
  return node;
}

/*auto Parser::postfix_expr() -> parse_result {
  parse_result node = expr_val();
  while (is)
  return node;
}*/
auto Parser::expr_val() -> parse_result {
  switch (cur_token.kind) {
  case donsus_token_kind::LPAR: {
    if (peek(2).kind == donsus_token_kind::COMM)
      return tuple();
    parser_next();
    parse_result node = expr();

    parser_next();

    if (cur_token.kind != donsus_token_kind::RPAR) {
      syntax_error(tree->get_current_node(), cur_token,
                   "Expected ')' after expression");
    }
    return node;
  }
  case donsus_token_kind::LSQB: {
    return array();
  }

  case donsus_token_kind::CASE_KW: {
    return case_expr();
  }
  case donsus_token_kind::INT: {
    if (peek().kind == donsus_token_kind::TWO_DOTS)
      return range_expression();
  }
  case donsus_token_kind::TRUE_KW:
  case donsus_token_kind::FALSE_KW:
  case donsus_token_kind::IDENTIFIER:
  case donsus_token_kind::FLOAT:
  case donsus_token_kind::STRING:
  case donsus_token_kind::OTHERWISE_KW:
  case donsus_token_kind::UNDEFINED: {
    if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
        peek().kind == donsus_token_kind::LPAR) {
      return function_call();
    }
    if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
        peek().kind == donsus_token_kind::LSQB) {
      return indices();
    }
    return create_expr_w_value(cur_token);
  }
  default: {

    syntax_error(tree->get_current_node(), cur_token, "Wrong token!");
    parse_result empty;
    return empty;
  }
  }
}

auto Parser::create_expr() -> parse_result {
  return tree->create_node<donsus_ast::expression>(
      donsus_ast::donsus_node_type::EXPRESSION);
}
auto Parser::create_expr_w_value(token value_) -> parse_result {
  parse_result expr = create_expr();
  auto &body = expr->get<donsus_ast::expression>();
  expr->first_token_in_ast = cur_token;
  body.value = value_;
  return expr;
}
auto Parser::create_assignments() -> parse_result {
  return tree->create_node<donsus_ast::assignment>(
      donsus_ast::donsus_node_type::ASSIGNMENT);
}
auto Parser::assignments() -> parse_result {
  parse_result assignment = create_assignments();
  assignment->first_token_in_ast = cur_token;

  if (is_specifier(cur_token.kind))
    syntax_error(assignment, cur_token,
                 "Specifier:" + std::string(cur_token.type_name()) +
                     "can't be used on assignments");

  auto &body = assignment->get<donsus_ast::assignment>();
  parser_except_current(assignment, donsus_token_kind::IDENTIFIER);

  body.lvalue = expr();

  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::PLUS_EQUAL:
  case donsus_token_kind::MINUS_EQUAL:
  case donsus_token_kind::STAR_EQUAL:
  case donsus_token_kind::SLASH_EQUAL:
  case donsus_token_kind::LARROW2_EQUAL:
  case donsus_token_kind::RARROW2_EQUAL:
  case donsus_token_kind::PIPE_EQUAL:
  case donsus_token_kind::AMPERSAND_EQUAL:
    break;
  default:
    syntax_error(assignment, cur_token,
                 std::string(cur_token.type_name()) +
                     " is not a valid assignment");
  }
  body.op = cur_token;
  parser_next();

  body.rvalue = expr();
  return assignment;
}

auto Parser::create_variable_multi_def_multi_value() -> parse_result {
  return tree->create_node<donsus_ast::multi_var_multi_def>(
      donsus_ast::donsus_node_type::MULTI_VAR_DEF_MUTLI_VALUE);
}
/*
 *  a: int = 12, b = 10;
 * */
auto Parser::variable_multi_def_multi_value(donsus_ast::variable_def &first_var)
    -> Tomi::Vector<parse_result> {
  Tomi::Vector<parse_result> a;

  while (peek().kind != donsus_token_kind::SEMICOLON) {
    parse_result var = create_variable_def();
    parser_except_current(tree->get_current_node(),
                          donsus_token_kind::IDENTIFIER);
    var->first_token_in_ast = cur_token;
    auto &body = var->get<donsus_ast::variable_def>();
    body.identifier_name = identifier();
    body.specifiers = first_var.specifiers;
    body.identifier_type = first_var.identifier_type;
    parser_except(donsus_token_kind::EQUAL);
    // jump to expression
    parser_next();
    var->children.push_back(expr());
    a.push_back(var);

    if (peek().kind == donsus_token_kind::COMM) {
      parser_next();
      parser_next();
    }
  }
  return a;
}
auto Parser::create_variable_multi_def() -> parse_result {
  return tree->create_node<donsus_ast::multi_var_def>(
      donsus_ast::donsus_node_type::MULTI_VAR_DEF);
}
/*
 * a, b: int = 12;
 * */
auto Parser::variable_multi_def() -> parse_result {
  parse_result multi_def = create_variable_multi_def();
  multi_def->first_token_in_ast = cur_token;

  Tomi::Vector<parse_result> a;
  donsus_ast::specifiers_ s{};
  token first_token_for_multiple = cur_token;
  while (is_specifier(cur_token.kind)) {
    auto value = lexeme_value(cur_token, file.source);
    if (value == "comptime") {
      s = set_specifiers(multi_def, s, donsus_ast::specifiers_::comptime);
    }
    if (value == "static") {
      s = set_specifiers(multi_def, s, donsus_ast::specifiers_::static_);
    }
    if (value == "thread_local") {
      s = set_specifiers(multi_def, s, donsus_ast::specifiers_::thread_local_);
    }
    if (value == "mut") {
      s = set_specifiers(multi_def, s, donsus_ast::specifiers_::mut);
    }
    parser_except(donsus_token_kind::COMM);
    parser_next();
  }

  parser_except_current(multi_def, donsus_token_kind::IDENTIFIER);
  Tomi::Vector<std::string> identifier_names;

  while (cur_token.kind != donsus_token_kind::COLON) {
    if (cur_token.kind == donsus_token_kind::COMM)
      parser_next();
    parse_result var = create_variable_def();
    var->first_token_in_ast = cur_token;
    auto &body = var->get<donsus_ast::variable_def>();
    body.identifier_name = identifier();
    body.specifiers = s;
    a.push_back(var);
    if (is_specifier(cur_token.kind))
      syntax_error(var, cur_token,
                   "Multiple variable definition only accepts specifiers"
                   "before the first identifier");
    parser_next();
  }
  parser_except_current(multi_def, donsus_token_kind::COLON);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::UNKNOWN:
    syntax_error(a[0], cur_token,
                 lexeme_value(cur_token, file.source) + "is not a valid type!");
    break;

  case donsus_token_kind::EQUAL:
    syntax_error(a[0], cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified before equal sign");
    break;

  case donsus_token_kind::STAR:
    syntax_error(a[0], cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for pointer ");
    break;

  case donsus_token_kind::AMPERSAND:
    syntax_error(a[0], cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for reference");
    break;

  default:
    break;
  }
  parse_result concrete_type = type();
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::STAR: {
    for (auto n : a) {
      auto n_ = n->get<donsus_ast::variable_def>();
      n_.identifier_type = pointer(concrete_type);
    }
    break;
  }

  case donsus_token_kind::AMPERSAND: {
    for (auto n : a) {
      auto n_ = n->get<donsus_ast::variable_def>();
      n_.identifier_type = reference(concrete_type);
    }
    break;
  }

  default: {
    for (auto n : a) {
      auto &n_ = n->get<donsus_ast::variable_def>();
      n_.identifier_type = concrete_type;
    }
    break;
  }
  }
  parser_except_current(multi_def, donsus_token_kind::EQUAL);
  parser_next();
  parse_result expression = expr();

  // set values
  for (auto n : a) {
    n->children.push_back(expression);
  }
  multi_def->get<donsus_ast::multi_var_def>().vars = a;
  return multi_def;
}

auto Parser::create_range_expression() -> parse_result {
  return tree->create_node<donsus_ast::range_expr>(
      donsus_ast::donsus_node_type::RANGE_EXPRESSION);
}
auto Parser::range_expression() -> parse_result {
  parse_result range_statement = create_range_expression();
  range_statement->first_token_in_ast = cur_token;
  auto &body_range = range_statement->get<donsus_ast::range_expr>();
  parse_result start = create_expr_w_value(cur_token);
  parser_next();
  body_range.start = start;
  parser_except_current(range_statement, donsus_token_kind::TWO_DOTS);
  parser_next();
  parse_result end = create_expr_w_value(cur_token);
  body_range.end = end;
  return range_statement;
}

auto Parser::create_function_call() -> parse_result {
  return tree->create_node<donsus_ast::function_call>(
      donsus_ast::donsus_node_type::FUNCTION_CALL);
}
auto Parser::function_call() -> parse_result {
  parse_result f_call = create_function_call();
  f_call->first_token_in_ast = cur_token;
  auto &body_f_call = f_call->get<donsus_ast::function_call>();
  parser_except_current(f_call, donsus_token_kind::IDENTIFIER);
  body_f_call.func_name = lexeme_value(cur_token, file.source);
  parser_except(donsus_token_kind::LPAR);
  parser_next();
  while (cur_token.kind != donsus_token_kind::RPAR) {
    body_f_call.arguments.push_back(expr());
    if (peek().kind == donsus_token_kind::COMM) {
      parser_next();
      parser_next();
      continue;
    }
    parser_next();
    break;
  }
  parser_except_current(f_call, donsus_token_kind::RPAR);
  return f_call;
}
auto Parser::create_function_def() -> parse_result {
  return tree->create_node<donsus_ast::function_def>(
      donsus_ast::donsus_node_type::FUNCTION_DEF);
}
auto Parser::function_def() -> parse_result {
  donsus_ast::specifiers_ s{};
  parse_result definition = create_function_def();
  definition->first_token_in_ast = cur_token;

  parser_except_current(definition, donsus_token_kind::FUNCTION_DEFINITION_KW);
  parser_next();

  auto &body_def = definition->get<donsus_ast::function_def>();
  body_def.func_name = lexeme_value(cur_token, file.source);

  parser_except(donsus_token_kind::LPAR);
  parser_next();

  body_def.parameters = param_list();

  parser_except_current(definition, donsus_token_kind::RPAR);
  parser_next();

  while (is_specifier(cur_token.kind)) {
    auto value = lexeme_value(cur_token, file.source);
    if (value == "comptime") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::comptime);
    }
    if (value == "static") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::static_);
    }
    if (value == "thread_local") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::thread_local_);
    }
    if (value == "mut") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::mut);
    }
    if (value == "private") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::private_);
    }
    parser_next();
  }

  body_def.specifiers = s;

  parser_except_current(definition, donsus_token_kind::ARROW);
  parser_next();

  body_def.return_type = return_from_func();
  if (peek().kind == donsus_token_kind::SEMICOLON) {
    body_def.function_type = donsus_ast::FunctionType::DECL;
    parser_next();
    return definition;
  }
  body_def.function_type = donsus_ast::FunctionType::DEF;

  parser_except(donsus_token_kind::LBRACE);
  body_def.body = statements();
  parser_except_current(definition, donsus_token_kind::RBRACE);
  return definition;
}

auto Parser::statements() -> Tomi::Vector<parse_result> {
  Tomi::Vector<parse_result> body{};
  // Todo: Do not duplicate this that much!
  while (cur_token.kind != donsus_token_kind::RBRACE) {
    // start/declaration statement
    if (cur_token.kind == donsus_token_kind::LET_KW &&
        is_specifier(peek().kind)) {

      parse_result result = variable_def();
      // multiple definitions
      if (peek().kind == donsus_token_kind::COMM) {
        parser_except(donsus_token_kind::COMM);
        parser_next();
        Tomi::Vector<parse_result> vars = variable_multi_def_multi_value(
            result->get<donsus_ast::variable_def>());
        body.push_back(result);
        for (auto var : vars) {
          tree->add_node(var);
        }
        parser_except(donsus_token_kind::SEMICOLON);
      } else {
        body.push_back(result);
        parser_except(donsus_token_kind::SEMICOLON);
      }
    } else if (cur_token.kind == donsus_token_kind::RETURN_KW) {
      parse_result result = return_statement();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::INSTANCE_KW) {
      parse_result result = instance();
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::AT) {
      parse_result result = language_extension();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::TYPECLASS_KW) {
      parse_result result = typeclass();
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::ALIAS_KW) {
      parse_result result = alias();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::DOUBLE_COLON) {
      parse_result result = generics_decl();
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::COLON &&
               peek(3).kind != donsus_token_kind::LSQB) {
      parse_result result = variable_def();
      if (peek().kind == donsus_token_kind::COMM) {
        parser_except(donsus_token_kind::COMM);
        parser_next();
        Tomi::Vector<parse_result> vars = variable_multi_def_multi_value(
            result->get<donsus_ast::variable_def>());
        tree->add_node(result);
        for (auto var : vars) {
          tree->add_node(var);
        }
        parser_except(donsus_token_kind::SEMICOLON);
      } else {
        body.push_back(result);
        parser_except(donsus_token_kind::SEMICOLON);
      }
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               is_assignment(peek().kind)) {
      parse_result result = assignments();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::COMM &&
               peek(2).kind == donsus_token_kind::IDENTIFIER) {
      parse_result ab = variable_multi_def();
      token previous = cur_token;
      parser_next();
      if (cur_token.kind != donsus_token_kind::SEMICOLON)
        parser_except_current_token(ab, donsus_token_kind::SEMICOLON, previous);

      for (auto node : ab->get<donsus_ast::multi_var_def>().vars) {
        body.push_back(node);
      }
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek(3).kind == donsus_token_kind::LSQB) {
      parse_result result = array_def();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::IDENTIFIER &&
               peek().kind == donsus_token_kind::LSQB) {
      parse_result result = indices();
      parser_except(donsus_token_kind::SEMICOLON);
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::FUNCTION_DEFINITION_KW) {
      parse_result result = function_def();
      body.push_back(result);
    }
    // end/declaration statement
    // start/expressions statement
    // func-call
    // start/selection statement
    else if (cur_token.kind == donsus_token_kind::IF_KW) {
      parse_result result = if_statement();
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::CASE_KW) {
      parse_result result = case_expr();
      body.push_back(result);
    }
    // end/ selection statement
    //  start/iteration statement
    else if (cur_token.kind == donsus_token_kind::FOR_KW) {
      parse_result result = for_loop();
      body.push_back(result);
    } else if (cur_token.kind == donsus_token_kind::WHILE_KW) {
      parse_result result = while_loop();
      body.push_back(result);
    }
    // end/iteration statement
    // start/classdecl statement
    else if (cur_token.kind == donsus_token_kind::CLASS_KW) {
      parse_result result = class_def();
      body.push_back(result);
    } else {
      /*
       If cur_token is a newline or cur_token is the beginning of a
       statement block don't start an expression.
       * */
      if (cur_token.kind != donsus_token_kind::NEWLINE &&
          cur_token.kind != donsus_token_kind::LBRACE) {
        parse_result result = expr();
        parser_except(donsus_token_kind::SEMICOLON);
        body.push_back(result);
      }
    }
    parser_next();
  }
  return body;
}
auto Parser::create_instance() -> parse_result {
  return tree->create_node<donsus_ast::instance>(
      donsus_ast::donsus_node_type::INSTANCE);
}
auto Parser::instance() -> parse_result {
  parse_result instance = create_instance();
  instance->first_token_in_ast = cur_token;
  auto &body_ = instance->get<donsus_ast::instance>();

  parser_except_current(instance, donsus_token_kind::INSTANCE_KW);
  parser_except(donsus_token_kind::IDENTIFIER);

  body_.identifier_name = identifier();
  parser_next();
  body_.type = type();
  parser_except(donsus_token_kind::LBRACE);

  body_.body = statements();
  parser_except_current(instance, donsus_token_kind::RBRACE);
  return instance;
}
auto Parser::create_language_extension() -> parse_result {
  return tree->create_node<donsus_ast::language_extension>(
      donsus_ast::donsus_node_type::LANGUAGE_EXTENSION);
}

auto Parser::language_extension() -> parse_result {
  parse_result language_extension = create_language_extension();
  auto &body = language_extension->get<donsus_ast::language_extension>();

  language_extension->first_token_in_ast = cur_token;
  parser_except_current(language_extension, donsus_token_kind::AT);
  parser_except(donsus_token_kind::LPAR);
  body.extensions = tuple();

  return language_extension;
}
auto Parser::create_typeclass() -> parse_result {
  return tree->create_node<donsus_ast::typeclass>(
      donsus_ast::donsus_node_type::TYPECLASS);
}

auto Parser::typeclass() -> parse_result {
  parse_result typeclass = create_typeclass();
  typeclass->first_token_in_ast = cur_token;
  auto &body = typeclass->get<donsus_ast::typeclass>();
  parser_except_current(typeclass, donsus_token_kind::TYPECLASS_KW);
  parser_next();
  parser_except_current(typeclass, donsus_token_kind::IDENTIFIER);
  body.identifier_name = identifier();

  if (peek().kind == donsus_token_kind::LESS) {
    parser_except(donsus_token_kind::LESS);
    body.template_decl = template_decl();
  }
  parser_except(donsus_token_kind::LBRACE);
  parser_next();
  body.body = statements();
  return typeclass;
}

auto Parser::create_template_decl() -> parse_result {
  return tree->create_node<donsus_ast::template_decl>(
      donsus_ast::donsus_node_type::TEMPLATE_DECL);
}
auto Parser::template_decl() -> parse_result {
  parse_result template_decl = create_template_decl();
  template_decl->first_token_in_ast = cur_token;
  auto &body = template_decl->get<donsus_ast::template_decl>();
  parser_except_current(template_decl, donsus_token_kind::LESS);
  parser_next();
  parser_except_current(template_decl, donsus_token_kind::IDENTIFIER);
  while (cur_token.kind != donsus_token_kind::GREATER) {
    body.types.push_back(identifier());
    if (peek().kind == donsus_token_kind::COMM) {
      parser_except(donsus_token_kind::COMM);
      parser_next();
      continue;
    }
    parser_next();
  }
  return template_decl;
}
auto Parser::create_alias() -> parse_result {
  return tree->create_node<donsus_ast::alias>(
      donsus_ast::donsus_node_type::ALIAS_STATEMENT);
}
auto Parser::alias() -> parse_result {
  parse_result alias_decl = create_alias();
  alias_decl->first_token_in_ast = cur_token;
  auto &body = alias_decl->get<donsus_ast::alias>();
  parser_except_current(alias_decl, donsus_token_kind::ALIAS_KW);
  parser_next();
  parser_except_current(alias_decl, donsus_token_kind::IDENTIFIER);
  body.synonym = type();
  parser_next();
  parser_except_current(alias_decl, donsus_token_kind::EQUAL);
  parser_next();
  parser_except_current(alias_decl, donsus_token_kind::IDENTIFIER);
  body.existing_type = type();
  return alias_decl;
}

auto Parser::create_generics_decl() -> parse_result {
  return tree->create_node<donsus_ast::generics_decl>(
      donsus_ast::donsus_node_type::GENERICS_DECL);
}
// Support for multiple constraints
auto Parser::generics_decl() -> parse_result {
  // second try
  /*  parse_result decl_statement = create_generics_decl();
    decl_statement->first_token_in_ast = cur_token;
    auto &body_decl = decl_statement->get<donsus_ast::generics_decl>();
    parser_except_current(decl_statement, donsus_token_kind::IDENTIFIER);
    body_decl.name = identifier();

    parser_next(); // to double colon

    auto is_function_or_class_kw = [](donsus_token_kind kind) {
      return kind == donsus_token_kind::FUNCTION_DEFINITION_KW ||
             kind == donsus_token_kind::CLASS_KW;
    };

    if (peek().kind == donsus_token_kind::IDENTIFIER &&
        is_function_or_class_kw(peek(2).kind)) {
      parser_except(donsus_token_kind::IDENTIFIER);
      body_decl.params.push_back(identifier());
      body_decl.type_of_decl = donsus_ast::generics_type::PARAMETRIC;
      parser_next();
      return decl_statement;
    }

    if (peek().kind == donsus_token_kind::IDENTIFIER &&
        peek(2).kind == donsus_token_kind::ARROW) {
      parser_except(donsus_token_kind::IDENTIFIER);
      body_decl.params.push_back(identifier());
      body_decl.type_of_decl = donsus_ast::generics_type::PARAMETRIC;
      while (!is_function_or_class_kw(cur_token.kind)) {
        parser_except(donsus_token_kind::ARROW);
        body_decl.params.push_back(identifier());
      }
      return decl_statement;
    }

    if (peek().kind == donsus_token_kind::IDENTIFIER) {
      parser_next();
      parser_except_current(decl_statement, donsus_token_kind::IDENTIFIER);
      body_decl.type_of_decl = donsus_ast::generics_type::AD_HOC;
      body_decl.constraint = constraint();
    }
    parser_except_current(decl_statement, donsus_token_kind::IMPLIES);
    if (peek().kind == donsus_token_kind::IDENTIFIER &&
        peek(2).kind == donsus_token_kind::ARROW) {
      parser_except(donsus_token_kind::IDENTIFIER);
      body_decl.params.push_back(identifier());
      while (!is_function_or_class_kw(cur_token.kind)) {
        parser_except(donsus_token_kind::ARROW);
        body_decl.params.push_back(identifier());
      }
      return decl_statement;
    }*/

  /*  switch (cur_token.kind) {

    case donsus_token_kind::DOUBLE_COLON: {
      body_decl.type_of_decl = donsus_ast::generics_type::PARAMETRIC;
      peek();
      if (peek().kind == donsus_token_kind::IMPLIES) {
        syntax_error(decl_statement, cur_token, "Constraint left empty");
        parser_next();
        break;
      }
      parser_next();
      if (cur_token.kind == donsus_token_kind::LPAR) {
        while (cur_token.kind != donsus_token_kind::RPAR) {
          body_decl.constraints.push_back(constraint());
          parser_except(donsus_token_kind::COMM);
        }
        break;
      }

      body_decl.constraints.push_back(constraint());
      parser_except_current(decl_statement, donsus_token_kind::IMPLIES);
      break;
    }
    case donsus_token_kind::ARROW:
      break;
    default:
      syntax_error(decl_statement, cur_token,
                   lexeme_value(cur_token, file.source) +
                       "should be followed by -> or ::");
    }
    parser_next();

    if (cur_token.kind != donsus_token_kind::ARROW)
      syntax_error(decl_statement, cur_token,
                   lexeme_value(cur_token, file.source) +
                       "must be followed by ->");

    while (cur_token.kind != donsus_token_kind::NEWLINE) {
      body_decl.params.push_back(identifier());
      parser_next();
      parser_except_current(decl_statement, donsus_token_kind::ARROW);
    }
    return decl_statement;*/
  /*  return decl_statement;*/
}

auto Parser::create_array() -> parse_result {
  return tree->create_node<donsus_ast::array>(
      donsus_ast::donsus_node_type::ARRAY);
}
auto Parser::array() -> parse_result {
  parse_result array = create_array();
  array->first_token_in_ast = cur_token;
  auto &body = array->get<donsus_ast::array>();
  parser_except_current(array, donsus_token_kind::LSQB);
  parser_next();
  while (cur_token.kind != donsus_token_kind::RSQB) {
    body.items.push_back(expr());
    if (peek().kind == donsus_token_kind::COMM) {
      parser_except(donsus_token_kind::COMM);
      parser_next();
      continue;
    }
    break;
  }
  parser_except(donsus_token_kind::RSQB);
  return array;
}
auto Parser::create_array_def() -> parse_result {
  return tree->create_node<donsus_ast::array_def>(
      donsus_ast::donsus_node_type::ARRAY_DEF);
}

auto Parser::array_def() -> parse_result {
  donsus_ast::specifiers_ s;
  parse_result definition = create_array_def();
  definition->first_token_in_ast = cur_token;
  auto &body_def = definition->get<donsus_ast::array_def>();

  while (is_specifier(cur_token.kind)) {
    auto value = lexeme_value(cur_token, file.source);
    if (value == "comptime") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::comptime);
    }
    if (value == "static") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::static_);
    }
    if (value == "thread_local") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::thread_local_);
    }
    if (value == "mut") {
      s = set_specifiers(definition, s, donsus_ast::specifiers_::mut);
    }
  }
  parser_except_current(definition, donsus_token_kind::IDENTIFIER);
  body_def.identifier_name = identifier();
  body_def.specifiers = s;

  parser_except(donsus_token_kind::COLON);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::UNKNOWN: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) + "is not a valid type!");
    parse_result empty;
    return empty;
  }
  case donsus_token_kind::EQUAL: {
    syntax_error(definition, cur_token,
                 "Type must be specified before equal sign");
    parse_result empty;
    return empty;
  }
  case donsus_token_kind::STAR: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for pointer ");
    parse_result empty;
    return empty;
  }
  case donsus_token_kind::AMPERSAND: {
    syntax_error(definition, cur_token,
                 lexeme_value(cur_token, file.source) +
                     "Type must be specified for reference");
    parse_result empty;
    return empty;
  }
  default:
    break;
  }
  parse_result concrete_type = type();
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::STAR:
    body_def.identifier_type = pointer(concrete_type);
    break;

  case donsus_token_kind::AMPERSAND:
    body_def.identifier_type = reference(concrete_type);
    parser_next();
    break;
  default:
    body_def.identifier_type = concrete_type;
    break;
  }
  parser_except_current(definition, donsus_token_kind::LSQB);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::RSQB: {
    body_def.array_type = donsus_ast::ArrayType::DYNAMIC;
    break;
  }
  default: {
    body_def.indices = expr();
    parser_except(donsus_token_kind::RSQB);
    if (peek().kind == donsus_token_kind::DOT) {
      body_def.array_type = donsus_ast::ArrayType::STATIC;
      parser_next();
    } else
      body_def.array_type = donsus_ast::ArrayType::FIXED;
  }
  }
  parser_next();
  parser_except_current(definition, donsus_token_kind::EQUAL);
  parser_next();
  body_def.body = expr();
  return definition;
}
auto Parser::create_if_statement() -> parse_result {
  return tree->create_node<donsus_ast::if_statement>(
      donsus_ast::donsus_node_type::IF_STATEMENT);
}
auto Parser::if_statement() -> parse_result {
  parse_result statement = create_if_statement();
  statement->first_token_in_ast = cur_token;
  auto &body_if = statement->get<donsus_ast::if_statement>();
  parser_except_current(statement, donsus_token_kind::IF_KW);
  parser_except(donsus_token_kind::LPAR);

  switch (cur_token.kind) {
  case donsus_token_kind::LPAR:
    break;
  default:
    syntax_error(statement, cur_token, "if statement must have condition");
  }
  if (peek().kind == donsus_token_kind::IDENTIFIER &&
      peek(2).kind == donsus_token_kind::COLON) {
    parser_except(donsus_token_kind::IDENTIFIER);
    body_if.if_var_decls = if_var_decls();
  }
  parser_next();
  parse_result condition_expression = expr();
  body_if.condition = condition_expression;
  parser_next();
  parser_except_current(statement, donsus_token_kind::RPAR);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::LBRACE:
    break;
  default:
    syntax_error(statement, cur_token,
                 "Block must be specified for if statement");
  }
  // jump to the body
  parser_next();
  body_if.body = statements();

  // here because I have to advance even if I don't have an else keyword
  // would result in errors when dealing with multiple if statement,
  // otherwise somehow I can't get into it.

  while (peek().kind == donsus_token_kind::ELSE_KW &&
         peek(2).kind == donsus_token_kind::IF_KW) {
    parser_next();
    body_if.alternate.push_back(else_if_statement());
  }
  while (peek().kind == donsus_token_kind::ELSE_KW) {
    parser_next();
    body_if.alternate.push_back(else_statement());
    parser_next();
  }
  return statement;
}
auto Parser::if_var_decls() -> Tomi::Vector<parse_result> {
  Tomi::Vector<parse_result> a;
  parser_except_current(tree->get_current_node(),
                        donsus_token_kind::IDENTIFIER);
  parse_result result = variable_def();
  if (peek().kind == donsus_token_kind::COMM) {
    parser_except(donsus_token_kind::COMM);
    parser_next();
    Tomi::Vector<parse_result> vars =
        variable_multi_def_multi_value(result->get<donsus_ast::variable_def>());
    a.push_back(result);
    for (auto var : vars) {
      a.push_back(var);
    }
  } else {
    a.push_back(result);
  }
  parser_except(donsus_token_kind::SEMICOLON);
  return a;
}
auto Parser::create_else_if_statement() -> parse_result {
  return tree->create_node<donsus_ast::else_if_statement>(
      donsus_ast::donsus_node_type::ELSE_IF_STATEMENT);
}
auto Parser::else_if_statement() -> parse_result {
  parse_result else_if_statement = create_else_if_statement();
  else_if_statement->first_token_in_ast = cur_token;

  auto &body_else_if = else_if_statement->get<donsus_ast::else_if_statement>();
  parser_except_current(else_if_statement, donsus_token_kind::ELSE_KW);
  parser_next();
  parser_except_current(else_if_statement, donsus_token_kind::IF_KW);

  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::LPAR:
    break;
  default:
    syntax_error(else_if_statement, cur_token,
                 "else if statement must have condition");
  }
  if (peek().kind == donsus_token_kind::IDENTIFIER &&
      peek(2).kind == donsus_token_kind::COLON) {
    body_else_if.if_var_decls = if_var_decls();
  }
  parser_next();
  parse_result condition_expression = expr();
  body_else_if.condition = condition_expression;
  parser_next();
  parser_except_current(else_if_statement, donsus_token_kind::RPAR);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::LBRACE:
    break;
  default:
    syntax_error(else_if_statement, cur_token,
                 "Block must be specified for if statement");
  }
  body_else_if.body = statements();

  return else_if_statement;
}
auto Parser::create_else_statement() -> parse_result {
  return tree->create_node<donsus_ast::else_statement>(
      donsus_ast::donsus_node_type::ELSE_STATEMENT);
}

auto Parser::else_statement() -> parse_result {
  parse_result else_statement = create_else_statement();
  else_statement->first_token_in_ast = cur_token;
  auto &body_else = else_statement->get<donsus_ast::else_statement>();
  parser_except_current(else_statement, donsus_token_kind::ELSE_KW);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::LBRACE:
    break;
  default:
    syntax_error(else_statement, cur_token,
                 "Block must be specified for if statement");
  }
  body_else.body = statements();
  return else_statement;
}
auto Parser::create_case_expr() -> parse_result {
  return tree->create_node<donsus_ast::case_expr>(
      donsus_ast::donsus_node_type::CASE);
}
auto Parser::case_expr() -> parse_result {
  parse_result statement = create_case_expr();
  statement->first_token_in_ast = cur_token;
  auto &body = statement->get<donsus_ast::case_expr>();
  parser_except_current(statement, donsus_token_kind::CASE_KW);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::OF_KW:
    syntax_error(statement, cur_token,
                 "Scrutinee must be provided before of keyword");
    break;
  default:
    break;
  }
  body.scrutinee = expr();
  parser_except(donsus_token_kind::OF_KW);
  parser_next();
  body.patterns = patterns();
  return statement;
}
auto Parser::pattern() -> parse_result {
  parse_result pattern_statement = create_pattern();
  pattern_statement->first_token_in_ast = cur_token;
  auto &body_pattern = pattern_statement->get<donsus_ast::pattern>();

  switch (cur_token.kind) {
  case donsus_token_kind::ARROW:
    syntax_error(pattern_statement, cur_token, "Guard must be provided");
    break;

  default:
    break;
  }

  body_pattern.guard = expr();

  parser_except(donsus_token_kind::ARROW);
  parser_next();
  // get to string here
  switch (cur_token.kind) {
  case donsus_token_kind::COMM:
    syntax_error(pattern_statement, cur_token,
                 "Expression must be provided for a pattern");
    break;
  default:
    break;
  }
  // after expression is parsed down why is it an int?
  body_pattern.result_expression = expr();
  // how is it an integer here
  return pattern_statement;
}

auto Parser::create_pattern() -> parse_result {
  return tree->create_node<donsus_ast::pattern>(
      donsus_ast::donsus_node_type::PATTERN);
}
auto Parser::patterns() -> Tomi::Vector<utility::handle<donsus_ast::node>> {
  Tomi::Vector<utility::handle<donsus_ast::node>> patterns_a;
  while (peek().kind != donsus_token_kind::SEMICOLON) {
    patterns_a.push_back(pattern());
    if (peek().kind != donsus_token_kind::SEMICOLON)
      parser_next();
    if (cur_token.kind == donsus_token_kind::COMM)
      parser_next();
  }
  return patterns_a;
}
auto Parser::create_for_loop() -> parse_result {
  return tree->create_node<donsus_ast::for_loop>(
      donsus_ast::donsus_node_type::FOR_LOOP);
}
auto Parser::for_loop() -> parse_result {
  parse_result for_statement = create_for_loop();
  for_statement->first_token_in_ast = cur_token;
  auto &body_for = for_statement->get<donsus_ast::for_loop>();
  parser_except_current(for_statement, donsus_token_kind::FOR_KW);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::INT: {
    body_for.type_of_loop = donsus_ast::loop_type::RANGE_BASED;
    ;
    body_for.iterator = range_expression();
    parser_except(donsus_token_kind::LBRACE);
    break;
  }
  case donsus_token_kind::IDENTIFIER: {
    if (peek().kind == donsus_token_kind::LBRACE) {
      body_for.type_of_loop = donsus_ast::loop_type::RANGE_BASED;
      body_for.iterator = expr();
      parser_except(donsus_token_kind::LBRACE);
      break;
    } else if (peek().kind == donsus_token_kind::COLON) {
      body_for.type_of_loop = donsus_ast::loop_type::RANGE_BASED;
      body_for.loop_variable = identifier();
      parser_except(donsus_token_kind::COLON);
      parser_next();
      body_for.iterator = expr();
      parser_except(donsus_token_kind::LBRACE);
      break;
    }
  }
  case donsus_token_kind::LPAR: {
    body_for.type_of_loop = donsus_ast::loop_type::TRADITIONAL;

    parser_except(donsus_token_kind::IDENTIFIER);
    body_for.init_statement = variable_def();
    parser_except(donsus_token_kind::SEMICOLON);

    parser_next();
    body_for.condition = expr();
    parser_except(donsus_token_kind::SEMICOLON);

    parser_next();
    body_for.increment_expr = assignments();
    parser_except(donsus_token_kind::RPAR);
    parser_except(donsus_token_kind::LBRACE);
  }
  default:
    break;
  }
  parser_except_current(for_statement, donsus_token_kind::LBRACE);
  body_for.body = statements();
  parser_except_current(for_statement, donsus_token_kind::RBRACE);

  return for_statement;
}
auto Parser::create_while_loop() -> parse_result {
  return tree->create_node<donsus_ast::while_loop>(
      donsus_ast::donsus_node_type::WHILE_LOOP);
}
auto Parser::while_loop() -> parse_result {
  parse_result while_statement = create_while_loop();
  while_statement->first_token_in_ast = cur_token;
  auto &body_while = while_statement->get<donsus_ast::while_loop>();
  parser_except_current(while_statement, donsus_token_kind::WHILE_KW);
  parser_next();

  switch (cur_token.kind) {
  case donsus_token_kind::LBRACE:
    syntax_error(while_statement, cur_token,
                 "While loop must have a condition!");
  default:
    break;
  }
  body_while.condition = expr();

  body_while.type_of_loop = (peek().kind == donsus_token_kind::DO_KW)
                                ? donsus_ast::while_type::DO_BASED
                                : donsus_ast::while_type::TRADITIONAL;

  if (body_while.type_of_loop == donsus_ast::while_type::DO_BASED) {
    parser_next();
  }

  parser_next();

  parser_except_current(while_statement, donsus_token_kind::LBRACE);
  body_while.body = statements();
  parser_except_current(while_statement, donsus_token_kind::RBRACE);
  return while_statement;
}

auto Parser::create_data_constructor() -> parse_result {
  return tree->create_node<donsus_ast::data_constructor>(
      donsus_ast::donsus_node_type::DATA_CONSTRUCTOR);
}
// Todo: Finish this
/*auto Parser::data_constructor() -> parse_result {
  parse_result constructor = create_data_constructor();
  constructor->first_token_in_ast = cur_token;
  auto &body_constructor = constructor->get<donsus_ast::data_constructor>();
}*/

auto Parser::data_constructor_part() -> parse_result {}
auto Parser::create_type_constructor() -> parse_result {
  return tree->create_node<donsus_ast::type_constructor>(
      donsus_ast::donsus_node_type::TYPE_CONSTRUCTOR);
}

/*auto Parser::type_constructor() -> parse_result {
  parse_result constructor = create_type_constructor();
  auto &body_constructor = constructor->get<donsus_ast::type_constructor>();
  constructor->first_token_in_ast = cur_token;
  parser_except(donsus_token_kind::TYPE_KW);
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::EQUAL:
    syntax_error(&constructor, cur_token,
                 "A type constructor must have a name specified");
    break;
  case donsus_token_kind::IDENTIFIER:
    break;
  default:
    syntax_error(&constructor, cur_token, "Type constructor is not valid");
  }
  body_constructor.name = identifier();
  parser_next();
  // Todo: will result in an infinite loop if Equal is not specified.
  while (cur_token.kind !=
         (donsus_token_kind::EQUAL || donsus_token_kind::END)) {
    body_constructor.arguments(identifier());
    parser_next();
  }
  parser_next();
  body_constructor.body = expr();
  return constructor;
}*/

auto Parser::create_indices() -> parse_result {
  return tree->create_node<donsus_ast::indices>(
      donsus_ast::donsus_node_type::INDICES);
}
auto Parser::indices() -> parse_result {
  parse_result indices_expr = create_indices();
  indices_expr->first_token_in_ast = cur_token;
  auto &body = indices_expr->get<donsus_ast::indices>();
  parser_except_current(indices_expr, donsus_token_kind::IDENTIFIER);
  body.name = identifier();
  parser_except(donsus_token_kind::LSQB);
  parser_next();

  if (cur_token.kind == donsus_token_kind::RSQB) {
    syntax_error(indices_expr, cur_token, "Index must be specified after [");
  }
  body.index = expr();
  parser_except(donsus_token_kind::RSQB);
  return indices_expr;
}
auto Parser::create_pointer() -> parse_result {
  return tree->create_node<donsus_ast::pointer>(
      donsus_ast::donsus_node_type::POINTER);
}
auto Parser::create_constraint() -> parse_result {
  return tree->create_node<donsus_ast::constraint>(
      donsus_ast::donsus_node_type::CONSTRAINT);
}
// Todo: Might need to make type variables lowercase
auto Parser::constraint() -> parse_result {
  parse_result constraint_statement = create_constraint();
  constraint_statement->first_token_in_ast = cur_token;
  auto &body_constraint = constraint_statement->get<donsus_ast::constraint>();
  parser_except_current(constraint_statement, donsus_token_kind::IDENTIFIER);

  body_constraint.class_constraint = identifier();
  parser_next();
  switch (cur_token.kind) {
  case donsus_token_kind::IMPLIES:
    syntax_error(constraint_statement, cur_token,
                 "Type variable must be specified for constraint");
    break;
  default:
    break;
  }
  while (cur_token.kind != donsus_token_kind::IMPLIES) {
    body_constraint.type_variables.push_back(identifier());
    if (peek().kind != donsus_token_kind::IMPLIES) {
      parser_except(donsus_token_kind::COMM);
      continue;
    }
    parser_next();
  }
  return constraint_statement;
}
auto Parser::pointer(parse_result pointee) -> parse_result {
  parse_result pointer = create_pointer();
  auto &body = pointer->get<donsus_ast::pointer>();
  body.pointee = pointee;
  return pointer;
}

auto Parser::create_reference() -> parse_result {
  return tree->create_node<donsus_ast::reference>(
      donsus_ast::donsus_node_type::REFERENCE);
}

auto Parser::reference(parse_result referent) -> parse_result {
  parse_result reference = create_reference();
  auto &body = reference->get<donsus_ast::reference>();
  body.referent = referent;
  return reference;
}
auto Parser::create_class_def() -> parse_result {
  return tree->create_node<donsus_ast::class_def>(
      donsus_ast::donsus_node_type::CLASS);
}

auto Parser::class_def() -> parse_result {
  donsus_ast::specifiers_class_ s;
  parse_result definition = create_class_def();
  definition->first_token_in_ast = cur_token;
  auto &body_def = definition->get<donsus_ast::class_def>();

  parser_except_current(definition, donsus_token_kind::CLASS_KW);
  parser_next();

  if (cur_token.kind != donsus_token_kind::IDENTIFIER)
    syntax_error(definition, cur_token,
                 "Class name must be specified after class kw");

  body_def.name = identifier();
  parser_next();
  while (is_class_specifier(cur_token.kind)) {
    auto value = lexeme_value(cur_token, file.source);
    if (value == "abstract") {
      s = set_class_specifiers(definition, s,
                               donsus_ast::specifiers_class_::abstract);
    }
    if (value == "final") {
      s = set_class_specifiers(definition, s,
                               donsus_ast::specifiers_class_::final);
    }
    parser_next();
  }

  body_def.specifiers = s;

  if (cur_token.kind == donsus_token_kind::COLON &&
      peek().kind == donsus_token_kind::LBRACE) {
    syntax_error(definition, cur_token, "Must define a class to inherit from!");
  }
  if (cur_token.kind == donsus_token_kind::COLON) {
    parser_next();
    while (cur_token.kind != donsus_token_kind::LBRACE) {
      body_def.inherits.push_back(identifier());
      parser_next();
    }
  }
  parser_except_current(definition, donsus_token_kind::LBRACE);
  parser_next();
  body_def.body = statements();
  return definition;
}

auto Parser::type() -> parse_result {
  if (cur_token.kind == donsus_token_kind::LPAR &&
      peek(2).kind == donsus_token_kind::COMM) {
    return tuple();
  }
  if (cur_token.kind == donsus_token_kind::LPAR)
    return expr();
  return identifier();
}

auto Parser::create_identifier() -> parse_result {
  return tree->create_node<donsus_ast::identifier>(
      donsus_ast::donsus_node_type::IDENTIFIER);
}
auto Parser::create_return_statement() -> parse_result {
  return tree->create_node<donsus_ast::return_statement>(
      donsus_ast::donsus_node_type::RETURN_STATEMENT);
}
auto Parser::return_statement() -> parse_result {
  parse_result return_statement = create_return_statement();
  return_statement->first_token_in_ast = cur_token;
  auto &body_ret = return_statement->get<donsus_ast::return_statement>();
  parser_except_current(return_statement, donsus_token_kind::RETURN_KW);
  parser_next();
  body_ret.body = expr();
  return return_statement;
}
auto Parser::identifier() -> parse_result {
  parse_result identifier = create_identifier();
  auto &body = identifier->get<donsus_ast::identifier>();
  parser_except_current(identifier, donsus_token_kind::IDENTIFIER);
  body.identifier_name = lexeme_value(cur_token, file.source);
  return identifier;
}

void Parser::parser_except(donsus_token_kind type) {
  token error_on = cur_token;
  token a = parser_next();
  if (a.kind != type) {
    syntax_error(tree->get_current_node(), error_on,
                 "Expected token: " + std::string(token::type_name(type)) +
                     ", got instead: " + std::string(token::type_name(a.kind)) +
                     "\n");
    cur_token = error_on;
  }
}
// Error handling
/*
 * If nullptr is passed in, show_error_on_line doesn't apply
 * */
auto Parser::param_list() -> Tomi::Vector<parse_result> {
  Tomi::Vector<parse_result> a;
  while (cur_token.kind != donsus_token_kind::RPAR) {
    parser_except_current(tree->get_current_node(),
                          donsus_token_kind::IDENTIFIER);
    parse_result v_d = param_decl();
    a.push_back(v_d);

    if (cur_token.kind == donsus_token_kind::COMM) {
      parser_next();
      continue;
    }
    break;
  }
  return a;
}

auto Parser::set_specifiers(parse_result node, donsus_ast::specifiers_ s,
                            donsus_ast::specifiers_ v)
    -> donsus_ast::specifiers_ {
  if (donsus_ast::to_bool(s & v)) {
    syntax_error(node, cur_token,
                 std::string(donsus_ast::specifiers_utils::type_name(v)) +
                     " is already present!");
  }
  return static_cast<donsus_ast::specifiers_>(s | v);
}

auto Parser::set_class_specifiers(parse_result node,
                                  donsus_ast::specifiers_class_ s,
                                  donsus_ast::specifiers_class_ v)
    -> donsus_ast::specifiers_class_ {
  if (donsus_ast::to_bool(s & v)) {
    syntax_error(node, cur_token,
                 std::string(donsus_ast::specifiers_class_utils::type_name(v)) +
                     " is already present!");
  }
  return static_cast<donsus_ast::specifiers_class_>(s | v);
}

void Parser::parser_except_current(parse_result node, donsus_token_kind type) {
  if (cur_token.kind != type) {
    syntax_error(node, cur_token,
                 "Expected token: " + std::string(token::type_name(type)) +
                     ", got instead: " +
                     std::string(token::type_name(cur_token.kind)) + "\n");
  }
}

void Parser::parser_except_current_token(Parser::parse_result node,
                                         donsus_token_kind type, token token_) {
  if (token_.kind != type) {
    syntax_error(node, token_,
                 "Expected token: " + std::string(token_.type_name(type)) +
                     ", got instead: " +
                     std::string(token_.type_name(token_.kind)) + "\n");
  }
}
// Error handling
/*
 * If nullptr is passed in, show_error_on_line doesn't apply
 * */
void Parser::syntax_error(Parser::parse_result optional_node,
                          token err_on_token, const std::string &message,
                          Error::type error_type) {
  error.print_meta_syntax(err_on_token, message, file.absolute_path);
  if (!optional_node) {
    file.error_count += 1;
    error.error_out_coloured("", rang::fg::reset);
    std::cout << error.output_stream.toString();
    return;
  }

  error.show_error_on_line(optional_node->first_token_in_ast, err_on_token,
                           file.source);
  std::cout << error.output_stream.toString();
  // reset
  error.error_out_coloured("", rang::fg::reset);
  file.error_count += 1;
  Error::pos pos_l;
  pos_l.column = cur_token.column;
  pos_l.line = cur_token.line;
  pos_l.token_on = cur_token;

  create_error(error_type, message, pos_l);
}

void Parser::create_error(Error::type error_type, const std::string &message,
                          Error::pos position) {
  errors.emplace_back(error_type, message, position);
}

void ParserError::error_out_coloured(const std::string &message,
                                     rang::fg colour) {
  // need overload in the future
  std::cout << rang::style::bold << colour << message << rang::style::reset
            << rang::fg::reset;
}

void ParserError::print_meta_syntax(token err_on_token,
                                    const std::string &message,
                                    const std::string &full_path) {
  std::string message_c = "(" + std::to_string(err_on_token.line) + ":" +
                          std::to_string(err_on_token.column) + ") ";
  if (Tomi::has_ansi_colours()) {
    error_out_coloured(full_path, rang::fg::reset);
    error_out_coloured(message_c, rang::fg::green);
    error_out_coloured(" SYNTAX ERROR: ", rang::fg::red);
    error_out_coloured(message, rang::fg::reset);
    output_stream << "\n";
  } else {
    output_stream << full_path;
    output_stream << " SYNTAX ERROR: ";
    output_stream << message_c;
    output_stream << message;
    output_stream << "\n";
  }
}

// Todo: bug here with printing
void ParserError::show_error_on_line(token first_token, token cur_token,
                                     std::string &source) {
  std::string ast_in_source =
      source.substr(first_token.offset, cur_token.offset + cur_token.length);
  error_out_coloured(ast_in_source, rang::fg::reset);
  error_out_coloured("\n", rang::fg::reset);
  unsigned int indent = -1;
  for (unsigned int i = 0; i < (cur_token.column + indent); i++) {
    output_stream << " ";
  }

  error_out_coloured("^", rang::fg::green);

  for (unsigned int i = 0; i < cur_token.length; i++) {
    error_out_coloured("~", rang::fg::green);
  }
  error_out_coloured("^", rang::fg::green);
  error_out_coloured("\n", rang::fg::reset);
}
