#ifndef TYPE_H
#define TYPE_H

#include "sstream"

class Type {
public:
  Type(unsigned int type);
  static int DBOT = 0;
  static int DTOP = 1;
  static int DSIMPLE = 2;
  static int DINT = 3;
  int type_;
  bool isSimple();
  bool isConstant();

  static Type BOTTOM;
  std::ostringstream print_(std::ostringstream& b);
};

#endif