#ifndef TREE_H
#define TREE_H
#include "../Include/Donsus/tomi.h"
#include "../utility/handle.h"
#include "../utility/memory_alloc.h"
#include "node.h"

namespace donsus_ast {
class tree {
public:
  tree();
  void add_node(utility::handle<node>);
  utility::handle<donsus_ast::node> get_current_node();
  auto get_nodes() -> Tomi::Vector<utility::handle<node>>;
  auto get_allocator() -> utility::DonsusAllocator;
  void allocate_node_list(std::uint64_t count);

  template <typename extra_type>
  auto create_node(donsus_node_type type) -> utility::handle<node> {
    const utility::handle<node> node_ptr = allocator.r_alloc<node>();
    // type
    node_ptr->type = type;
    node_ptr->children =
        {}; // initialise it as an empty vector rather than the nodes
    node_ptr->set_property(allocator.r_alloc<extra_type>());
    debug_nodes.push_back(node_ptr);
    if (n != 0)
      n++;

    return node_ptr;
  };

private:
  unsigned int n{};
  Tomi::Vector<utility::handle<node>> nodes;
  Tomi::Vector<utility::handle<node>> debug_nodes;
  utility::DonsusAllocator allocator;
};
} // namespace donsus_ast
#endif