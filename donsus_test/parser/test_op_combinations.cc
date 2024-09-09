#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(OpCombinations, PlusMinus) {
  std::string a = R"(
    a: int = 12+-1;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();
  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
}

TEST(OpCombinations, MinusPlus) {
  std::string a = R"(
    a: int = 12-+1;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();
  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
}

TEST(OpCombinations, MinusWParenMinus) {
  std::string a = R"(
    a: int = 12+(-1);
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();
  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
}

TEST(OpCombinations, PlusWParenPlus) {
  std::string a = R"(
    a: int = 12-(+1);
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();
  auto var_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
}
