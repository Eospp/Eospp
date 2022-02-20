#pragma once
#include <initializer_list>
#include <iterator.hpp>
#include <utility.hpp>
namespace estd {

template <typename Driver>
class list_node_base {
public:
    void link_next(list_node_base *rhs) {
        if (next_) {
            next_->prv_ = rhs;
            rhs->next_ = next_;
        }

        next_ = rhs;
        rhs->prv_ = this;
    }
    void link_prv(list_node_base *rhs) {
        if (prv_) {
            prv_->next_ = rhs;
            rhs->prv_ = prv_;
        }

        rhs->next_ = this;
        prv_ = rhs;
    }
    void unlink_next() {
        if (next_) {
            next_->prv_ = nullptr;
        }
        next_ = nullptr;
    }
    void unlink_prv() {
        if (prv_) {
            prv_->next_ = nullptr;
        }
        prv_ = nullptr;
    }
    void unlink() {
        if (next_) {
            next_->prv_ = prv_;
        }
        if (prv_) {
            prv_->next_ = next_;
        }

        next_ = nullptr;
        prv_ = nullptr;
    }

    Driver *cast_to_driver() {
        return static_cast<Driver *>(this);
    }

    const Driver *cast_to_driver() const {
        return static_cast<const Driver *>(this);
    }


    Driver *get_next() {
        return next_->cast_to_driver();
    }

    const Driver *get_next() const {
        return next_->cast_to_driver();
    }
    Driver *get_prv() {
        return prv_->cast_to_driver();
    }
    const Driver *get_prv() const {
        return prv_->cast_to_driver();
    }

private:
    list_node_base *next_{nullptr};
    list_node_base *prv_{nullptr};
};

template <typename T>
class list_node : public list_node_base<list_node<T>> {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

public:
    list_node(const T &v) : value_(v) {}

    list_node(T &&v) : value_(estd::move(v)) {}

    template <typename... Args>
    list_node(Args &&...args) : value_(estd::forward<Args>(args)...) {}

    reference get_value() {
        return value_;
    }
    const_reference get_value() const {
        return value_;
    }
    list_node &operator=(list_node rhs) {
        rhs.swap(*this);
        return *this;
    }
    void swap(list_node &rhs) {
        estd::swap(value_, rhs.value_);
    }

private:
    value_type value_;
};

template <typename T>
class list_iterator {
public:
    using value_type = T;
    using pointer = value_type *;
    using node_type = list_node<T>;
    using reference = T &;
    using const_reference = const T &;

public:
    explicit list_iterator(node_type *node) : current_(node) {}

    reference operator*() {
        return current_->get_value();
    }

    const_reference operator*() const {
        return current_->get_value();
    }

    pointer operator->() const {
        return &current_->get_value();
    }

    list_iterator &operator++() {
        current_ = current_->get_next();
        return *this;
    }
    list_iterator operator++(int) {
        list_iterator it = *this;
        current_ = current_->get_next();
        return it;
    }

    list_iterator &operator--() {
        current_ = current_->get_prv();
        return *this;
    }

    list_iterator operator--(int) {
        list_iterator it = *this;
        current_ = current_->get_prv();
        return it;
    }

    bool operator==(const list_iterator &rhs) {
        return current_ == rhs.current_;
    }
    bool operator!=(const list_iterator &rhs) {
        return current_ != rhs.current_;
    }

    node_type *current_;
};


template <typename T>
class const_list_iterator {
public:
    using value_type = T;
    using pointer = value_type *;
    using node_type = list_node<T>;
    using reference = const T &;
    using const_reference = const T &;

public:
    explicit const_list_iterator(const node_type *node) : current_(node) {}

    reference operator*() {
        return current_->get_value();
    }

    const_reference operator*() const {
        return current_->get_value();
    }

    pointer operator->() const {
        return &current_->get_value();
    }

    const_list_iterator &operator++() {
        current_ = current_->get_next();
        return *this;
    }
    const_list_iterator operator++(int) {
        list_iterator it = *this;
        current_ = current_->get_next();
        return it;
    }

    const_list_iterator &operator--() {
        current_ = current_->get_prv();
        return *this;
    }

    const_list_iterator operator--(int) {
        list_iterator it = *this;
        current_ = current_->get_prv();
        return it;
    }

    bool operator==(const const_list_iterator &rhs) {
        return current_ == rhs.current_;
    }
    bool operator!=(const const_list_iterator &rhs) {
        return current_ != rhs.current_;
    }

    const node_type *current_;
};

template <typename T>
class list {
public:
    using size_type = size_t;
    using pointer = T *;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rvalue_reference = T &&;
    using node_type = list_node<T>;
    using base_type = list_node_base<node_type>;
    using iterator = list_iterator<T>;
    using const_iterator = const_list_iterator<T>;

public:
    list() : list_size_(0) {
        node_.link_next(&node_);
    }

