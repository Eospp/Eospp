#pragma once
#include <atomic.hpp>
#include <tuple.hpp>
#include <type_traits.hpp>
namespace estd {

template <typename T>
class default_deleter {
public:
    constexpr default_deleter() = default;

    /** @brief Converting constructor.
     *
     * Allows conversion from a deleter for arrays of another type, @p U,
     * only if @p U* is convertible to @p T*.
     */
    template <typename U, typename = enable_if_t<is_convertible_v<U *, T *>>>
    default_deleter(const default_deleter<U> &) noexcept {}

    void operator()(T *ptr) const {
        static_assert(sizeof(T) > 0, "T must be complete");
        static_assert(!is_void_v<T>, "can't delete pointer to incomplete type");
        delete ptr;
    }
};

template <typename T>
class default_deleter<T[]> {
public:
    constexpr default_deleter() = default;
    /** @brief Converting constructor.
     *
     * Allows conversion from a deleter for arrays of another type, such as
     * a const-qualified version of @p T.
     *
     * Conversions from types derived from @c T are not allowed because
     * it is unsafe to @c delete[] an array of derived types through a
     * pointer to the base type.
     */
    template <typename U, typename = enable_if_t<is_convertible_v<U (*)[], T (*)[]>>>
    default_deleter(const default_deleter<U[]> &) noexcept {}

    template <typename U, typename = enable_if_t<is_convertible_v<U (*)[], T (*)[]>>>
    void operator()(U *ptr) const {
        static_assert(sizeof(T) > 0, "T must be complete");
        delete[] ptr;
    }
};

template <typename T, typename D, typename = void>
struct ptr_type {
    using type = T *;
};

template <typename T, typename D>
struct ptr_type<T, D, estd::void_t<typename estd::remove_reference_t<D>::pointer>> {
    using type = typename estd::remove_reference_t<D>::pointer;
};

template <typename T, typename D>
class unique_ptr_impl {
public:
    using pointer = typename ptr_type<T, D>::type;
    using deleter_type = D;
    unique_ptr_impl() = default;
    unique_ptr_impl(pointer p) {
        ptr() = p;
    }
    template <typename Deleter>
    unique_ptr_impl(pointer p, Deleter &&d) : data_(p, estd::forward<Deleter>(d)) {}

    pointer &ptr() {
        return estd::get<0>(data_);
    }
    pointer ptr() const {
        return estd::get<0>(data_);
    }
    deleter_type &deleter() {
        return estd::get<1>(data_);
    }
    const deleter_type &deleter() const {
        return estd::get<1>(data_);
    }

private:
    estd::tuple<pointer, deleter_type> data_;
};

template <typename T, typename D = default_deleter<T>>
class unique_ptr {
public:
    using pointer = typename unique_ptr_impl<T, D>::pointer;
    using element_type = T;
    using deleter_type = D;

    constexpr unique_ptr() noexcept {}

    explicit unique_ptr(pointer p) noexcept : impl_(p) {}

    unique_ptr(pointer p, estd::conditional_t<is_reference_v<D>, D, const D &> d) : impl_(p, d) {}
    unique_ptr(pointer p, remove_reference_t<D> &&d) noexcept
        : impl_(estd::move(p), estd::move(d)) {
        static_assert(!estd::is_reference_v<deleter_type>, "rvalue deleter bound to reference");
    }

    template <typename U, typename E, typename = estd::enable_if_t<estd::is_base_of_v<T, U>>>
    unique_ptr(unique_ptr<U, E> &&u) noexcept : impl_(u.release(), estd::move(u.get_deleter())) {}

    unique_ptr(unique_ptr &&p) noexcept
        : impl_(p.release(), estd::forward<deleter_type>(p.get_deleter())) {}

    ~unique_ptr() noexcept {
        auto &ptr = impl_.ptr();
        if (ptr != nullptr) impl_.deleter()(ptr);
        ptr = pointer();
    }

    unique_ptr &operator=(unique_ptr &&p) noexcept {
        reset(p.release());
        get_deleter() = estd::forward<deleter_type>(p.get_deleter());
        return *this;
    }

    unique_ptr &operator=(nullptr_t) noexcept {
        reset();
        return *this;
    }

