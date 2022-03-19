#include <core/e820.hpp>
#include <io/vesa.hpp>
#include <core/memory_literals.hpp>
namespace eospp::memory::e820 {

const char* Manager::E280ToStr(E820_TYPE type) {
    switch (type) {
        case Free:
            return "Free";
        case Reserved:
            return "Reserved";
        case ACPI:
            return "ACPI";
        case Unusable:
            return "Unusable";
        case Disabled:
            return "Disabled";
        case Kernel:
            return "Kernel";
        default:
            return "Unknown";
    }
}

void Manager::Init() {
    Entry* entry      = reinterpret_cast<Entry*>(e820_addr);
    length_           = 0;
    avaliable_memory_ = 0;
    for (estd::size_t i = 0; i < max_size; i++) {
        if (entry->type > 4 || entry->length == 0 || entry->type < 1)
            break;

        if(entry->type == Free){
            avaliable_memory_ += entry->length;
        }

        entrys_[length_].address = entry->address;
        entrys_[length_].length  = entry->length;
        entrys_[length_].type    = entry->type;

        length_++;
        entry++;
    }
    io::printf("e820 size = %ul total avalible memory = %ulMB\n", length_, avaliable_memory_ / 1_MB);
}

} // namespace eospp::memory::e820