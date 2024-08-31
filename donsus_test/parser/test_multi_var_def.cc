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

  // first var
  EXPECT_EQ(multi_var_def_1->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  // second var
  EXPECT_EQ(multi_var_def_2->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "b");

  EXPECT_EQ(main_type_1.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(main_type_2.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(file.error_count, 0);
}

TEST(MultiVarDef, MultiVarDefExpression) {

  std::string a = R"(
    a, b:int = 12 + 12;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type_1 = result->get_nodes()[0]->type;
  auto multi_var_def_1 = result->get_nodes()[0];

  auto main_type_2 = result->get_nodes()[1]->type;
  auto multi_var_def_2 = result->get_nodes()[1];

  // first var
  EXPECT_EQ(multi_var_def_1->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  // second var
  EXPECT_EQ(multi_var_def_2->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "b");

  EXPECT_EQ(file.error_count, 0);
}

TEST(MultiVarDef, MultiVarDefMultiValue) {
  std::string a = R"(
    a: int = 12, b = 12;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type_1 = result->get_nodes()[0]->type;
  auto multi_var_def_1 = result->get_nodes()[0];

  auto main_type_2 = result->get_nodes()[1]->type;
  auto multi_var_def_2 = result->get_nodes()[1];

  // opposite order here
  EXPECT_EQ(multi_var_def_1->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  // second var
  EXPECT_EQ(multi_var_def_2->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "b");

  EXPECT_EQ(main_type_1.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(main_type_2.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(file.error_count, 0);
}
