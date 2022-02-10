#pragma once
#include <utility.hpp>
namespace estd {

enum class memory_order : int {
    relaxed,
    consume,
    acquire,
    release,
    acq_rel,
    seq_cst,
};
enum memory_order_modifier { memory_order_mask = 0x0ffff };

constexpr inline memory_order memory_order_relaxed = memory_order::relaxed;
constexpr inline memory_order memory_order_consum = memory_order::consume;
constexpr inline memory_order memory_order_acquire = memory_order::acquire;
constexpr inline memory_order memory_order_release = memory_order::release;
constexpr inline memory_order memory_order_acq_rel = memory_order::acq_rel;
constexpr inline memory_order memory_order_seq_cst = memory_order::seq_cst;

template <typename T>
class atomic_impl : NoCopyAble {
public:
    using value_type = T;

    atomic_impl() noexcept = default;
    ~atomic_impl() noexcept = default;

    constexpr atomic_impl(value_type v) noexcept : value_(v) {}

    value_type load(memory_order m = memory_order_seq_cst) const noexcept {
        return __atomic_load_n(&value_, static_cast<int>(m));
    }
    value_type load(memory_order m = memory_order_seq_cst) const volatile noexcept {
        return __atomic_load_n(&value_, static_cast<int>(m));
    }
    void store(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_store_n(&value_, v, static_cast<int>(m));
    }
    void store(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_store_n(&value_, v, static_cast<int>(m));
    }
    value_type exchange(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_exchange_n(&value_, v, static_cast<int>(m));
    }
    value_type exchange(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_exchange_n(&value_, v, static_cast<int>(m));
    }

    bool compare_exchange_weak(value_type& expect,
                               value_type v,
                               memory_order m1,
                               memory_order m2) noexcept {
        return __atomic_compare_exchange_n(
            &value_, &expect, v, 1, static_cast<int>(m1), static_cast<int>(m2));
    }

    bool compare_exchange_weak(value_type& expect,
                               value_type v,
                               memory_order m1,
                               memory_order m2) volatile noexcept {
        return __atomic_compare_exchange_n(
            &value_, &expect, v, 1, static_cast<int>(m1), static_cast<int>(m2));
    }

    bool compare_exchange_strong(value_type& expect,
                                 value_type v,
                                 memory_order m1,
                                 memory_order m2) volatile noexcept {
        return __atomic_compare_exchange_n(
            &value_, &expect, v, 0, static_cast<int>(m1), static_cast<int>(m2));
    }

    bool compare_exchange_strong(value_type& expect,
                                 value_type v,
                                 memory_order m1,
                                 memory_order m2) noexcept {
        return __atomic_compare_exchange_n(
            &value_, &expect, v, 0, static_cast<int>(m1), static_cast<int>(m2));
    }

    value_type fetch_add(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_fetch_add(&value_, v, static_cast<int>(m));
    }

    value_type fetch_add(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_add(&value_, v, static_cast<int>(m));
    }

    value_type fetch_sub(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_fetch_sub(&value_, v, static_cast<int>(m));
    }

    value_type fetch_sub(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_sub(&value_, v, static_cast<int>(m));
    }

    value_type fetch_and(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_and(&value_, v, static_cast<int>(m));
    }

    value_type fetch_and(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_fetch_and(&value_, v, static_cast<int>(m));
    }

    value_type fetch_or(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_fetch_or(&value_, v, static_cast<int>(m));
    }

    value_type fetch_or(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_or(&value_, v, static_cast<int>(m));
    }

    value_type fetch_xor(value_type v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return __atomic_fetch_xor(&value_, v, static_cast<int>(m));
    }

    value_type fetch_xor(value_type v, memory_order m = memory_order_seq_cst) noexcept {
        return __atomic_fetch_xor(&value_, v, static_cast<int>(m));
    }

    operator value_type() const noexcept {
        return load();
    }
    operator value_type() const volatile noexcept {
        return load();
    }

    value_type operator=(value_type v) noexcept {
        store(v);
        return v;
    }

    value_type operator=(value_type v) volatile noexcept {
        store(v);
        return v;
    }

