#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

TEST(TypeClass, NormalTypeClass) {
  std::string a = R"(
    typeclass Default<T> {
  def default() -> T;
  }
  )";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto typeclass_type = result->get_nodes()[0]->type;
  auto typeclass_def = result->get_nodes()[0];

  auto typeclass_def_body = typeclass_def->get<donsus_ast::typeclass>();
  EXPECT_EQ(typeclass_def_body.identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "Default");

  EXPECT_EQ(typeclass_def_body.body.size(), 1);

  // template decl
  EXPECT_EQ(typeclass_def_body.template_decl->get<donsus_ast::template_decl>()
                .types.size(),
            1);
  EXPECT_EQ(typeclass_def_body.template_decl->get<donsus_ast::template_decl>()
                .types[0]
                ->get<donsus_ast::identifier>()
                .identifier_name,

            "T");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::TYPECLASS, typeclass_type.type);
}

TEST(TypeClass, Instance) {
  std::string a = R"(
  instance Default int {
  def default() -> int {
      return 0;
  }
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto instance_type = result->get_nodes()[0]->type;
  auto instance_def = result->get_nodes()[0];

  auto instance_def_body = instance_def->get<donsus_ast::instance>();

  EXPECT_EQ(instance_def_body.identifier_name->get<donsus_ast::identifier>()
                .identifier_name,
            "Default");

  EXPECT_EQ(instance_def_body.body.size(), 1);
  EXPECT_EQ(
      instance_def_body.type->get<donsus_ast::identifier>().identifier_name,
      "int");

  EXPECT_EQ(file.error_count, 0);
  EXPECT_EQ(donsus_ast::donsus_node_type::INSTANCE, instance_type.type);
}
