#pragma once
#include <rb_tree.hpp>
namespace estd {


template <typename T, typename Compare = estd::less<T>>
class set {
public:
    using key_type = T;
    using value_type = T;
    using key_compare = Compare;
    using tree_type = rb_tree<value_type, key_compare>;

    using node_type = typename tree_type::node_type;
    using reference = typename tree_type::const_reference;
    using const_reference = typename tree_type::const_reference;
    using iterator = typename tree_type::const_iterator;
    using const_iterator = typename tree_type::const_iterator;
    using size_type = typename tree_type::size_type;

public:
    set() = default;

    explicit set(const key_compare& compare = key_compare()) : tree_(compare) {}


    set(const set& rhs) : tree_(rhs.tree_) {}

    set(set&& rhs) noexcept : tree_(estd::move(rhs.tree_)) {}

    set(const std::initializer_list<value_type>& list) {
        tree_.insert_unique(list.begin(), list.end());
    }

    template <typename It>
    set(It first, It last, const key_compare& compare = key_compare()) : set(compare) {
        tree_.insert_unique(first, last);
    }

    set& operator=(set rhs) {
        rhs.swap(*this);
        return *this;
    }

    set& operator=(const std::initializer_list<value_type>& list) {
        set(list).swap(*this);
        return *this;
    }
    void swap(set& rhs) {
        tree_.swap(rhs.tree_);
    }

    key_compare kee_comp() const {
        return tree_.compare();
    }

    iterator begin() {
        return tree_.begin();
    }
    const_iterator begin() const {
        return tree_.begin();
    }
    iterator end() {
        return tree_.end();
    }
    const_iterator end() const {
        return tree_.end();
    }

    bool empty() const {
        return tree_.empty();
    }

    size_type size() const {
        return tree_.size();
    }

    template <typename... Args>
    pair<iterator, bool> emplace(Args&&... args) {
        return tree_.emplace_unique(estd::forward<Args>(args)...);
    }

    pair<iterator, bool> insert(const value_type& value) {
        return tree_.insert_unique(value);
    }

    pair<iterator, bool> insert(value_type&& value) {
        return tree_.insert_unique(estd::move(value));
    }

    template <typename It>
    void insert(It first, It last) {
        tree_.insert_unique(first, last);
    }

    void erase(iterator it) {
        tree_.erase(it);
    }
    size_type erase(const key_type& key) {
        return tree_.erase_unique(key);
    }

    void erase(iterator first, iterator last) {
        tree_.erase(first, last);
    }

    void clear() {
        tree_.clear();
    }

    iterator find(const key_type& key) {
        return tree_.find(key);
    }

    const_iterator find(const key_type& key) const {
        return tree_.find(key);
    }

    size_type count(const key_type& key) const {
        return tree_.count_unique(key);
    }

    iterator lower_bound(const key_type& key) {
        return tree_.lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const {
        return tree_.lower_bound(key);
    }

    iterator uppper_bound(const key_type& key) {
        return tree_.upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const {
        return tree_.upper_bound(key);
    }

    pair<iterator, iterator> euqal_range(const key_type& key) {
        return tree_.equal_range_unique(key);
    }

    pair<const_iterator, const_iterator> euqal_range(const key_type& key) const {
        return tree_.equal_range_unique(key);
    }

private:
    tree_type tree_;
};


template <typename T, typename Compare = estd::less<T>>
class multiset {
public:
    using key_type = T;
    using value_type = T;
    using key_compare = Compare;
    using tree_type = rb_tree<value_type, key_compare>;

    using node_type = typename tree_type::node_type;
    using reference = typename tree_type::const_reference;
    using const_reference = typename tree_type::const_reference;
    using iterator = typename tree_type::const_iterator;
    using const_iterator = typename tree_type::const_iterator;
    using size_type = typename tree_type::size_type;

public:
    multiset() = default;

    explicit multiset(const key_compare& compare = key_compare()) : tree_(compare) {}


    multiset(const multiset& rhs) : tree_(rhs.tree_) {}

    multiset(multiset&& rhs) noexcept : tree_(estd::move(rhs.tree_)) {}

    multiset(const std::initializer_list<value_type>& list) {
        tree_.insert_multi(list.begin(), list.end());
    }

    template <typename It>
    multiset(It first, It last, const key_compare& compare = key_compare()) : multiset(compare) {
        tree_.insert_multi(first, last);
    }

    multiset& operator=(multiset rhs) {
        rhs.swap(*this);
        return *this;
    }

    multiset& operator=(const std::initializer_list<value_type>& list) {
        multiset(list).swap(*this);
        return *this;
    }
    void swap(multiset& rhs) {
        tree_.swap(rhs.tree_);
    }

    key_compare kee_comp() const {
        return tree_.compare();
    }

    iterator begin() {
        return tree_.begin();
    }
    const_iterator begin() const {
        return tree_.begin();
    }
    iterator end() {
        return tree_.end();
    }
    const_iterator end() const {
        return tree_.end();
    }

    bool empty() const {
        return tree_.empty();
    }

    size_type size() const {
        return tree_.size();
    }

    template <typename... Args>
    pair<iterator, bool> emplace(Args&&... args) {
        return tree_.emplace_multi(estd::forward<Args>(args)...);
    }

    pair<iterator, bool> insert(const value_type& value) {
        return tree_.insert_multi(value);
    }

    pair<iterator, bool> insert(value_type&& value) {
        return tree_.insert_multi(estd::move(value));
    }

    template <typename It>
    void insert(It first, It last) {
        tree_.insert_multi(first, last);
    }

    void erase(iterator it) {
        tree_.erase(it);
    }
    size_type erase(const key_type& key) {
        return tree_.erase_multi(key);
    }

    void erase(iterator first, iterator last) {
        tree_.erase(first, last);
    }

    void clear() {
        tree_.clear();
    }

    iterator find(const key_type& key) {
        return tree_.find(key);
    }

    const_iterator find(const key_type& key) const {
        return tree_.find(key);
    }

    size_type count(const key_type& key) const {
        return tree_.count_multi(key);
    }

    iterator lower_bound(const key_type& key) {
        return tree_.lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const {
        return tree_.lower_bound(key);
    }

    iterator uppper_bound(const key_type& key) {
        return tree_.upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const {
        return tree_.upper_bound(key);
    }

    pair<iterator, iterator> euqal_range(const key_type& key) {
        return tree_.equal_range_multi(key);
    }

    pair<const_iterator, const_iterator> euqal_range(const key_type& key) const {
        return tree_.equal_range_multi(key);
    }

private:
    tree_type tree_;
};


}   // namespace estd