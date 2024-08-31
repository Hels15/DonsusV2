#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Indices, Single) {
  std::string a = R"(
    a[0];
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto indices_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(indices_def->get<donsus_ast::indices>()
                .name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");
  EXPECT_EQ(lexeme_value(indices_def->get<donsus_ast::indices>()
                             .index->get<donsus_ast::expression>()
                             .value,
                         a),
            "0");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::INDICES);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Indices, WithExpression) {
  std::string a = R"(
  a[n+1];
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto indices_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(indices_def->get<donsus_ast::indices>()
                .name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");
  EXPECT_EQ(lexeme_value(indices_def->get<donsus_ast::indices>()
                             .index->get<donsus_ast::expression>()
                             .value,
                         a),
            "+");

  EXPECT_EQ(lexeme_value(indices_def->get<donsus_ast::indices>()
                             .index->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "n");

  EXPECT_EQ(lexeme_value(indices_def->get<donsus_ast::indices>()
                             .index->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "1");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::INDICES);
  EXPECT_EQ(file.error_count, 0);
}