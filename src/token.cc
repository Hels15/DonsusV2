#include "../Include/token.h"

const char *token::type_name() const { return token::type_name(kind); }
const char *token::type_name(donsus_token_kind kind) {  switch (kind) {
#define X(name, text)                                                          \
  case donsus_token_kind::name:                                                        \
    return #name;
    TOKEN_TYPES
#undef X
  }
}