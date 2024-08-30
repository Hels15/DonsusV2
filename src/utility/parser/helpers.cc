#include "helpers.h"

bool is_assignment(donsus_token_kind kind) {
  return (kind == donsus_token_kind::PLUS_EQUAL ||
          kind == donsus_token_kind::MINUS_EQUAL ||
          kind == donsus_token_kind::SLASH_EQUAL ||
          kind == donsus_token_kind::STAR_EQUAL ||
          kind == donsus_token_kind::EQUAL || kind == donsus_token_kind::LSQB ||
          kind == donsus_token_kind::AMPERSAND_EQUAL ||
          kind == donsus_token_kind::PIPE_EQUAL ||
          kind == donsus_token_kind::CARET_EQUAL ||
          kind == donsus_token_kind::LARROW2_EQUAL ||
          kind == donsus_token_kind::RARROW2_EQUAL);
}

bool is_compare_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::LESS ||
          kind == donsus_token_kind::GREATER ||
          kind == donsus_token_kind::LESS_EQUAL ||
          kind == donsus_token_kind::GREATER_EQUAL ||kind == donsus_token_kind::DOUBLE_EQUAL);
}
bool is_specifier(donsus_token_kind kind) {
  return (kind == donsus_token_kind::COMPTIME_KW ||
          kind == donsus_token_kind::MUT_KW ||
          kind == donsus_token_kind::THREAD_LOCAL_KW ||
          kind == donsus_token_kind::STATIC_KW ||
          kind == donsus_token_kind::PRIVATE_KW);
}

bool is_class_specifier(donsus_token_kind kind) {
  return (kind == donsus_token_kind::FINAL_KW ||
          kind == donsus_token_kind::ABSTRACT_KW);
}

bool is_addition_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::PLUS || kind == donsus_token_kind::MINUS);
}
bool is_multi_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::PERCENT ||
          kind == donsus_token_kind::STAR || kind == donsus_token_kind::SLASH);
}

bool is_bitwise_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::AMPERSAND ||
          kind == donsus_token_kind::PIPE || kind == donsus_token_kind::CARET);
}

bool is_bitshift_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::LARROW2 ||
          kind == donsus_token_kind::RARROW2);
}

bool is_as_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::AS_KW);
}
bool is_prefix_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::MINUS ||
          kind == donsus_token_kind::CARET ||
          kind == donsus_token_kind::AMPERSAND ||
          kind == donsus_token_kind::EXCLAMATION ||
          kind == donsus_token_kind::STAR || kind == donsus_token_kind::TILDE);
}

bool is_member_access_op(donsus_token_kind kind) {
  return (kind == donsus_token_kind::DOT);
}
TYPE make_type(donsus_token_kind type) {
  TYPE a;
  a.type_un = a.from_parse(type);
  return a;
}