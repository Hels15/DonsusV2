#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "../../../Include/Internal/type.h"
#include "../../../Include/ast/tree/tree.h"
#include "../src/utility/handle.h"

void print_ast(utility::handle<donsus_ast::tree> tree, std::string &source);
std::string lexeme_value(token token_, const std::string &source);
#endif