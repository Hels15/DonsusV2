//===----------------------------------------------------------------------===//
// Main entry point for the Donsus Compiler.
// It calls all the steps needed from the top to the bottom.
//===----------------------------------------------------------------------===//
#include "../Include/file.h"
#include "../Include/parser.h"

#include <filesystem>

Parser Du_Parse(std::string result, AstFile &file) {
  donsus_lexer lexer(std::move(result));
  Parser parser(lexer, file);
  return parser;
}

int Du_Main(int argc, const char **argv) {
  AstFile file;

  std::string result = handle_file(argv[1]);
  std::string path = argv[1];
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);

  std::string::size_type p(base_filename.find_last_of('.'));

  std::string file_without_extension =
      base_filename.substr(0, p); // Obtain file without the extension(.du)

  std::string file_extension = base_filename.substr(p + 1);
  if (file_extension != "du") {
    throw std::runtime_error("File extension must be: .du");
  }
  file.extension = file_extension;
  file.filename = file_without_extension;
  file.fullpath = path;
  file.absolute_path = std::filesystem::absolute(path).string();
  file.id = 0;
  file.source = result;

  Parser parser = Du_Parse(result, file);
  Parser::end_result parser_result = parser.parse();
  if (file.error_count) {
    std::cout << rang::fg::reset;
    return 1;
  }

#if DEBUG
  std::cout << "\n";

  std::cout << "-----Parsing completed successfully-----\n";

  std::cout << "\n";
#endif
  return 0;
}
