#pragma once
#include <deque.hpp>

namespace estd {

template <typename T, typename C = deque<T>>
class stack {
public:
    using value_type       = T;
    using reference        = T&;
    using rvalue_reference = T&&;
    using const_ference    = const T&;
    using size_type        = size_t;

    stack() = default;

    stack(const stack& rhs)
            : container_(rhs.container_) {}

    stack(stack&& rhs) noexcept
            : container_(estd::move(rhs.container_)) {}

    void swap(stack& rhs) {
        container_.swap(rhs.container_);
    }

    stack& operator=(stack rhs) {
        rhs.swap(*this);
        return *this;
    }

    void push(const_ference rhs) {
        emplace(rhs);
    }

    void push(rvalue_reference rhs) {
        emplace(estd::move(rhs));
    }

    reference top() {
        return container_.back();
    }

    reference top() const {
        return container_.back();
    }

    void pop() {
        container_.pop_back();
    }

    bool empty() const {
        return container_.empty();
    }

    size_type size() const {
        return container_.size();
    }

    template <typename... Args>
    reference emplace(Args&&... args) {
        return container_.emplace_back(estd::forward<Args>(args)...);
    }

private:
    C container_;
};
} // namespace estd
