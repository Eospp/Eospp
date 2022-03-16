#include <io/vesa.hpp>
#include <core/interrupt.hpp>
#include <core/physical_allocator.hpp>
extern "C" int main()
{
    using namespace eospp;
    asm volatile("andq $-16, %rsp");
    Arch::EnableSse();
    io::Vesa::Init();
    core::Interrupt::Init();
    io::printf("enable irq %d\n",Arch::EnabledIrq());
    double n  = 3.0 * 1 / 2;
    memory::init();
    while(1){
        
    }
    return 0;
}