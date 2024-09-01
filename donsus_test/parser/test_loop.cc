#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(ForLoop, RangeBased) {
  std::string a = R"(
    for number: my_array {
  printf("We got a : ", number);
  }

)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto loop_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto loop_def_body = loop_def->get<donsus_ast::for_loop>();

  EXPECT_EQ(loop_def_body.type_of_loop, donsus_ast::loop_type::RANGE_BASED);

  EXPECT_EQ(loop_def_body.loop_variable->get<donsus_ast::identifier>()
                .identifier_name,
            "number");

  EXPECT_EQ(lexeme_value(
                loop_def_body.iterator->get<donsus_ast::expression>().value, a),
            "my_array");

  EXPECT_EQ(loop_def_body.body.size(), 1);

  // body
  EXPECT_EQ(loop_def_body.body[0]->get<donsus_ast::function_call>().func_name,
            "printf");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FOR_LOOP);
  EXPECT_EQ(file.error_count, 0);
}

TEST(ForLoop, RangeIterator) {
  std::string a = R"(
    for 1..10 {
  print("in loop");
  }
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto loop_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto loop_def_body = loop_def->get<donsus_ast::for_loop>();

  // No loop variable
  // EXPECT_EQ(loop_def_body.loop_variable->get<donsus_ast::identifier>().identifier_name,
  // "number");
  EXPECT_EQ(loop_def_body.loop_variable.get(), nullptr);
  EXPECT_EQ(loop_def_body.type_of_loop, donsus_ast::loop_type::RANGE_BASED);
  // ITERATOR
  EXPECT_EQ(lexeme_value(loop_def_body.iterator->get<donsus_ast::range_expr>()
                             .start->get<donsus_ast::expression>()
                             .value,
                         a),
            "1");
  EXPECT_EQ(lexeme_value(loop_def_body.iterator->get<donsus_ast::range_expr>()
                             .end->get<donsus_ast::expression>()
                             .value,
                         a),
            "10");

  EXPECT_EQ(loop_def_body.iterator->type.type,
            donsus_ast::donsus_node_type::RANGE_EXPRESSION);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FOR_LOOP);
  EXPECT_EQ(file.error_count, 0);
}

TEST(ForLoop, Traditional) {
  std::string a = R"(
    for (i: auto = 1; i < 10; i += 1) {
  # introduces new scope
  }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto loop_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto loop_def_body = loop_def->get<donsus_ast::for_loop>();

  // init_statement
  EXPECT_EQ(loop_def_body.init_statement->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "i");

  // condition
  EXPECT_EQ(
      lexeme_value(loop_def_body.condition->get<donsus_ast::expression>().value,
                   a),
      "<");
  // increment_expr
  EXPECT_EQ(
      lexeme_value(
          loop_def_body.increment_expr->get<donsus_ast::assignment>().op, a),
      "+=");
  // body
  EXPECT_EQ(loop_def_body.body.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FOR_LOOP);
  EXPECT_EQ(file.error_count, 0);
}
/*

TEST(WhileLoop, Normal) {
  std::string a = R"(
      while n < 10 {
    n += 1;
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto loop_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FOR_LOOP);
  EXPECT_EQ(file.error_count, 0);
}

TEST(WhileLoop, DoBased) {
  std::string a = R"(
    while n < 10 {
    n += 1;
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto loop_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FOR_LOOP);
  EXPECT_EQ(file.error_count, 0);
}
*/
