#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(Alias, AliasSimple) {
  std::string a = R"(
    alias newname = char;

  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto alias_def = result->get_nodes()[0];

  EXPECT_EQ(alias_def->get<donsus_ast::alias>()
                .existing_type->get<donsus_ast::identifier>()
                .identifier_name,
            "char");
  EXPECT_EQ(alias_def->get<donsus_ast::alias>()
                .synonym->get<donsus_ast::identifier>()
                .identifier_name,
            "newname");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::ALIAS_STATEMENT);
}