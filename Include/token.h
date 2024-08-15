#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>

#define DONSUS_TOKEN_TYPES                                                     \
  X(DONSUS_UNKNOWN, "DONSUS_UNKNOWN")                                          \
  X(DONSUS_IDENTIFIER, "DONSUS_IDENTIFIER")                                    \
  X(DONSUS_END, "DONSUS_END")                                                  \
  X(DONSUS_INT, "DONSUS_INT")                                                  \
  X(DONSUS_FLOAT, "DONSUS_FLOAT")                                              \
  X(DONSUS_BIN, "DONSUS_BIN")                                                  \
  X(DONSUS_OCT, "DONSUS_OCT")                                                  \
  X(DONSUS_HEX, "DONSUS_HEX")                                                  \
  X(DONSUS_STRING, "DONSUS_STRING")                                            \
  X(DONSUS_NEWLINE, "DONSUS_NEWLINE")                                          \
  X(DONSUS_AMPERSAND, "DONSUS_AMPERSAND")                                      \
  X(DONSUS_AMPERSAND_EQUAL, "DONSUS_AMPERSAND_EQUAL")                          \
  X(DONSUS_PIPE, "DONSUS_PIPE")                                                \
  X(DONSUS_PIPE_EQUAL, "DONSUS_PIPE_EQUAL")                                    \
  X(DONSUS_CARET, "DONSUS_CARET")                                              \
  X(DONSUS_CARET_EQUAL, "DONSUS_CARET_EQUAL")                                  \
  X(DONSUS_LARROW2, "DONSUS_LARROW2")                                          \
  X(DONSUS_LARROW2_EQUAL, "DONSUS_LARROW2_EQUAL")                              \
  X(DONSUS_RARROW2, "DONSUS_RARROW2")                                          \
  X(DONSUS_RARROW2_EQUAL, "DONSUS_RARROW2_EQUAL")                              \
  X(DONSUS_LPAR, "DONSUS_LPAR")                                                \
  X(DONSUS_RPAR, "DONSUS_RPAR")                                                \
  X(DONSUS_LSQB, "DONSUS_LSQB")                                                \
  X(DONSUS_RSQB, "DONSUS_RSQB")                                                \
  X(DONSUS_COLO, "DONSUS_COLO")                                                \
  X(DONSUS_COMM, "DONSUS_COMM")                                                \
  X(DONSUS_SEMICOLON, "DONSUS_SEMICOLON")                                      \
  X(DONSUS_PLUS, "DONSUS_PLUS")                                                \
  X(DONSUS_PLUS_EQUAL, "DONSUS_PLUS_EQUAL")                                    \
  X(DONSUS_MINUS, "DONSUS_MINUS")                                              \
  X(DONSUS_MINUS_EQUAL, "DONSUS_MINUS_EQUAL")                                  \
  X(DONSUS_STAR, "DONSUS_STAR")                                                \
  X(DONSUS_STAR_EQUAL, "DONSUS_STAR_EQUAL")                                    \
  X(DONSUS_BACK_SLASH, "DONSUS_BACK_SLASH")                                    \
  X(DONSUS_SLASH, "DONSUS_SLASH")                                              \
  X(DONSUS_SLASH_EQUAL, "DONSUS_SLASH_EQUAL")                                  \
  X(DONSUS_LESS, "DONSUS_LESS")                                                \
  X(DONSUS_LESS_EQUAL, "DONSUS_LESS_EQUAL")                                    \
  X(DONSUS_GREATER, "DONSUS_GREATER")                                          \
  X(DONSUS_GREATER_EQUAL, "DONSUS_GREATER_EQUAL")                              \
  X(DONSUS_EQUAL, "DONSUS_EQUAL")                                              \
  X(DONSUS_DOUBLE_EQUAL, "DONSUS_DOUBLE_EQUAL")                                \
  X(DONSUS_NOT_EQUAL, "DONSUS_NOT_EQUAL")                                      \
  X(DONSUS_DOT, "DONSUS_DOT")                                                  \
  X(DONSUS_DOUBLE_DOT, "DONSUS_DOUBLE_DOT")                                    \
  X(DONSUS_PERCENT, "DONSUS_PERCENT")                                          \
  X(DONSUS_LBRACE, "DONSUS_LBRACE")                                            \
  X(DONSUS_RBRACE, "DONSUS_RBRACE")                                            \
  X(DONSUS_EXCLAMATION, "DONSUS_EXCLAMATION")                                  \
  X(DONSUS_COMMENT, "DONSUS_COMMENT")                                          \
  X(DONSUS_SINGLE_QUOTE, "DONSUS_SINGLE_QUOTE")                                \
  X(DONSUS_THREE_DOTS, "DONSUS_THREE_DOTS")                                    \
  X(DONSUS_TWO_DOTS, "DONSUS_TWO_DOTS")                                        \
  X(DONSUS_NULL_VALUE, "DONSUS_NULL_VALUE")                                    \
  X(DONSUS_BASIC_INT, "DONSUS_BASIC_INT")                                      \
  X(DONSUS_I8, "DONSUS_I8")                                                    \
  X(DONSUS_I16, "DONSUS_I16")                                                  \
  X(DONSUS_I32, "DONSUS_I32")                                                  \
  X(DONSUS_I64, "DONSUS_I64")                                                  \
  X(DONSUS_U32, "DONSUS_U32")                                                  \
  X(DONSUS_U64, "DONSUS_U64")                                                  \
  X(DONSUS_UNDERSCORE, "DONSUS_UNDERSCORE")                                    \
  X(DONSUS_F32, "DONSUS_F32")                                                  \
  X(DONSUS_F64, "DONSUS_F64")                                                  \
  X(DONSUS_BOOL, "DONSUS_BOOL")                                                \
  X(DONSUS_VOID, "DONSUS_VOID")                                                \
  X(DONSUS_ARROW, "DONSUS_ARROW")                                              \
  X(DONSUS_STRING_TYPE, "DONSUS_STRING_TYPE")                                  \
  X(DONSUS_CHAR, "DONSUS_CHAR")                                                \
  X(DONSUS_FUNCTION_DEFINITION_KW, "DONSUS_FUNCTION_DEFINITION_KW")            \
  X(DONSUS_IF_KW, "DONSUS_IF_KW")                                              \
  X(DONSUS_ELIF_KW, "DONSUS_ELIF_KW")                                          \
  X(DONSUS_ELSE_KW, "DONSUS_ELSE_KW")                                          \
  X(DONSUS_RETURN_KW, "DONSUS_RETURN_KW")                                      \
  X(DONSUS_PRINT_KW, "DONSUS_PRINT_KW")                                        \
  X(DONSUS_TRUE_KW, "DONSUS_TRUE_KW")                                          \
  X(DONSUS_FALSE_KW, "DONSUS_FALSE_KW")                                        \
  X(DONSUS_WHILE_KW, "DONSUS_WHILE_KW")                                        \
  X(DONSUS_FOR_KW, "DONSUS_FOR_KW")                                            \
  X(DONSUS_ABSTRACT_KW, "DONSUS_ABSTRACT_KW")                                  \
  X(DONSUS_FINAL_KW, "DONSUS_FINAL_KW")                                        \
  X(DONSUS_CLASS_KW, "DONSUS_CLASS_KW")                                        \
  X(DONSUS_TYPE_KW, "DONSUS_TYPE_KW")                                          \
  X(DONSUS_ALIAS_KW, "DONSUS_ALIAS_KW")                                        \
  X(DONSUS_TYPECLASS_KW, "DONSUS_TYPECLASS_KW")                                \
  X(DONSUS_INSTANCE_KW, "DONSUS_INSTANCE_KW")                                  \
  X(DONSUS_CONTINUE_KW, "DONSUS_CONTINUE_KW")                                  \
  X(DONSUS_BREAK_KW, "DONSUS_BREAK_KW")                                        \
  X(DONSUS_FALLTHROUGH_KW, "DONSUS_FALLTHROUGH_KW")                            \
  X(DONSUS_CASE_KW, "DONSUS_CASE_KW")                                          \
  X(DONSUS_DO_KW, "DONSUS_DO_KW")                                              \
  X(DONSUS_OF_KW, "DONSUS_OF_KW")                                              \
  X(DONSUS_OTHERWISE_KW, "DONSUS_OTHERWISE_KW")                                \
  X(DONSUS_AS_KW, "DONSUS_AS_KW")

enum class donsus_token_kind {
#define X(name, str) name,
  DONSUS_TOKEN_TYPES
#undef X
};

// Todo: Make it smaller
struct token {
  donsus_token_kind kind{};
  std::string value{};
  unsigned int length{};
  unsigned int line{};
  unsigned int column{};

  // zero by default
  unsigned int precedence{};

  // absolute offset
  unsigned int offset{};

  const char *type_name() const;
  static const char *type_name(donsus_token_kind kind);
};

#endif