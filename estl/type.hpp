#pragma once
namespace estd{

    // using uint64_t = unsigned long long;
    // using uint32_t = unsigned int;
    // using uint16_t = unsigned short int;
    // using uint8_t  = unsigned char;
    typedef unsigned int uint8_t __attribute__((__mode__(__QI__))); ///< An unsigned 8-bit number
    typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__))); ///< An unsigned 16-bit number
    typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__))); ///< An unsigned 32-bit number
    typedef unsigned int uint64_t __attribute__ ((__mode__ (__DI__))); ///< An unsigned 64-bit number
    using u_char   = unsigned char;
    using uintptr_t= unsigned long int;
    using intptr_t = long int;
    using size_t   = uint64_t;

    static_assert(sizeof(uint64_t) == 8,"sizeof(uint64_t) != 8 bytes!");
    static_assert(sizeof(uint32_t) == 4,"sizeof(uint32_t) != 4 bytes!");
    static_assert(sizeof(uint16_t) == 2,"sizeof(uint16_t) != 2 bytes!");
    static_assert(sizeof(uint8_t)  == 1,"sizeof(uint8_t)  != 1 bytes!");

    using int64_t  = long long;
    using int32_t  = int;
    using int16_t  = short;
    using int8_t   = char;

    static_assert(sizeof(int64_t) == 8,"sizeof(int64_t) != 8 bytes!");
    static_assert(sizeof(int32_t) == 4,"sizeof(int32_t) != 4 bytes!");
    static_assert(sizeof(int16_t) == 2,"sizeof(int16_t) != 2 bytes!");
    static_assert(sizeof(int8_t)  == 1,"sizeof(int8_t)  != 1 bytes!");

    using nullptr_t = decltype(nullptr);

}