#pragma once
#include <algorithm.hpp>
#include <type.hpp>
#include <utility.hpp>
namespace estd {

template <typename T>
class basic_string_view {
public:
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = estd::size_t;

    constexpr basic_string_view() : data_(nullptr), size_(0) {}

    constexpr basic_string_view(const_pointer str) : data_(str), size_(cstrlen(str)) {}

    constexpr basic_string_view(const_pointer str, size_t len) : data_(str), size_(len) {}

    constexpr basic_string_view(const basic_string_view &) = default;
    constexpr basic_string_view &operator=(const basic_string_view &) = default;

    constexpr const_iterator begin() const {
        return data_;
    }

    constexpr const_iterator end() const {
        return data_ + size_;
    }

    constexpr size_type size() const {
        return size_;
    }

    constexpr size_type length() const {
        return size_;
    }
    constexpr bool empty() const {
        return size() == 0;
    }

    constexpr const_reference operator[](size_type pos) const {
        return data_[pos];
    }

    constexpr const_reference at(size_type pos) const {
        return data_[pos];
    }
    constexpr const_reference front() const {
        return data_[0];
    }

    constexpr const_reference back() const {
        return data_[size_ - 1];
    }

    constexpr const_pointer data() const {
        return data_;
    }

    constexpr basic_string_view substr(size_type pos, size_t len = npos) {
        return {data_ + pos, estd::min(size_ - pos, len)};
    }

    void swap(basic_string_view &rhs) {
        estd::swap(data_, rhs.data_);
        estd::swap(size_, rhs.size_);
    }

    constexpr int compare(const basic_string_view &rhs) const {
        if (size_ < rhs.size_) {
            return -1;
        }
        if (size_ > rhs.size_) {
            return 1;
        }
        for (size_type i = 0; i < size_; i++) {
            if (at(i) < rhs[i]) {
                return -1;
            }
            if (at(i) > rhs[i]) {
                return 1;
            }
        }
        return 0;
    }

public:
    static constexpr const size_t npos = -1;

private:
    const_pointer data_;
    size_type size_;
};

using string_view = basic_string_view<char>;


template <typename T>
constexpr bool operator==(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) == 0;
}

template <typename T>
constexpr bool operator!=(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) != 0;
}

template <typename T>
constexpr bool operator<(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) < 0;
}

template <typename T>
constexpr bool operator>(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) > 0;
}

template <typename T>
constexpr bool operator<=(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) <= 0;
}


template <typename T>
bool operator>=(const basic_string_view<T> &x, const basic_string_view<T> &y) noexcept {
    return x.compare(y) >= 0;
}


}   // namespace estd