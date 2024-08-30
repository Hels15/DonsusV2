#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(IfStatement, Single) {
  std::string a = R"(
    if(1 < 2) {
      printf("some_function_call");
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  // condition
  EXPECT_EQ(lexeme_value(if_def->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            "<");
  // body
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().body.size(), 1);
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  // if var_decls
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().if_var_decls.size(), 0);

  // alternates(else, else if)
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().alternate.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatement, Multiple) {
  std::string a = R"(
    if(1 < 2) {
      printf("some_function_call");
    }
    if (a == 2) {
      printf("a is 2");
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatement, ElseIf) {
  std::string a = R"(
    if(2 > 1) {
      printf("some_function_call");
    }
    else if (a == 2) {
      printf("a is 2");
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}

TEST(ElseStatement, Else) {
  std::string a = R"(
    if(2 > 1) {
      printf("some_function_call");
    }
    else {
      printf("a is 2");
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}

/*
TEST(IfStatement, IfVarDecls) {
  std::string a = R"(

)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}
*/

TEST(IfStatement, MultipleAlternative) {
  std::string a = R"(
    if(a > 1) {
      printf("some_function_call");
    }
    else if(a == 2) {
      printf("a is 2");
    }
    else if(a == 3) {
      printf("a is 3");
} else {
  printf("else");
}
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}