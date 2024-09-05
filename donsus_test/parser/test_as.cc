#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(As, basic) {
  std::string a = R"(
    a: int8 = b as int8;
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto variable_def = result->get_nodes()[0];

  EXPECT_EQ(variable_def->get<donsus_ast::variable_def>()
                .identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "a");

  EXPECT_EQ(
      lexeme_value(
          variable_def->children[0]->get<donsus_ast::expression>().value, a),
      "as");

  // operands[0]
  EXPECT_EQ(lexeme_value(variable_def->children[0]
                             ->children[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "b");

  // operands[0]
  EXPECT_EQ(lexeme_value(variable_def->children[0]
                             ->children[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "int8");
  // operands[1]
  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::VARIABLE_DEFINITION);
}
