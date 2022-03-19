#pragma once
#include <array.hpp>
#include <type.hpp>
namespace eospp::memory::e820 {

struct Entry {
    estd::uint64_t address;
    estd::uint64_t length;
    estd::uint32_t type;
}__attribute__((packed));

enum E820_TYPE{
    Free = 1,
    Reserved,
    ACPI = 4,
    Unusable,
    Disabled,
    Kernel,
    Unkown,
};

class Manager {
public:
    static constexpr estd::size_t max_size  = 20;
    static constexpr estd::size_t e820_addr = 0xFFFF800000007E00;
    
    static const char* E280ToStr(E820_TYPE type);

    void Init();

    estd::size_t Avalible() const{
        return avaliable_memory_;
    }
    estd::size_t Size() const{
        return length_;
    }
    const Entry& operator[](estd::size_t pos){
        return entrys_[pos];
    }
private:
    estd::array<Entry, max_size> entrys_;
    estd::size_t length_;
    estd::size_t avaliable_memory_;
};

inline Manager manager;

} // namespace eospp::memory::e820
