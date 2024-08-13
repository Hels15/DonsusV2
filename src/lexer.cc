//===----------------------------------------------------------------------===//
//
// This file is the lexer of the Donsus Compiler. It produces tokens on the fly.
//
//===----------------------------------------------------------------------===//

#include "../Include/Donsus/tomi.h"
#include "../Include/parser.h"
#include "../Include/token.h"

#include <iostream>

// implement map for Tomi so this dependency can be ignored
std::map<std::string, donsus_token_kind> TYPES_FOR_LEXER {
  {"int", DONSUS_BASIC_INT}, {"int8", DONSUS_I8}, {"int16", DONSUS_I16},
      {"int32", DONSUS_I32}, {"int64", DONSUS_I64}, {"u32", DONSUS_U32},
      {"u64", DONSUS_U64}, {"f32", DONSUS_F32}, {"f64", DONSUS_F64},
      {"bool", DONSUS_BOOL}, {"void", DONSUS_VOID},
      {"string", DONSUS_STRING_TYPE}, {
    "char", DONSUS_CHAR
  }
}

// implement map for TOmi so this dependency can be ignored
std::map<std::string, Donsus::donsus_token_kind> DONSUS_KEYWORDS {
  {"def", DONSUS_FUNCTION_DEFINITION_KW}, {"if", DONSUS_IF_KW},
      {"elif", DONSUS_ELIF_KW}, {"else", DONSUS_ELSE_KW},
      {"return", DONSUS_RETURN_KW}, {"printf", DONSUS_PRINT_KW},
      {"true", DONSUS_TRUE_KW}, {"false", DONSUS_FALSE_KW},
      {"while", DONSUS_WHILE_KW}, {"for", DONSUS_FOR_KW},
      {"abstract", DONSUS_ABSTRACT_KW}, {"final", DONSUS_FINAL_KW},
      {"class", DONSUS_CLASS_KW}, {"type", DONSUS_TYPE_KW},
      {"alias", DONSUS_ALIAS_KW}, {"typeclass", DONSUS_TYPECLASS_KW},
      {"instance", DONSUS_INSTANCE_KW}, {"continue", DONSUS_CONTINUE_KW},
      {"break", DONSUS_BREAK_KW}, {"fallthrough", DONSUS_FALLTHROUGH_KW},
      {"case", DONSUS_CASE_KW}, {"do", DONSUS_DO_KW}, {"of", DONSUS_OF_KW},
      {"otherwise", DONSUS_OTHERWISE_KW}, {
    "as", DONSUS_AS_KW
  }
}

