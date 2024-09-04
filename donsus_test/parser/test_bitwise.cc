#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Bitwise, MainBitwiseOps) {
  std::string a = R"(
    a:int = a & b;
    b:int = a | b;
    c:int = a ^ b;
  )";
  AstFile file;
  file.source = a;

  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def_1 = result->get_nodes()[0];
  auto main_type_1 = result->get_nodes()[0]->type;

  EXPECT_EQ(lexeme_value(
                var_def_1->children[0]->get<donsus_ast::expression>().value, a),
            "&");

  EXPECT_EQ(main_type_1.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  auto var_def_2 = result->get_nodes()[1];
  auto main_type_2 = result->get_nodes()[1]->type;

  EXPECT_EQ(lexeme_value(
                var_def_2->children[0]->get<donsus_ast::expression>().value, a),
            "|");
  EXPECT_EQ(main_type_2.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  auto var_def_3 = result->get_nodes()[2];
  auto main_type_3 = result->get_nodes()[2]->type;

  EXPECT_EQ(lexeme_value(
                var_def_3->children[0]->get<donsus_ast::expression>().value, a),
            "^");
  EXPECT_EQ(main_type_3.type,
            donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(file.error_count, 0);
}

TEST(Bitwise, BitShift) {
  std::string a = R"(
    a:int = a << b;
    b:int = b >> a;
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def_1 = result->get_nodes()[0];
  auto main_type_1 = result->get_nodes()[0]->type;

  auto var_def_2 = result->get_nodes()[1];
  auto main_type_2 = result->get_nodes()[1]->type;

  EXPECT_EQ(lexeme_value(
                var_def_1->children[0]->get<donsus_ast::expression>().value, a),
            "<<");

  EXPECT_EQ(lexeme_value(
                var_def_2->children[0]->get<donsus_ast::expression>().value, a),
            ">>");
  EXPECT_EQ(file.error_count, 0);
}
