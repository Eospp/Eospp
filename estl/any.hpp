#pragma once
#include <type_traits.hpp>
#include <utility.hpp>
namespace estd {
class any {
public:
    any() : content_(nullptr) {}

    template <typename T>
    any(const T &value) : content_(new Holder(estd::remove_cv_t<estd::decay<T>>(value))) {}

    any(const any &value) : content_(value.content_ ? value.content_->clone() : nullptr) {}

    template <typename T, typename = estd::enable_if_t<!estd::is_same_v<any, estd::decay_t<T>>>>
    any(T &&value) : content_(new Holder<estd::decay_t<T>>(estd::move(value))) {}

    any(any &&value) noexcept : content_(value.content_) {
        value.content_ = nullptr;
    }

    template <typename T, typename... Args>
    any(T *, Args &&... args)
        : content_(new Holder<estd::remove_cv_t<estd::decay_t<T>>>(estd::forward<Args>(args)...)) {}

    ~any() {
        if (content_) delete content_;
    }

    any &swap(any &rhs) {
        estd::swap(content_, rhs.content_);
        return *this;
    }

    template <typename T>
    any &operator=(T &&value) {
        any(estd::forward<T>(value)).swap(*this);
        return *this;
    }

    any &operator=(any rhs) {
        rhs.swap(*this);
        return *this;
    }

    operator bool() const {
        return !empty();
    }

    bool empty() const {
        return !content_;
    }

    void clear() {
        any().swap(*this);
    }

    template <typename T, typename... Args>
    void emplace(Args &&... args) {
        clear();
        content_ = new Holder<estd::remove_cv_t<estd::decay_t<T>>>(estd::forward<Args>(args)...);
    }

private:
    class PlaceHolder {
    public:
        virtual ~PlaceHolder(){};
        virtual PlaceHolder *clone() const = 0;
    };

    template <typename T>
    class Holder : public PlaceHolder {
    public:
        Holder(const T &value) : val(value) {}

        Holder(T &&value) : val(estd::move(value)) {}

        template <typename... Args>
        Holder(Args &&... args) : val(estd::forward<Args>(args)...) {}

        virtual PlaceHolder *clone() const override {
            return new Holder(val);
        }

        virtual ~Holder() {}

        T val;
    };

    template <typename T>
    friend T *any_cast(any &v) noexcept;

private:
    PlaceHolder *content_;
};

template <typename T>
T *any_cast(any &v) noexcept {
    auto holder = dynamic_cast<any::Holder<T> *>(v.content_);

    return holder ? &holder->val : nullptr;
}

template <typename T, typename... Args>
inline any make_any(Args &&... args) {
    return any(static_cast<T *>(nullptr), estd::forward<Args>(args)...);
}

inline void swap(any &lhs, any &rhs) {
    lhs.swap(rhs);
}

}   // namespace estd