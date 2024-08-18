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

bool is_specifier(donsus_token_kind kind) {
  return (kind == donsus_token_kind::COMPTIME_KW ||
          kind == donsus_token_kind::MUT_KW ||
          kind == donsus_token_kind::THREAD_LOCAL_KW ||
          kind == donsus_token_kind::STATIC_KW);
}

bool is_class_decl(donsus_token_kind kind) {
  return (kind == donsus_token_kind::FINAL_KW ||
          kind == donsus_token_kind::ABSTRACT_KW);
}

TYPE make_type(donsus_token_kind type) {
  TYPE a;
  a.type_un = a.from_parse(type);
  return a;
}