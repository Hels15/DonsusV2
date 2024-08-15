#ifndef MEMORY_ALLOC_H
#define MEMORY_ALLOC_H
#include <cstddef>

namespace utility {
class DonsusAllocator {
public:
  DonsusAllocator() = default;
  DonsusAllocator(std::size_t block_size);
  // free(all) the memory
  /*  ~DonsusAllocator();*/

  template <typename type> auto r_alloc() -> type * {
    /*    auto a = new (allocate(sizeof(type), 2 * sizeof(void *))) type;*/
    auto a = new type;

    return a;
  }

private:
  void *allocate(std::size_t size, std::size_t alignment = 0);
  std::size_t calculate_padding(std::size_t address, std::size_t alignment);
  std::size_t total_size;

  void *start_ptr;
  std::size_t offset;
};
} // namespace utility
#endif