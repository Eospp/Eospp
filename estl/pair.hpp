#pragma once
#include <utility.hpp>
namespace estd {

template <typename T1, typename T2>
class pair {
public:
    using first_type = T1;
    using second_type = T2;

    constexpr pair() = default;
    constexpr pair(const pair &) = default;
    constexpr pair(pair &&) = default;

    constexpr pair(const first_type &a, const second_type &b) : first(a), second(b) {}

    template <typename U1, typename U2>
    constexpr pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

    template <typename U1, typename U2>
    constexpr pair(pair<U1, U2> &&p) : first(forward<U1>(p.first)), second(forward<U2>(p.second)) {}

    template <typename U1, typename U2>
    pair &operator=(const pair<U1, U2> &p) {
        first = p.first;
        second = p.second;
        return *this;
    }

    template <typename U1, typename U2>
    pair &operator=(pair<U1, U2> &&p) {
        first = forward<U1>(p.first);
        second = forward<U2>(p.second);
        return *this;
    }
    void swap(pair &p) noexcept {
        estd::swap(first, p.first);
        estd::swap(second, p.second);
    }

public:
    first_type first;
    second_type second;
};
template <typename T1, typename T2>
inline bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2>
inline bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return lhs.first < rhs.first ? lhs.first < rhs.first : lhs.second < rhs.second;
}

template <typename T1, typename T2>
inline bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return lhs < rhs || lhs == rhs;
}

template <typename T1, typename T2>
inline bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return !(lhs == rhs);
}

template <typename T1, typename T2>
inline bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return rhs < lhs;
}

template <typename T1, typename T2>
inline bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return !(lhs < rhs);
}

template <typename T1, typename T2>
inline pair<decay_t<T1>, decay_t<T2>> make_pair(T1 &&lhs, T2 &&rhs) {
    return pair<decay_t<T1>, decay_t<T2>>(estd::forward<T1>(lhs), estd::forward<T2>(rhs));
}

template <typename T1, typename T2>
inline void swap(pair<T1, T2> &lhs, pair<T1, T2> &rhs) {
    lhs.swap(rhs);
}
}   // namespace estd