std::string de_get_name_from_token(Donsus::donsus_token_kind kind) {
  switch (kind) {
  case Donsus::DONSUS_IDENTIFIER:
    return "DONSUS_IDENTIFIER";
  case Donsus::DONSUS_END:
    return "DONSUS_END";
  case Donsus::DONSUS_NUMBER:
    return "DONSUS_NUMBER";
  case Donsus::DONSUS_FLOAT:
    return "DONSUS_FLOAT";
  case Donsus::DONSUS_STRING:
    return "DONSUS_STRING";
  case Donsus::DONSUS_NEWLINE:
    return "DONSUS_NEWLINE";
  case Donsus::DONSUS_AMPERSAND:
    return "DONSUS_AMPERSAND";
  case Donsus::DONSUS_AMPERSAND_EQUAl:
    return "DONSUS_AMPERSAND_EQUAl";
  case Donsus::DONSUS_PIPE:
    return "DONSUS_PIPE";
  case Donsus::DONSUS_PIPE_EQUAL:
    return "DONSUS_PIPE_EQUAL";
  case Donsus::DONSUS_CARET:
    return "DONSUS_CARET";
  case Donsus::DONSUS_CARET_EQUAL:
    return "DONSUS_CARET_EQUAL";
  case Donsus::DONSUS_LARROW2:
    return "DONSUS_LARROW2";
  case Donsus::DONSUS_LARROW2_EQUAL:
    return "DONSUS_LARROW2_EQUAL";
  case Donsus::DONSUS_RARROW2:
    return "DONSUS_RARROW2";
  case Donsus::DONSUS_RARROW2_EQUAL:
    return "DONSUS_RARROW2_EQUAL";
  case Donsus::DONSUS_LPAR:
    return "DONSUS_LPAR";
  case Donsus::DONSUS_RPAR:
    return "DONSUS_RPAR";
  case Donsus::DONSUS_LSQB:
    return "DONSUS_LSQB";
  case Donsus::DONSUS_RSQB:
    return "DONSUS_RSQB";
  case Donsus::DONSUS_COLO:
    return "DONSUS_COLO";
  case Donsus::DONSUS_COMM:
    return "DONSUS_COMM";
  case Donsus::DONSUS_SEMICOLON:
    return "DONSUS_SEMICOLON";
  case Donsus::DONSUS_PLUS:
    return "DONSUS_PLUS";
  case Donsus::DONSUS_PLUS_EQUAL:
    return "DONSUS_PLUS_EQUAL";
  case Donsus::DONSUS_MINUS:
    return "DONSUS_MINUS";
  case Donsus::DONSUS_MINUS_EQUAL:
    return "DONSUS_MINUS_EQUAL";
  case Donsus::DONSUS_STAR:
    return "DONSUS_STAR";
  case Donsus::DONSUS_STAR_EQUAL:
    return "DONSUS_STAR_EQUAL";
  case Donsus::DONSUS_BACK_SLASH:
    return "DONSUS_BACK_SLASH";
  case Donsus::DONSUS_SLASH:
    return "DONSUS_SLASH";
  case Donsus::DONSUS_SLASH_EQUAL:
    return "DONSUS_SLASH_EQUAL";
  case Donsus::DONSUS_LESS:
    return "DONSUS_LESS";
  case Donsus::DONSUS_LESS_EQUAL:
    return "DONSUS_LESS_EQUAL";
  case Donsus::DONSUS_GREATER:
    return "DONSUS_GREATER";
  case Donsus::DONSUS_GREATER_EQUAL:
    return "DONSUS_GREATER_EQUAL";
  case Donsus::DONSUS_EQUAL:
    return "DONSUS_EQUAL";
  case Donsus::DONSUS_DOUBLE_EQUAL:
    return "DONSUS_DOUBLE_EQUAL";
  case Donsus::DONSUS_NOT_EQUAL:
    return "DONSUS_NOT_EQUAL";
  case Donsus::DONSUS_DOT:
    return "DONSUS_DOT";
  case Donsus::DONSUS_DOUBLE_DOT:
    return "DONSUS_DOUBLE_DOT";
  case Donsus::DONSUS_PERCENT:
    return "DONSUS_PERCENT";
  case Donsus::DONSUS_LBRACE:
    return "DONSUS_LBRACE";
  case Donsus::DONSUS_RBRACE:
    return "DONSUS_RBRACE";
  case Donsus::DONSUS_CIRCUMFLEX:
    return "DONSUS_CIRCUMFLEX";
  case Donsus::DONSUS_EXCLAMATION:
    return "DONSUS_EXCLAMATION";
  case Donsus::DONSUS_COMMENT:
    return "DONSUS_COMMENT";
  case Donsus::DONSUS_SINGLE_QUOTE:
    return "DONSUS_SINGLE_QUOTE";
  case Donsus::DONSUS_THREE_DOTS:
    return "DONSUS_THREE_DOTS";
  case Donsus::DONSUS_TWO_DOTS:
    return "DONSUS_TWO_DOTS";
  case Donsus::DONSUS_NULL_VALUE:
    return "DONSUS_NULL_VALUE";
  case Donsus::DONSUS_BASIC_INT:
    return "DONSUS_BASIC_INT";
  case Donsus::DONSUS_I8:
    return "DONSUS_I8";
  case Donsus::DONSUS_I16:
    return "DONSUS_I16";
  case Donsus::DONSUS_I32:
    return "DONSUS_I32";
  case Donsus::DONSUS_I64:
    return "DONSUS_I64";
  case Donsus::DONSUS_U32:
    return "DONSUS_U32";
  case Donsus::DONSUS_U64:
    return "DONSUS_U64";
  case Donsus::DONSUS_UNDERSCORE:
    return "DONSUS_UNDERSCORE";
  case Donsus::DONSUS_F32:
    return "DONSUS_F32";
  case Donsus::DONSUS_F64:
    return "DONSUS_F64";
  case Donsus::DONSUS_BOOL:
    return "DONSUS_BOOL";
  case Donsus::DONSUS_VOID:
    return "DONSUS_VOID";
  case Donsus::DONSUS_ARROW:
    return "DONSUS_ARROW";
  case Donsus::DONSUS_STRING_TYPE:
    return "DONSUS_STRING_TYPE";
  case Donsus::DONSUS_CHAR:
    return "DONSUS_CHAR";
  case Donsus::DONSUS_FUNCTION_DEFINITION_KW:
    return "DONSUS_FUNCTION_DEFINITION_KW";
  case Donsus::DONSUS_IF_KW:
    return "DONSUS_IF_KW";
  case Donsus::DONSUS_ELIF_KW:
    return "DONSUS_ELIF_KW";
  case Donsus::DONSUS_ELSE_KW:
    return "DONSUS_ELSE_KW";
  case Donsus::DONSUS_RETURN_KW:
    return "DONSUS_RETURN_KW";
  case Donsus::DONSUS_PRINT_KW:
    return "DONSUS_PRINT_KW";
  case Donsus::DONSUS_TRUE_KW:
    return "DONSUS_TRUE_KW";
  case Donsus::DONSUS_FALSE_KW:
    return "DONSUS_FALSE_KW";
  case Donsus::DONSUS_WHILE_KW:
    return "DONSUS_WHILE_KW";
  case Donsus::DONSUS_FOR_KW:
    return "DONSUS_FOR_KW";
  case Donsus::DONSUS_ABSTRACT_KW:
    return "DONSUS_ABSTRACT_KW";
  case Donsus::DONSUS_FINAL_KW:
    return "DONSUS_FINAL_KW";
  case Donsus::DONSUS_CLASS_KW:
    return "DONSUS_CLASS_KW";
  case Donsus::DONSUS_TYPE_KW:
    return "DONSUS_TYPE_KW";
  case Donsus::DONSUS_ALIAS_KW:
    return "DONSUS_ALIAS_KW";
  case Donsus::DONSUS_TYPECLASS_KW:
    return "DONSUS_TYPECLASS_KW";
  case Donsus::DONSUS_INSTANCE_KW:
    return "DONSUS_INSTANCE_KW";
  case Donsus::DONSUS_CONTINUE_KW:
    return "DONSUS_CONTINUE_KW";
  case Donsus::DONSUS_BREAK_KW:
    return "DONSUS_BREAK_KW";
  case Donsus::DONSUS_FALLTHROUGH_KW:
    return "DONSUS_FALLTHROUGH_KW";
  case Donsus::DONSUS_CASE_KW:
    return "DONSUS_CASE_KW";
  case Donsus::DONSUS_DO_KW:
    return "DONSUS_DO_KW";
  case Donsus::DONSUS_OF_KW:
    return "DONSUS_OF_KW";
  case Donsus::DONSUS_OTHERWISE_KW:
    return "DONSUS_OTHERWISE_KW";
  case Donsus::DONSUS_AS_KW:
    return "DONSUS_AS_KW";
  default:
    return "UNKNOWN_TOKEN";
  }
}

