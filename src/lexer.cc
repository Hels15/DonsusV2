//===----------------------------------------------------------------------===//
//
// This file is the lexer of the Donsus Compiler. It produces tokens on the fly.
//
//===----------------------------------------------------------------------===//

#include "../Include/Donsus/tomi.h"
#include "../Include/parser.h"
#include "../Include/token.h"

#include <iostream>

std::map<std::string, donsus_token_kind> TYPES_FOR_LEXER{
    {"int", donsus_token_kind::DONSUS_BASIC_INT},
    {"int8", donsus_token_kind::DONSUS_I8},
    {"int16", donsus_token_kind::DONSUS_I16},
    {"int32", donsus_token_kind::DONSUS_I32},
    {"int64", donsus_token_kind::DONSUS_I64},
    {"u32", donsus_token_kind::DONSUS_U32},
    {"u64", donsus_token_kind::DONSUS_U64},
    {"f32", donsus_token_kind::DONSUS_F32},
    {"f64", donsus_token_kind::DONSUS_F64},
    {"bool", donsus_token_kind::DONSUS_BOOL},
    {"void", donsus_token_kind::DONSUS_VOID},
    {"string", donsus_token_kind::DONSUS_STRING_TYPE},
    {"char", donsus_token_kind::DONSUS_CHAR}};

std::map<std::string, donsus_token_kind> DONSUS_KEYWORDS{
    {"def", donsus_token_kind::DONSUS_FUNCTION_DEFINITION_KW},
    {"if", donsus_token_kind::DONSUS_IF_KW},
    {"elif", donsus_token_kind::DONSUS_ELIF_KW},
    {"else", donsus_token_kind::DONSUS_ELSE_KW},
    {"return", donsus_token_kind::DONSUS_RETURN_KW},
    {"printf", donsus_token_kind::DONSUS_PRINT_KW},
    {"true", donsus_token_kind::DONSUS_TRUE_KW},
    {"false", donsus_token_kind::DONSUS_FALSE_KW},
    {"while", donsus_token_kind::DONSUS_WHILE_KW},
    {"for", donsus_token_kind::DONSUS_FOR_KW},
    {"abstract", donsus_token_kind::DONSUS_ABSTRACT_KW},
    {"final", donsus_token_kind::DONSUS_FINAL_KW},
    {"class", donsus_token_kind::DONSUS_CLASS_KW},
    {"type", donsus_token_kind::DONSUS_TYPE_KW},
    {"alias", donsus_token_kind::DONSUS_ALIAS_KW},
    {"typeclass", donsus_token_kind::DONSUS_TYPECLASS_KW},
    {"instance", donsus_token_kind::DONSUS_INSTANCE_KW},
    {"continue", donsus_token_kind::DONSUS_CONTINUE_KW},
    {"break", donsus_token_kind::DONSUS_BREAK_KW},
    {"fallthrough", donsus_token_kind::DONSUS_FALLTHROUGH_KW},
    {"case", donsus_token_kind::DONSUS_CASE_KW},
    {"do", donsus_token_kind::DONSUS_DO_KW},
    {"of", donsus_token_kind::DONSUS_OF_KW},
    {"otherwise", donsus_token_kind::DONSUS_OTHERWISE_KW},
    {"as", donsus_token_kind::DONSUS_AS_KW}};

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

