#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Tuple, AsVarManual) {

  std::string a = R"(
    a:(int, int, int ,int) = (1, 2, 3, 4);
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // tuple def
  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto tuple_type = var_def->get<donsus_ast::variable_def>()
                        .identifier_type->get<donsus_ast::tuple>();

  EXPECT_EQ(tuple_def_body.items.size(), 4);
  EXPECT_EQ(tuple_type.items.size(), 4);

  // int
  EXPECT_EQ(
      lexeme_value(tuple_type.items[0]->get<donsus_ast::expression>().value, a),
      "int");

  // 1
  EXPECT_EQ(
      lexeme_value(tuple_def_body.items[0]->get<donsus_ast::expression>().value,
                   a),
      "1");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}

TEST(Tuple, AllAuto) {

  std::string a = R"(
    a:(auto, auto, auto) = (1,1.2, "String");
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // tuple def
  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto tuple_type = var_def->get<donsus_ast::variable_def>()
                        .identifier_type->get<donsus_ast::tuple>();

  EXPECT_EQ(tuple_def_body.items.size(), 3);
  EXPECT_EQ(tuple_type.items.size(), 3);

  // int
  EXPECT_EQ(
      lexeme_value(tuple_type.items[0]->get<donsus_ast::expression>().value, a),
      "auto");
  EXPECT_EQ(
      lexeme_value(tuple_def_body.items[0]->get<donsus_ast::expression>().value,
                   a),
      "1");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}

TEST(Tuple, JustAuto) {
  std::string a = R"(
    a:auto = (1, 1.2, "String");
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // tuple def
  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto auto_type = var_def->get<donsus_ast::variable_def>()
                       .identifier_type->get<donsus_ast::identifier>();

  EXPECT_EQ(tuple_def_body.items.size(), 3);

  EXPECT_EQ(auto_type.identifier_name, "auto");
  EXPECT_EQ(
      lexeme_value(tuple_def_body.items[0]->get<donsus_ast::expression>().value,
                   a),
      "1");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}
TEST(Tuple, InFunction) {

  std::string a = R"(
   def a(a: (int, int, int)) -> int{
   return 1;
  }
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto function_body_def = var_def->get<donsus_ast::function_def>();
  // parameters
  auto param_declaration =
      function_body_def.parameters[0]->get<donsus_ast::param_decl>();

  EXPECT_EQ(function_body_def.parameters.size(), 1);
  EXPECT_EQ(
      param_declaration.identifier_type->get<donsus_ast::tuple>().items.size(),
      3);

  EXPECT_EQ(
      lexeme_value(param_declaration.identifier_type->get<donsus_ast::tuple>()
                       .items[0]
                       ->get<donsus_ast::expression>()
                       .value,
                   a),
      "int");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::FUNCTION_DEF, main_type.type);
}

TEST(Tuple, NotTuple) {

  std::string a = R"(
  a: (int) = 12; # not a tuple
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  // type
  auto var_def_body = var_def->get<donsus_ast::variable_def>();
  auto type_ = var_def_body.identifier_type->get<donsus_ast::expression>();
  auto type_expr = var_def->children[0]->get<donsus_ast::expression>();

  EXPECT_EQ(lexeme_value(type_.value, a), "int");
  EXPECT_EQ(lexeme_value(type_expr.value, a), "12");

  // (int)
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}
TEST(Tuple, SingleElementTuple) {
  std::string a = R"(
  a: (int,) = (1,);
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto tuple_type = var_def->get<donsus_ast::variable_def>()
                        .identifier_type->get<donsus_ast::tuple>();

  EXPECT_EQ(tuple_def_body.items.size(), 1);
  EXPECT_EQ(tuple_type.items.size(), 1);

  EXPECT_EQ(
      lexeme_value(tuple_def_body.items[0]->get<donsus_ast::expression>().value,
                   a),
      "1");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}

TEST(Tuple, SingleWSpecifiers) {

  std::string a = R"(
  let mut a: (int,) = (1,); # single tuple
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto tuple_type = var_def->get<donsus_ast::variable_def>()
                        .identifier_type->get<donsus_ast::tuple>();

  EXPECT_EQ(tuple_def_body.items.size(), 1);
  EXPECT_EQ(tuple_type.items.size(), 1);

  // mut
  EXPECT_EQ(var_def->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 3));

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}

TEST(Tuple, TrailingComma) {

  std::string a = R"(
  let mut a: (int,int) = (1,2,); # trailing comma at the end
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  auto var_def_body = var_def->get<donsus_ast::variable_def>();

  auto tuple_def_body = var_def->children[0]->get<donsus_ast::tuple>();

  // tuple type
  auto tuple_type = var_def->get<donsus_ast::variable_def>()
                        .identifier_type->get<donsus_ast::tuple>();

  EXPECT_EQ(tuple_def_body.items.size(), 2);
  EXPECT_EQ(tuple_type.items.size(), 2);

  EXPECT_EQ(var_def->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 3));

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}
