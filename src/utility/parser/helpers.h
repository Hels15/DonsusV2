#ifndef HELPERS_H
#define HELPERS_H
#include "../../../Include/Internal/type.h"
#include "token.h"

bool is_assignment(donsus_token_kind kind);
bool is_keyword(donsus_token_kind kind);
bool is_specifier(donsus_token_kind kind);
bool is_class_specifier(donsus_token_kind kind);
TYPE make_type(donsus_token_kind);
#endif