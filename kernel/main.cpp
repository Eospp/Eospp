#include <io/vesa.hpp>
#include <core/interrupt.hpp>
#include "util/string.hpp"
extern "C" int main()
{
    using namespace eospp;
    io::Vesa::Init();
    io::printf("start eospp\n");
    core::Interrupt::Init();
    int i = 0;
    while(1){
        io::printf("loop %d times\n",i++);
    }
    return 0;
}