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
forward(typename estd::remove_reference_t<T>&& t) noexcept
{
    static_assert(!estd::is_lvalue_reference<T>::value, "cant' cast rvalue refernce to lvalue reference");

    return static_cast<T&&>(t);
}

template<typename T>
constexpr estd::remove_reference_t<T>&& move(T &&t)
{
    return static_cast<estd::remove_reference_t<T>&&>(t);
}

template<typename T>
add_rvalue_reference_t<T> declval();

template<typename T>
void swap(T &lhs,T &rhs)
{
    T tmp(estd::move(lhs));
    lhs = estd::move(rhs);
    rhs = estd::move(tmp);
}

}