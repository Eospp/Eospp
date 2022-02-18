#pragma once
#include <type_traits.hpp>
namespace estd {

using ptrdiff_t = long int;

///  Marking input iterators.
struct input_iterator_tag {};

///  Marking output iterators.
struct output_iterator_tag {};

/// Forward iterators support a superset of input iterator operations.
struct forward_iterator_tag : public input_iterator_tag {};

/// Bidirectional iterators support a superset of forward iterator
/// operations.
struct bidirectional_iterator_tag : public forward_iterator_tag {};

/// Random-access iterators support a superset of bidirectional
/// iterator operations.
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename Category,
          typename T,
          typename Distance = ptrdiff_t,
          typename Pointer = T *,
          typename Reference = T &>
struct iterator {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};


template <typename Iterator, typename = void>
struct iterator_traits;

template <typename Iterator>
struct iterator_traits<Iterator,
                       estd::void_t<typename Iterator::iterator_category,
                                    typename Iterator::value_type,
                                    typename Iterator::pointer,
                                    typename Iterator::reference>> {
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
};

template <typename T>
struct iterator_traits<T *, void> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;
};

template <typename T>
struct iterator_traits<const T *, void> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;
};

template <typename Iterator, typename Container>
class normal_iterator {
    using traits_type = iterator_traits<Iterator>;

public:
    using iterator_type = Iterator;
    using iterator_category = typename traits_type::iterator_category;
    using value_type = typename traits_type::value_type;
    using difference_type = typename traits_type::difference_type;
    using reference = typename traits_type::reference;
    using pointer = typename traits_type::pointer;

    constexpr normal_iterator() noexcept : M_current_(Iterator()) {}

    explicit normal_iterator(const Iterator &i) noexcept : M_current_(i) {}

    template <typename Iter>
    normal_iterator(
        const normal_iterator<Iter,
                              estd::enable_if_t<estd::is_same_v<Iter, typename Container::pointer>,Container>>
            &i) noexcept
        : M_current_(i.base()) {}

    reference operator*() const {
        return *M_current_;
    }

    pointer operator->() const {
        return M_current_;
    }

    normal_iterator &operator++() {
        ++M_current_;
        return *this;
    }

    normal_iterator operator++(int) {
        return normal_iterator(M_current_++);
    }

    normal_iterator &operator--() {
        --M_current_;
        return *this;
    }

    normal_iterator operator--(int) {
        return normal_iterator(M_current_--);
    }

    reference operator[](difference_type n) const {
        return M_current_[n];
    }

    normal_iterator &operator+=(difference_type n) {
        M_current_ += n;
        return *this;
    }

    normal_iterator operator+(difference_type n) const {
        return normal_iterator(M_current_ + n);
    }

    normal_iterator &operator-=(difference_type n) {
        M_current_ -= n;
        return *this;
    }

    normal_iterator operator-(difference_type n) const {
        return normal_iterator(M_current_ - n);
    }

    const Iterator &base() const {
        return M_current_;
    }

protected:
    Iterator M_current_;
};

template <typename T, typename U, typename Container>
inline bool operator==(const normal_iterator<T, Container> &lhs,
                       const normal_iterator<U, Container> &rhs) {
    return lhs.base() == rhs.base();
}

template <typename T, typename U, typename Container>
inline bool operator!=(const normal_iterator<T, Container> &lhs,
                       const normal_iterator<U, Container> &rhs) {
    return lhs.base() != rhs.base();
}

template <typename T, typename U, typename Container>
inline bool operator<(const normal_iterator<T, Container> &lhs,
                      const normal_iterator<U, Container> &rhs) {
    return lhs.base() < rhs.base();
}

template <typename T, typename U, typename Container>
inline bool operator<=(const normal_iterator<T, Container> &lhs,
                       const normal_iterator<U, Container> &rhs) {
    return lhs.base() <= rhs.base();
}

template <typename T, typename U, typename Container>
inline bool operator>(const normal_iterator<T, Container> &lhs,
                      const normal_iterator<U, Container> &rhs) {
    return lhs.base() > rhs.base();
}

