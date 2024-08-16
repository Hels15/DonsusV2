#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "../src/utility/handle.h"
#include "../src/ast/tree.h"

void print_ast(utility::handle<donsus_ast::tree> tree);
std::string lexeme_value(token token_);
#endif