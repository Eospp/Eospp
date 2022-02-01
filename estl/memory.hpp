#pragma once
#include <type_traits.hpp>
namespace estd{

template<typename T>
class default_deleter
{
    constexpr default_deleter() = default;

     /** @brief Converting constructor.
      *
      * Allows conversion from a deleter for arrays of another type, @p U,
      * only if @p U* is convertible to @p T*.
      */
    template<typename U, typename = enable_if_t<is_convertible_v<U*, T*>>>
    default_delete(const default_delete<U>&) noexcept { }
    
    void operator()(T *ptr) const 
    {
        static_assert(sizeof(T) > 0,"T must be complete");
        static_assert(!is_void_v<T>,"can't delete pointer to incomplete type")
        delete ptr;
    }

};

template<typename T>
class default_deleter<T[]>
{
    constexpr default_deleter() = default;
    /** @brief Converting constructor.
     *
     * Allows conversion from a deleter for arrays of another type, such as
     * a const-qualified version of @p T.
     *
     * Conversions from types derived from @c T are not allowed because
     * it is unsafe to @c delete[] an array of derived types through a
     * pointer to the base type.
     */
    template<typename U,typename = enable_if_t<is_convertible_v<U(*)[],T(*)[]>>>
    default_deleter(const default_deleter<U[]>&) noexcept{}

    template<typename U,typename = enable_if_t<is_convertible_v<U(*)[],T(*)[]>>>
    void operator()(U *ptr) const
    {
        static_assert(sizeof(T) > 0,"T must be complete");
        delete[] ptr;
    }
};

}