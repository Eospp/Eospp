#pragma once
#include <utility.hpp>

namespace estd {


template <typename Fn>
class defer {
public:
    explicit defer(Fn &&fn) : exit_scope_(fn), dismiss_(false) {}

    defer(const defer &rhs) : exit_scope_(rhs.exit_scope_), dismiss_(rhs.dismiss_) {}

    defer(defer &&rhs) : exit_scope_(estd::move(exit_scope_)), dismiss_(rhs.dismiss_) {}

    defer &operator=(defer rhs) {
        rhs.swap(*this);
        return *this;
    }

    void dismiss() {
        dismiss_ = true;
    }

    ~defer() {
        if (!dismiss_) exit_scope_();
    }

    void swap(defer &rhs) {
        estd::swap(exit_scope_, rhs.exit_scope_);
        estd::swap(dismiss_, rhs.dismiss_);
    }

private:
    Fn exit_scope_;
    bool dismiss_;
};

template<typename Fn>
inline defer<Fn> make_defer(Fn &&fn)
{
    return defer<Fn>(estd::forward<Fn>(fn));
}

struct defer_helper
{
    template<typename Fn>
    defer<Fn> operator-(Fn &&fn)
    {
        return make_defer(estd::forward<Fn>(fn));
    }

};


}   // namespace estd

#define defer_scope auto defer_##__COUNTER__  = estd::defer_helper() - 
#define defer_lamda defer_scope [&]