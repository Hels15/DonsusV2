#ifndef FILE_H
#define FILE_H
#include <string>
#include <cstdef>


std::string handle_file(const char *path_base);
std::size_t get_file_size(std::ifstream &file);

#endif // FILE_H