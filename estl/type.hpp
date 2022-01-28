#pragma once

using uint64_t = unsigned long long;
using uint32_t = unsigned int;
using uint16_t = unsigned short;
using uint8_t  = unsigned char;
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