#pragma once
#include <type.hpp>
namespace eospp {


extern "C"{
    extern void _arch_enable_sse();
}

class Arch {
public:
    static estd::size_t getRflags() {
        estd::size_t rflags;
        asm volatile("pushfq; pop %0;"
                     : "=m"(rflags));
        return rflags;
    }

    static void DisableHwint(estd::size_t &rflags){
        asm volatile("pushfq; pop %0; cli;" : "=m"(rflags));

    }

    static void EnableHwint(estd::size_t &rflags){
        asm volatile("push %0;popfq;" : :"m"(rflags));
    }

    static bool EnabledIrq(){
        estd::size_t rflags;
        asm volatile("pushfq; pop %0;"
                     : "=g"(rflags));
        return rflags & 0x200;
    }

    static void EnableSse(){
        _arch_enable_sse();
    }
};

} // namespace eospp
