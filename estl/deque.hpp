#pragma once

#include <allocator.hpp>
#include <pair.hpp>
#include <utility.hpp>
#include <initializer_list>
namespace estd {

template <typename T, typename alloc = allocator<T>>
class deque;

template <typename T>
class deque_iterator {
public:
    using containter = deque<remove_cv_t<T>>;
    using containter_pointer = conditional_t<is_const_v<T>, const containter*, containter*>;
    using value_type = T;
    using size_type = size_t;
    using pointer = add_pointer_t<T>;
    using reference = add_lvalue_reference_t<T>;
    using const_reference = add_const_t<add_lvalue_reference_t<T>>;
    using diference_type = int64_t;

    deque_iterator(containter_pointer deque, int64_t index) : deque_(deque), index_(index) {}

    reference operator*() {
        return (*deque_)[index_];
    }
    const_reference operator*() const {
        return (*deque_)[index_];
    }

    pointer operator->() {
        return &(*deque_)[index_];
    }

    const pointer operator->() const {
        return &(*deque_)[index_];
    }

    diference_type operator-(const deque_iterator& rhs) const {
        return index_ - rhs.index_;
    }

    deque_iterator& operator++() {
        index_++;
        return *this;
    }

    deque_iterator operator++(int) {
        deque_iterator it(*this);
        index_++;
        return it;
    }

    deque_iterator& operator--() {
        index_--;
        return *this;
    }
    deque_iterator operator--(int) {
        deque_iterator it(*this);
        index_--;
        return it;
    }

    deque_iterator& operator+=(size_type n) {
        index_ += n;
        return *this;
    }
    deque_iterator& operator-=(size_type n) {
        index_ -= n;
        return *this;
    }

    deque_iterator operator+(size_type n) {
        deque_iterator it(*this);
        it += n;
        return it;
    }
    deque_iterator operator-(size_type n) {
        deque_iterator it(*this);
        it -= n;
        return it;
    }

    bool operator==(const deque_iterator& rhs) {
        return deque_ == rhs.deque_ && index_ == rhs.index_;
    }

    bool operator!=(const deque_iterator& rhs) {
        return !(*this == rhs);
    }

private:
    containter_pointer deque_;
    int64_t index_;
};


template <typename T, typename alloc>
class deque {
public:
    using value_type = T;
    using pointer = add_pointer_t<T>;
    using size_type = size_t;
    using reference = add_lvalue_reference_t<T>;
    using const_reference = const T&;
    using rvalue_reference = add_rvalue_reference_t<T>;
    using iterator = deque_iterator<T>;
    using const_iterator = deque_iterator<const T>;
    using control_block = T**;

    constexpr static size_type block_elements = 16;

public:
    deque() : blocks_(nullptr), block_size_(0), size_(0), first_(0), last_(0) {}

    deque(const deque& rhs) : deque() {
        for (auto& it : rhs) push_back(it);
    }

    deque(deque&& rhs) noexcept : deque() {
        rhs.swap(*this);
    }

    deque(std::initializer_list<T> list) : deque()
    {
        for(auto &it : list)
           push_back(estd::move(it));
    }
    ~deque() {
        clear();

        for (int i = 0; i < block_size_; i++) {
            alloc::deallocate(blocks_[i]);
        }
        destory_control_blocks(blocks_);
    }

    deque& operator=(deque rhs) {
        rhs.swap(*this);
        return *this;
    }

    reference operator[](size_type pos) {
        return *get_obj_pointer(pos);
    }

    const_reference operator[](size_type pos) const {
        return *get_obj_pointer(pos);
    }


    reference front() {
        return (*this)[first_];
    }

    const_reference front() const {
        return (*this)[first_];
    }

    reference back(){
        return (*this)[last_];
    }

    const_reference back() const{
        return (*this)[last_];
    }


    void push_back(const_reference rhs) {
        emplace_back(rhs);
    }

    void push_back(rvalue_reference rhs) {
        emplace_back(estd::move(rhs));
    }

    void push_front(const_reference rhs) {
        emplace_front(rhs);
    }

