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

  auto parse() -> end_result;
  void print_token();
  token peek(int loop = 1);

  Parser &operator=(const Parser &f) {
    if (this != &f) {
      cur_token = f.cur_token;
      lexer = f.lexer;
      tree = f.tree;
      allocator = f.allocator;
      file = f.file;
    }
    return *this;
  }
  token cur_token;
  donsus_lexer lexer;
  // Top level tree
  utility::handle<donsus_ast::tree> tree;
  utility::DonsusAllocator allocator;
  // creating nodes
  auto create_expression(donsus_ast::donsus_node_type type,
                         std::uint64_t child_count) -> parse_result;

  auto create_variable_def() -> parse_result;
  auto variable_def() -> parse_result;

  auto create_variable_multi_def() -> parse_result;
  auto variable_multi_def() -> parse_result;

  auto create_array_def() -> parse_result;
  auto array_def() -> parse_result;

  auto create_array_decl() -> parse_result;
  auto array_decl() -> parse_result;

  auto create_integer_expression() -> parse_result;
  auto integer_expression() -> parse_result;

  auto create_while_loop() -> parse_result;
  auto while_loop() -> parse_result;

  auto create_for_loop() -> parse_result;
  auto for_loop() -> parse_result;

  auto create_range_expression() -> parse_result;
  auto range_expression() -> parse_result;

  auto create_float_expression() -> parse_result;
  auto float_expression() -> parse_result;

  auto create_binary_expression() -> parse_result;
  auto binary_expression() -> parse_result;

  auto create_octal_expression() -> parse_result;
  auto octal_expression() -> parse_result;

  auto create_hex_expression() -> parse_result;
  auto hex_expression() -> parse_result;

  auto create_print() -> parse_result;
  auto print() -> parse_result;

  auto create_function_decl() -> parse_result;
  auto function_decl() -> parse_result;

  auto create_array_access() -> parse_result;
  auto array_access() -> parse_result;
  auto create_string_expression() -> parse_result;

  auto create_bool_expression() -> parse_result;

  auto create_unary_expression() -> parse_result;

  auto create_function_def() -> parse_result;
  auto function_def() -> parse_result;

  auto create_continue_statement() -> parse_result;

  auto create_break_statement() -> parse_result;

  auto create_fallthrough_statement() -> parse_result;

  auto create_if_statement() -> parse_result;
  auto if_statement() -> parse_result;

  auto create_else_statement() -> parse_result;

  auto create_assignments() -> parse_result;
  auto assignments() -> parse_result;

  auto create_case_statement() -> parse_result;
  auto case_statement() -> parse_result;

  auto create_do_statement() -> parse_result;
  auto create_of_statement() -> parse_result;

  auto create_otherwise_statement() -> parse_result;

  auto create_as_statement() -> parse_result;
  auto create_type_statement() -> parse_result;
  auto create_alias() -> parse_result;
  auto alias() -> parse_result;

  auto create_typeclass() -> parse_result;
  auto typeclass() -> parse_result;

  auto create_class() -> parse_result;
  auto create_final() -> parse_result;
  auto create_abstract() -> parse_result;

  auto create_bitshift() -> parse_result;
  auto create_bitwise() -> parse_result;

  auto create_type_constructor() -> parse_result;
  auto type_constructor() -> parse_result;

  auto create_instance() -> parse_result;
  auto instance() -> parse_result;

  auto create_language_extension() -> parse_result;
  auto language_extension() -> parse_result;

  auto create_identifier() -> parse_result;
  auto create_function_call() -> parse_result;

  auto create_generics_decl() -> parse_result;
  auto generics_decl() -> parse_result;
  auto create_return_statement() -> parse_result;

  // handle error
  void display_error();

private:
  DonsusParserError error;
  AstFile &file;
};

#endif // PARSER_H