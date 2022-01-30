#pragma once

template<typename T>
class default_deleter
{
    constexpr default_deleter() = default;
    constexpr default_deleter(const default_deleter&) = default;

    void operator()(T *ptr) const 
    {
        static_assert(sizeof(T) > 0,"T must be complete");
        delete ptr;
    }

};

template<typename T>
class default_deleter<T[]>
{
    constexpr default_deleter() = default;
    constexpr default_deleter(const default_deleter&) = default;
    
    void operator()(T *ptr) const
    {
        static_assert(sizeof(T) > 0,"T must be complete");
        delete[] ptr;
    }
};