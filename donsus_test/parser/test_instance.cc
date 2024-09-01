#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Instance, SingleType) {
  std::string a = R"(
instance Test MyClass{
    def isEqual(a: MyClass, b:MyClass) -> bool {
        return a == b;
    }
}
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto instance_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;

  EXPECT_EQ(instance_def->get<donsus_ast::instance>()
                .identifier->get<donsus_ast::identifier>()
                .identifier_name,
            "Test");

  EXPECT_EQ(instance_def->get<donsus_ast::instance>()
                .type->get<donsus_ast::identifier>()
                .identifier_name,
            "MyClass");

  EXPECT_EQ(instance_def->get<donsus_ast::instance>().body.size(), 1);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::INSTANCE);
  EXPECT_EQ(file.error_count, 0);
}
