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
  EXPECT_EQ(main_type.type, donsus_ast::donsus_node_type::CLASS);
}
/*

TEST(Class, Final){


}

TEST(Class, Abstract){


}

TEST(Class, AbstractFinal){


}


TEST(Class, Constructor){

}


TEST(Class, Encapsulation){


}


TEST(Class, Generics){


}

TEST(Class, Encapsulation){


}


TEST(Class, Objects){


}

*/
