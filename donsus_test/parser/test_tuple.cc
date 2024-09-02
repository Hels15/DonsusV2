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

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::VARIABLE_DEFINITION, main_type.type);
}

/*
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

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::FUNCTION_DEF, main_type.type);
}


TEST(Tuple, NotTuple) {

  std::string a = R"(
  a: (int) = 12; # single tuple
  )";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::FUNCTION_DEF, main_type.type);
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

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::FUNCTION_DEF, main_type.type);
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

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::FUNCTION_DEF, main_type.type);
}

*/
