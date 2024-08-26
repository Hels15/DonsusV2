#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Case, CaseUnconditionalWString) {
  std::string a = R"(
  a:auto = case x of
  1 -> "one"
  2 -> "two"
  _ -> "Other";
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_plain = result->get_nodes()[0];

  auto main_type = result->get_nodes()[0]->type;
  auto var_def = result->get_nodes()[0]->get<donsus_ast::variable_def>();
  auto case_def = var_plain->children[0]->get<donsus_ast::case_expr>();

  // Variable type
  EXPECT_EQ(
      var_def.identifier_type->get<donsus_ast::identifier>().identifier_name,
      "auto");

  // Scrutinee
  EXPECT_EQ(case_def.scrutinee->type.type,
            donsus_ast::donsus_node_type::EXPRESSION);

  // Number of patterns
  EXPECT_EQ(case_def.patterns.size(), 3);

  // pattern1
  auto pattern_1 = case_def.patterns[0]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_1.type, donsus_ast::PatternType::UNCONDITIONAL);
  auto pattern_1_guard = pattern_1.guard;
  auto pattern_1_result_expr = pattern_1.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_1_guard->get<donsus_ast::expression>().value, a),
      "1");
  EXPECT_EQ(lexeme_value(
                pattern_1_result_expr->get<donsus_ast::expression>().value, a),
            "one");
  // pattern2
  auto pattern_2 = case_def.patterns[1]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_2.type, donsus_ast::PatternType::UNCONDITIONAL);
  auto pattern_2_guard = pattern_2.guard;
  auto pattern_2_result_expr = pattern_2.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_2_guard->get<donsus_ast::expression>().value, a),
      "2");
  EXPECT_EQ(lexeme_value(
                pattern_2_result_expr->get<donsus_ast::expression>().value, a),
            "two");

  // pattern3
  auto pattern_3 = case_def.patterns[2]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_3.type, donsus_ast::PatternType::UNCONDITIONAL);
  auto pattern_3_guard = pattern_3.guard;
  auto pattern_3_result_expr = pattern_3.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_3_guard->get<donsus_ast::expression>().value, a),
      "_");
  EXPECT_EQ(lexeme_value(
                pattern_3_result_expr->get<donsus_ast::expression>().value, a),
            "Other");

  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(var_plain->children[0]->type.type,
            donsus_ast::donsus_node_type::CASE);
}

TEST(Case, ConditionalWString) {
  std::string a = R"(
    f: auto = case x of
        | y > 0 -> "Positive"
        | y < 0  -> "Negative"
        | otherwise -> "Zero";

  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_plain = result->get_nodes()[0];

  auto main_type = result->get_nodes()[0]->type;
  auto var_def = result->get_nodes()[0]->get<donsus_ast::variable_def>();
  auto case_def = var_plain->children[0]->get<donsus_ast::case_expr>();

  // Variable type
  EXPECT_EQ(
      var_def.identifier_type->get<donsus_ast::identifier>().identifier_name,
      "auto");

  // Scrutinee
  EXPECT_EQ(case_def.scrutinee->type.type,
            donsus_ast::donsus_node_type::EXPRESSION);

  // Number of patterns
  EXPECT_EQ(case_def.patterns.size(), 3);

  // pattern1
  auto pattern_1 = case_def.patterns[0]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_1.type, donsus_ast::PatternType::CONDITIONAL);
  auto pattern_1_guard = pattern_1.guard;
  auto pattern_1_result_expr = pattern_1.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_1_guard->get<donsus_ast::expression>().value, a),
      ">");

  EXPECT_EQ(lexeme_value(
                pattern_1_result_expr->get<donsus_ast::expression>().value, a),
            "Positive");

  // pattern2
  auto pattern_2 = case_def.patterns[1]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_2.type, donsus_ast::PatternType::CONDITIONAL);
  auto pattern_2_guard = pattern_2.guard;
  auto pattern_2_result_expr = pattern_2.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_2_guard->get<donsus_ast::expression>().value, a),
      "<");

  EXPECT_EQ(lexeme_value(
                pattern_2_result_expr->get<donsus_ast::expression>().value, a),
            "Negative");

  // pattern3
  auto pattern_3 = case_def.patterns[2]->get<donsus_ast::pattern>();
  EXPECT_EQ(pattern_3.type, donsus_ast::PatternType::CONDITIONAL);
  auto pattern_3_guard = pattern_3.guard;
  auto pattern_3_result_expr = pattern_3.result_expression;

  EXPECT_EQ(
      lexeme_value(pattern_3_guard->get<donsus_ast::expression>().value, a),
      "otherwise");

  EXPECT_EQ(lexeme_value(
                pattern_3_result_expr->get<donsus_ast::expression>().value, a),
            "Zero");

  EXPECT_EQ(file.error_count, 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(var_plain->children[0]->type.type,
            donsus_ast::donsus_node_type::CASE);
}
