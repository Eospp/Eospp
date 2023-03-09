#pragma once
namespace eospp::global_obj {

using Constructor = void (*)();
extern "C" Constructor start_ctors;
extern "C" Constructor end_ctors;

inline void Init() {
    for (Constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

} // namespace eospp::global_obj