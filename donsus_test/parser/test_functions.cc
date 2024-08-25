#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(FuncDef, FuncDefEmptyBody) {
  std::string a = R"(
  def func() comptime -> void {
    }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 0));

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_def->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "void");
  EXPECT_EQ(func_def->get<donsus_ast::function_def>().body.size(), 0);
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);
}

TEST(FuncDef, FuncDefReturn) {
  std::string a = R"(
  def func() comptime -> int {
  return 1;
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_def = result->get_nodes()[0];

  EXPECT_EQ(func_def->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 0));

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
}

TEST(FuncDecl, FuncDeclSpecifiers) {
  std::string a = R"(
  def func() comptime -> int;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_decl = result->get_nodes()[0];

  EXPECT_EQ(func_decl->get<donsus_ast::function_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 0));

  EXPECT_EQ(func_decl->get<donsus_ast::function_def>().func_name, "func");

  EXPECT_EQ(lexeme_value(func_decl->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "int");
  EXPECT_EQ(func_decl->get<donsus_ast::function_def>().body.size(), 0);

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_DEF);
  EXPECT_EQ(func_decl->get<donsus_ast::function_def>().function_type,
            donsus_ast::FunctionType::DECL);
}

TEST(FuncCall, ZeroArgs) {
  std::string a = R"(
  a();
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_call = result->get_nodes()[0];

  EXPECT_EQ(func_call->get<donsus_ast::function_call>().func_name, "a");
  EXPECT_EQ(func_call->get<donsus_ast::function_call>().arguments.size(), 0);

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_CALL);
}

TEST(FuncCall, Args) {
  std::string a = R"(
  a(2);
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto func_call = result->get_nodes()[0];

  EXPECT_EQ(func_call->get<donsus_ast::function_call>().func_name, "a");
  EXPECT_EQ(func_call->get<donsus_ast::function_call>().arguments.size(), 1);

  EXPECT_EQ(lexeme_value(func_call->get<donsus_ast::function_call>()
                             .arguments[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "2");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::FUNCTION_CALL);
}