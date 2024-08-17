#include "type.h"

/*
 * \brief convert donsus_token_kind to
 * actual TYPE.
 * */
TYPE::kind TYPE::from_parse(donsus_token_kind type) {
  switch (type) {
  case donsus_token_kind::BASIC_INT:
    return TYPE::BASIC_INT;

  case donsus_token_kind::I32:
    return TYPE::I32;

  case donsus_token_kind::U64:
    return TYPE::U64;

  case donsus_token_kind::STRING:
    return TYPE::STRING;

  case donsus_token_kind::I8:
    return TYPE::I8;

  case donsus_token_kind::I64:
    return TYPE::I64;

  case donsus_token_kind::BOOL:
    return TYPE::BOOL;

  case donsus_token_kind::I16:
    return TYPE::I16;

  case donsus_token_kind::U32:
    return TYPE::U32;

  case donsus_token_kind::F32:
    return TYPE::F32;

  case donsus_token_kind::F64:
    return TYPE::F64;

  case donsus_token_kind::VOID:
    return TYPE::VOID;

  default: {
    return TYPE::UNKNOWN;
  }
  }
}

/*
 * Convert Type to token kind
 *
 * */

donsus_token_kind TYPE::to_parse(TYPE::kind type) {
  switch (type) {
  case TYPE::BASIC_INT:
    return donsus_token_kind::BASIC_INT;
  case TYPE::I32:
    return donsus_token_kind::I32;
  case TYPE::U64:
    return donsus_token_kind::U64;
  case TYPE::STRING:
    return donsus_token_kind::STRING;
  case TYPE::I8:
    return donsus_token_kind::I8;
  case TYPE::I64:
    return donsus_token_kind::I64;
  case TYPE::BOOL:
    return donsus_token_kind::BOOL;
  case TYPE::I16:
    return donsus_token_kind::I16;
  case TYPE::U32:
    return donsus_token_kind::U32;
  case TYPE::F32:
    return donsus_token_kind::F32;
  case TYPE::F64:
    return donsus_token_kind::F64;
  case TYPE::VOID:
    return donsus_token_kind::VOID;
  default: {
    std::cerr
        << "unhandled type in to_parse from TYPE::kind to donsus_token_kind";
  }
  }
}

[[nodiscard]] auto TYPE::to_string(TYPE::kind type_) -> std::string {
  switch (type_) {
#define X(name, text) case kind::name: \
    return #name;
    TYPES
#undef X
  }
}

[[nodiscard]] auto TYPE::to_string() const -> std::string{
  return to_string(type_un);
}