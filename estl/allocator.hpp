#pragma once
#include <type_traits.hpp>
#include <utility.hpp>
namespace estd {

template <typename T, typename... Args>
inline void construct(T *p, Args &&...args) {
    new (p) T(estd::forward<Args>(args)...);
}

template <typename T>
inline void destory(T *p) {
  if constexpr(!estd::is_trivially_destructible_v<T>)
  {
      p->~T();
  }
}

template <typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator end) {
     if constexpr (!estd::is_trivially_destructible_v<typename ForwardIterator::value_type>) {
        for (; first != end; first++) {
            destory(&*first);
        }
     }
}


template <typename T>
class allocator {
public:
    allocator() = default;
    ~allocator() = default;

    static T *allocate(size_t size) {
        return reinterpret_cast<T *>(new uint8_t[size * sizeof(T)]);
    }

    static void deallocate(T *p) {
        delete[](reinterpret_cast<uint8_t *>(p));
    }
};


}   // namespace estd