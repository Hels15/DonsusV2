#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(FuncDef, FuncDefWithParams) {
  std::string a = R"(
  def func(a: int) -> int {
    return a;
})";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body[0]->type.type,
            donsus_ast::donsus_node_type::RETURN_STATEMENT);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 1);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(
      func_def->get<donsus_ast::function_def>().parameters[0]->children.size(),
      0);
}

TEST(FuncDef, FuncDefWithPointer) {
  std::string a = R"(
  def func(a: int*) -> int {
    return a;
})";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body[0]->type.type,
            donsus_ast::donsus_node_type::RETURN_STATEMENT);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 1);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::pointer>()
                .pointee->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(
      func_def->get<donsus_ast::function_def>().parameters[0]->children.size(),
      0);
}

TEST(FuncDef, FuncDefWithReference) {
  std::string a = R"(
  def func(a: int&) -> int {
    return a;
})";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body[0]->type.type,
            donsus_ast::donsus_node_type::RETURN_STATEMENT);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 1);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::reference>()
                .referent->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(
      func_def->get<donsus_ast::function_def>().parameters[0]->children.size(),
      0);
}

// Todo: gonna get supported later but not sure
// E.g rust doesn't support it
/*
TEST(FuncDef, FuncDefDefaultParam) {
  std::string a = R"(
  def func(a: int) -> int {
    return a;
})";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body[0]->type.type,
            donsus_ast::donsus_node_type::RETURN_STATEMENT);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 1);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().parameters[0]->children.size(),
0);

}
*/

TEST(FuncDef, FuncDefMultiParams) {
  std::string a = R"(
  def func(a: int&, b: int) -> int {
    return a;
})";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body[0]->type.type,
            donsus_ast::donsus_node_type::RETURN_STATEMENT);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 1);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[0]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::reference>()
                .referent->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[1]
                ->get<donsus_ast::param_decl>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "b");

  EXPECT_EQ(func_def->get<donsus_ast::function_def>()
                .parameters[1]
                ->get<donsus_ast::param_decl>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(
      func_def->get<donsus_ast::function_def>().parameters[0]->children.size(),
      0);
}