#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "../src/ast/tree.h"
#include "../src/utility/handle.h"
#include "../../../Include/Internal/type.h"

void print_ast(utility::handle<donsus_ast::tree> tree, std::string &source);
std::string lexeme_value(token token_, std::string &source);
#endif