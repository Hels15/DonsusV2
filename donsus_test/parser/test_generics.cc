#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>
#include <iostream>

/*

TEST(Generics, Unconstrained)  {
  std::string a = R"(
    test :: T
    def test() -> T {

    }
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  EXPECT_EQ(var_def->get<donsus_ast::generics_decl>().name->get<donsus_ast::identifier>().identifier_name,
"test");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Generics, Constrained){
  std::string a = R"(
    test2 :: Num A => A
    def test2(a: A, b:A) -> A {
      return a + b;
   }
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;


  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
  EXPECT_EQ(file.error_count, 0);
}*/
