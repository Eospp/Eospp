#pragma once

#include <allocator.hpp>
#include <iterator.hpp>
#include <initializer_list>
namespace estd {

template <typename T, typename alloc = allocator<T>>
class vector {
public:
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const T *;
    using const_reference = const T &;
    using iterator = normal_iterator<pointer, vector>;
    using const_iterator = normal_iterator<const_pointer, vector>;
    using const_reverse_iterator = estd::reverse_iterator<const_iterator>;
    using reverse_iterator = estd::reverse_iterator<iterator>;
    using size_type = size_t;
    using diference_type = ptrdiff_t;

    vector() : M_start_(nullptr), M_finish_(nullptr), M_end_of_storage_(nullptr) {}
    vector(std::initializer_list<T> list)
    {
        allocate(list.size());

        for(auto &it : list)
          push_back(it);
    }
    vector(size_type size, const T &v) {
        allocate(size);

        for(size_t i = 0;i < size; i++)
           construct(M_start_ + i,v);

        M_finish_ = M_start_ + size;
    }

    explicit vector(size_type size)
    {
        allocate(size);

        for(size_t i = 0;i < size; i++)
           construct(M_start_ + i);
        
        M_finish_ = M_start_ + size;
    }

    vector(const vector &rhs) : vector() {
        if (!rhs.empty()) {
            allocate(rhs.size());

            for (auto &it : rhs) {
                construct(M_finish_++, it);
            }
        }
    }

    vector(vector &&rhs) noexcept : vector() {
        rhs.swap(*this);
    }

    ~vector() {
        if (!empty()) {
            destory(begin(), end());
        }

        if (capacity() != 0) {
            alloc::deallocate(M_start_);
        }
    }

    void push_back(const value_type &v) {
        ensure_capacity(size() + 1);
        construct(M_finish_++, v);
    }

    void push_back(value_type &&v) {
        ensure_capacity(size() + 1);
        construct(M_finish_++, estd::move(v));
    }

    void push_front(const value_type &v) {
        ensure_capacity(size() + 1);
        construct(M_finish_);
        for (size_type i = 0; i < size(); i++) {
            M_start_[size() - i] = estd::move(M_start_[size() - i - 1]);
        }

        M_start_[0] = v;

        M_finish_++;
    }

    void push_front(value_type &&v) {
        ensure_capacity(size() + 1);
        construct(M_finish_);
        for (size_type i = 0; i < size(); i++) {
            M_start_[size() - i] = estd::move(M_start_[size() - i - 1]);
        }

        M_start_[0] = estd::move(v);

        M_finish_++;
    }

    void insert(iterator pos, const value_type &v) {
        size_type index = pos - begin();

        ensure_capacity(size() + 1);
        construct(M_finish_);

        for (size_type i = size(); i > index; i--) {
            M_start_[i] = estd::move(M_start_[i - 1]);
        }
        M_start_[index] = v;

        M_finish_++;
    }

    void insert(iterator pos, value_type &&v) {
        size_type index = pos - begin();

        ensure_capacity(size() + 1);
        construct(M_finish_);


        for (size_type i = size(); i > index; i--) {
            M_start_[i] = estd::move(M_start_[i - 1]);
        }

        M_start_[index] = estd::move(v);

        M_finish_++;
    }

    template <typename... Args>
    reference emplace(iterator pos, Args &&...args) {
        size_type index = pos - begin();

        ensure_capacity(size() + 1);
        construct(M_finish_);

        for (size_type i = size(); i > index; i--) {
            M_start_[i] = estd::move(M_start_[i - 1]);
        }

        destory(M_start_ + index);

        construct(M_start_ + index, estd::forward<Args>(args)...);

        M_finish_++;

        return *(M_start_ + index);
    }

    iterator erase(iterator pos) {
        size_type index = pos - begin();

        for (size_type i = index; i < size() - 1; i++) {
            M_start_[i] = estd::move(M_start_[i + 1]);
        }

        destory(--M_finish_);

        return iterator(M_start_ + index);
    }

