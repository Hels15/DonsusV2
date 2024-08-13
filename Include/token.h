#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>

typedef enum {
  // Tokens
  DONSUS_UNKNOWN,
  DONSUS_IDENTIFIER, // IDENTIFIER
  DONSUS_END,        // mark the end of the code
  DONSUS_NUMBER,     // 69
  DONSUS_FLOAT,      // 12.73
  DONSUS_STRING,     // "hello world"
  DONSUS_NEWLINE,    // \n

  // BITWISE
  DONSUS_AMPERSAND,       // &
  DONSUS_AMPERSAND_EQUAl, // &=
  DONSUS_PIPE,            // |
  DONSUS_PIPE_EQUAL,      // |=
  DONSUS_CARET,           // ^
  DONSUS_CARET_EQUAL,     // ^=

  // BITSHIFT
  DONSUS_LARROW2,          // <<
  DONSUS_LARROW2_EQUAL,    // <<=
  DONSUS_RARROW2           // >>
      DONSUS_RARROW2_EQUAL // >>=

          DONSUS_LPAR,  // (
  DONSUS_RPAR,          // )
  DONSUS_LSQB,          // [
  DONSUS_RSQB,          // ]
  DONSUS_COLO,          // :
  DONSUS_COMM,          // ,
  DONSUS_SEMICOLON,     // ;
  DONSUS_PLUS,          // +
  DONSUS_PLUS_EQUAL,    // +=
  DONSUS_MINUS,         // -
  DONSUS_MINUS_EQUAL,   // -=
  DONSUS_STAR,          // *
  DONSUS_STAR_EQUAL,    // *=
  DONSUS_BACK_SLASH,    // -- \ -- /
  DONSUS_SLASH,         // /
  DONSUS_SLASH_EQUAL,   // /=
  DONSUS_LESS,          // x < y
  DONSUS_LESS_EQUAL,    // x <= y
  DONSUS_GREATER,       // x > y
  DONSUS_GREATER_EQUAL, // x >= y
  DONSUS_EQUAL,         // =
  DONSUS_DOUBLE_EQUAL,  // ==
  DONSUS_NOT_EQUAL,     // !=
  DONSUS_DOT,           // .
  DONSUS_DOUBLE_DOT,    // ..

  DONSUS_PERCENT,      // %
  DONSUS_LBRACE,       // {
  DONSUS_RBRACE,       // }
  DONSUS_EXCLAMATION,  // !
  DONSUS_COMMENT,      // #
  DONSUS_SINGLE_QUOTE, // '
  DONSUS_THREE_DOTS,   // ...
  DONSUS_TWO_DOTS,     // ..
  DONSUS_NULL_VALUE,

  // INT
  DONSUS_BASIC_INT, // int
  DONSUS_I8,        // [FROM -(2^7) TO (2^7) -1]
  DONSUS_I16,       // [FROM −(2^15) TO 2^15 − 1]
  DONSUS_I32,       // [FROM 0 TO 2^32 -1]
  DONSUS_I64,       // [FROM 0 TO 2^64 − 1]
  DONSUS_U32,       // [FROM 0 TO 2^32 -1]
  DONSUS_U64,       // [FROM 0 TO 2^64 − 1]

  DONSUS_UNDERSCORE, // _

  // FLOAT
  DONSUS_F32, // float
  DONSUS_F64,

  DONSUS_BOOL,        // bool
  DONSUS_VOID,        // void
  DONSUS_ARROW,       // ->
  DONSUS_STRING_TYPE, // string

  // chars
  DONSUS_CHAR, // char
  // keywords
  DONSUS_FUNCTION_DEFINITION_KW, // def
  DONSUS_IF_KW,                  // if
  DONSUS_ELIF_KW,                // elif
  DONSUS_ELSE_KW,                // else
  DONSUS_RETURN_KW,              // return
  DONSUS_PRINT_KW,               // printf
  DONSUS_TRUE_KW,                // true
  DONSUS_FALSE_KW,               // false
  DONSUS_WHILE_KW,               // while
  DONSUS_FOR_KW,                 // for
  DONSUS_ABSTRACT_KW,            // abstract
  DONSUS_FINAL_KW,               // final
  DONSUS_CLASS_KW,               // class

  DONSUS_TYPE_KW,      // type
  DONSUS_ALIAS_KW,     // alias
  DONSUS_TYPECLASS_KW, // typeclass
  DONSUS_INSTANCE_KW   // instance

      DONSUS_CONTINUE_KW, // Continue
  DONSUS_BREAK_KW,        // break
  DONSUS_FALLTHROUGH_KW,  // fallthrough
  DONSUS_CASE_KW,         // case
  DONSUS_DO_KW,           // do
  DONSUS_OF_KW,           // of
  DONSUS_OTHERWISE_KW,    // otherwise
  DONSUS_AS_KW            // as
} donsus_token_kind;

struct token {
  donsus_token_kind kind{};
  std::string value{};
  unsigned int length{};
  unsigned int line{};
  unsigned int column{};
  /*
   *1: (..), x, x()
2: x.y x->y
3: -x, ^x, &x, !x, *x, ~x
4: as
5: x << y, x >> y(new-one)
6: x*y, x/y, x%y, x & y, x | y, x ^ y,
7: x + y, x - y
8:  x < y,  x <= y, x > y, x >= y (new-one)
9: x == y, x != y,
10: x && y, x || y
11: [x += y, x-=y, x*=y, x /= y, x %= y, x >>= y, x <<= y, y ^= y, x &= y, x |=
y]
   * */
  // ascending order, first one is the highest priority.
  // precedence is zero if doesn't have correct precedence
  // e.g keywords
  unsigned int precedence{};

  // absolute offset from the beginning
  // of the codebase
  unsigned int offset{};
};
#endif