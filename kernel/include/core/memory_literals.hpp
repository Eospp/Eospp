#pragma once
#include <type.hpp>
namespace eospp::memory{

constexpr inline estd::size_t operator"" _GB(estd::uint64_t n){
    return n  << 30;
}

constexpr inline estd::size_t operator"" _MB(estd::uint64_t n){
    return n << 20;
}

constexpr inline estd::size_t operator"" _KB(estd::uint64_t n){
    return n << 10;
}

}