// Declarations for parser.cc
#ifndef PARSER_H
#define PARSER_H

#include "../Include/Donsus/terminal_coloured.h"
#include "../Include/Donsus/tomi.h"
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

class ParserError {
public:
  void print_meta_syntax(token err_on_token, const std::string &message,
                         const std::string &full_path);
  void error_out_coloured(const std::string &message, rang::fg colour);
};

struct AstFile {
  int id;
  int flags;

  // source of the file
  std::string source;

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

  // expressions
  auto create_expr() -> parse_result;
  auto expr() -> parse_result;

  auto bool_or_expr() -> parse_result;
  auto bool_and_expr() -> parse_result;
  auto compare_expr() -> parse_result;
  auto arithmetic_expr() -> parse_result;
  auto bitwise_expr() -> parse_result;
  auto addition_expr() -> parse_result;

  auto multiply_expr() -> parse_result;
  auto bitshift_expr() -> parse_result;

  auto as_expr() -> parse_result;
  auto prefix_expr() -> parse_result;

  auto postfix_expr() -> parse_result;
  auto expr_val() -> parse_result;

  auto create_variable_def() -> parse_result;
  auto variable_def() -> parse_result;

  auto create_arg_decl() -> parse_result;
  auto arg_decl() -> parse_result;

  auto variable_multi_def() -> Tomi::Vector<parse_result>;

  auto create_array_def() -> parse_result;
  auto array_def() -> parse_result;

  auto create_array_decl() -> parse_result;
  auto array_decl() -> parse_result;

  auto create_integer_expression() -> parse_result;
  auto integer_expression() -> parse_result;

  auto create_indices() -> parse_result;
  auto indices() -> parse_result;

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

  auto create_template_decl() -> parse_result;
  auto template_decl() -> parse_result;
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

  auto if_var_decls() -> Tomi::Vector<parse_result>;
  auto create_else_statement() -> parse_result;
  auto else_statement() -> parse_result;

  auto create_else_if_statement() -> parse_result;
  auto else_if_statement() -> parse_result;

  auto statements() -> Tomi::Vector<parse_result>;
  auto create_assignments() -> parse_result;
  auto assignments() -> parse_result;

  // case/patterns
  auto create_case_expr() -> parse_result;
  auto case_expr() -> parse_result;

  auto create_pattern() -> parse_result;
  auto pattern() -> parse_result;

  auto patterns() -> Tomi::Vector<utility::handle<donsus_ast::node>>;

  auto create_do_statement() -> parse_result;
  auto create_of_statement() -> parse_result;

  auto create_otherwise_statement() -> parse_result;

  auto create_as_statement() -> parse_result;

  auto create_alias() -> parse_result;
  auto alias() -> parse_result;
  /*(typeclass)
   * typeclass Test<T> {
   *    ...
   *                   }
   * */
  auto create_typeclass() -> parse_result;
  auto typeclass() -> parse_result;

  auto create_class_def() -> parse_result;
  auto class_def() -> parse_result;

  auto create_final() -> parse_result;
  auto create_abstract() -> parse_result;

  auto create_loop_var(token) -> parse_result;
  auto create_bitshift() -> parse_result;
  auto create_bitwise() -> parse_result;
  /* (typeconstructor)
   * type MyList a = Empty | Cons a (MyList a)
   * */
  auto create_type_constructor() -> parse_result;
  auto type_constructor() -> parse_result;

  auto create_data_constructor() -> parse_result;
  auto data_constructor() -> parse_result;

  // data_constructor helper
  auto data_constructor_part() -> parse_result;

  auto create_instance() -> parse_result;
  auto instance() -> parse_result;

  auto create_language_extension() -> parse_result;
  auto language_extension() -> parse_result;

  auto create_identifier() -> parse_result;
  auto identifier() -> parse_result;

  auto create_function_call() -> parse_result;

  auto create_generics_decl() -> parse_result;
  auto generics_decl() -> parse_result;
  auto create_return_statement() -> parse_result;

  auto create_type() -> parse_result;
  /* (type)
   * a: int = 12; # int here as present in the source
   * */
  auto create_constraint() -> parse_result;
  auto constraint() -> parse_result;

  auto type() -> parse_result;
  auto tuple() -> parse_result;
  auto create_tuple() -> parse_result;

  auto create_pointer() -> parse_result;
  auto pointer(parse_result) -> parse_result;

  auto create_reference() -> parse_result;
  auto reference(parse_result referent) -> parse_result;

  // errors
  void syntax_error(Parser::parse_result *node, token err_on_token,
                    const std::string &message);

  void parser_except(donsus_token_kind type);
  void parser_except_current(parse_result node, donsus_token_kind type);

  // helpers
  auto arg_list() -> Tomi::Vector<parse_result>;
  auto return_from_func() -> parse_result;
  auto set_specifiers(parse_result node, donsus_ast::specifiers_ s,
                      donsus_ast::specifiers_ v) -> donsus_ast::specifiers_;

  auto set_class_specifiers(parse_result node, donsus_ast::specifiers_class_ s,
                            donsus_ast::specifiers_class_ v)
      -> donsus_ast::specifiers_class_;

private:
  ParserError error;
  AstFile &file;
};

#endif // PARSER_H