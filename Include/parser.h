// Declarations for parser.cc
#ifndef PARSER_H
#define PARSER_H

#include "../src/ast/node.h"
#include "../src/ast/tree.h"
#include "../src/utility/handle.h"
#include "../Include/token.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

struct AstFile {
  int id;
  int flags;

  std::string fullpath;
  std::string filename;
  std::string directory;

  std::string absolute_path;
  std::string extension;

  unsigned int error_count{};
};

/**
 *  \brief Lexer API
 */
struct lexer {
  donsus_lexer(const std::string &input)
      : string(input), cur_pos(0), cur_column(0), cur_line(1),
        cur_char(input[0]) {}
  donsus_lexer() = default;

  std::string string;
  char cur_char;
  unsigned cur_column;
  unsigned int cur_pos, cur_line;
};
class Parser {
public:
  using parse_result = utility::handle<donsus_ast::node>;
  using end_result = utility::handle<donsus_ast::tree>;

  Parser(lexer &lexer, AstFile& file);

  donsus_token cur_token;
  lexer lexer;
  // Top level tree
  utility::handle<donsus_ast::tree> donsus_tree;
};

#endif // PARSER_H