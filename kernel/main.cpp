#include <io/vesa.hpp>
extern "C" int main()
{
    using namespace eospp;
    
    io::Vesa::Init();
    io::printf("hello eospp!\n");

    while(1){}
    return 0;
}