#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(ErrorVar, Var) {
  std::string a = R"(a: int = 12)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  Error::pos expected_pos;
  // basically end token
  expected_pos.line = 1;
  expected_pos.column = 0;
  expected_pos.token_on = {donsus_token_kind::END, 0, parser.lexer.cur_line};

  Error errorObj = parser.errors[0];
  EXPECT_EQ(errorObj.ErrorType, Error::type::MissingToken);
  EXPECT_EQ(errorObj.message, "Expected token: SEMICOLON, got instead: END\n");
  EXPECT_EQ(errorObj.ErrorPos, expected_pos);

  EXPECT_EQ(parser.errors.size(), 1);
  EXPECT_EQ(file.error_count, 1);
}