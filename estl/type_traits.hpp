#pragma once
#include "type.hpp"
namespace estd
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
struct is_function : public false_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) > : public true_type { };

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) volatile && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) volatile && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes...) const volatile && > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile & > : public true_type {};

template<typename Res, typename... ArgTypes >
struct is_function<Res(ArgTypes......) const volatile && > : public true_type {};

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

template<typename T>
struct remove_reference
{
    using type = T;
};

template<typename T>
struct remove_reference<T&>
{
    using type = T;
};

template<typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

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

template<typename T>
struct is_lvalue_reference : false_type{};

template<typename T>
struct is_lvalue_reference<T&> : true_type{};

template<typename T>
struct is_rvalue_reference : false_type{};

template<typename T>
struct is_rvalue_reference<T&&> : true_type{};

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

template<bool V,typename T = void>
using enable_if_t = typename enable_if<V,T>::type;

template<bool V,typename T,typename U>
struct conditional
{
    using type = T;
};

template<typename T,typename U>
struct conditional<false,T,U>
{
    using type = U;
};

template<bool V,typename T,typename U>
using conditional_t = typename conditional<V,T,U>::type;

template<typename ...Args>
using void_t = void;


template<typename T,typename U = void>
struct add_rvalue_reference
{
    using type = T&&;
};

template<typename T>
struct add_rvalue_reference<T,enable_if_t<is_rvalue_reference<T>::value>>
{
    using type = T;
};

template<typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template<typename T,typename U = void>
struct add_lvalue_reference
{
    using type = T&;
};

template<typename T>
struct add_lvalue_reference<T,enable_if_t<is_lvalue_reference<T>::value>>
{
    using type = T;
};

template<typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template<typename T>
struct add_pointer
{
    using type = remove_reference_t<T>*;
};

template<typename T>
using add_pointer_t = typename add_pointer<T>::type;

template<typename T>
struct remove_extent
{
    using type = T;
};

template<typename T>
struct remove_extent<T[]>
{
    using type = T;
};

template<typename T,size_t N>
struct remove_extent<T[N]>
{
    using type = T;
};

template<typename T>
using remove_extent_t = typename remove_extent<T>::type;

template<typename T>
struct remove_all_extent
{
    using type = T;
};

template<typename T>
struct remove_all_extent<T[]>
{
    using type = typename remove_all_extent<T>::type;
};

template<typename T,size_t N>
struct remove_all_extent<T[N]>
{
    using type = typename remove_all_extent<T>::type;
};

template<typename T>
using remove_all_extent_t = typename remove_all_extent<T>::type;

template<typename T,
        bool IsArray    = is_array<T>::value,
        bool IsFunction = is_function<T>::value>
struct decay_selector;

template<typename T>
struct decay_selector<T,false,false>
{
    using type = remove_cv_t<T>;
};

template<typename T>
struct decay_selector<T,true,false>
{
    using type = remove_extent_t<T>*;
};

template<typename T>
struct decay_selector<T,false,true>
{
    using type = add_pointer_t<T>;
};

template<typename T>
struct decay
{
    using type = typename decay_selector<remove_reference_t<T>>::type;
};

template<typename T>
using decay_t = typename decay<T>::type;

}
