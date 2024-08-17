#ifndef HELPERS_H
#define HELPERS_H
#include "token.h"

bool is_assignment(donsus_token_kind kind);
bool is_keyword(donsus_token_kind kind);
bool is_qualifier(donsus_token_kind kind);
bool is_class_decl(donsus_token_kind kind);
#endif