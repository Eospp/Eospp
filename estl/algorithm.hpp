#pragma once
#include <type.hpp>
namespace estd {

template <typename T>
inline constexpr const T& min(const T& lhs, const T& rhs) {
    return lhs < rhs ? lhs : rhs;
}

template <typename T>
inline constexpr const T& max(const T& lhs, const T& rhs) {
    return lhs > rhs ? lhs : rhs;
}

inline size_t cstrlen(const char* str) {
    size_t n = 0;
    while (*(str++) != '\0')
        n++;
    return n;
}

template<typename Iterator,typename T>
void fill(Iterator first,Iterator end,const T &value){

    while(first != end){
        *(first++) = value;
    }

}

template<typename Iterator,typename T>
void fill_n(Iterator first,size_t count,const T &value){

    while(count-- > 0){
        *(first++) = value;
    }

}


} // namespace estd