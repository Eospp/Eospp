#pragma once
#include <utility.hpp>
#include <type_traits.hpp>

namespace estd{

template<typename... Args>
class tuple;

template<size_t N,typename... Args>
struct tuple_element;

template<size_t N,typename... Args>
struct tuple_element<N,tuple<Args...>>
{
    using type = nth_type_t<N,Args...>;
};

template<size_t N,typename... Args>
using tuple_element_t = typename tuple_element<N,Args...>::type;

template<size_t I,typename... Args>
nth_type_t<I,Args...>& get(tuple<Args...> &t) noexcept;

template<size_t I,typename... Args>
const nth_type_t<I,Args...>& get(const tuple<Args...> &t) noexcept;

template<size_t I,typename... Args>
nth_type_t<I,Args...>&& get(tuple<Args...> &&t) noexcept;

using swallow = bool[];

template<typename... Args>
inline void swap(tuple<Args...> &lhs,tuple<Args...> &rhs)
{
    swap_impl(lhs,rhs,index_sequence_for<Args...>());
}

template<size_t I,typename T,bool = is_empty_v<T> && !is_final_v<T>>
class tuple_value
{
public:
    constexpr tuple_value() = default;

    tuple_value(const T &v) : value(v){}

    tuple_value(T &&v) : value(estd::move(v)){}

    template<typename... Args>
    tuple_value(Args&&... args) : value(estd::forward<Args>(args)...){}

    tuple_value(const tuple_value&) = default;
    tuple_value(tuple_value&&)      = default;

    constexpr T& get() noexcept
    {
        return value;
    }
    constexpr const T& get() const noexcept
    {
        return value;
    }
private:
    T value;
};

template<size_t I,typename T>
class tuple_value<I,T,true> : private T
{
public:
       constexpr tuple_value(){}

       tuple_value(const T& v) : T(v){}

       template<typename... Args>
       tuple_value(Args&&... args) : T(estd::forward<Args>(args)...){}

       tuple_value(const tuple_value&) = default;
       tuple_value(tuple_value&&)      = default;

       constexpr T &get() noexcept
       {
           return static_cast<T&>(*this);
       }
       constexpr const T &get() const noexcept
       {
           return static_cast<const T&>(*this);
       }
};

template<typename T,typename... Args>
class tuple_impl;

template<size_t ...I,typename ...T>
class tuple_impl<estd::index_sequence<I...>,T...> : public tuple_value<I,T>...
{
public:
     constexpr tuple_impl()                  = default;
     constexpr tuple_impl(const tuple_impl&) = default;
     constexpr tuple_impl(tuple_impl&&)      = default;

     template<typename... Args,
              typename = estd::enable_if_t<sizeof...(Args) == sizeof...(T)>,
              typename = estd::void_t<estd::enable_if_t<estd::is_constructible_v<Args,T>>...> >
     constexpr explicit tuple_impl(Args&&... values) : tuple_value<I,T>(estd::forward<Args>(values))...
     {

     }
     template<typename... Args>
     constexpr explicit tuple_impl(const tuple<Args...> &values) : tuple_value<I,T>(estd::get<I>(values))...
     {

     }
     template<typename... Args>
     constexpr explicit tuple_impl(tuple<Args...> &&values) : tuple_value<I,T>(estd::move(estd::get<I>(values)))...
     {

     }

     tuple_impl& operator=(const tuple_impl &t)
     {
         swallow{(tuple_value<I,T>::operator=(static_cast<tuple_value<I,T>&>(t).get()),true)...};
         return *this;
     }

     tuple_impl& operator=(tuple_impl &&t)
     {
         swallow{(tuple_value<I,T>::operator=(move(static_cast<tuple_value<I,T>>(t).get())),true)...};
         return *this;
     }
   
};


template<typename... T>
class tuple
{
public:
      using value_type = tuple_impl<make_index_sequence<sizeof...(T)>,T...>;

      tuple(const tuple&) = default;
      tuple(tuple&&)      = default;
      constexpr tuple()   = default;
      constexpr tuple(const T& ...values) : impl_(values...){}


      template<typename... Args,
               typename = estd::enable_if_t<estd::is_constructible_v<value_type,Args...>> >
      constexpr tuple(Args&& ...values) : impl_(estd::forward<Args>(values)...)
      {

      }

      template<typename... Args>
      constexpr tuple(const tuple<Args...> &t) : impl_(t)
      {

      }

      template<typename... Args>
      constexpr tuple(tuple<Args...> &&t) : impl_(move(t))
      {

      }
      constexpr size_t size() const noexcept
      {
          return sizeof...(T);
      }

      tuple& operator=(const tuple &t)
      {
          tuple(t).swap(*this);
          return *this;
      }
      tuple& operator=(tuple &&t)
      {
          tuple(estd::move(t)).swap(*this);
          return *this;
      }
      void swap(tuple &t)
      {
          estd::swap(*this,t);
      }

      template <size_t I, typename... Args>
      friend nth_type_t<I, Args...> &get(tuple<Args...> &t) noexcept;

      template <size_t I, typename... Args>
      friend const nth_type_t<I, Args...> &get(const tuple<Args...> &t) noexcept;

      template <size_t I, typename... Args>
      friend nth_type_t<I, Args...> &&get(tuple<Args...> &&t) noexcept;

private:
      value_type impl_;
};

template<size_t I,typename... Args>
nth_type_t<I,Args...>& get(tuple<Args...> &t) noexcept
{
    return static_cast<tuple_value<I,nth_type_t<I,Args...>>&>(t.impl_).get();
}

template<size_t I,typename... Args>
const nth_type_t<I,Args...>& get(const tuple<Args...> &t) noexcept
{
    return static_cast<const tuple_value<I,nth_type_t<I,Args...>>&>(t.impl_).get();
}

template<size_t I,typename... Args>
nth_type_t<I,Args...>&& get(tuple<Args...> &&t) noexcept
{
    using type = nth_type_t<I,Args...>&&;

    return static_cast<type>(static_cast<tuple_value<I,type>>(t.impl_).get());
}

template<typename... Args>
inline tuple<Args...> make_tuple(Args&& ...args)
{
    return tuple<Args...>(forward<Args>(args)...);
}

template<typename T,size_t... I>
static inline void swap_impl(T &lhs,T &rhs,index_sequence<I...>)
{
    swallow{(swap(get<I>(lhs),get<I>(rhs)),true)...};
}



}
