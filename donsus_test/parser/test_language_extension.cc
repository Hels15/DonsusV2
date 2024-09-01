#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(LanguageExtension, Simple) {
  std::string a = R"(
    @(Feature1, Feature2, Feature3);
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto language_def = result->get_nodes()[0];
  auto main_type = result->get_nodes()[0]->type;
  EXPECT_EQ(language_def->get<donsus_ast::language_extension>()
                .extensions->get<donsus_ast::tuple>()
                .items.size(),
            3);

  EXPECT_EQ(lexeme_value(language_def->get<donsus_ast::language_extension>()
                             .extensions->get<donsus_ast::tuple>()
                             .items[0]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "Feature1");

  EXPECT_EQ(lexeme_value(language_def->get<donsus_ast::language_extension>()
                             .extensions->get<donsus_ast::tuple>()
                             .items[1]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "Feature2");

  EXPECT_EQ(lexeme_value(language_def->get<donsus_ast::language_extension>()
                             .extensions->get<donsus_ast::tuple>()
                             .items[2]
                             ->get<donsus_ast::expression>()
                             .value,
                         a),
            "Feature3");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::LANGUAGE_EXTENSION);
  EXPECT_EQ(file.error_count, 0);
}