    estd::add_lvalue_reference_t<element_type> operator*() const {
        return *get();
    }
    pointer operator->() const noexcept {
        return get();
    }
    pointer get() const noexcept {
        return impl_.ptr();
    }

    deleter_type &get_deleter() noexcept {
        return impl_.deleter();
    }

    const deleter_type &get_deleter() const noexcept {
        return impl_.deleter();
    }

    explicit operator bool() const noexcept {
        return get() == pointer() ? false : true;
    }

    pointer release() noexcept {
        pointer p = get();
        impl_.ptr() = pointer();
        return p;
    }

    void reset(pointer p = pointer()) noexcept {
        estd::swap(impl_.ptr(), p);
        if (p != pointer()) get_deleter()(p);
    }

    void swap(unique_ptr &p) noexcept {
        estd::swap(impl_, p.impl_);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

private:
    unique_ptr_impl<T, D> impl_;
};

template <typename T, typename D>
class unique_ptr<T[], D> {
public:
    using pointer = typename unique_ptr_impl<T, D>::pointer;
    using element_type = T;
    using deleter_type = D;

    constexpr unique_ptr() noexcept {}

    explicit unique_ptr(pointer p) noexcept : impl_(p) {}

    unique_ptr(pointer p, estd::conditional_t<is_reference_v<D>, D, const D &> d) : impl_(p, d) {}
    unique_ptr(pointer p, remove_reference_t<D> &&d) noexcept
        : impl_(estd::move(p), estd::move(d)) {}

    unique_ptr(unique_ptr &&p) noexcept
        : impl_(p.release(), estd::forward<deleter_type>(p.get_deleter())) {}

    ~unique_ptr() noexcept {
        auto &ptr = impl_.ptr();
        if (ptr != nullptr) impl_.deleter()(ptr);
        ptr = pointer();
    }

    unique_ptr &operator=(unique_ptr &&p) noexcept {
        reset(p.release());
        get_deleter() = estd::forward<deleter_type>(p.get_deleter());
        return *this;
    }

    unique_ptr &operator=(nullptr_t) noexcept {
        reset();
        return *this;
    }

    estd::add_lvalue_reference_t<element_type> operator*() const {
        return *get();
    }
    pointer operator->() const noexcept {
        return get();
    }
    pointer get() const noexcept {
        return impl_.ptr();
    }

    deleter_type &get_deleter() noexcept {
        return impl_.deleter();
    }

    const deleter_type &get_deleter() const noexcept {
        return impl_.deleter();
    }

    explicit operator bool() const noexcept {
        return get() == pointer() ? false : true;
    }

    pointer release() noexcept {
        pointer p = get();
        impl_.ptr() = pointer();
        return p;
    }

    estd::add_lvalue_reference_t<element_type> operator[](estd::size_t i) const {
        return get()[i];
    }

    void reset(pointer p = pointer()) noexcept {
        estd::swap(impl_.ptr(), p);
        if (p != pointer()) get_deleter()(p);
    }

