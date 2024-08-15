#ifndef TYPE_H
#define TYPE_H
#include "../token.h"

class TYPE {
public:
  enum kind : int {
    TYPE_UNKNOWN = 0,         // handled in type checker
    TYPE_UNSPECIFIED_INTEGER, // literal without specific type
    TYPE_BASIC_INT,
    TYPE_I32,
    TYPE_U64,
    TYPE_STRING,
    TYPE_I8,
    TYPE_I64,
    TYPE_BOOL,
    TYPE_I16,
    TYPE_U32,
    TYPE_F32,
    TYPE_F64,
    TYPE_VOID,
    // arrays
    TYPE_FIXED_ARRAY,
    TYPE_STATIC_ARRAY,
    TYPE_DYNAMIC_ARRAY
  };
  kind from_parse(donsus_token_kind type);
  donsus_token_kind to_parse(kind type);

  kind type_un{};

  bool operator==(const TYPE &rhs) const {return type_un == rhs.type_un;}
  bool operator!=(TYPE &rhs) const { return rhs != *this; }
  [[nodiscard]] auto to_string() const -> std::string;
  [[nodiscard]] auto is_integer() const -> bool;
  [[nodiscard]] auto is_float() const -> bool;
  [[nodiscard]] auto is_bool() const -> bool;
  [[nodiscard]] auto is_string() const -> bool;
};

#endif