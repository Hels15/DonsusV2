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