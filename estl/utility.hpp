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

template<typename... Args>
using index_sequence_for = make_index_sequence<sizeof...(Args)>;

template<typename T,typename U,typename Result>
struct binary_function
{
    using first_argument_type  = T;
    using second_argument_type = U;
    using result_type          = Result;
};

template<typename T,typename Result>
struct unary_function
{
    using arument_type = T;
    using result_type  = Result;
};

template<typename T>
struct plus : binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const
    {
        return x + y;
    }
};

template<typename T>
struct minus : binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const
    {
        return x - y;
    }
};

template<typename T>
struct multiplies : binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const
    {
        return x * y;
    }
};

template<typename T>
struct divides : binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const
    {
        return x / y;
    }
};

template<typename T>
struct modulus : binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const
    {
        return x % y;
    }
};

template<typename T>
struct negate : unary_function<T,T>
{
    T operator()(const T& x) const
    {
        return -x;
    }
};

template<typename T>
struct greater : binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const
    {
        return x > y;
    }
};

template<typename T>
struct less : binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const
    {
        return x < y;
    }
};

template<typename T>
struct greater_equal : binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const
    {
        return x >= y;
    }
};

template<typename T>
struct less_equal : binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y) const
    {
        return x <= y;
    }
};

}