#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>

#define TOKEN_TYPES                                                            \
  X(UNKNOWN, "UNKNOWN")                                                        \
  X(IDENTIFIER, "IDENTIFIER")                                                  \
  X(END, "END")                                                                \
  X(INT, "INT")                                                                \
  X(FLOAT, "FLOAT")                                                            \
  X(BIN, "BIN")                                                                \
  X(OCT, "OCT")                                                                \
  X(HEX, "HEX")                                                                \
  X(STRING, "STRING")                                                          \
  X(NEWLINE, "NEWLINE")                                                        \
  X(AMPERSAND, "AMPERSAND")                                                    \
  X(AMPERSAND_EQUAL, "AMPERSAND_EQUAL")                                        \
  X(PIPE, "PIPE")                                                              \
  X(PIPE_EQUAL, "PIPE_EQUAL")                                                  \
  X(CARET, "CARET")                                                            \
  X(CARET_EQUAL, "CARET_EQUAL")                                                \
  X(LARROW2, "LARROW2")                                                        \
  X(LARROW2_EQUAL, "LARROW2_EQUAL")                                            \
  X(RARROW2, "RARROW2")                                                        \
  X(RARROW2_EQUAL, "RARROW2_EQUAL")                                            \
  X(LPAR, "LPAR")                                                              \
  X(RPAR, "RPAR")                                                              \
  X(LSQB, "LSQB")                                                              \
  X(RSQB, "RSQB")                                                              \
  X(COLON, "COLON")                                                            \
  X(COMM, "COMM")                                                              \
  X(SEMICOLON, "SEMICOLON")                                                    \
  X(PLUS, "PLUS")                                                              \
  X(PLUS_EQUAL, "PLUS_EQUAL")                                                  \
  X(MINUS, "MINUS")                                                            \
  X(MINUS_EQUAL, "MINUS_EQUAL")                                                \
  X(STAR, "STAR")                                                              \
  X(STAR_EQUAL, "STAR_EQUAL")                                                  \
  X(BACK_SLASH, "BACK_SLASH")                                                  \
  X(SLASH, "SLASH")                                                            \
  X(SLASH_EQUAL, "SLASH_EQUAL")                                                \
  X(LESS, "LESS")                                                              \
  X(LESS_EQUAL, "LESS_EQUAL")                                                  \
  X(GREATER, "GREATER")                                                        \
  X(GREATER_EQUAL, "GREATER_EQUAL")                                            \
  X(EQUAL, "EQUAL")                                                            \
  X(DOUBLE_EQUAL, "DOUBLE_EQUAL")                                              \
  X(NOT_EQUAL, "NOT_EQUAL")                                                    \
  X(DOT, "DOT")                                                                \
  X(DOUBLE_DOT, "DOUBLE_DOT")                                                  \
  X(PERCENT, "PERCENT")                                                        \
  X(LBRACE, "LBRACE")                                                          \
  X(RBRACE, "RBRACE")                                                          \
  X(EXCLAMATION, "EXCLAMATION")                                                \
  X(COMMENT, "COMMENT")                                                        \
  X(SINGLE_QUOTE, "SINGLE_QUOTE")                                              \
  X(THREE_DOTS, "THREE_DOTS")                                                  \
  X(TWO_DOTS, "TWO_DOTS")                                                      \
  X(NULL_VALUE, "NULL_VALUE")                                                  \
  X(BASIC_INT, "BASIC_INT")                                                    \
  X(I8, "I8")                                                                  \
  X(I16, "I16")                                                                \
  X(I32, "I32")                                                                \
  X(I64, "I64")                                                                \
  X(U32, "U32")                                                                \
  X(U64, "U64")                                                                \
  X(UNDERSCORE, "UNDERSCORE")                                                  \
  X(F32, "F32")                                                                \
  X(F64, "F64")                                                                \
  X(BOOL, "BOOL")                                                              \
  X(VOID, "VOID")                                                              \
  X(ARROW, "ARROW")                                                            \
  X(STRING_TYPE, "STRING_TYPE")                                                \
  X(CHAR, "CHAR")                                                              \
  X(FUNCTION_DEFINITION_KW, "FUNCTION_DEFINITION_KW")                          \
  X(IF_KW, "IF_KW")                                                            \
  X(ELIF_KW, "ELIF_KW")                                                        \
  X(ELSE_KW, "ELSE_KW")                                                        \
  X(RETURN_KW, "RETURN_KW")                                                    \
  X(PRINT_KW, "PRINT_KW")                                                      \
  X(TRUE_KW, "TRUE_KW")                                                        \
  X(FALSE_KW, "FALSE_KW")                                                      \
  X(WHILE_KW, "WHILE_KW")                                                      \
  X(FOR_KW, "FOR_KW")                                                          \
  X(ABSTRACT_KW, "ABSTRACT_KW")                                                \
  X(FINAL_KW, "FINAL_KW")                                                      \
  X(CLASS_KW, "CLASS_KW")                                                      \
  X(TYPE_KW, "TYPE_KW")                                                        \
  X(ALIAS_KW, "ALIAS_KW")                                                      \
  X(TYPECLASS_KW, "TYPECLASS_KW")                                              \
  X(INSTANCE_KW, "INSTANCE_KW")                                                \
  X(CONTINUE_KW, "CONTINUE_KW")                                                \
  X(BREAK_KW, "BREAK_KW")                                                      \
  X(FALLTHROUGH_KW, "FALLTHROUGH_KW")                                          \
  X(CASE_KW, "CASE_KW")                                                        \
  X(DO_KW, "DO_KW")                                                            \
  X(OF_KW, "OF_KW")                                                            \
  X(OTHERWISE_KW, "OTHERWISE_KW")                                              \
  X(AS_KW, "AS_KW")                                                            \
  X(MUT_KW, "MUT_KW")                                                          \
  X(PRIVATE_KW, "PRIVATE_KW")                                                  \
  X(STATIC_KW, "STATIC_KW")                                                    \
  X(COMPTIME_KW, "COMPTIME_KW")                                                \
  X(THREAD_LOCAL_KW, "THREAD_LOCAL_KW")                                        \
  X(AT, "AT")                                                                  \
  X(DOUBLE_COLON, "DOUBLE_COLON")                                              \
  X(IMPLIES, "IMPLIES")                                                        \
  X(UNDEFINED, "UNDEFINED")                                                    \
  X(DOUBLE_PIPE, "DOUBLE_PIPE")                                                \
  X(DOUBLE_AMPERSAND, "DOUBLE_AMPERSAND")                                      \
  X(TILDE, "TILDE")

enum class donsus_token_kind {
#define X(name, str) name,
  TOKEN_TYPES
#undef X
};

struct token {
  donsus_token_kind kind{};
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