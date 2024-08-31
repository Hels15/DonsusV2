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

  auto if_def_1 = result->get_nodes()[0];
  auto main_type_1 = result->get_nodes()[0]->type;

  auto if_def_2 = result->get_nodes()[1];
  auto main_type_2 = result->get_nodes()[1]->type;

  // condition(0)
  EXPECT_EQ(lexeme_value(if_def_1->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            "<");

  // body(0)
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().body.size(), 1);
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  // if var_decls(0)
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().if_var_decls.size(), 0);

  // alternates(0)
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().alternate.size(), 0);

  // condition(1)
  EXPECT_EQ(lexeme_value(if_def_2->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            "==");

  // body(1)
  EXPECT_EQ(if_def_2->get<donsus_ast::if_statement>().body.size(), 1);
  EXPECT_EQ(if_def_2->get<donsus_ast::if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");
  // if var_decls(1)
  EXPECT_EQ(if_def_2->get<donsus_ast::if_statement>().if_var_decls.size(), 0);
  // alternates(1)
  EXPECT_EQ(if_def_2->get<donsus_ast::if_statement>().alternate.size(), 0);

  EXPECT_EQ(main_type_1.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(main_type_2.type, donsus_ast::donsus_node_type::IF_STATEMENT);

  EXPECT_EQ(file.error_count, 0);
}

TEST(IfStatement, ElseIf) {
  std::string a = R"(
    if(1 < 2) {
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

  auto if_def_1 = result->get_nodes()[0];
  auto main_type_1 = result->get_nodes()[0]->type;

  EXPECT_EQ(lexeme_value(if_def_1->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            "<");

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().body.size(), 1);
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  // if var_decls(0)
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().if_var_decls.size(), 0);
  // alternate
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().alternate[0]->type.type,
            donsus_ast::donsus_node_type::ELSE_IF_STATEMENT);

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .alternate[0]
                ->get<donsus_ast::else_if_statement>()
                .body.size(),
            1);

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .alternate[0]
                ->get<donsus_ast::else_if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().alternate.size(), 1);

  EXPECT_EQ(main_type_1.type, donsus_ast::donsus_node_type::IF_STATEMENT);
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

  auto if_def_1 = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(lexeme_value(if_def_1->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            ">");

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().body.size(), 1);

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  // if var_decls(0)
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().if_var_decls.size(), 0);

  // alternate
  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().alternate[0]->type.type,
            donsus_ast::donsus_node_type::ELSE_STATEMENT);

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .alternate[0]
                ->get<donsus_ast::else_statement>()
                .body.size(),
            1);

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>()
                .alternate[0]
                ->get<donsus_ast::else_statement>()
                .body[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "printf");

  EXPECT_EQ(if_def_1->get<donsus_ast::if_statement>().alternate.size(), 1);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}

/*// Todo: test this out

TEST(IfStatement, IfVarDecls) {
  std::string a = R"(
  if(a: int = Func1(); a > b) {
  printf("a is greater than b");
}
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[0]
                ->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[0]
                ->children[0]
                ->get<donsus_ast::function_call>()
                .func_name,
            "Func1");

  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[0]
                ->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);
  EXPECT_EQ(file.error_count, 0);
}*/

/*TEST(IfStatement, MultipleIfVarDecls) {
  // Todo: Multiple types here?
  // TOdo: need to allow a: int = 12, b = 12;
  // first

  std::string a = R"(
    if(a: int = Func1(), b = Func2(); a > b) {
  printf("a is greater than b");
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[0]
                ->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[0]
                ->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[1]
                ->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "b");

  EXPECT_EQ(if_def->get<donsus_ast::if_statement>()
                .if_var_decls[1]
                ->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

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
}  else {
   printf("else");
}
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto if_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(lexeme_value(if_def->get<donsus_ast::if_statement>()
                             .condition->get<donsus_ast::expression>()
                             .value,
                         a),
            ">");
  auto else_if_1 = if_def->get<donsus_ast::if_statement>()
                       .alternate[0]
                       ->get<donsus_ast::else_if_statement>();

  auto else_if_2 = if_def->get<donsus_ast::if_statement>()
                       .alternate[1]
                       ->get<donsus_ast::else_if_statement>();
  auto else_3 = if_def->get<donsus_ast::if_statement>()
                    .alternate[2]
                    ->get<donsus_ast::else_statement>();

  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().alternate[0]->type.type,
            donsus_ast::donsus_node_type::ELSE_IF_STATEMENT);
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().alternate[1]->type.type,
            donsus_ast::donsus_node_type::ELSE_IF_STATEMENT);
  EXPECT_EQ(if_def->get<donsus_ast::if_statement>().alternate[2]->type.type,
            donsus_ast::donsus_node_type::ELSE_STATEMENT);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::IF_STATEMENT);

  EXPECT_EQ(file.error_count, 0);
}