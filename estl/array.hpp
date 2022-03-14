#pragma once
#include <utility.hpp>
#include <algorithm.hpp>
namespace estd {

template <typename T, estd::size_t N>
class array {
public:
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using iterator        = T*;
    using const_iterator  = const T*;
    using const_pointer   = const T*;
    using size_type       = estd::size_t;

    constexpr iterator begin() {
        return data_;
    }
    constexpr const_iterator begin() const {
        return data_;
    }

    constexpr void fill(const value_type &v){
        estd::fill(begin(),end(),v);
    }

    constexpr iterator end() {
        return data_ + N;
    }

    constexpr const_iterator end() const {
        return data_ + N;
    }

    constexpr bool empty() const {
        return N == 0;
    }

    constexpr size_type size() const {
        return N;
    }

    constexpr pointer data() const {
        return data_;
    }

    constexpr const_pointer data() {
        return data_;
    }

    reference front() {
        return data_[0];
    }

    const_reference front() const {
        return data_[0];
    }

    reference back() {
        return data_[N - 1];
    }
    const_reference back() const {
        return data_[N - 1];
    }

    reference operator[](size_type pos) {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const {
        return data_[pos];
    }

private:
    value_type data_[N];
};

} // namespace estd