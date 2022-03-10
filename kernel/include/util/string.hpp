#pragma once
#include <stdarg.h>
#include <type.hpp>
namespace eospp::util {

void *memcpy(void *dest, const void *src, estd::size_t n);

void *memmove(void *dest, const void *src, estd::size_t n);

estd::size_t strlen(const char *str);

char *strcpy(char *dest, const char *src);

char *sprintf_num(char *buf,
                  char *last,
                  estd::uint64_t ui64,
                  estd::u_char zero,
                  estd::uintptr_t hexadecimal,
                  estd::uintptr_t width);

char *snprintf(char *buf, estd::size_t max, const char *fmt, ...);

char *vslprintf(char *buf, char *last, const char *fmt, va_list args);

}   // namespace eospp::util