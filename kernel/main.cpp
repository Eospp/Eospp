#include <io/vesa.hpp>
#include <core/interrupt.hpp>
#include <core/physical_allocator.hpp>
#include <core/e820.hpp>
#include <core/global_obj.hpp>
#include "type.hpp"
extern char _text;
extern char _etext;
extern char _edata;
extern char _end;
class A{
public:
    A(){
        eospp::io::printf("A::A()\n");
    }
};
A obj;

extern estd::uint64_t PDE;

extern "C" int main()
{
    
    using namespace eospp;
    asm volatile("andq $-16, %rsp");
    Arch::EnableSse();
    io::Vesa::Init();
    core::Interrupt::Init();
    memory::mm_struct.start_code = reinterpret_cast<unsigned long>(&_text);
    memory::mm_struct.end_code = reinterpret_cast<unsigned long>(&_etext);
    memory::mm_struct.end_data = reinterpret_cast<unsigned long>(&_edata);
    memory::mm_struct.end_brk = reinterpret_cast<unsigned long>(&_end);
    memory::e820::manager.Init();
    //memory::init_memory();
    global_obj::Init();
    estd::uint64_t *p = reinterpret_cast<estd::uint64_t*>(0xFFFF800000400100);
    *p = 2;
    *(&PDE + 2) &= ~2ul;
    io::printf("*(&PDE + 2) = %xl\n",*(&PDE + 2));
    io::printf("page read only\n");
    *p = 2;
    // io::printf("&end_ = %xl\n",&_end);
    // io::printf("*p = %ul\n",*p);
    // io::printf("p = %xl\n",p);
    io::printf("*p = %ul\n",*p);
    while(1){
        
    }
    return 0;
}