static std::string next_number(Parser &parser, token token_,
                               unsigned int start_pos) {

  // TBD: we are checking isdigit 2 times here. We can't eat.

  while (isdigit(parser.lexer.cur_char)) {

    token_.length++;

    eat(parser);
  }

  return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

static std::string next_hex(Parser &parser, token token_,
                            unsigned int start_pos) {
  while (isdigit(parser.lexer.cur_char) ||
         isstart_identifier(parser.lexer.cur_char)) {
    token_.length++;
    eat(parser);
  }
  return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

std::string donsus_float(Parser &parser, token &token, unsigned int start_pos) {
  std::string integer_part = next_number(parser, token, start_pos);
  std::string fractional_part{};
  if (parser.lexer.cur_char == '.' && peek_back_for_char(parser) != '.') {
    eat(parser);
    fractional_part = next_number(parser, token, start_pos);
  }
  return fractional_part;
}

static std::string next_identifier(Parser &parser, token &token,
                                   unsigned int start_pos) {
  // Check if the current character is a continue identifier
  if (!iscontinue_identifier(parser.lexer.cur_char)) {
    /*    parser.donsus_syntax_error(
            nullptr, parser.lexer.cur_column, parser.lexer.cur_line,
            "Invalid identifier: '" + std::string(1, parser.lexer.cur_char) +
       "'");*/
  }

  // Proceed with the while loop if the initial check passes
  while (iscontinue_identifier(parser.lexer.cur_char)) {
    token.length++;
    eat(parser);
  }

  return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
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

static token make_type(Parser &parser, std::string &value,
                       unsigned int length) {

  // construct type token
  token token;

  token.line = parser.lexer.cur_line;
  token.column = parser.lexer.cur_column;

  token.kind = TYPES_FOR_LEXER[value];

  token.length = length;

  token.value = value;
  token.offset = parser.lexer.cur_pos;
  return token;
}

static token make_keyword(Parser &parser, std::string &value,
                          unsigned int length) {

  // construct keyword token
  token token;

  token.line = parser.lexer.cur_line;
  token.column = parser.lexer.cur_column;

  token.kind = DONSUS_KEYWORDS[value];

  token.length = length;

  token.value = value;
  token.offset = parser.lexer.cur_pos;

  return token;
}

void consume_spaces(Parser &parser) {
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
  token cur_token{donsus_token_kind::DONSUS_END, "", 0, parser.lexer.cur_line};
  consume_spaces(parser);

  switch (parser.lexer.cur_char) {
  case '0': {
    if (peek_for_char(parser) == 'b') {
      // binary.
      cur_token.kind = donsus_token_kind::DONSUS_BIN;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      // eat it until the Int part
      eat(parser);
      eat(parser);
      // sets length as well
      cur_token.value = next_number(parser, cur_token, parser.lexer.cur_pos);
      return cur_token;
    } else if (peek_for_char(parser) == 'x') {
      cur_token.kind = donsus_token_kind::DONSUS_HEX;
      cur_token.offset = parser.lexer.cur_pos;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      eat(parser);
      eat(parser);
      cur_token.value = next_hex(parser, cur_token, parser.lexer.cur_pos);
      return cur_token;
    }
  }
  case '\n': {
    parser.lexer.cur_column = 1;

    cur_token.line = ++parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    cur_token.kind = donsus_token_kind::DONSUS_NEWLINE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;
    eat(parser);

    return cur_token;
  }

  case '+': {
    // Check for +=
    if (peek_for_char(parser) == '=') {
      cur_token.kind = donsus_token_kind::DONSUS_PLUS_EQUAL;

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

      cur_token.kind = donsus_token_kind::DONSUS_PLUS;

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
      cur_token.kind = donsus_token_kind::DONSUS_MINUS_EQUAL;

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
      cur_token.kind = donsus_token_kind::DONSUS_ARROW;

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

      cur_token.kind = donsus_token_kind::DONSUS_MINUS;

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
      cur_token.kind = donsus_token_kind::DONSUS_STAR_EQUAL;

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

      cur_token.kind = donsus_token_kind::DONSUS_STAR;

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
      cur_token.kind = donsus_token_kind::DONSUS_SLASH_EQUAL;

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
      cur_token.kind = donsus_token_kind::DONSUS_SLASH;

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

      cur_token.kind = donsus_token_kind::DONSUS_DOUBLE_EQUAL;

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
      cur_token.kind = donsus_token_kind::DONSUS_EQUAL;

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

    cur_token.kind = donsus_token_kind::DONSUS_LPAR;

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

    cur_token.kind = donsus_token_kind::DONSUS_SEMICOLON;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ";";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case ':': {
    cur_token.kind = donsus_token_kind::DONSUS_COLO;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ":";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }
  case ')': {

    cur_token.kind = donsus_token_kind::DONSUS_RPAR;

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
      cur_token.kind = donsus_token_kind::DONSUS_GREATER_EQUAL;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ">=";

      cur_token.precedence = 8;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character

      eat(parser); // Move to the next character

      return cur_token;

    } else if (peek_for_char(parser) == '>') {

      cur_token.kind = donsus_token_kind::DONSUS_RARROW2;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = ">>";

      cur_token.precedence = 5;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character
      eat(parser); // Move to the next character
      if (peek_for_char(parser) == '=') {
        cur_token.kind = donsus_token_kind::DONSUS_RARROW2_EQUAL;
        cur_token.length = 3;
        cur_token.offset = parser.lexer.cur_pos;
        cur_token.value = ">>=";
        cur_token.precedence = 11;
        cur_token.line = parser.lexer.cur_line;
        cur_token.column = parser.lexer.cur_column;
        eat(parser);
        return cur_token;
      }
      return cur_token;

    } else {

      cur_token.kind = donsus_token_kind::DONSUS_GREATER;

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
      cur_token.kind = donsus_token_kind::DONSUS_LESS_EQUAL;

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
    else if (peek_for_char(parser) == '<') {
      cur_token.kind = donsus_token_kind::DONSUS_LARROW2;

      cur_token.length = 2; // Set length to 2 for +=
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "<<";

      cur_token.precedence = 5;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      eat(parser); // Consume the '=' character
      eat(parser); // Move to the next character
      if (peek_for_char(parser) == '=') {
        cur_token.kind = donsus_token_kind::DONSUS_LARROW2_EQUAL;
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

      cur_token.kind = donsus_token_kind::DONSUS_LESS;

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
    cur_token.kind = donsus_token_kind::DONSUS_COMM;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = ",";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '{': {
    cur_token.kind = donsus_token_kind::DONSUS_LBRACE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "{";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '}': {
    cur_token.kind = donsus_token_kind::DONSUS_RBRACE;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "}";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case '[': {
    cur_token.kind = donsus_token_kind::DONSUS_LSQB;

    cur_token.length = 1;
    cur_token.offset = parser.lexer.cur_pos;

    cur_token.value = "[";

    cur_token.line = parser.lexer.cur_line;
    cur_token.column = parser.lexer.cur_column;

    eat(parser);

    return cur_token;
  }

  case ']': {
    cur_token.kind = donsus_token_kind::DONSUS_RSQB;

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
      cur_token.kind = donsus_token_kind::DONSUS_TWO_DOTS;

      cur_token.length = 2;

      cur_token.value = "..";
      // Todo: It should be the highest one
      cur_token.precedence = 1;
      cur_token.line = parser.lexer.cur_line;

      eat(parser); // Consume the second dot

      eat(parser); // Move to the next character

      return cur_token;

    } else {
      cur_token.kind = donsus_token_kind::DONSUS_DOT;

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
    cur_token.kind = donsus_token_kind::DONSUS_PERCENT;

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
      cur_token.kind = donsus_token_kind::DONSUS_NOT_EQUAL;

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
      cur_token.kind = donsus_token_kind::DONSUS_EXCLAMATION;

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
    cur_token.kind = donsus_token_kind::DONSUS_STRING;

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
    cur_token.kind = donsus_token_kind::DONSUS_SINGLE_QUOTE;

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
      cur_token.kind = donsus_token_kind::DONSUS_AMPERSAND_EQUAL;
      cur_token.length = 2;
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.value = "&=";
      cur_token.precedence = 11;
      cur_token.value = "&";

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_char;

      eat(parser);
      eat(parser);

      return cur_token;
    } else {
      cur_token.kind = donsus_token_kind::DONSUS_AMPERSAND;

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
      cur_token.kind = donsus_token_kind::DONSUS_PIPE_EQUAL;
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
      cur_token.kind = donsus_token_kind::DONSUS_PIPE;
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
    if (peek_for_char(parser) == '=') {
      cur_token.kind = donsus_token_kind::DONSUS_CARET_EQUAL;
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
      cur_token.kind = donsus_token_kind::DONSUS_CARET;
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
      cur_token.kind = donsus_token_kind::DONSUS_INT;

      cur_token.length = 0; // will be changed during next_number
      cur_token.value = next_number(parser, cur_token, parser.lexer.cur_pos);
      cur_token.offset = parser.lexer.cur_pos;

      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;

      // float part
      if (parser.lexer.cur_char == '.' && peek_for_char(parser) != '.') {
        cur_token.kind = donsus_token_kind::DONSUS_FLOAT;
        cur_token.length = 0;
        cur_token.value +=
            donsus_float(parser, cur_token, parser.lexer.cur_pos);

        cur_token.line = parser.lexer.cur_line;
      }
      return cur_token;
    }
    // OCTAL
    if (parser.lexer.cur_char == '0') {
      cur_token.kind = donsus_token_kind::DONSUS_OCT;
      cur_token.offset = parser.lexer.cur_pos;
      cur_token.line = parser.lexer.cur_line;
      cur_token.column = parser.lexer.cur_column;
      eat(parser);
      cur_token.value = next_number(parser, cur_token, parser.lexer.cur_pos);
      return cur_token;
    }
    // identifiers
    if (isstart_identifier(parser.lexer.cur_char)) {

      cur_token.kind = donsus_token_kind::DONSUS_IDENTIFIER;

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