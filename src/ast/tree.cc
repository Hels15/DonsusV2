#include "tree.h"
#include "../Include/Donsus/tomi.h"

donsus_ast::tree::tree() : allocator(1024) {}

void donsus_ast::tree::add_node(utility::handle<node> node) {
  nodes.push_back(node);
}
auto donsus_ast::tree::get_nodes() -> Tomi::Vector<utility::handle<node>> {
  return nodes;
}
auto donsus_ast::tree::get_allocator() -> utility::DonsusAllocator {
  return allocator;
}
void donsus_ast::tree::allocate_node_list(std::uint64_t count) {}

utility::handle<donsus_ast::node> donsus_ast::tree::get_current_node() {
  return debug_nodes[n];
}
/*
 * When get_current_node() is called, only the main nodes will get returned.
 * A main node is something that causes the invokation of ome other node.
 * For example, in the case of variable definition, it executes
 * 3 nodes creation. One for the identifier, type, expression.
 * When an error is caught, we want to print out the whole node, so we have to
 * find where it started(mainly because of the first_token_in_ast property).
 * */
bool donsus_ast::tree::is_main_node(donsus_ast::donsus_node_type type_) {
  switch (type_.type) {
  case donsus_node_type::VARIABLE_DEFINITION:
  case donsus_node_type::FUNCTION_DECL:
    return true;
  default:
    return false;
  }
}