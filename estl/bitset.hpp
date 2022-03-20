#pragma once
#include <utility.hpp>

#include "type.hpp"
namespace estd {

template <estd::size_t N>
class bitset {
public:
    using bit_type = estd::uint64_t;

    static constexpr estd::uint8_t bits_per_type = sizeof(bit_type) * 8;
    static constexpr estd::size_t n              = (N + bits_per_type - 1) / bits_per_type;
    static constexpr bit_type empty              = 0;
    static constexpr bit_type full               = ~empty;

    constexpr bitset(bool flag = false){
        reset(flag);
    }

    void reset(bool flag) {
        for (int i = 0; i < n; i++) {
            data_[i] = flag ? full : empty;
        }
    }

    void set(estd::size_t pos){
         data_[type_offset(pos)] |= bit_mask(pos);
    }
    
    void clear(estd::size_t pos){
         data_[type_offset(pos)] &= (~bit_mask(pos));
    }

    bool test(estd::size_t pos) const {
        return data_[type_offset(pos)] & bit_mask(pos);
    }

    estd::size_t type_offset(estd::size_t pos) const{
        return pos / bits_per_type;
    }

    bit_type bit_mask(estd::size_t pos) const {
        return 1 << (pos % bits_per_type);
    }

    bool operator[](estd::size_t pos) const {
        return data_[type_offset(pos)] & bit_mask(pos);
    }
    
    estd::size_t count() const{
        return N;
    }

private:
    bit_type data_[n];
};

} // namespace estd
