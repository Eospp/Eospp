#pragma once
#include <type.hpp>
namespace eospp::io {

inline estd::uint8_t in_byte(estd::uint16_t port) {
    estd::uint8_t v;
    asm volatile("inb %[v] %[port]"
                 : [v] "=a"(v)
                 : [port] "dN"(port)
                 : "memory");
    return v;
}

inline void out_byte(estd::uint16_t _port, estd::uint8_t _data) {
    asm volatile("outb %[data],%[port]"
                 :
                 : [port] "dN"(_port), [data] "a"(_data):);
}

inline estd::uint16_t in_word(estd::uint16_t port) {
    estd::uint16_t v;
    asm volatile("inb %[v] %[port]"
                 : [v] "=a"(v)
                 : [port] "dN"(port)
                 : "memory");
    return v;
}

inline void out_word(estd::uint16_t port, estd::uint16_t data) {
    asm volatile("outw %[data] %[port]"
                 :
                 : [port] "dN"(port), [data] "a"(data));
}

inline estd::uint32_t in_dword(estd::uint16_t port) {
    estd::uint32_t v;
    asm volatile("inl %[v] %[port]"
                 : [v] "=a"(v)
                 : [port] "dN"(port)
                 : "memory");
    return v;
}

inline void out_dword(estd::uint16_t port, estd::uint32_t data) {
    __asm__ __volatile__("outl %[data] %[port]"
                 :
                 : [port] "dN"(port), [data] "a"(data));
}

} // namespace eospp::io