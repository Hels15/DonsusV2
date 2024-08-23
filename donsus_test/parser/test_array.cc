#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Array, ArrayDefLiteral) {
  std::string a = R"(
    a:int[] = [1, 2, 3];
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto array_def = result->get_nodes()[0];
  auto array_type = array_def->get<donsus_ast::array_def>().array_type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ARRAY_DEF);
  auto elements = result->get_nodes()[0]
                      ->get<donsus_ast::array_def>()
                      .body->get<donsus_ast::array>()
                      .items;
  EXPECT_EQ(elements.size(), 3);
  EXPECT_EQ(elements[0]->type.type, donsus_ast::donsus_node_type::EXPRESSION);
  EXPECT_EQ(elements[0]->get<donsus_ast::expression>().value.kind,
            donsus_token_kind::INT);
  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ(donsus_ast::ArrayType::DYNAMIC, array_type);
}

TEST(Array, ArrayDefRangeExpression) {
  std::string a = R"(
  a: int[] = 1..2;
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;

  auto array_def = result->get_nodes()[0];
  auto array_type = array_def->get<donsus_ast::array_def>().array_type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ARRAY_DEF);

  auto elements = result->get_nodes()[0]
                      ->get<donsus_ast::array_def>()
                      .body->get<donsus_ast::range_expr>();

  auto start = elements.start->get<donsus_ast::expression>().value;
  auto end = elements.end->get<donsus_ast::expression>().value;

  EXPECT_EQ(lexeme_value(start, a), "1");
  EXPECT_EQ(lexeme_value(end, a), "2");

  EXPECT_EQ(array_def->get<donsus_ast::array_def>().body->type.type,
            donsus_ast::donsus_node_type::RANGE_EXPRESSION);

  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ(elements.start->get<donsus_ast::expression>().value.kind,
            donsus_token_kind::INT);

  EXPECT_EQ(donsus_ast::ArrayType::DYNAMIC, array_type);
}

TEST(Array, ArrayDefLiteralFixed) {
  std::string a = R"(
    a:int[3] = [1, 2, 3];
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto array_def = result->get_nodes()[0];
  auto array_type = array_def->get<donsus_ast::array_def>().array_type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ARRAY_DEF);
  auto elements = result->get_nodes()[0]
                      ->get<donsus_ast::array_def>()
                      .body->get<donsus_ast::array>()
                      .items;
  EXPECT_EQ(elements.size(), 3);
  EXPECT_EQ(elements[0]->type.type, donsus_ast::donsus_node_type::EXPRESSION);
  EXPECT_EQ(elements[0]->get<donsus_ast::expression>().value.kind,
            donsus_token_kind::INT);
  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ("3", lexeme_value(array_def->get<donsus_ast::array_def>()
                                  .indices->get<donsus_ast::expression>()
                                  .value,
                              a));

  EXPECT_EQ(donsus_ast::ArrayType::FIXED, array_type);
}

TEST(Array, ArrayDefLiteralStatic) {
  std::string a = R"(
    a:int[3]. = [1, 2, 3];
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto array_def = result->get_nodes()[0];
  auto array_type = array_def->get<donsus_ast::array_def>().array_type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ARRAY_DEF);
  auto elements = result->get_nodes()[0]
                      ->get<donsus_ast::array_def>()
                      .body->get<donsus_ast::array>()
                      .items;
  EXPECT_EQ(elements.size(), 3);
  EXPECT_EQ(elements[0]->type.type, donsus_ast::donsus_node_type::EXPRESSION);
  EXPECT_EQ(elements[0]->get<donsus_ast::expression>().value.kind,
            donsus_token_kind::INT);
  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ("3", lexeme_value(array_def->get<donsus_ast::array_def>()
                                  .indices->get<donsus_ast::expression>()
                                  .value,
                              a));
  EXPECT_EQ(donsus_ast::ArrayType::STATIC, array_type);
}