    value_type operator++() noexcept {
        return __atomic_add_fetch(&value_, 1, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator++() volatile noexcept {
        return __atomic_add_fetch(&value_, 1, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator++(int) noexcept {
        return fetch_add(1);
    }

    value_type operator++(int) volatile noexcept {
        return fetch_add(1);
    }

    value_type operator--() noexcept {
        return __atomic_sub_fetch(&value_, 1, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator--() volatile noexcept {
        return __atomic_sub_fetch(&value_, 1, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator--(int) noexcept {
        return fetch_sub(1);
    }

    value_type operator--(int) volatile noexcept {
        return fetch_sub(1);
    }

    value_type operator+=(value_type v) noexcept {
        return __atomic_add_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator+=(value_type v) volatile noexcept {
        return __atomic_add_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator-=(value_type v) noexcept {
        return __atomic_sub_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator-=(value_type v) volatile noexcept {
        return __atomic_sub_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator&=(value_type v) noexcept {
        return __atomic_and_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator&=(value_type v) volatile noexcept {
        return __atomic_and_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator|=(value_type v) noexcept {
        return __atomic_or_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator|=(value_type v) volatile noexcept {
        return __atomic_or_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator^=(value_type v) noexcept {
        return __atomic_xor_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

    value_type operator^=(value_type v) volatile noexcept {
        return __atomic_xor_fetch(&value_, v, static_cast<int>(memory_order_seq_cst));
    }

private:
    value_type value_;
};

template <typename T>
class atomic;

template <>
class atomic<int64_t> : public atomic_impl<int64_t> {
    using base_type = atomic_impl<int64_t>;
    using atomic_impl<int64_t>::atomic_impl;
};

template <>
class atomic<int32_t> : public atomic_impl<int32_t> {
    using base_type = atomic_impl<int32_t>;
    using atomic_impl<int32_t>::atomic_impl;
};

template <>
class atomic<int16_t> : public atomic_impl<int16_t> {
    using base_type = atomic_impl<int16_t>;
    using atomic_impl<int16_t>::atomic_impl;
};

template <>
class atomic<int8_t> : public atomic_impl<int8_t> {
    using base_type = atomic_impl<int8_t>;
    using atomic_impl<int8_t>::atomic_impl;
};

template <>
class atomic<uint64_t> : public atomic_impl<uint64_t> {
    using base_type = atomic_impl<uint64_t>;
    using atomic_impl<uint64_t>::atomic_impl;
};

template <>
class atomic<uint32_t> : public atomic_impl<uint32_t> {
    using base_type = atomic_impl<uint32_t>;
    using atomic_impl<uint32_t>::atomic_impl;
};

template <>
class atomic<uint16_t> : public atomic_impl<uint16_t> {
    using base_type = atomic_impl<uint16_t>;
    using atomic_impl<uint16_t>::atomic_impl;
};

template <>
class atomic<uint8_t> : public atomic_impl<uint8_t> {
    using base_type = atomic_impl<uint8_t>;
    using atomic_impl<uint8_t>::atomic_impl;
};

template <>
class atomic<bool> : NoCopyAble {
public:
    using value_type = bool;

    constexpr atomic(bool v) noexcept : value_(v) {}

    bool operator=(bool v) noexcept {
        return value_.operator=(v);
    }

    bool operator=(bool v) volatile noexcept {
        return value_.operator=(v);
    }

    operator bool() const noexcept {
        return value_.load();
    }
    operator bool() const volatile noexcept {
        return value_.load();
    }

    void store(bool v, memory_order m = memory_order_seq_cst) noexcept {
        value_.store(v, m);
    }

    void store(bool v, memory_order m = memory_order_seq_cst) volatile noexcept {
        value_.store(v, m);
    }

    bool load(memory_order m = memory_order_seq_cst) const noexcept {
        return value_.load(m);
    }

    bool load(memory_order m = memory_order_seq_cst) const volatile noexcept {
        return value_.load(m);
    }

    bool exchange(bool v, memory_order m = memory_order_seq_cst) noexcept {
        return value_.exchange(v, m);
    }

    bool exchange(bool v, memory_order m = memory_order_seq_cst) volatile noexcept {
        return value_.exchange(v, m);
    }

    bool compare_exchange_weak(bool& expect, bool v, memory_order m1, memory_order m2) noexcept {
        return value_.compare_exchange_weak(expect, v, m1, m2);
    }

    bool compare_exchange_weak(bool& expect,
                               bool v,
                               memory_order m1,
                               memory_order m2) volatile noexcept {
        return value_.compare_exchange_weak(expect, v, m1, m2);
    }

    bool compare_exchange_strong(bool& expect, bool v, memory_order m1, memory_order m2) noexcept {
        return value_.compare_exchange_strong(expect, v, m1, m2);
    }

    bool compare_exchange_strong(bool& expect,
                                 bool v,
                                 memory_order m1,
                                 memory_order m2) volatile noexcept {
        return value_.compare_exchange_strong(expect, v, m1, m2);
    }

private:
    atomic_impl<bool> value_;
};

}   // namespace estd