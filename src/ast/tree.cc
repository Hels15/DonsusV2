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