// primitive addition OP - (+!)
#ifndef ADD_NODE_H
#define ADD_NODE_H
#include "node.h"

class AddNode: public donsus_ast::node {
  std::string label() override;
  std::string glabel() override;

};
#endif