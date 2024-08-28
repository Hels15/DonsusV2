#include "parser.h"
#include "print_ast.h"
#include <gtest/gtest.h>

// Todo: continue from here
TEST(Class, ClassDecl) {
  std::string a = R"(
    class Obj {
  }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.name->get<donsus_ast::identifier>().identifier_name,
            "Obj");
  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(class_def.inherits.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Class, Final) {
  std::string a = R"(
    class Obj final{
  }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.name->get<donsus_ast::identifier>().identifier_name,
            "Obj");

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(1 << 1));

  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(class_def.inherits.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Class, Abstract) {
  std::string a = R"(
    class Obj abstract {

  }
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.name->get<donsus_ast::identifier>().identifier_name,
            "Obj");

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(1 << 0));

  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(class_def.inherits.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Class, AbstractFinal) {
  std::string a = R"(
    class Obj abstract final{

  }
)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.name->get<donsus_ast::identifier>().identifier_name,
            "Obj");

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>((1 << 0) | (1 << 1)));

  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(class_def.inherits.size(), 0);

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Class, Inherits) {
  std::string a = R"(
    class Obj: Test{

  }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.name->get<donsus_ast::identifier>().identifier_name,
            "Obj");

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(0));

  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(
      class_def.inherits[0]->get<donsus_ast::identifier>().identifier_name,
      "Test");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

//
/*TEST(Class, InheritsFromGenerics) {
  std::string a = R"(
    class Obj: Test<T>{

  }
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(0));

  EXPECT_EQ(class_def.body.size(), 0);
  EXPECT_EQ(class_def.inherits.size(), 1);

  // Todo: Add opportunity to inherit from generics
  *//* EXPECT_EQ(class_def.inherits[0]->get<donsus_ast::identifier>().identifier_name,
   * "Test");*//*

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}*/
TEST(Class, Constructor) {
  std::string a = R"(
    class A final{
def A() -> Self{
# just something here
return self;
}
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(1 << 1));

  EXPECT_EQ(class_def.body.size(), 1);
  EXPECT_EQ(class_def.inherits.size(), 0);

  // Constructor as a regular function
  EXPECT_EQ(class_def.body[0]->get<donsus_ast::function_def>().func_name, "A");

  EXPECT_EQ(class_def.body[0]->get<donsus_ast::function_def>().function_type,
            donsus_ast::FunctionType::DEF);
  EXPECT_EQ(lexeme_value(class_def.body[0]
                             ->get<donsus_ast::function_def>()
                             .return_type->get<donsus_ast::expression>()
                             .value,
                         a),
            "Self");
  EXPECT_EQ(
      class_def.body[0]->get<donsus_ast::function_def>().parameters.size(), 0);

  // Constructor body
  EXPECT_EQ(lexeme_value(class_def.body[0]
                             ->get<donsus_ast::function_def>()
                             .body[0]
                             ->get<donsus_ast::return_statement>()
                             .body->get<donsus_ast::expression>()
                             .value,
                         a),
            "self");

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

TEST(Class, Encapsulation) {
  std::string a = R"(
    class A final{
    let private a: int = 12;
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto main_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(1 << 1));

  // encapsulated private variable
  EXPECT_EQ(class_def.body[0]->get<donsus_ast::variable_def>().specifiers,
            static_cast<donsus_ast::specifiers_>(1 << 4));

  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
  EXPECT_EQ(file.error_count, 0);
}

// Todo: Think about this again
/*TEST(Class, Generics){
  std::string a = R"(
    A :: T
    class A final{
    let private a: T = 12;
}
)";
  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto generics_type = result->get_nodes()[0]->type;
  auto generics_def = result->get_nodes()[0]->get<donsus_ast::generics_decl>();

  auto class_type = result->get_nodes()[1]->type;
  auto class_def = result->get_nodes()[1]->get<donsus_ast::class_def>();

  EXPECT_EQ(class_def.specifiers,
            static_cast<donsus_ast::specifiers_class_>(1 << 1));

  // generics
  EXPECT_EQ(generics_def.type_of_decl, donsus_ast::generics_type::PARAMETRIC);
  EXPECT_EQ(generics_def.name, 'A');

  EXPECT_EQ(generics_type.type, donsus_ast::donsus_node_type::GENERICS_DECL);
  EXPECT_EQ(class_type.type, donsus_ast::donsus_node_type::CLASS);
}*/

TEST(Class, Objects) {
  std::string a = R"(
class A final{
  def A() -> Self{
    # just something here
    return self;
  }
}
my_instance: A = A();

)";

  AstFile file;
  file.source = a;
  Parser parser = Du_Parse(a, file);
  Parser::end_result result = parser.parse();

  auto class_type = result->get_nodes()[0]->type;
  auto class_def = result->get_nodes()[0]->get<donsus_ast::class_def>();

  // instance
  auto instance_type = result->get_nodes()[1]->type;
  auto instance_def = result->get_nodes()[1];

  EXPECT_EQ(
      instance_def->children[0]->get<donsus_ast::function_call>().func_name,
      "A");

  EXPECT_EQ(class_type.type, donsus_ast::donsus_node_type::CLASS);
}
