#pragma once
#include "type.hpp"
namespace std
{

template<typename T,T V>
struct integral_constant
{
    using value_type         = T;
    using type               = integral_constant<T,V>;
    static constexpr T value = V;
    
    constexpr value_type operator()() const noexcept { return value; }
    //cast to value_type
    constexpr operator value_type() const noexcept   { return value;}
};

using true_type  = integral_constant<bool,true>;
using false_type = integral_constant<bool,false>;

template<typename>
struct is_function : public false_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile && > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile & > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile && > : public true_type { };

// is_const
template<typename>
struct is_const : public false_type { };

template<typename T>
struct is_const<T const> : public true_type { };

// is_volatile
template<typename>
struct is_volatile : public false_type { };

template<typename T>
struct is_volatile<T volatile> : public true_type { };

// remove_const
template<typename T>
struct remove_const { using type = T; };

template<typename T>
struct remove_const<T const> { using type = T;};

template<typename T>
using remove_const_t = typename remove_const<T>::type;

// remove_volatile
template<typename T>
struct remove_volatile { using type = T;};

template<typename T>
struct remove_volatile<T volatile> { using type = T; };

template<typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

// remove_cv
template<typename T>
struct remove_cv
{
    using type = remove_const_t<remove_volatile_t<T>>;
};

template<typename T>
using remove_cv_t = typename remove_cv<T>::type;

// add_const
template<typename T>
struct add_const { using type = T const; };

template<typename T>
using add_const_t = typename add_const<T>::type;
// add_volatile
template<typename T>
struct add_volatile { using type = T volatile;};

template<typename T>
using add_volatile_t = typename add_volatile<T>::type;

// add_cv
template<typename T>
struct add_cv
{
    using type = add_volatile_t<add_const_t<T>>;
};

template<typename T>
using add_cv_t  = typename add_cv<T>::type;

template<typename T1,typename T2>
struct is_same : false_type {};

template<typename T>
struct is_same<T,T> : true_type {};

template<typename T>
struct is_pointer : false_type {};

template<typename T>
struct is_pointer<T*> : true_type{};

template<typename T>
struct is_reference : false_type{};

template<typename T>
struct is_reference<T&> : true_type{};

template<typename T>
struct is_reference<T&&> : true_type{};

template<typename T>
struct is_array : false_type {};

template<typename T>
struct is_array<T[]> : true_type {};

template<typename T,size_t N>
struct is_array<T[N]> : true_type {};


template<bool,typename T = void>
struct enable_if{};

template<typename T>
struct enable_if<true,T> { using type = T;};

template<bool V,typename T>
using enable_if_t = typename enable_if<V,T>::type;

template<typename ...Args>
using void_t = void;


}