    void swap(unique_ptr &p) noexcept {
        estd::swap(impl_, p.impl_);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

private:
    unique_ptr_impl<T, D> impl_;
};

template <typename T, typename... Args>
inline enable_if_t<!is_array_v<T>, estd::unique_ptr<T>> make_unique(Args &&... args) {
    return estd::unique_ptr<T>(new T(estd::forward<Args>(args)...));
}

template <typename T>
inline enable_if_t<is_array_v<T>, estd::unique_ptr<T>> make_unique(estd::size_t size) {
    return estd::unique_ptr<T>(new remove_extent_t<T>[size]());
}

template <typename T, typename D>
inline bool operator==(const estd::unique_ptr<T, D> &x, nullptr_t) noexcept {
    return !x;
}

template <typename T, typename D>
inline bool operator==(nullptr_t, const estd::unique_ptr<T, D> &x) noexcept {
    return !x;
}

template <typename T, typename D>
inline bool operator!=(const estd::unique_ptr<T, D> &x, nullptr_t) noexcept {
    return static_cast<bool>(x);
}

template <typename T, typename D>
inline bool operator!=(nullptr_t, const estd::unique_ptr<T, D> &x) noexcept {
    return static_cast<bool>(x);
}

template <typename T, typename D, typename U, typename E>
inline bool operator!=(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<U, E> &rhs) {
    return lhs.get() != rhs.get();
}

template <typename T, typename D, typename U, typename E>
inline bool operator==(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<U, E> &rhs) {
    return lhs.get() == rhs.get();
}

template <typename T, typename D>
inline bool operator<(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<T, D> &rhs) {
    return estd::less<typename estd::unique_ptr<T, D>::pointer>()(lhs.get(), rhs.get());
}

template <typename T, typename D>
inline bool operator<=(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<T, D> &rhs) {
    return estd::less_equal<typename estd::unique_ptr<T, D>::pointer>()(lhs.get(), rhs.get());
}

template <typename T, typename D>
inline bool operator>(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<T, D> &rhs) {
    return estd::greater<typename estd::unique_ptr<T, D>::pointer>()(lhs.get(), rhs.get());
}

template <typename T, typename D>
inline bool operator>=(const estd::unique_ptr<T, D> &lhs, const estd::unique_ptr<T, D> &rhs) {
    return estd::greater_equal<typename estd::unique_ptr<T, D>::pointer>()(lhs.get(), rhs.get());
}

class count_base : NoCopyAble {
public:
    using count_type = estd::atomic<uint64_t>;

    count_base() : shared_count_(1), weak_count_(1) {}

    uint64_t get_shared_count() const {
        return shared_count_.load(estd::memory_order_relaxed);
    }

    uint64_t get_weak_count() const {
        return weak_count_.load(estd::memory_order_relaxed);
    }

    void copy_weak_ref() {
        ++weak_count_;
    }
    void copy_shared_ref() {
        ++shared_count_;
    }

    bool lock()
    {
        uint64_t count = shared_count_.load(estd::memory_order_relaxed);

        do
        {
            if(count == 0)
              return false;

        }while(shared_count_.compare_exchange_weak(count,count + 1,estd::memory_order_acq_rel,estd::memory_order_relaxed));

        return true;
    }

    uint64_t shared_release() {
        return --shared_count_;
    }
    uint64_t weak_release() {
        return --weak_count_;
    }

    virtual void dispose() = 0;
    virtual void distory() = 0;

private:
    count_type shared_count_;
    count_type weak_count_;
};

template <typename T, typename Deleter = default_deleter<T>>
class count_impl : public count_base {
public:
    using element_type = T;
    using reference_type = T &;
    using pointer_type = T *;

    count_impl(T *ptr, const Deleter &d) : ptr_(ptr), deleter_(d) {}

    count_impl(T *ptr, Deleter &&d) : ptr_(ptr), deleter_(estd::forward<Deleter>(d)) {}

    virtual void dispose() override {
        if (ptr_) {
            deleter_(ptr_);
        }
    }

    virtual void distory() override {
        delete this;
    }

private:
    pointer_type ptr_;
    Deleter deleter_;
};

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr;

class weak_count;

class shared_count {
public:
    shared_count() : impl_(nullptr) {}

    template <typename T, typename Deleter = default_deleter<T>>
    shared_count(T *ptr, Deleter &&d)
        : impl_(new count_impl<T, estd::decay_t<Deleter>>(ptr, estd::forward<Deleter>(d))) {}

    shared_count(const shared_count &rhs) : impl_(rhs.impl_) {
        if (impl_) {
            impl_->copy_shared_ref();
        }
    }

    shared_count(count_base *impl) : impl_(impl)
    {
        
    }

    shared_count(shared_count &&rhs) noexcept : impl_(rhs.impl_) {
        rhs.impl_ = nullptr;
    }

    shared_count &operator=(shared_count rhs) {
        swap(rhs);
        return *this;
    }

    void swap(shared_count &rhs) noexcept {
        estd::swap(impl_, rhs.impl_);
    }

    uint64_t use_count() const {
        return impl_ ? impl_->get_shared_count() : 0;
    }

    void release() {
        if (impl_ && impl_->shared_release() == 0) {
            impl_->dispose();

            if (impl_->weak_release() == 0) {
                impl_->distory();
            }
            impl_ = nullptr;
        }
    }

