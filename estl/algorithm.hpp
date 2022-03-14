#pragma once
#include <type.hpp>
namespace estd {

template <typename T>
inline constexpr const T& min(const T& lhs, const T& rhs) {
    return lhs < rhs ? lhs : rhs;
}

template <typename T>
inline constexpr const T& max(const T& lhs, const T& rhs) {
    return lhs > rhs ? lhs : rhs;
}

inline size_t cstrlen(const char* str) {
    size_t n = 0;
    while (*(str++) != '\0')
        n++;
    return n;
}

} // namespace estd