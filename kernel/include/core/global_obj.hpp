#pragma once
namespace eospp::global_obj {

using constructor = void (*)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

inline void Init() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

} // namespace eospp::global_obj