#ifndef HELPERS_H
#define HELPERS_H
#include "../../../Include/Internal/type.h"
#include "token.h"

bool is_assignment(donsus_token_kind kind);
bool is_keyword(donsus_token_kind kind);
bool is_specifier(donsus_token_kind kind);
bool is_class_specifier(donsus_token_kind kind);
bool is_compare_op(donsus_token_kind kind);
bool is_addition_op(donsus_token_kind kind);
bool is_multi_op(donsus_token_kind kind);
bool is_bitwise_op(donsus_token_kind kind);
bool is_bitshift_op(donsus_token_kind kind);
bool is_as_op(donsus_token_kind kind);
bool is_prefix_op(donsus_token_kind kind);
bool is_member_access_op(donsus_token_kind kind);
TYPE make_type(donsus_token_kind);
#endif