    ~shared_count() {
        release();
    }

    friend class weak_count;

private:
    count_base *impl_;
};

inline void swap(shared_count &lhs, shared_count &rhs) noexcept {
    lhs.swap(rhs);
}

class weak_count {
public:
    uint64_t use_count() const {
        return impl_ ? impl_->get_shared_count() : 0;
    }

    weak_count() : impl_(nullptr) {}

    weak_count(const weak_count &rhs) : impl_(rhs.impl_) {
        if (impl_) {
            impl_->copy_weak_ref();
        }
    }

    weak_count(weak_count &&rhs) noexcept : impl_(rhs.impl_) {
        rhs.impl_ = nullptr;
    }

    weak_count(const shared_count &rhs) noexcept : impl_(rhs.impl_) {
        if (impl_) {
            impl_->copy_weak_ref();
        }
    }

    weak_count& operator=(weak_count rhs) {
        rhs.swap(*this);
        return *this;
    }

    void swap(weak_count &rhs) {
        estd::swap(impl_, rhs.impl_);
    }

    void release() {
        if (impl_ && impl_->weak_release() == 0) {
            impl_->distory();
            impl_ = nullptr;
        }
    }
    
    count_base* get() const 
    {
        return impl_;
    }

    bool lock()
    {
        return impl_ ? impl_->lock() : false; 
    }

    ~weak_count() {
        release();
    }

    friend class shared_count;

private:
    count_base *impl_;
};

inline void swap(weak_count &lhs, weak_count &rhs) {
    lhs.swap(rhs);
}

template <typename T>
class shared_ptr {
public:
    using element_type = T;
    using reference_type = T &;
    using pointer_type = T *;

    shared_ptr() : ptr_(nullptr), refcount_() {}

    shared_ptr(const shared_ptr &rhs) : ptr_(rhs.ptr_), refcount_(rhs.refcount_) {}

    shared_ptr(shared_ptr &&rhs) noexcept : ptr_(rhs.ptr_), refcount_(estd::move(rhs.refcount_)) {
        rhs.ptr_ = nullptr;
    }

    template <typename Deleter = default_deleter<T>>
    shared_ptr(T *ptr, Deleter &&deleter = Deleter())
        : ptr_(ptr), refcount_(ptr, estd::forward<Deleter>(deleter)) {}

    template <typename U,
              typename Deleter = default_deleter<U>,
              typename = estd::enable_if_t<estd::is_base_of_v<T, U>>>
    shared_ptr(U *ptr, Deleter &&deleter = Deleter())
        : ptr_(ptr), refcount_(ptr, estd::forward<Deleter>(deleter)) {}

    template <typename U, typename = estd::enable_if_t<estd::is_base_of_v<T, U>>>
    shared_ptr(const shared_ptr<U> &rhs) : ptr_(rhs.ptr_), refcount_(rhs.refcount_) {}

    template <typename U, typename = estd::enable_if_t<estd::is_base_of_v<T, U>>>
    shared_ptr(shared_ptr<U> &&rhs) noexcept
        : ptr_(rhs.ptr_), refcount_(estd::move(rhs.refcount_)) {
        rhs.ptr_ = nullptr;
    }

    shared_ptr &operator=(shared_ptr rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    template <typename U, typename = estd::enable_if_t<estd::is_base_of_v<T, U>>>
    shared_ptr &operator=(shared_ptr<U> rhs) noexcept {
        reset();
        ptr_ = rhs.ptr_;
        refcount_ = rhs.refcount_;
        return *this;
    }

    shared_ptr &operator=(nullptr_t) noexcept {
        estd::shared_ptr<T>().swap(*this);
        return *this;
    }

    void swap(shared_ptr &rhs) {
        estd::swap(refcount_, rhs.refcount_);
        estd::swap(ptr_, rhs.ptr_);
    }

    uint64_t use_count() const {
        return refcount_.use_count();
    }

    template <typename U, typename Deleter = default_deleter<T>>
    void reset(U *ptr = nullptr, Deleter &&deleter = Deleter()) {
        shared_count(ptr, estd::forward<Deleter>(deleter)).swap(refcount_);
        ptr_ = ptr;
    }

    void reset() {
        ptr_ = nullptr;
        shared_count().swap(refcount_);
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }

    pointer_type get() const noexcept {
        return ptr_;
    }

    reference_type operator*() const noexcept {
        return *get();
    }

    pointer_type operator->() const noexcept {
        return get();
    }

    explicit operator bool() const noexcept {
        return get() == pointer_type() ? false : true;
    }

    template <typename U>
    friend class shared_ptr;

    template <typename U>
    friend class weak_ptr;
private:
    shared_ptr(const weak_ptr<T> &rhs) : ptr_(rhs.ptr_),refcount_(rhs.refcount_.get())
    {

    }
private:
    pointer_type ptr_;
    shared_count refcount_;
};

template <typename T>
class weak_ptr {
public:
    using element_type = T;
    using reference_type = T &;
    using pointer_type = T *;

