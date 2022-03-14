#pragma once

#include <deque.hpp>

namespace estd {

template <typename T, typename C = deque<T>>
class queue {
public:
    using value_type       = T;
    using reference        = T&;
    using rvalue_reference = T&&;
    using const_ference    = const T&;
    using size_type        = size_t;

    queue() = default;

    queue(const queue& rhs)
            : container_(rhs.container_) {}

    queue(queue&& rhs) noexcept
            : container_(estd::move(rhs.container_)) {}

    queue& operator=(queue rhs) {
        rhs.swap(*this);
        return *this;
    }

    void swap(queue& rhs) {
        estd::swap(container_, rhs.container_);
    }

    void push(const_ference rhs) {
        emplace(rhs);
    }

    void push(rvalue_reference rhs) {
        emplace(estd::move(rhs));
    }

    void pop() {
        container_.pop_front();
    }
    template <typename... Args>
    reference emplace(Args&&... args) {
        return container_.emplace_back(estd::forward<Args>(args)...);
    }

    reference front() {
        return container_.front();
    }

    reference front() const {
        return container_.front();
    }

    size_type size() const {
        return container_.size();
    }

    bool empty() const {
        return container_.empty();
    }

private:
    C container_;
};
} // namespace estd