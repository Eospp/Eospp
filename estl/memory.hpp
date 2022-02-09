#pragma once
#include <type_traits.hpp>
#include <tuple.hpp>
namespace estd{

template<typename T>
class default_deleter
{
public:
    constexpr default_deleter() = default;

     /** @brief Converting constructor.
      *
      * Allows conversion from a deleter for arrays of another type, @p U,
      * only if @p U* is convertible to @p T*.
      */
    template<typename U, typename = enable_if_t<is_convertible_v<U*, T*>> >
    default_deleter(const default_deleter<U> &) noexcept {}
    
    void operator()(T *ptr) const 
    {
        static_assert(sizeof(T) > 0,"T must be complete");
        static_assert(!is_void_v<T>,"can't delete pointer to incomplete type");
        delete ptr;
    }

};

template<typename T>
class default_deleter<T[]>
{
public:
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


template<typename T,typename D,typename = void>
struct ptr_type
{
    using type = T*;
};

template<typename T,typename D>
struct ptr_type<T,D,estd::void_t<typename estd::remove_reference_t<D>::pointer> >
{
    using type = typename estd::remove_reference_t<D>::pointer;
};

template<typename T,typename D>
class unique_ptr_impl
{
public:
     using pointer = typename ptr_type<T,D>::type;

     unique_ptr_impl() = default;
     unique_ptr_impl(pointer p) 
     {
         ptr() = p;
     }
     template<typename Deleter>
     unique_ptr_impl(pointer p,Deleter &&d) : data_(p,estd::forward<Deleter>(d)){}

     pointer& ptr()            { return estd::get<0>(data_);}
     pointer  ptr() const      { return estd::get<0>(data_);}
     D&       deleter ()       { return estd::get<1>(data_);}
     const D& deleter () const { return estd::get<1>(data_);}
private:
     estd::tuple<pointer,D> data_;
};

template<typename T,typename D = default_deleter<T>>
class unique_ptr
{
public:
       using pointer      = typename unique_ptr_impl<T,D>::pointer;
       using element_type = T;
       using deleter_type = D;
    
       constexpr unique_ptr() noexcept{}

       explicit  unique_ptr(pointer p) noexcept : impl_(p){}

       unique_ptr(pointer p,
                  estd::conditional_t<is_reference_v<D>,D,const D&> d) : impl_(p,d)
       {}
       unique_ptr(pointer p,remove_reference_t<D> &&d) noexcept : impl_(estd::move(p),estd::move(d))
       {
            static_assert(!estd::is_reference_v<deleter_type>,"rvalue deleter bound to reference");
       }
       
       unique_ptr(unique_ptr &&p) noexcept : impl_(p.release(),estd::forward<deleter_type>(p.get_deleter())){}

       ~unique_ptr() noexcept
       {
           auto &ptr = impl_.ptr();
           if(ptr != nullptr)
             impl_.deleter()(ptr);  
           ptr = pointer();      
       }

       unique_ptr& operator=(unique_ptr &&p) noexcept
       {
           reset(p.release());
           get_deleter() = estd::forward<deleter_type>(p.get_deleter());
           return *this;
       }

       unique_ptr& operator=(nullptr_t) noexcept
       {
           reset();
           return *this;
       }

       estd::add_lvalue_reference_t<element_type> operator*() const 
       {
           return *get();
       }
       pointer operator->() const noexcept
       {
           return get();
       }
       pointer get() const noexcept
       {
           return impl_.ptr();
       }

       deleter_type& get_deleter() noexcept
       {
            return impl_.deleter();
       }

       const deleter_type& get_deleter() const noexcept
       {
           return impl_.deleter();
       }

       explicit operator bool() const noexcept
       {
           return get() == pointer() ? false : true;
       }

       pointer release() noexcept
       {
           pointer p   = get();
           impl_.ptr() = pointer();
           return p;
       }

       void reset(pointer p = pointer()) noexcept
       {
           estd::swap(impl_.ptr(),p);
           if(p != pointer())
             get_deleter()(p);
       }

       void swap(unique_ptr &p) noexcept
       {
           estd::swap(impl_,p.impl_);
       }

       unique_ptr(const unique_ptr&)            = delete;
       unique_ptr& operator=(const unique_ptr&) = delete;

private:
       unique_ptr_impl<T,D> impl_;
};

template<typename T,typename D>
class unique_ptr<T[],D>
{
public:
       using pointer      = typename unique_ptr_impl<T,D>::pointer;
       using element_type = T;
       using deleter_type = D;
    