    void push_front(rvalue_reference rhs) {
        emplace_front(estd::move(rhs));
    }

    void pop_front() {
        destory(get_obj_pointer(first_++));
        decrease_element();
    }

    void pop_back() {
        destory(get_obj_pointer(last_--));
        decrease_element();
    }
    size_type size() const {
        return size_;
    }

    bool empty() const {
        return size() == 0;
    }

    iterator begin() {
        return iterator(this, first_);
    }

    const_iterator begin() const {
        return const_iterator(this, first_);
    }

    iterator end() {
        return iterator(this, last_ + 1);
    }

    const_iterator end() const {
        return const_iterator(this, last_ + 1);
    }
    void clear() {
        if(!empty()){
            destory(begin(), end());

            size_ = 0;
            first_ = last_ = block_size_ / 2 * block_elements;
        }
    }

    void swap(deque& rhs) {
        estd::swap(blocks_, rhs.blocks_);
        estd::swap(block_size_, rhs.block_size_);
        estd::swap(size_, rhs.size_);
        estd::swap(first_, rhs.first_);
        estd::swap(last_, rhs.last_);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        ensure_capacity_back(1);

        size_++ == 0 ? last_ : ++last_;

        pointer obj = get_obj_pointer(last_);

        construct(obj, estd::forward<Args>(args)...);

        return *obj;
    }

    template <typename... Args>
    reference emplace_front(Args&&... args) {
        ensure_capacity_front(1);

        size_++ == 0 ? first_ : --first_;

        pointer obj = get_obj_pointer(first_);

        construct(obj, estd::forward<Args>(args)...);

        return *obj;
    }

private:
    void ensure_capacity_front(size_type n) {
        size_type front_capacity = first_;
        if (front_capacity >= n) {
            return;
        }

        size_type new_blocks = (n + block_elements - 1) / block_elements;

        control_block new_blocks_pointer =
            alloc_control_blocks(new_blocks + block_size_, 0, new_blocks);

        if (block_size_ == 0) {
            first_ = total_elements(new_blocks) - 1;
            last_ = first_;
        } else {
            for (int i = 0; i < block_size_; i++) {
                new_blocks_pointer[i + block_size_] = blocks_[i];
            }
            first_ += total_elements(new_blocks);
            last_ += total_elements(new_blocks);
        }
        estd::swap(blocks_, new_blocks_pointer);
        block_size_ += new_blocks;
        destory_control_blocks(new_blocks_pointer);
    }


    void ensure_capacity_back(size_type n) {
        size_type back_capacity = block_size_ == 0 ? 0 : total_elements(block_size_) - (last_ + 1);
        if (back_capacity >= n) {
            return;
        }

        size_type new_blocks = (n + block_elements - 1) / block_elements;

        control_block new_blocks_pointer =
            alloc_control_blocks(new_blocks + block_size_, block_size_, new_blocks + block_size_);

        if (block_size_ == 0) {
            first_ = 0;
            last_ = 0;
        } else {
            for (int i = 0; i < block_size_; i++) {
                new_blocks_pointer[i] = blocks_[i];
            }
        }
        estd::swap(blocks_, new_blocks_pointer);
        block_size_ += new_blocks;
        destory_control_blocks(new_blocks_pointer);
    }

    control_block alloc_control_blocks(size_type size, size_type start, size_type end) {
        control_block new_blocks = new pointer[size];

        while (start != end) {
            new_blocks[start++] = alloc::allocate(block_elements);
        }
        return new_blocks;
    }
    void destory_control_blocks(control_block blocks) {
        if (blocks) {
            delete[] blocks;
        }
    }

    size_type total_elements(size_type blocks) {
        return block_elements * blocks;
    }

    pointer get_obj_pointer(size_type n) const {
        return &blocks_[n / block_elements][n % block_elements];
    }

    void decrease_element() {
        if (--size_ == 0) {
            first_ = last_ = block_size_ / 2 * block_elements;
        }
    }

private:
    control_block blocks_;
    size_type block_size_;
    size_type size_;
    size_type first_;
    size_type last_;
};
}   // namespace estd