template <typename T, typename U, typename Container>
inline bool operator>=(const normal_iterator<T, Container> &lhs,
                       const normal_iterator<U, Container> &rhs) {
    return lhs.base() >= rhs.base();
}

template <typename T, typename U, typename Container>
inline decltype(auto) operator-(const normal_iterator<T, Container> &lhs,
                                const normal_iterator<U, Container> &rhs) {
    return lhs.base() - rhs.base();
}

template <typename T, typename Container>
inline normal_iterator<T, Container> operator+(
    typename normal_iterator<T, Container>::difference_type n,
    const normal_iterator<T, Container> &i) {
    return normal_iterator<T, Container>(i.base() + n);
}

template <typename Iterator>
class reverse_iterator : public iterator<typename iterator_traits<Iterator>::iterator_category,
                                         typename iterator_traits<Iterator>::value_type,
                                         typename iterator_traits<Iterator>::difference_type,
                                         typename iterator_traits<Iterator>::pointer,
                                         typename iterator_traits<Iterator>::reference> {
    using traits_type = iterator_traits<Iterator>;

public:
    using iterator_type = Iterator;
    using reference = typename traits_type::reference;
    using pointer = typename traits_type::pointer;
    using difference_type = typename traits_type::difference_type;
    constexpr reverse_iterator() : current_() {}

    explicit constexpr reverse_iterator(iterator_type x) : current_(x) {}

    constexpr reverse_iterator(const reverse_iterator &x) : current_(x.current_) {}

    reverse_iterator &operator=(const reverse_iterator &) = default;

    template <typename Iter>
    reverse_iterator(const reverse_iterator<Iter> &x) : current_(x.base()) {}

    constexpr iterator_type base() const {
        return current_;
    }

    constexpr reference operator*() const {
        Iterator tmp = current_;
        return *--tmp;
    }
    constexpr pointer operator->() const
    {
        Iterator tmp = current_;
        --tmp;
        return to_pointer(tmp);
    }

    constexpr reverse_iterator &operator++() {
        --current_;
        return *this;
    }

    constexpr reverse_iterator operator++(int) {
        reverse_iterator tmp = *this;
        --current_;
        return tmp;
    }

    constexpr reverse_iterator operator--(int) {
        reverse_iterator tmp = *this;
        ++current_;
        return tmp;
    }

    constexpr reverse_iterator &operator--() {
        ++current_;
        return *this;
    }

    constexpr reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current_ - n);
    }

    constexpr reverse_iterator &operator+=(difference_type n) {
        current_ -= n;
        return *this;
    }
    constexpr reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current_ + n);
    }

    constexpr reverse_iterator &operator-=(difference_type n) {
        current_ += n;
        return *this;
    }

    constexpr reference operator[](difference_type n) const {
        return *(*this + n);
    }

private:
    template <typename T>
    static constexpr T *to_pointer(T *p) {
        return p;
    }

    template <typename T>
    static constexpr pointer to_pointer(T t) {
        return t.operator->();
    }


protected:
    iterator_type current_;
};

template<typename T>
constexpr inline bool operator==(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() == y.base();
}

template<typename T>
constexpr inline bool operator!=(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() != y.base();
}

template<typename T>
constexpr inline bool operator<(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() < y.base();
}
   
template<typename T>
constexpr inline bool operator<=(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() <= y.base();
}

template<typename T>
constexpr inline bool operator>(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() > y.base();
}

template<typename T>
constexpr inline bool operator>=(const reverse_iterator<T> &x,const reverse_iterator<T> &y)
{
    return x.base() >= y.base();
}


template<typename T,typename U>
constexpr inline bool operator==(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() == y.base();
}

template<typename T,typename U>
constexpr inline bool operator!=(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() != y.base();
}

template<typename T,typename U>
constexpr inline bool operator<(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() < y.base();
}
   
template<typename T,typename U>
constexpr inline bool operator<=(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() <= y.base();
}

template<typename T,typename U>
constexpr inline bool operator>(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() > y.base();
}

template<typename T,typename U>
constexpr inline bool operator>=(const reverse_iterator<T> &x,const reverse_iterator<U> &y)
{
    return x.base() >= y.base();
}        
        




}   // namespace estd