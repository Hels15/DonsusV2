#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Expr, Arithmetic) {
  std::string a = R"(
    a:int = 2 + 3;
    b:int = 3 + 2 * 2;
    c:int = 4 / 2 - 1;
    d:int = 2 - 2 + 1;
    e:int = (2 + 2) * 2;
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  // a:int = 2 + 3;
  auto var_type_1 = result->get_nodes()[0]->type;
  auto var_def_1 = result->get_nodes()[0];

  EXPECT_EQ(var_type_1.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(lexeme_value(
                var_def_1->children[0]->get<donsus_ast::expression>().value, a),
            "+");

  //  3 + 2 * 2;
  auto var_type_2 = result->get_nodes()[1]->type;
  auto var_def_2 = result->get_nodes()[1];

  EXPECT_EQ(lexeme_value(
                var_def_2->children[0]->get<donsus_ast::expression>().value, a),
            "+");
  EXPECT_EQ(lexeme_value(var_def_2->children[0]
                             ->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "*");

  EXPECT_EQ(var_type_2.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  // c:int = 4 / 2 - 1;
  auto var_type_3 = result->get_nodes()[2]->type;
  auto var_def_3 = result->get_nodes()[2];

  EXPECT_EQ(lexeme_value(
                var_def_3->children[0]->get<donsus_ast::expression>().value, a),
            "/");
  EXPECT_EQ(lexeme_value(var_def_3->children[0]
                             ->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "-");

  EXPECT_EQ(var_type_3.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  // d:int = 2 - 2 + 1;
  auto var_type_4 = result->get_nodes()[3]->type;
  auto var_def_4 = result->get_nodes()[3];

  EXPECT_EQ(lexeme_value(
                var_def_4->children[0]->get<donsus_ast::expression>().value, a),
            "+");
  EXPECT_EQ(lexeme_value(var_def_4->children[0]
                             ->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "-");
  EXPECT_EQ(var_type_4.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  // e:int = (2 + 2) * 2;
  auto var_type_5 = result->get_nodes()[4]->type;
  auto var_def_5 = result->get_nodes()[4];

  EXPECT_EQ(lexeme_value(
                var_def_5->children[0]->get<donsus_ast::expression>().value, a),
            "*");
  EXPECT_EQ(lexeme_value(var_def_5->children[0]
                             ->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "+");

  EXPECT_EQ(var_type_5.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(file.error_count, 0);
}

/*
// precedence showcase
TEST(Expr, BitwisePrecedence){

}

TEST(Expr, Comparison){

}*/
