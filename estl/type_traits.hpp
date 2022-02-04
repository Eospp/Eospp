#pragma once
#include <type.hpp>
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

template<bool V>
using bool_constant = integral_constant<bool,V>;

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

template<typename T>
constexpr inline bool is_function_v = is_function<T>::value;

// is_const
template<typename>
struct is_const : public false_type { };

template<typename T>
struct is_const<T const> : public true_type { };

template<typename T>
constexpr inline bool is_const_v = is_const<T>::value;

// is_volatile
template<typename>
struct is_volatile : public false_type { };

template<typename T>
struct is_volatile<T volatile> : public true_type { };

template<typename T>
constexpr inline bool is_volatile_v = is_volatile<T>::value;
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
constexpr inline bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template<typename T>
struct is_rvalue_reference : false_type{};

template<typename T>
struct is_rvalue_reference<T&&> : true_type{};

template<typename T>
constexpr inline bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template<typename T1,typename T2>
struct is_same : false_type {};

template<typename T>
struct is_same<T,T> : true_type {};

template<typename T1,typename T2>
constexpr inline bool is_same_v = is_same<T1,T2>::value;


template<typename T>
struct is_pointer : false_type {};

template<typename T>
struct is_pointer<T*> : true_type{};

template<typename T>
constexpr inline bool is_pointer_v = is_pointer<T>::value;

template<typename T>
struct is_reference : false_type{};

template<typename T>
struct is_reference<T&> : true_type{};

template<typename T>
struct is_reference<T&&> : true_type{};

template<typename T>
constexpr inline bool is_reference_v = is_reference<T>::value;

template<typename T>
struct is_array : false_type {};

template<typename T>
struct is_array<T[]> : true_type {};

template<typename T,size_t N>
struct is_array<T[N]> : true_type {};

template<typename T>
constexpr inline bool is_array_v = is_array<T>::value;

template<typename T>
struct is_member_pointer : false_type {};

template<typename T,typename C>
struct is_member_pointer<T C::*> : true_type {};

template<typename T>
constexpr inline bool is_member_pointer_v = is_member_pointer<T>::value;

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

template<typename T>
add_rvalue_reference_t<T> declval();

template<typename... T>
struct or_;

template<>
struct or_<> : public false_type{};

template<typename T>
struct or_<T> : public T{};

template<typename T1,typename T2>
struct or_<T1,T2> : public conditional_t<T1::value,T1,T2>{};

template<typename T1,typename T2,typename T3,typename... Args>
struct or_<T1,T2,T3,Args...> : public conditional_t<T1::value,T1,or_<T2,T3,Args...>>{};

template<typename... Args>
constexpr inline bool or_v = or_<Args...>::value;

template<typename... Args>
struct and_;

template<>
struct and_<> : public true_type{};

template<typename T>
struct and_<T> : public T{};

template<typename T1,typename T2>
struct and_<T1,T2> : public conditional_t<T1::value,T1,T2>{};

template<typename T1,typename T2,typename T3,typename ...Args>
struct and_<T1,T2,T3,Args...> : public conditional_t<T1::value,T1,and_<T2,T3,Args...>>{};

template<typename... Args>
constexpr inline bool and_v = and_<Args...>::value;

template<typename T>
struct not_ : public bool_constant<!T::value>{};

template<typename,typename T,typename ...Args>
struct is_convertible_impl : false_type{};

template<typename T,typename... Args>
struct is_convertible_impl<void_t<decltype(T(declval<Args>()...))>,T,Args...> : true_type{};

template<typename T,typename... Args>
struct is_convertible : is_convertible_impl<void,T,Args...>{};

template<typename T,typename... Args>
constexpr inline bool is_convertible_v = is_convertible<T,Args...>::value;

template<typename T,typename... Args>
struct is_constructible : public bool_constant<__is_constructible(T,Args...)>{};

template<typename T,typename... Args>
constexpr inline bool is_constructible_v = is_constructible<T,Args...>::value;

template<typename T>
struct is_default_constructible : public is_constructible<T>{};

template<typename T>
constexpr inline bool is_default_constructible_v = is_default_constructible<T>::value;

template<typename... Args>
struct conjunction : and_<Args...>{};

template<typename... Args>
struct disconjunction : or_<Args...>{};

template<typename... Args>
constexpr inline bool conjunction_v = conjunction<Args...>::value;

template<typename... Args>
constexpr inline bool disconjunction_v = disconjunction<Args...>::value;

template<typename T>
struct is_enum : public bool_constant<__is_enum(T)>{};

template<typename T>
struct is_class : public bool_constant<__is_class(T)>{};

template<typename T>
struct is_union : public bool_constant<__is_union(T)>{};

template<typename T>
constexpr inline bool is_enum_v = is_enum<T>::value;

template<typename T>
constexpr inline bool is_class_v = is_class<T>::value;

template<typename T>
constexpr inline bool is_union_v = is_union<T>::value;

template<typename T>
struct is_integral_helper : false_type{};

template<>
struct is_integral_helper<int8_t> : true_type{};

template<>
struct is_integral_helper<int16_t> : true_type{};

template<>
struct is_integral_helper<int32_t> : true_type{};

template<>
struct is_integral_helper<int64_t> : true_type{};

template<>
struct is_integral_helper<uint8_t> : true_type{};

template<>
struct is_integral_helper<uint16_t> : true_type{};

template<>
struct is_integral_helper<uint32_t> : true_type{};

template<>
struct is_integral_helper<uint64_t> : true_type{};

template<typename T>
struct is_integral : public is_integral_helper<remove_cv_t<T>>{};

template<typename T>
constexpr inline bool is_integral_v = is_integral<T>::value;

template<typename T>
struct is_float_helper : false_type{};

template<>
struct is_float_helper<float> : true_type{};

template<>
struct is_float_helper<double> : true_type{};

template<>
struct is_float_helper<long double> : true_type{};

template<typename T>
struct is_float : is_float_helper<remove_cv_t<T>>{};

template<typename T>
constexpr inline bool is_float_v = is_float<T>::value;

template<typename T>
struct is_void_helper : false_type{};

template<>
struct is_void_helper<void> : true_type{};

template<typename T>
struct is_void : is_void_helper<remove_cv_t<T>>{};

template<typename T>
constexpr inline bool is_void_v = is_void<T>::value;

template<typename T>
struct is_empty : bool_constant<__is_empty(T)>{};

template<typename T>
constexpr inline bool is_empty_v = is_empty<T>::value;

template<typename T>
struct is_abstract : bool_constant<__is_abstract(T)>{};

template<typename T>
constexpr inline bool is_abstract_v = is_abstract<T>::value;

template<typename T>
struct is_final : bool_constant<__is_final(T)>{};

template<typename T>
constexpr inline bool is_final_v = is_final<T>::value;


}
