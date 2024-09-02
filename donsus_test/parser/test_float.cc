#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Float, BasicFloat) {
  std::string a = R"(
    a: float = 1.2;
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto float_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  auto float_var_def = float_def->get<donsus_ast::variable_def>();
  EXPECT_EQ(lexeme_value(float_def->children[0]->get<donsus_ast::expression>().value, a), "1.2");

  EXPECT_EQ(float_def->children[0]->get<donsus_ast::expression>().value.kind, donsus_token_kind::FLOAT);

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}
