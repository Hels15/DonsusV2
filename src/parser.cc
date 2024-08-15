#include "../Include/parser.h"
#include "../Include/Donsus/tomi.h"
#include "../src/ast/node.h"
#include "../src/utility/parser/print_ast.h"

// Todo: Replace it when switching to multi-threading
Tomi::Vector<AstFile *> global_files;

Parser::Parser(donsus_lexer &lexer, AstFile &file)
    : lexer(lexer), allocator(1024), file(file) {
  cur_token = donsus_lexer_next(*this);
  donsus_tree = allocator.r_alloc<donsus_ast::tree>();
  file.parser = this;
  global_files.push_back(&file);
}

token Parser::parser_next() {
  cur_token = donsus_lexer_next(*this);
  return cur_token;
}
// Move it out from here
static std::ostream &operator<<(std::ostream &o, token &token) {
  o << "Value: " << token.value << "\n";
  o << "Kind: " << token.type_name() << "\n";
  o << "Length: " << token.length << "\n";
  o << "Line: " << token.line << "\n";
  return o;
}

void Parser::print_token() {
  while (cur_token.kind != donsus_token_kind::END) {
    std::cout << cur_token << "\n";
    parser_next();
  }
}

token Parser::peek(int loop) {
  donsus_lexer save = lexer;
  token result_main;
  for (int i = 0; i < loop; i++) {
    result_main = donsus_lexer_next(*this);
  }
  lexer = save;
  return result_main;
}

auto Parser::donsus_parse() -> end_result {
#if DEBUG
  std::cout << "LEXER: "
            << "\n";
  Parser save = *this;
  print_token();
  *this = save;
#endif
  // statement:  declarationstatement
  //           | expressionstatement
  //           | compoundstatement
  //           | selectionstatement
  //           | iterationstatement
  //           | classdeclstatement;
  while (cur_token.kind != donsus_token_kind::END) {
  }
  // return smt here
}