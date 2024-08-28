#include "parser.h"
#include <iostream>
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Vardef, VarDefWithLiteral) {
  std::string a = R"(
    a: int = 12;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  std::cout << "got here";
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");


  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(lexeme_value(
                var_def->children[0]->get<donsus_ast::expression>().value, a),
            "12");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Vardef, VarDefWithExpression) {
  std::string a = R"(
    a: int = 12 * 2;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(lexeme_value(
                var_def->children[0]->get<donsus_ast::expression>().value, a),
            "*");

  EXPECT_EQ(lexeme_value(var_def->children[0]
                             ->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "12");

  EXPECT_EQ(lexeme_value(var_def->children[0]
                             ->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "2");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);

  EXPECT_EQ(file.error_count, 0);
}

TEST(vardef, VarDefWithSpecifiers) {
  std::string a = R"(
    let mut comptime a:int = 12;
    )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // mut | comptime
  EXPECT_EQ(var_def->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>((1 << 0) | (1 << 3)));
  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(lexeme_value(
                var_def->children[0]->get<donsus_ast::expression>().value, a),
            "12");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}

TEST(vardef, VarDefWithPointer) {
  std::string a = R"(
    a:int* = 12;
    )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // mut | comptime
  EXPECT_EQ(var_def->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::pointer>()
                .pointee->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(lexeme_value(
                var_def->children[0]->get<donsus_ast::expression>().value, a),
            "12");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}

TEST(vardef, VarDefWithReference) {
  std::string a = R"(
    a:int& = 12;
    )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // mut | comptime
  EXPECT_EQ(var_def->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(0));

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>()
                .identifier_type->get<donsus_ast::reference>()
                .referent->get<donsus_ast::identifier>()
                .identifier_name,
            "int");

  EXPECT_EQ(lexeme_value(
                var_def->children[0]->get<donsus_ast::expression>().value, a),
            "12");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}
