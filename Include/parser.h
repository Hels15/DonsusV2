// Declarations for parser.cc
#ifndef PARSER_H
#define PARSER_H

#include "../Include/token.h"
#include "../src/ast/node.h"
#include "../src/ast/tree.h"
#include "../src/utility/exception.h"
#include "../src/utility/handle.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

class Parser;

struct AstFile {
  int id;
  int flags;

  std::string fullpath;
  std::string filename;
  std::string directory;

  std::string absolute_path;
  std::string extension;
  Parser *parser;
  unsigned int error_count{};
};

/**
 *  \brief Lexer API
 */
struct donsus_lexer {
  donsus_lexer(const std::string &input)
      : string(input), cur_pos(0), cur_column(0), cur_line(1),
        cur_char(input[0]) {}
  donsus_lexer() = default;

  std::string string;
  char cur_char;
  unsigned cur_column;
  unsigned int cur_pos, cur_line;
};
token donsus_lexer_next(Parser &parser);

class Parser {
public:
  using parse_result = utility::handle<donsus_ast::node>;
  using end_result = utility::handle<donsus_ast::tree>;

  Parser(donsus_lexer &lexer, AstFile &file);
  token parser_next();
  void parser_except(donsus_token_kind type);
  void donsus_parser_except_current(donsus_token_kind type);

  auto donsus_parse() -> end_result;
  void print_token();
  token peek(int loop = 1);

  Parser &operator=(const Parser &f) {
    if (this != &f) {
      cur_token = f.cur_token;
      lexer = f.lexer;
      donsus_tree = f.donsus_tree;
      allocator = f.allocator;
      file = f.file;
    }
    return *this;
  }
  token cur_token;
  donsus_lexer lexer;
  // Top level tree
  utility::handle<donsus_ast::tree> donsus_tree;
  utility::DonsusAllocator allocator;
  // creating nodes
  auto create_expression(donsus_ast::donsus_node_type type,
                         std::uint64_t child_count) -> parse_result;
  auto create_variable_definition(donsus_ast::donsus_node_type type,
                                  uint64_t child_count) -> parse_result;

  auto create_array_definition() -> parse_result;

  auto create_array_declaration() -> parse_result;

  auto create_integer_expression() -> parse_result;

  auto create_while_loop() -> parse_result;

  auto create_for_loop() -> parse_result;

  auto create_range_expression() -> parse_result;

  auto create_float_expression() -> parse_result;

  auto create_donsus_print() -> parse_result;

  auto create_function_decl() -> parse_result;

  auto create_array_access() -> parse_result;

  auto create_string_expression() -> parse_result;

  auto create_bool_expression() -> parse_result;

  auto create_unary_expression() -> parse_result;

  auto create_function_definition() -> parse_result;

  auto create_if_statement() -> parse_result;

  auto create_else_statement() -> parse_result;

  auto create_assignments() -> parse_result;

  auto create_identifier() -> parse_result;

  auto create_function_call() -> parse_result;

  auto create_return_statement() -> parse_result;

private:
  DonsusParserError error;
  AstFile &file;
};

#endif // PARSER_H