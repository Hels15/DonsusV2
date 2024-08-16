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
// Move it out from here
static std::ostream &operator<<(std::ostream &o, token &token) {
  // Todo: reconstruct this with lexeme value static
  /*  o << "Value: " << token << "\n";*/
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

auto Parser::function_decl() -> parse_result {}

auto Parser::variable_def() -> parse_result {}

auto Parser::assignments() -> parse_result {}

auto Parser::variable_multi_def() -> parse_result {}
auto Parser::function_def() -> parse_result {}
auto Parser::instance() -> parse_result {}
auto Parser::language_extension() -> parse_result {}
auto Parser::typeclass() -> parse_result {}
auto Parser::alias() -> parse_result {}
auto Parser::generics_decl() -> parse_result {}
auto Parser::array_decl() -> parse_result {}
auto Parser::if_statement() -> parse_result {}
auto Parser::case_statement() -> parse_result {}
auto Parser::for_loop() -> parse_result {}
auto Parser::while_loop() -> parse_result {}
auto Parser::type_constructor() -> parse_result {}