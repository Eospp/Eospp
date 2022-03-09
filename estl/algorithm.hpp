#pragma once

namespace estd {


template <typename T>
inline constexpr const T &min(const T &lhs, const T &rhs) {
    return lhs < rhs ? lhs : rhs;
}

template<typename T>
inline constexpr const T &max(const T &lhs,const T &rhs){
    return lhs > rhs ? lhs : rhs;
}



}   // namespace estd