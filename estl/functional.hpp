#pragma once
#include <tuple.hpp>
#include <type_traits.hpp>
#include <utility.hpp>
namespace estd {

template <typename Fn, typename = void>
struct invoke_function {
    template <typename... Args>
    static constexpr decltype(auto) call(const Fn& fn, Args&&... args) {
        return fn(estd::forward<Args>(args)...);
    }
};
template <typename Fn>
struct invoke_function<Fn, void_t<enable_if_t<is_pointer_v<decay_t<Fn>>>>> {
    template <typename... Args>
    static constexpr decltype(auto) call(const Fn& fn, Args&&... args) {
        return (*fn)(estd::forward<Args>(args)...);
    }
};

struct invoke_member_function {
    template <typename Fn, typename T, typename... Args>
    static constexpr auto call(Fn&& fn, T&& obj, Args&&... args)
        -> decltype((obj.*fn)(estd::forward<Args>(args)...)) {
        return (obj.*fn)(estd::forward<Args>(args)...);
    }

    template <typename Fn, typename T, typename... Args>
    static constexpr auto call(Fn&& fn, T&& obj, Args&&... args)
        -> decltype((obj->*fn)(estd::forward<Args>(args)...)) {
        return (obj->*fn)(estd::forward<Args>(args)...);
    }
};

template <typename Fn,
          typename T,
          bool = is_member_function_pointer_v<Fn>,
          bool = is_member_object_pointer_v<T>>
struct invoke_helper : invoke_member_function {};

template <typename Fn, typename T>
struct invoke_helper<Fn, T, false, false> : invoke_function<Fn> {};

template <typename Fn, typename... Args>
struct invoke_impl;

template <typename Fn>
struct invoke_impl<Fn> : invoke_function<Fn> {};

template <typename Fn, typename T, typename... Args>
struct invoke_impl<Fn, T, Args...> : invoke_helper<Fn, T> {};

template <typename Fn, typename... Args>
constexpr decltype(auto) invoke(Fn&& fn, Args&&... args) {
    return invoke_impl<Fn, Args...>::call(estd::forward<Fn>(fn), estd::forward<Args>(args)...);
}

template <typename Fn, typename T, estd::size_t... I>
constexpr decltype(auto) apply_impl(Fn&& fn, T&& t, estd::index_sequence<I...>) {
    return invoke(fn, estd::get<I>(t)...);
}

template <typename Fn, typename T>
constexpr decltype(auto) apply(Fn&& fn, T&& t) {
    return apply_impl(
        estd::forward<Fn>(fn), estd::forward<T>(t), estd::make_index_sequence<t.size()>());
}

template <typename R, typename... Args>
class interface {
public:
    virtual R operator()(const Args&... args) = 0;

    virtual interface<R, Args...>* clone() = 0;
};

template <typename Fn, typename R, typename... Args>
class function_helper : public interface<R, Args...> {
public:
    function_helper(const Fn& f)
            : f_(f) {}

    function_helper(Fn&& f)
            : f_(estd::move(f)) {}

    function_helper(const function_helper& rhs)
            : f_(rhs.f_) {}

    function_helper(function_helper&& rhs)
            : f_(estd::move(rhs.f_)) {}

    virtual R operator()(const Args&... args) override {
        return estd::invoke(f_, args...);
    }

    virtual interface<R, Args...>* clone() override {
        return new function_helper<Fn, R, Args...>(f_);
    }

private:
    Fn f_;
};

template <typename R, typename... Args>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
public:
    function() = default;

    template <typename Fn, typename = enable_if_t<!is_same_v<function, decay_t<Fn>>>>
    function(Fn&& f)
            : impl_(new function_helper<Fn, R, Args...>(estd::forward<Fn>(f))) {}

    function(const function& rhs)
            : impl_(rhs.impl_->clone()) {}

    function(function&& rhs) noexcept
            : impl_(rhs.impl_) {
        rhs.impl_ = nullptr;
    }

    R operator()(Args... args) const {
        // Disable the expection,if the impl don't init,kernel will handle it when coredump
        return estd::invoke(impl_, args...);
    }

    template <typename Fn>
    function& operator=(Fn&& f) {
        function(estd::forward<Fn>(f)).swap(*this);
        return *this;
    }

    function& operator=(function rhs) {
        rhs.swap(*this);
        return *this;
    }
    void swap(function& rhs) {
        estd::swap(impl_, rhs.impl_);
    }

    operator bool() const {
        return impl_ != nullptr;
    }

    ~function() {
        if (impl_)
            delete impl_;
    }

private:
    interface<R, Args...>* impl_{nullptr};
};

} // namespace estd