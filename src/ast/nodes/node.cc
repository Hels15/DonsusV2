#include "../../../Include/ast/nodes/node.h"

donsus_ast::donsus_node_type::donsus_node_type(
    donsus_ast::donsus_node_type::underlying type)
    : type(type) {}

const char *donsus_ast::donsus_node_type::type_name() const {
  return donsus_ast::donsus_node_type::type_name(type);
}
const char *donsus_ast::donsus_node_type::type_name(underlying type_) const {
  switch (type_) {
#define X(name, text)                                                          \
  case underlying::name:                                                       \
    return #name;
    UNDERLYING_NODE_TYPES
#undef X
  }
}

std::string donsus_ast::node::uniqueName() {return label() + nid;}
std::string donsus_ast::node::glabel() {return label();}

utility::handle<node> donsus_ast::node::in(int i) {return inputs[i];}
int donsus_ast::node::nIns() {return inputs.size();}

utility::handle<node> donsus_ast::node::out(int i) {return outputs.size();}
int donsus_ast::node::nOuts() {return outputs.size();}

bool donsus_ast::node::isUnused() {return nOuts() == 0;}
bool donsus_ast::node::isCFG() {return false;}


