#ifndef TYPE_H
#define TYPE_H
#include "../token.h"

#define TYPES                                                           \
  X(UNKNOWN, "UNKNOWN")                                                      \
  X(UNSPECIFIED_INTEGER, "UNSPECIFIED_INTEGER")                              \
  X(BASIC_INT, "BASIC_INT")                                                  \
  X(I32, "I32")                                                              \
  X(U64, "U64")                                                              \
  X(STRING, "STRING")                                                        \
  X(I8, "I8")                                                                \
  X(I64, "I64")                                                              \
  X(BOOL, "BOOL")                                                            \
  X(I16, "I16")                                                              \
  X(U32, "U32")                                                              \
  X(F32, "F32")                                                              \
  X(F64, "F64")                                                              \
  X(VOID, "VOID")                                                            \
  X(FIXED_ARRAY, "FIXED_ARRAY")                                              \
  X(STATIC_ARRAY, "STATIC_ARRAY")                                            \
  X(DYNAMIC_ARRAY, "DYNAMIC_ARRAY")

class TYPE {
public:
  enum kind : int {
#define X(name, str) name,
    TYPES
#undef X
  };
  kind from_parse(donsus_token_kind type);
  donsus_token_kind to_parse(kind type);

  kind type_un{};

  bool operator==(const TYPE &rhs) const { return type_un == rhs.type_un; }
  bool operator!=(const TYPE &rhs) const { return !(rhs == *this); }
  [[nodiscard]] auto to_string() const -> std::string;
  [[nodiscard]] static auto to_string(TYPE::kind type_) -> std::string;

  [[nodiscard]] auto is_integer() const -> bool;
  [[nodiscard]] auto is_float() const -> bool;
  [[nodiscard]] auto is_bool() const -> bool;
  [[nodiscard]] auto is_string() const -> bool;
};

#endif