// Function to peek at the next character without consuming it
char peek_for_char(Parser &parser) {
  char next_char = parser.lexer.string[parser.lexer.cur_pos + 1];
  return (next_char == '\0') ? '\0' : next_char;
}

char peek_back_for_char(Parser &parser) {
  char back_char = parser.lexer.string[parser.lexer.cur_pos - 1];
  return back_char;
}

char peek_for_specific(Parser &parser, int number) {
  char specific = parser.lexer.string[parser.lexer.cur_pos + number];
  return specific;
}

static bool isstart_identifier(char c) {

  // entry point of an identifier
  return (isalpha(c) != 0) || c == '_';
}

static bool iscontinue_identifier(char c) {

  // continue point of an identifier
  return isstart_identifier(c) || isdigit(c);
}

static bool is_type(std::string &s) {

  if (TYPES_FOR_LEXER.find(s) != TYPES_FOR_LEXER.end()) {

    return true;
  }

  return false;
}

static bool is_keyword(std::string &s) {
  if (DONSUS_KEYWORDS.find(s) != DONSUS_KEYWORDS.end()) {
    return true;
  }

  return false;
}

bool eat(Parser &parser) {
  ++parser.lexer.cur_column;
  if ((parser.lexer.cur_char = parser.lexer.string[++parser.lexer.cur_pos]) !=
      '\0') {

    return true;
  }

  return false;
}

