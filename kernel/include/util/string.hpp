#pragma once
#include <stdarg.h>
#include <type.hpp>
namespace eospp::util {

void *memcpy(void *dest,const void *src,estd::size_t n);

void *memmove(void *dest,const void *src,estd::size_t n);

char* strcpy(char *dest,const char *src);

char* strncpy(char *dest,const char *src,estd::size_t n);

int strcmp(const char *s1,const char* s2);

estd::u_char *sprintf_num(estd::u_char *buf,estd::u_char *last,estd::uint64_t ui64,estd::u_char zero,estd::uintptr_t width);

estd::u_char *snprintf(estd::u_char *buf,estd::size_t max,const char *fmt,...);

estd::u_char *vslprintf(estd::u_char *buf,estd::u_char *last,const char *fmt,va_list args);

}   // namespace eospp::util