    iterator erase(iterator first, iterator last) {
        size_type src = last - begin();
        size_type des = first - begin();
        size_type n = end() - last;

        for (size_type i = 0; i < n; i++) {
            M_start_[des + i] = estd::move(M_start_[i + src]);
        }

        for (size_type i = des + n; i < size(); i++) {
            destory(M_start_ + i);
        }

        M_finish_ = M_start_ + des + n;

        return iterator(M_start_ + des);
    }

    void pop_back() {
        erase(end() - 1);
    }

    void pop_front() {
        erase(begin());
    }

    template <typename... Args>
    reference emplace_back(Args &&...args) {
        ensure_capacity(size() + 1);
        construct(M_finish_++, estd::forward<Args>(args)...);
        return *(M_finish_ - 1);
    }


    vector &operator=(vector rhs) {
        rhs.swap(*this);
        return *this;
    }

    bool operator==(const vector &rhs) const {
        if (size() != rhs.size()) return false;

        for (int i = 0; i < size(); i++) {
            if (data()[i] != rhs[i]) return false;
        }

        return true;
    }

    pointer data() noexcept {
        return M_start_;
    }

    const pointer data() const noexcept {
        return M_start_;
    }

    reference front() noexcept {
        return *M_start_;
    }

    const_reference front() const noexcept {
        return *M_start_;
    }

    reference back() noexcept {
        return *(end() - 1);
    }

    const_reference back() const noexcept {
        return *(end() - 1);
    }

    bool empty() const noexcept {
        return size() == 0;
    }

    void swap(vector &rhs) {
        estd::swap(M_start_, rhs.M_start_);
        estd::swap(M_finish_, rhs.M_finish_);
        estd::swap(M_end_of_storage_, rhs.M_end_of_storage_);
    }


    void resize(size_type new_size) {
        if (new_size > size()) {
            ensure_capacity(new_size);

            size_type n = new_size - size();

            for (size_type i = 0; i < n; i++) {
                construct(M_finish_ + i);
            }

            M_finish_ += n;
        } else if (new_size < size()) {
            size_type n = size() - new_size;

            for (size_type i = 0; i < n; i++) {
                destory(M_finish_ - i - 1);
            }

            M_finish_ -= n;
        }
    }

    void clear() {
        destory(begin(), end());
        M_finish_ = M_start_;
    }

    void reserve(size_type size) {
        return ensure_capacity(size);
    }

    iterator begin() {
        return iterator(M_start_);
    }

    iterator end() {
        return iterator(M_finish_);
    }

    const_iterator begin() const {
        return const_iterator(M_start_);
    }

    const_iterator end() const {
        return const_iterator(M_finish_);
    }

    size_type size() const {
        return M_finish_ - M_start_;
    }

    size_type capacity() const {
        return M_end_of_storage_ - M_start_;
    }

    reference operator[](size_t index) {
        return *(M_start_ + index);
    }

    const_reference operator[](size_t index) const {
        return *(M_start_ + index);
    }

private:
    void allocate(size_type capacity) {
        M_start_ = alloc::allocate(capacity);
        M_finish_ = M_start_;
        M_end_of_storage_ = M_start_ + capacity;
    }

    void ensure_capacity(size_type size) {
        size_t now_capacity = capacity();

        if (now_capacity >= size) return;

        size_type new_capacity = now_capacity * 2 > size ? now_capacity * 2 : size;

        vector v(estd::move(*this));

        allocate(new_capacity);

        for(auto &it : v)
        {
            push_back(estd::move(it));
        }
    }

private:
    pointer M_start_;
    pointer M_finish_;
    pointer M_end_of_storage_;
};

template <typename T>
inline void swap(estd::vector<T> &lhs, estd::vector<T> &rhs) {
    lhs.swap(rhs);
}

}   // namespace estd