static std::string get_text_between_pos(Parser &parser, unsigned int start,
                                        unsigned int end) {

  // returns string from the starting point to the end
  return {std::begin(parser.lexer.string) + start,
          std::begin(parser.lexer.string) + end};
}

static std::string get_rid_of_esc_seq(std::string &formal) {
  for (size_t i = 1; i < formal.length(); i++) {
    if (formal[i] == '\"' && formal[i - 1] == '\\') {
      formal.erase(i - 1, 1);
    }
  }
  return formal;
}

static std::string next_number(Parser &parser, donsus_token token,
                               unsigned int start_pos) {

  // TBD: we are checking isdigit 2 times here. We can't eat.

  while (isdigit(parser.lexer.cur_char)) {

    token.length++;

    eat(parser);
  }

  return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

std::string donsus_float(Parser &parser, donsus_token &token,
                         unsigned int start_pos) {
  std::string integer_part = next_number(parser, token, start_pos);
  std::string fractional_part{};
  if (parser.lexer.cur_char == '.' && peek_back_for_char(parser) != '.') {
    eat(parser);
    fractional_part = next_number(parser, token, start_pos);
  }
  return fractional_part;
}

static std::string next_identifier(Parser &parser, donsus_token &token,
                                   unsigned int start_pos) {
  // Check if the current character is a continue identifier
  if (!iscontinue_identifier(parser.lexer.cur_char)) {
    parser.donsus_syntax_error(
        nullptr, parser.lexer.cur_column, parser.lexer.cur_line,
        "Invalid identifier: '" + std::string(1, parser.lexer.cur_char) + "'");
  }

  // Proceed with the while loop if the initial check passes
  while (iscontinue_identifier(parser.lexer.cur_char)) {
    token.length++;
    eat(parser);
  }
  token.precedence =
      0 return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

static bool next_string(Parser &parser) {
  if (eat(parser) && peek_for_char(parser) != '\"') {
    return true;
  }
  if (parser.lexer.cur_char == '\\' && peek_for_char(parser) == '\"') {
    parser.lexer.cur_char = '\0';
    return true;
  }
  return false;
}

static donsus_token make_type(Parser &parser, std::string &value,
                              unsigned int length) {

  // construct type token
  donsus_token token;

  token.line = parser.lexer.cur_line;
  token.column = parser.lexer.cur_column;

  token.kind = DONSUS_TYPES_LEXER[value];

  token.length = length;

  token.value = value;
  token.offset = parser.lexer.cur_pos;
  return token;
}

static donsus_token make_keyword(Parser &parser, std::string &value,
                                 unsigned int length) {

  // construct keyword token
  donsus_token token;

  token.line = parser.lexer.cur_line;
  token.column = parser.lexer.cur_column;

  token.kind = DONSUS_KEYWORDS[value];

  token.length = length;

  token.value = value;
  token.offset = parser.lexer.cur_pos;

  return token;
}

void consume_spaces(DonsusParser &parser) {
  while (true) {
    switch (parser.lexer.cur_char) {
    case ' ': {
      while (parser.lexer.cur_char == ' ') {
        eat(parser);
      }
      break;
    }
    case '#': {
      auto cur_line = parser.lexer.cur_line;
      while (parser.lexer.cur_line == cur_line &&
             parser.lexer.cur_char != '\0') {
        if (peek_for_char(parser) == '\n') {
          eat(parser);
          break;
        }
        eat(parser);
      }
      break;
    }
    case '/': {
      if (peek_for_char(parser) == '*') {
        eat(parser); // consume '*'
        eat(parser); // consume '/'
        while (parser.lexer.cur_char != '*') {
          eat(parser); // get next token
        }
        if (peek_for_char(parser) == '/') {
          eat(parser); // consume '/' after '*'
          eat(parser); // consume '/' after '*'
        }

        break;
      } else {
        // It's a divisor(DONSUS_SLASH) operator and not a comment
        return;
      }
    default:
      return;
    }
    }
  }
}

token donsus_lexer_next(Parser &parser) {
  // aggregate initialisation
  token cur_token{DONSUS_END, "", 0, parser.lexer.cur_line};
  consume_spaces(parser);

  switch (parser.lexer.cur_char) {
  case '\n': {
    parser.lexer.cur_column = 1;

    cur_token.line = ++parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    cur_token.kind = DONSUS_NEWLINE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;
    eat(parser);

    return cur_token;
  }

  case '+': {
    // Check for +=
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_PLUS_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "+=";

      cur_token.precedence = 11;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } /*else if (peek_for_char(parser) == '+') {
      cur_token.kind = DONSUS_INCREMENT;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "++";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } */
    else {
      cur_token.precedence = 10; // lower precedence

      cur_token.kind = DONSUS_PLUS;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "+";
      cur_token.precedence = 7;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '-': {
    // Check for -=
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_MINUS_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "-=";
      cur_token.precedence = 11;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } /*else if (peek_for_char(parser) == '-') {
      cur_token.kind = DONSUS_DECREMENT;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "--";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } */
    else if (peek_for_char(parser) == '>') {
      cur_token.kind = DONSUS_ARROW;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "->";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      cur_token.precedence = 2;
      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;
    } else {
      cur_token.precedence = 10; // lower precedence

      cur_token.kind = DONSUS_MINUS;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "-";
      cur_token.precedence = 7;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '*': {
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_STAR_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "*=";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      cur_token.precedence = 11;
      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } else {

      cur_token.kind = DONSUS_STAR;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "*";
      cur_token.precedence = 6;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '/': {
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_SLASH_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "/=";
      cur_token.precedence = 11;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } else {
      cur_token.kind = DONSUS_SLASH;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "/";
      cur_token.precedence = 6;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '=': {

    if (peek_for_char(parser) == '=') {

      cur_token.kind = DONSUS_DOUBLE_EQUAL;

      cur_token.length = 2; // Set length to 2 for ==
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "==";

      cur_token.precedence = 9;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the second '=' character

      eat(parser); // Move to the next character

      return cur_token;
    } else {
      cur_token.kind = DONSUS_EQUAL;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "=";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '(': {

    cur_token.kind = DONSUS_LPAR;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "(";

    cur_token.precedence = 1;
    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case ';': {

    cur_token.kind = DONSUS_SEMICOLON;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ";";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case ':': {
    cur_token.kind = DONSUS_COLO;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ":";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }
  case ')': {

    cur_token.kind = DONSUS_RPAR;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;
    cur_token.precedence = 1;
    cur_token.value = ")";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '>': {

    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_GREATER_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ">=";

      cur_token.precedence = 8;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } else if (peek_for_char('>')) {

      cur_token.kind = DONSUS_RARROW2;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ">>";

      cur_token.precedence = 5;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character
      eat(parser); // Move to the next character
      if (peek_for_char('=')) {
        cur_token.kind = DONSUS_RARROW2_EQUAL;
        cur_token.length = 3;
        cur_token.offset = parser.lexer.cur_pos;
        cur_token.value = '>>=';
        cur_token.precedence = 11;
        cur_token.line = parser.lexer.cur_line;
        cur_token.column = parser.lexer.cur_column;
        eat(parser);
        return cur_token;
      }
      return cur_token;

    } else {

      cur_token.kind = DONSUS_GREATER;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ">";
      cur_token.precedence = 8;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '<': {

    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_LESS_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "<=";

      cur_token.precedence = 8;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    }
    // BITSHIFT LEFT
    else if (peek_for_char('<')) {
      cur_token.kind = DONSUS_LARROW2;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "<<";

      cur_token.precedence = 5;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character
      eat(parser); // Move to the next character
      if (peek_for_char(parser) == '=') {
        cur_token.kind = DONSUS_LARROW2_EQUAL;
        cur_token.length = 3;
        cur_token.value = "<<=";
        cur_token.precedence = 11;
        cur_token.line = parser.lexer.cur_line;
        cur_token.column = parser.lexer.cur_column;
        eat(parser);
        return cur_token;
      }
      return cur_token;

    } else {

      cur_token.kind = DONSUS_LESS;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "<";

      cur_token.precedence = 8; // lowest precedence

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case ',': {
    cur_token.kind = DONSUS_COMM;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ",";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '{': {
    cur_token.kind = DONSUS_LBRACE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "{";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '}': {
    cur_token.kind = DONSUS_RBRACE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "}";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '[': {
    cur_token.kind = DONSUS_LSQB;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "[";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case ']': {
    cur_token.kind = DONSUS_RSQB;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "]";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '.': {
    if (peek_for_char(parser) == '.') {
      cur_token.kind = DONSUS_TWO_DOTS;

      cur_token.length = 2;

      cur_token.value = "..";
      // Todo: It should be the highest one
      cur_token.precedence = 1;
      cur_token.line = parser.lexer.cur_line;

      eat(parser); // Consume the second dot

      eat(parser); // Move to the next character

      return cur_token;

    } else {
      cur_token.kind = DONSUS_DOT;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ".";

      cur_token.precedence = 2;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '%': {
    cur_token.kind = DONSUS_PERCENT;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "%";
    cur_token.precedence = 6;

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '!': {
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_NOT_EQUAL;

      cur_token.length = 2;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "!=";

      cur_token.precedence = 9;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } else {
      cur_token.kind = DONSUS_EXCLAMATION;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "!";
      cur_token.precedence = 3;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser);

      return cur_token;
    }
  }

  case '\"': {
    unsigned int start_pos = parser.lexer.cur_pos;
    cur_token.kind = DONSUS_STRING;

    cur_token.length = 0;
    cur_token.offset = parser.lexer.cur_pos;

    while (next_string(parser)) {
      cur_token.length++;
    }

    eat(parser);
    if (parser.lexer.cur_char == '\"') {
      eat(parser); // Consume the closing double quote
      std::string formal =
          get_text_between_pos(parser, start_pos + 1, parser.lexer.cur_pos - 1);
      cur_token.value = get_rid_of_esc_seq(formal);
    } else {
      std::cerr << "Error: Unterminated string literal at line "
                << cur_token.line << std::endl;
    }
    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    return cur_token;
  }

  case '\'': {
    cur_token.kind = DONSUS_SINGLE_QUOTE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "'";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }
  // BITWISE
  case '&': {
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_AMPERSAND_EQUAL;
      cur_token.length = 2;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value "&=";
      cur_token.precedence = 11;
      cur_token.value = "&";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);
      eat(parser);

      return cur_token;
    } else {
      cur_token.kind = DONSUS_AMPERSAND;

      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "&";
      cur_token.precedence = 3;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);
      return cur_token;
    }
  }
  case '|': {
    if (peek_for_char(parser) == '=') {
      cur_token.kind = DONSUS_PIPE_EQUAL;
      cur_token.length = 2;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "|=";
      cur_token.precedence = 11;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);
      eat(parser);
      return cur_token;
    } else {
      cur_token.kind = DONSUS_PIPE;
      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "|";
      cur_token.precedence = 6;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);

      return cur_token;
    }
  case '^': {
    if (peek_for_char(parser) == "=") {
      cur_token.kind = DONSUS_CARET_EQUAL;
      cur_token.length = 2;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "^=";
      cur_token.precedence = 11;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);
      eat(parser);
      return cur_token;
    } else {
      cur_token.kind = DONSUS_CARET;
      cur_token.length = 1;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "^";
      cur_token.precedence = 3;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);

      return cur_token;
    }
  }
  }
  default: {
    // numbers
    if (isdigit(parser.lexer.cur_char)) {
      cur_token.kind = DONSUS_NUMBER;

      cur_token.length = 0; // will be changed during next_number
      cur_token.value = next_number(parser, cur_token, parser.lexer.cur_pos);
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      // float part
      if (parser.lexer.cur_char == '.' && peek_for_char(parser) != '.') {
        cur_token.kind = DONSUS_FLOAT;
        cur_token.length = 0;
        cur_token.value +=
            donsus_float(parser, cur_token, parser.lexer.cur_pos);

        cur_token.line = parser.lexer.cur_line;
      }
      return cur_token;
    }
    // identifiers
    if (isstart_identifier(parser.lexer.cur_char)) {

      cur_token.kind = DONSUS_NAME;

      cur_token.length = 0; // will be changed during next_identifier
      std::string c_value =
          next_identifier(parser, cur_token, parser.lexer.cur_pos);

      // If it is a type
      if (is_type(c_value)) {

        return make_type(parser, c_value, cur_token.length);
      }

      if (is_keyword(c_value)) {

        return make_keyword(parser, c_value, cur_token.length);
      } else {

        cur_token.value = c_value; // default choice
      }

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      cur_token.offset = parser.lexer.cur_pos;

      return cur_token;
    }
    // if none of them are true we end the execution.(error|end of the file)
    return cur_token;
  }
  }
}