       constexpr unique_ptr() noexcept{}

       explicit  unique_ptr(pointer p) noexcept : impl_(p){}

       unique_ptr(pointer p,
                  estd::conditional_t<is_reference_v<D>,D,const D&> d) : impl_(p,d)
       {}
       unique_ptr(pointer p,remove_reference_t<D> &&d) noexcept : impl_(estd::move(p),estd::move(d))
       {
       }
       
       unique_ptr(unique_ptr &&p) noexcept : impl_(p.release(),estd::forward<deleter_type>(p.get_deleter())){}

       ~unique_ptr() noexcept
       {
           auto &ptr = impl_.ptr();
           if(ptr != nullptr)
             impl_.deleter()(ptr);  
           ptr = pointer();      
       }

       unique_ptr& operator=(unique_ptr &&p) noexcept
       {
           reset(p.release());
           get_deleter() = estd::forward<deleter_type>(p.get_deleter());
           return *this;
       }

       unique_ptr& operator=(nullptr_t) noexcept
       {
           reset();
           return *this;
       }

       estd::add_lvalue_reference_t<element_type> operator*() const 
       {
           return *get();
       }
       pointer operator->() const noexcept
       {
           return get();
       }
       pointer get() const noexcept
       {
           return impl_.ptr();
       }

       deleter_type& get_deleter() noexcept
       {
            return impl_.deleter();
       }

       const deleter_type& get_deleter() const noexcept
       {
           return impl_.deleter();
       }

       explicit operator bool() const noexcept
       {
           return get() == pointer() ? false : true;
       }

       pointer release() noexcept
       {
           pointer p   = get();
           impl_.ptr() = pointer();
           return p;
       }

       estd::add_lvalue_reference_t<element_type> operator[](estd::size_t i) const
       {
           return get()[i];
       }

       void reset(pointer p = pointer()) noexcept
       {
           estd::swap(impl_.ptr(),p);
           if(p != pointer())
             get_deleter()(p);
       }

       void swap(unique_ptr &p) noexcept
       {
           estd::swap(impl_,p.impl_);
       }

       unique_ptr(const unique_ptr&)            = delete;
       unique_ptr& operator=(const unique_ptr&) = delete;

private:
       unique_ptr_impl<T,D> impl_;
};


template<typename T,typename... Args>
inline enable_if_t<!is_array_v<T>,estd::unique_ptr<T>> 
make_unique(Args&&... args)
{
    return estd::unique_ptr<T>(new T(estd::forward<Args>(args)...));
}

template<typename T>
inline enable_if_t<is_array_v<T>,estd::unique_ptr<T>>
make_unique(estd::size_t size)
{
    return estd::unique_ptr<T>(new remove_extent_t<T>[size]());
}

template<typename T,typename D>
inline bool operator==(const estd::unique_ptr<T,D> &x,nullptr_t) noexcept
{
    return !x;
}

template<typename T,typename D>
inline bool operator==(nullptr_t,const estd::unique_ptr<T,D> &x) noexcept
{
    return !x;
}

template<typename T,typename D>
inline bool operator!=(const estd::unique_ptr<T,D> &x,nullptr_t) noexcept
{
    return static_cast<bool>(x);
}

template<typename T,typename D>
inline bool operator!=(nullptr_t,const estd::unique_ptr<T,D> &x) noexcept
{
    return static_cast<bool>(x);
}

template<typename T,typename D,typename U,typename E>
inline bool operator!=(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<U,E> &rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T,typename D,typename U,typename E>
inline bool operator==(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<U,E> &rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T,typename D>
inline bool operator<(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<T,D> &rhs)
{
    return estd::less<typename estd::unique_ptr<T,D>::pointer>()(lhs.get(),rhs.get());
}

template<typename T,typename D>
inline bool operator<=(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<T,D> &rhs)
{
    return estd::less_equal<typename estd::unique_ptr<T,D>::pointer>()(lhs.get(),rhs.get());
}

template<typename T,typename D>
inline bool operator>(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<T,D> &rhs)
{
    return estd::greater<typename estd::unique_ptr<T,D>::pointer>()(lhs.get(),rhs.get());
}

template<typename T,typename D>
inline bool operator>=(const estd::unique_ptr<T,D> &lhs,const estd::unique_ptr<T,D> &rhs)
{
    return estd::greater_equal<typename estd::unique_ptr<T,D>::pointer>()(lhs.get(),rhs.get());
}

}