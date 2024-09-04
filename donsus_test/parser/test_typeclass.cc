#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(TypeClass, NormalTypeClass) {
  std::string a = R"(
    typeclass Default<T> {
  def default() -> T;
  }
  )";
}
TEST(TypeClass, Instance) {
  std::string a R"(
  instance Default int {
  def default() -> int {
      return 0;
  }
}
)"
}