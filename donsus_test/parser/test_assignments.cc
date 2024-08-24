#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Assignment, AssignmentWithLiteral) {
  std::string a = R"(
    a += 2;
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto assignment_def = result->get_nodes()[0];

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .lvalue->get<donsus_ast::expression>()
                             .value,
                         a),
            "a");

  EXPECT_EQ(donsus_token_kind::PLUS_EQUAL,
            assignment_def->get<donsus_ast::assignment>().op.kind);

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .rvalue->get<donsus_ast::expression>()
                             .value,
                         a),
            "2");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ASSIGNMENT);
}

TEST(Assignment, AssignmentWithExpressions) {
  std::string a = R"(
    a += 2 + 2;
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto assignment_def = result->get_nodes()[0];

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .lvalue->get<donsus_ast::expression>()
                             .value,
                         a),
            "a");

  EXPECT_EQ(donsus_token_kind::PLUS_EQUAL,
            assignment_def->get<donsus_ast::assignment>().op.kind);

  auto plus_op = assignment_def->get<donsus_ast::assignment>()
                     .rvalue->get<donsus_ast::expression>();

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .rvalue->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "2");

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .rvalue->get<donsus_ast::expression>()
                             .value,
                         a),
            "+");

  EXPECT_EQ(lexeme_value(assignment_def->get<donsus_ast::assignment>()
                             .rvalue->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "2");
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ASSIGNMENT);
}