    weak_ptr() : ptr_(nullptr), refcount_() {}

    weak_ptr(const weak_ptr &rhs) : ptr_(rhs.ptr_), refcount_(rhs.refcount_) {}

    weak_ptr(weak_ptr &&rhs) : ptr_(rhs.ptr_), refcount_(estd::move(rhs.refcount_)) {
        rhs.ptr_ = nullptr;
    }

    weak_ptr(const shared_ptr<T> &rhs) : ptr_(rhs.ptr_),refcount_(rhs.refcount_)
    {

    }

    bool expired() const
    {
        return use_count() == 0;
    }

    uint64_t use_count() const
    {
        return refcount_.use_count();
    }

    shared_ptr<T> lock()
    {
        return refcount_.lock() ? shared_ptr<T>(*this) : nullptr;
    }

    weak_ptr &operator=(weak_ptr rhs) {
        rhs.swap(*this);
        return *this;
    }

    void swap(weak_ptr &rhs) {
        estd::swap(ptr_, rhs.ptr_);
        estd::swap(refcount_, rhs.refcount_);
    }

    template<typename U>
    friend class shared_ptr;

    template<typename U>
    friend class weak_ptr;
private:
    pointer_type ptr_;
    weak_count refcount_;
};

template <typename T>
inline bool operator==(const shared_ptr<T> &lhs, nullptr_t) {
    return !lhs;
}

template <typename T>
inline bool operator!=(const shared_ptr<T> &lhs, nullptr_t) {
    return static_cast<bool>(lhs);
}

template <typename T>
inline bool operator==(nullptr_t, const shared_ptr<T> &lhs) {
    return !lhs;
}

template <typename T>
inline bool operator!=(nullptr_t, const shared_ptr<T> &lhs) {
    return static_cast<bool>(lhs);
}

template <typename T, typename U>
inline bool operator==(estd::shared_ptr<T> &lhs, estd::shared_ptr<U> &rhs) {
    return lhs.get() == rhs.get();
}

template <typename T, typename U>
inline bool operator!=(estd::shared_ptr<T> &lhs, estd::shared_ptr<U> &rhs) {
    return lhs.get() != rhs.get();
}

template <typename T>
inline bool operator<(estd::shared_ptr<T> &lhs, estd::shared_ptr<T> &rhs) {
    return estd::less<typename estd::shared_ptr<T>::pointer_type>()(lhs.get(), rhs.get());
}

template <typename T>
inline bool operator<=(estd::shared_ptr<T> &lhs, estd::shared_ptr<T> &rhs) {
    return estd::less_equal<typename estd::shared_ptr<T>::pointer_type>()(lhs.get(), rhs.get());
}

template <typename T>
inline bool operator>(estd::shared_ptr<T> &lhs, estd::shared_ptr<T> &rhs) {
    return estd::greater<typename estd::shared_ptr<T>::pointer_type>()(lhs.get(), rhs.get());
}

template <typename T>
inline bool operator>=(estd::shared_ptr<T> &lhs, estd::shared_ptr<T> &rhs) {
    return estd::greater_equal<typename estd::shared_ptr<T>::pointer_type>()(lhs.get(), rhs.get());
}

template <typename T, typename... Args>
estd::shared_ptr<T> make_shared(Args &&... args) {
    return estd::shared_ptr<T>(new T(estd::forward<Args>(args)...));
}

}   // namespace estd