    list(const list &rhs) : list() {
        for (auto &it : rhs) {
            push_back(it);
        }
    }
    list(std::initializer_list<T> l) : list() {
        for (auto &it : l) {
            push_back(it);
        }
    }
    list(list &&rhs) noexcept : list() {
         rhs.swap(*this);
    }

    list &operator=(list rhs) {
        rhs.swap(*this);
        return *this;
    }

    void push_front(const_reference value) {
        emplace_front(value);
    }
    void push_front(rvalue_reference value) {
        emplace_front(estd::move(value));
    }

    void push_back(const_reference value) {
        emplace_back(value);
    }

    void push_back(rvalue_reference value) {
        emplace_back(estd::move(value));
    }

    iterator insert(iterator pos, const_reference value) {
        node_type *link_node = create_node(value);
        link_node->link_next(pos.current_);
        list_size_++;
        return iterator(link_node);
    }

    iterator insert(iterator pos, rvalue_reference value) {
        node_type *link_node = create_node(estd::move(value));
        link_node->link_next(pos.current_);
        list_size_++;
        return iterator(link_node);
    }

    iterator erase(iterator pos) {
        node_type *next = pos.current_->get_next();
        pos.current_->unlink();
        list_size_--;
        destory_node(pos.current_);
        return iterator(next);
    }

    iterator erase(iterator first, iterator last) {
        node_type *prv = first.current_->get_prv();
        node_type *next = last.current_->get_next();
        first.current_->unlink_prv();
        last.current_->unlink_next();
        prv->link_next(next);
        destory_node(first.current_, last.current_);
        return iterator(next);
    }

    void pop_back() {
        if (!empty()) {
            node_.get_prv()->unlink();
            list_size_--;
        }
    }

    void pop_front() {
        if (!empty()) {
            node_.get_next()->unlink();
            list_size_--;
        }
    }

    template <typename... Args>
    void emplace_back(Args &&...args) {
        node_type *link_node = create_node(estd::forward<Args>(args)...);
        node_.link_prv(link_node);
        list_size_++;
    }

    template <typename... Args>
    void emplace_front(Args &&...args) {
        node_type *link_node = create_node(estd::forward<Args>(args)...);
        node_.link_next(link_node);
        list_size_++;
    }

    reference front() {
        return *begin();
    }
    const_reference front() const {
        return *begin();
    }

    reference back() {
        return *(--end());
    }
    const_reference back() const {
        return *(--end());
    }

    iterator begin() {
        return iterator(node_.get_next());
    }

    const_iterator begin() const {
        return const_iterator(node_.get_next());
    }

    iterator end() {
        return iterator(node_.cast_to_driver());
    }

    const_iterator end() const {
        return const_iterator(node_.cast_to_driver());
    }

    bool empty() const {
        return size() == 0;
    }

    size_type size() const {
        return list_size_;
    }

    void swap(list &rhs) {
        node_type *rhs_next = rhs.node_.get_next();
        node_type *rhs_prv = rhs.node_.get_prv();

        node_type *lhs_next = node_.get_next();
        node_type *lhs_prv = node_.get_prv();

        rhs.node_.unlink_next();
        rhs.node_.unlink_prv();
        node_.unlink_next();
        node_.unlink_prv();

        if (!rhs.empty()) {
            node_.link_next(rhs_next);
            node_.link_prv(rhs_prv);
        } else {
            node_.link_next(&node_);
        }

        if (!empty()) {
            rhs.node_.link_next(lhs_next);
            rhs.node_.link_prv(lhs_prv);
        } else {
            rhs.node_.link_next(&rhs.node_);
        }

        estd::swap(list_size_, rhs.list_size_);
    }

    void clear() {
        if (empty()) return;

        for (node_type *cur = node_.get_next(); cur != node_.cast_to_driver();) {
            auto tmp = cur->get_next();
            destory_node(cur);
            cur = tmp;
        }

        list_size_ = 0;
    }

    ~list() {
        clear();
    }

private:
    template <typename... Args>
    node_type *create_node(Args &&...args) {
        return new node_type(estd::forward<Args>(args)...);
    }

    void destory_node(node_type *link_node) {
        delete link_node;
    }

    void destory_node(node_type *first, node_type *last) {
        for (node_type *cur = first; cur != nullptr;) {
            node_type *tmp = cur->get_next();
            destory_node(tmp);
            cur = tmp;
        }
    }

private:
    size_type list_size_;
    base_type node_;
};


}   // namespace estd
