#include "../Include/parser.h"
#include "../Include/Donsus/tomi.h"
#include "../src/ast/node.h"
#include "../src/utility/parser/helpers.h"
#include "../src/utility/parser/print_ast.h"

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
  return cur_token;
}
// Todo: maybe print value here
// Move it out from here
static std::ostream &operator<<(std::ostream &o, token &token) {
  o << "Kind: " << token.type_name() << "\n";
  o << "Length: " << token.length << "\n";
  o << "Line: " << token.line << "\n";
  return o;
}

void Parser::print_token() {
  while (cur_token.kind != donsus_token_kind::END) {
    std::cout << cur_token << "\n";
    parser_next();
  }
}

token Parser::peek(int loop) {
  donsus_lexer save = lexer;
  token result_main;
  for (int i = 0; i < loop; i++) {
    result_main = donsus_lexer_next(*this);
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

    Tomi::Vector<token> qualifier;
    while (is_qualifier(cur_token.kind)) {
      qualifier.push_back(cur_token);
      parser_next();
    }
    // start/declaration statement
    if (cur_token.kind == donsus_token_kind::TYPE_KW &&
        peek().kind == donsus_token_kind::IDENTIFIER) {
      parse_result result = type_constructor();
      tree->add_node(result);
    }
    if (cur_token.kind == donsus_token_kind::INSTANCE_KW &&
        peek().kind == donsus_token_kind::INSTANCE_KW) {
      parse_result result = instance();
      tree->add_node(result);
    }
    if (cur_token.kind == donsus_token_kind::AT) {
      parse_result result = language_extension();
      tree->add_node(result);
    }
    if (cur_token.kind == donsus_token_kind::TYPECLASS_KW) {
      parse_result result = typeclass();
      tree->add_node(result);
    }
    if (cur_token.kind == donsus_token_kind::IDENTIFIER) {
      if (qualifier.size() == 1 &&
          qualifier[0].kind == donsus_token_kind::ALIAS_KW) {
        parse_result result = alias();
        tree->add_node(result);
      }
      if (peek().kind == donsus_token_kind::COLON &&
          peek(2).kind == donsus_token_kind::COLON) {
        parse_result result = generics_decl();
        tree->add_node(result);
      }
      if (peek().kind == donsus_token_kind::LPAR) {
        parse_result result = function_decl();
        result->get<donsus_ast::function_decl>().qualifiers = qualifier;
        tree->add_node(result);
      } else if (peek().kind == donsus_token_kind::COLON) {
        parse_result result = variable_def();
        result->get<donsus_ast::variable_def>().qualifiers = qualifier;
        tree->add_node(result);
      } else if (is_assignment(peek().kind)) {
        parse_result result = assignments();
        tree->add_node(result);
      } else if ((peek().kind == donsus_token_kind::COMM &&
                  peek(2).kind == donsus_token_kind::IDENTIFIER)) {
        variable_multi_def();
      } else if (peek(2).kind == donsus_token_kind::LSQB) {
        parse_result result = array_decl();
        result->get<donsus_ast::array_decl>().qualifiers = qualifier;
        tree->add_node(result);
      } else {
        // error here;
      }
    } else if (cur_token.kind == donsus_token_kind::FUNCTION_DEFINITION_KW) {
      parse_result result = function_def();
      result->get<donsus_ast::function_def>().qualifiers = qualifier;
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
      parse_result result = case_statement();
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
    else if (is_class_decl(cur_token.kind) &&
             peek().kind == donsus_token_kind::CLASS_KW) {
      parse_result result = class_decl();
      result->get<donsus_ast::class_decl>().qualifiers = qualifier;
      tree->add_node(result);
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

auto Parser::create_function_decl() -> parse_result {
  return tree->create_node<donsus_ast::function_decl>(
      donsus_ast::donsus_node_type::FUNCTION_DECL);
}
auto Parser::function_decl() -> parse_result {}

auto Parser::create_variable_def() -> parse_result {
  return tree->create_node<donsus_ast::variable_def>(
      donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
}
auto Parser::variable_def() -> parse_result {
  parse_result definition = create_variable_def();
  definition->first_token_in_ast = cur_token;

  auto &body = definition->get<donsus_ast::variable_def>();
  body.identifier_name = lexeme_value(cur_token, file.source);

  return definition;
}

auto Parser::create_assignments() -> parse_result {
  return tree->create_node<donsus_ast::assignment>(
      donsus_ast::donsus_node_type::ASSIGNMENT);
}
auto Parser::assignments() -> parse_result {}

auto Parser::variable_multi_def() -> parse_result {}

auto Parser::create_function_def() -> parse_result {
  return tree->create_node<donsus_ast::function_def>(
      donsus_ast::donsus_node_type::FUNCTION_DEF);
}
auto Parser::function_def() -> parse_result {}

auto Parser::create_instance() -> parse_result {
  return tree->create_node<donsus_ast::instance>(
      donsus_ast::donsus_node_type::INSTANCE);
}
auto Parser::instance() -> parse_result {}
auto Parser::create_language_extension() -> parse_result {
  return tree->create_node<donsus_ast::language_extension>(
      donsus_ast::donsus_node_type::LANGUAGE_EXTENSION);
}

auto Parser::language_extension() -> parse_result {}
auto Parser::create_typeclass() -> parse_result {
  return tree->create_node<donsus_ast::typeclass>(
      donsus_ast::donsus_node_type::TYPECLASS);
}

auto Parser::typeclass() -> parse_result {}
auto Parser::create_alias() -> parse_result {
  return tree->create_node<donsus_ast::alias>(
      donsus_ast::donsus_node_type::ALIAS_STATEMENT);
}
auto Parser::alias() -> parse_result {}

auto Parser::create_generics_decl() -> parse_result {
  return tree->create_node<donsus_ast::generics_decl>(
      donsus_ast::donsus_node_type::GENERICS_DECL);
}
auto Parser::generics_decl() -> parse_result {}

auto Parser::array_decl() -> parse_result {
  return tree->create_node<donsus_ast::array_decl>(
      donsus_ast::donsus_node_type::ARRAY_DECL);
}
auto Parser::create_if_statement() -> parse_result {
  return tree->create_node<donsus_ast::if_statement>(
      donsus_ast::donsus_node_type::IF_STATEMENT);
}
auto Parser::if_statement() -> parse_result {}
auto Parser::create_case_statement() -> parse_result {
  return tree->create_node<donsus_ast::case_expr>(
      donsus_ast::donsus_node_type::CASE);
}
auto Parser::case_statement() -> parse_result {}

auto Parser::create_for_loop() -> parse_result {
  return tree->create_node<donsus_ast::for_loop>(
      donsus_ast::donsus_node_type::FOR_LOOP);
}
auto Parser::for_loop() -> parse_result {}
auto Parser::create_while_loop() -> parse_result {
  return tree->create_node<donsus_ast::while_loop>(
      donsus_ast::donsus_node_type::WHILE_LOOP);
}
auto Parser::while_loop() -> parse_result {}
auto Parser::create_type_constructor() -> parse_result {
  return tree->create_node<donsus_ast::type_constructor>(
      donsus_ast::donsus_node_type::TYPE_CONSTRUCTOR);
}
auto Parser::type_constructor() -> parse_result {}

auto Parser::create_class_decl() -> parse_result {
  return tree->create_node<donsus_ast::class_decl>(
      donsus_ast::donsus_node_type::CLASS);
}
auto Parser::class_decl() -> parse_result {}

void Parser::parser_except(donsus_token_kind type) {
  token a = parser_next();
  if (a.kind != type) {
    syntax_error(nullptr, cur_token,
                 "Expected token:" + std::string(token::type_name(type)) +
                     "got instead: " + std::string(token::type_name(cur_token.kind)) + "\n");
  }
}

void Parser::parser_except_current(donsus_token_kind type) {
  if (cur_token.kind != type) {
    syntax_error(nullptr, cur_token,
                 "Expected token:" + std::string(token::type_name(type)) +
                     "Got instead: " + std::string(token::type_name(cur_token.kind)) + "\n");
  }
}

// Error handling
/*
 * If nullptr is passed in, show_error_on_line doesn't apply
 * */
void Parser::syntax_error(Parser::parse_result *optional_node,
                          token err_on_token, const std::string &message) {
  error.print_meta_syntax(err_on_token, message, file.absolute_path);
  if (!optional_node){
      file.error_count += 1;
      error.error_out_coloured("", rang::fg::reset);
      return;
  }

  // reset
  error.error_out_coloured("", rang::fg::reset);
  file.error_count += 1;
}

void ParserError::error_out_coloured(const std::string &message,
                                     rang::fg colour) {
  std::cerr << rang::style::bold << colour << message << rang::style::reset
            << rang::fg::reset;
}

void ParserError::print_meta_syntax(token err_on_token,
                                    const std::string &message,
                                    const std::string &full_path) {
  std::string message_c = "(" + std::to_string(err_on_token.line) + "?" +
                          std::to_string(err_on_token.column) + ") ";
  if (Tomi::has_ansi_colours()) {
    error_out_coloured(full_path, rang::fg::reset);
    error_out_coloured(message_c, rang::fg::green);
    error_out_coloured(" SYNTAX ERROR: ", rang::fg::red);
    error_out_coloured(message, rang::fg::reset);
    std::cout << "\n";
  } else {
    std::cout << full_path;
    std::cout << "SYNTAX ERROR: ";
    std::cout << message_c;
    std::cout << message;
    std::cout << "\n";
  }
}
