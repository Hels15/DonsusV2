#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(MultiVarDef, MultiVarDefWithLiteral) {
  std::string a = R"(
  a, b:int = 12;
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type_1 = result->get_nodes()[0]->type;
  auto multi_var_def_1 = result->get_nodes()[0];

  auto main_type_2 = result->get_nodes()[1]->type;
  auto multi_var_def_2 = result->get_nodes()[1];

  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ(main_type_1.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(main_type_2.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
}

TEST(MultiVarDef, MultiVarDefExpression) {

  std::string a = R"(
    a, b:int = 12 + 12;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  EXPECT_EQ(file.error_count, 0);
}