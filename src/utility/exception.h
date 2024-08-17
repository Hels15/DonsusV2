#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <iostream>
#include "token.h"

/**
 *  \brief Represents the base exception class
 */
class DonsusException : public std::exception {
private:
  std::string error_message;

public:
  DonsusException() = default;
  DonsusException(const std::string &message) : error_message(message) {}

  // Returned value shouldn't be ignored
  [[nodiscard]] virtual const char *what() const throw() {
    return error_message.c_str();
  }
};

#endif