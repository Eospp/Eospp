#pragma once
#include <type_traits.hpp>

namespace estd{

template<typename T>
constexpr T&&
forward(estd::remove_reference_t<T>& t) noexcept
{    
    return static_cast<T&&>(t); 
}

template<typename T>
constexpr T&&
forward(estd::remove_reference_t<T>&& t) noexcept
{
    static_assert(!estd::is_lvalue_reference<T>::value, "cant' cast rvalue refernce to lvalue reference");

    return static_cast<T&&>(t);
}

template<typename T>
constexpr estd::remove_reference_t<T>&& move(T &&t)
{
    return static_cast<estd::remove_reference_t<T>&&>(t);
}

class NoCopyAble
{
public:
     NoCopyAble()  = default;
     ~NoCopyAble() = default;

     NoCopyAble(const NoCopyAble&) = delete;

     NoCopyAble& operator=(const NoCopyAble&) = delete;
     NoCopyAble& operator=(const NoCopyAble&) const = delete;
     NoCopyAble& operator=(const NoCopyAble&) volatile = delete;
};


template<typename T>
inline void swap(T &lhs,T &rhs)
{
    T tmp(estd::move(lhs));
    lhs = estd::move(rhs);
    rhs = estd::move(tmp);
}

template<typename T,T... idx>
struct integer_sequence
{
    using value_type = T;
    static constexpr size_t size() noexcept { return sizeof...(idx); }
};

template<size_t... idx>
using index_sequence = integer_sequence<size_t,idx...>;

template<typename T,T num>
using make_integer_sequence = integer_sequence<T,__integer_pack(num)...>;

template<size_t num>
using make_index_sequence = make_integer_sequence<size_t,num>;

}