#pragma once
#include <initializer_list>
#include <pair.hpp>
#include <type_traits.hpp>
#include <utility.hpp>
namespace estd {

enum class rb_tree_color { red, black };


template <typename Driver>
struct rb_tree_node_base {
    using rb_tree_node_base_ptr = rb_tree_node_base *;

    Driver *get_driver_ptr() {
        return static_cast<Driver *>(this);
    }

    const Driver *get_driver_ptr() const {
        return static_cast<const Driver *>(this);
    }


    rb_tree_node_base(rb_tree_color node_color,
                      rb_tree_node_base_ptr node_left = nullptr,
                      rb_tree_node_base_ptr node_right = nullptr,
                      rb_tree_node_base_ptr node_parent = nullptr)
        : color(node_color), left(node_left), right(node_right), parent(node_parent) {}
    bool is_black() {
        return color == rb_tree_color::black;
    }

    bool is_red() {
        return color == rb_tree_color::red;
    }

    void set_color_red() {
        color = rb_tree_color::red;
    }
    void set_color_black() {
        color = rb_tree_color::black;
    }

public:
    rb_tree_color color;
    rb_tree_node_base_ptr left;
    rb_tree_node_base_ptr right;
    rb_tree_node_base_ptr parent;
};


template <typename T>
struct rb_tree_node : rb_tree_node_base<rb_tree_node<T>> {
public:
    using rb_tree_node_base_type = rb_tree_node_base<rb_tree_node<T>>;
    using rb_tree_node_base_ptr = rb_tree_node_base_type *;
    using rb_tree_node_ptr = rb_tree_node *;
    using value_type = T;
    using reference = T &;
    using const_ference = const T &;

    rb_tree_node(rb_tree_color color,
                 rb_tree_node_base_ptr left,
                 rb_tree_node_base_ptr right,
                 rb_tree_node_base_ptr parent,
                 const value_type &v)
        : rb_tree_node_base_type(color, left, right, parent), value(v) {}

    rb_tree_node(rb_tree_color color,
                 rb_tree_node_base_ptr left,
                 rb_tree_node_base_ptr right,
                 rb_tree_node_base_ptr parent,
                 value_type &&v) noexcept
        : rb_tree_node_base_type(color, left, right, parent), value(estd::move(v)) {}

    template <typename... Args>
    rb_tree_node(rb_tree_color color,
                 rb_tree_node_base_ptr left,
                 rb_tree_node_base_ptr right,
                 rb_tree_node_base_ptr parent,
                 Args &&...args)
        : rb_tree_node_base_type(color, left, right, parent), value(estd::forward<Args>(args)...) {}

    value_type value;
};

template <typename T, bool>
struct rb_tree_value_traits_impl {
    using key_type = T;
    using value_type = T;
    using mapped_type = T;

    static const key_type &get_key(const key_type &key) {
        return key;
    }

    static const value_type &get_value(const value_type &value) {
        return value;
    }
};


template <typename T>
struct rb_tree_value_traits_impl<T, true> {
    using key_type = remove_cv_t<typename T::first_type>;
    using mapped_type = typename T::second_type;
    using value_type = T;

    template <typename U>
    static const key_type &get_key(const U &value) {
        return value.first;
    }

    template <typename U>
    static const value_type &get_value(const U &value) {
        return value;
    }
};

template <typename T>
struct rb_tree_value_traits {
    using value_traits_type = rb_tree_value_traits_impl<T, is_pair_v<T>>;

    using key_type = typename value_traits_type::key_type;
    using mapped_type = typename value_traits_type::mapped_type;
    using value_type = typename value_traits_type::value_type;

    template <typename U>
    static const key_type &get_key(const U &value) {
        return value_traits_type::get_key(value);
    }

    template <typename U>
    static const value_type &get_value(const U &value) {
        return value_traits_type::get_value(value);
    }
};

template <typename T>
struct rb_tree_traits {
    using value_traits = rb_tree_value_traits<T>;

    using key_type = typename value_traits::key_type;
    using mapped_type = typename value_traits::mapped_type;
    using value_type = typename value_traits::value_type;

    using pointer = value_type *;
    using reference = value_type &;
    using const_pointer = const value_type *;
    using const_reference = const value_type &;

    using node_type = rb_tree_node<T>;
    using base_node_type = rb_tree_node_base<node_type>;

    using base_node_ptr = base_node_type *;
    using node_ptr = node_type *;
};


template <typename T>
struct rb_tree_iterator_base {
public:
    using rb_tree_node_base_ptr = rb_tree_node_base<rb_tree_node<T>> *;

    rb_tree_iterator_base(rb_tree_node_base_ptr ptr) : node(ptr) {}

    rb_tree_iterator_base() = default;
    rb_tree_iterator_base(const rb_tree_iterator_base &) = default;
    void increment() {
        if (node->right) {
            node = node->right;
            while (node->left) {
                node = node->left;
            }
        } else {
            rb_tree_node_base_ptr parent = node->parent;
            while (parent->right == node) {
                node = parent;
                parent = parent->parent;
            }

            if (node->right != parent) node = parent;
        }
    }

    void decrement() {
        if (node->parent->parent == node && node->is_red()) {
            node = node->right;
        } else if (node->left) {
            node = node->left;
            while (node->right) {
                node = node->right;
            }

        } else {
            rb_tree_node_base_ptr parent = node->parent;
            while (node == parent->left) {
                node = parent;
                parent = parent->parent;
            }
            node = parent;
        }
    }

    rb_tree_node_base_ptr node;
};

template <typename T>
struct rb_tree_iterator : public rb_tree_iterator_base<T> {
public:
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using rb_tree_iterator_base<T>::rb_tree_iterator_base;
    using rb_tree_iterator_base<T>::node;

    pointer operator->() {
        return &node->get_driver_ptr()->value;
    }

    reference operator*() {
        return node->get_driver_ptr()->value;
    }

    rb_tree_iterator &operator++() {
        this->increment();
        return *this;
    }

    rb_tree_iterator operator++(int) {
        rb_tree_iterator current(*this);
        this->increment();
        return current;
    }

    rb_tree_iterator &operator--() {
        this->decrement();
        return *this;
    }

    rb_tree_iterator operator--(int) {
        rb_tree_iterator current(*this);
        this->decrement();
        return *this;
    }

    bool operator==(const rb_tree_iterator &rhs) const {
        return node == rhs.node;
    }
    bool operator!=(const rb_tree_iterator &rhs) const {
        return node != rhs.node;
    }
};


template <typename T>
struct rb_tree_const_iterator : public rb_tree_iterator_base<T> {
public:
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const T *;
    using const_refernce = const T &;
    using rb_tree_iterator_base<T>::rb_tree_iterator_base;
    using rb_tree_iterator_base<T>::node;

    const_pointer operator->() {
        return &node->get_driver_ptr()->value;
    }

    const_refernce operator*() {
        return node->get_driver_ptr()->value;
    }

    rb_tree_const_iterator &operator++() {
        this->increment();
        return *this;
    }

    rb_tree_const_iterator operator++(int) {
        rb_tree_iterator current(*this);
        this->increment();
        return current;
    }

    rb_tree_const_iterator &operator--() {
        this->decrement();
        return *this;
    }

    rb_tree_const_iterator operator--(int) {
        rb_tree_iterator current(*this);
        this->decrement();
        return *this;
    }

    bool operator==(const rb_tree_const_iterator &rhs) {
        return node == rhs.node;
    }

    bool operator!=(const rb_tree_const_iterator &rhs) {
        return node != rhs.node;
    }
};

template <typename T>
inline bool rb_tree_node_is_lchild(rb_tree_node_base<T> *node) {
    return node == node->parent->left;
}

template <class T>
rb_tree_node_base<T> *rb_tree_min(rb_tree_node_base<T> *node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

template <class T>
rb_tree_node_base<T> *rb_tree_max(rb_tree_node_base<T> *node) {
    while (node->right) {
        node = node->right;
    }
    return node;
}

template <typename T>
rb_tree_node_base<T> *rb_tree_next(rb_tree_node_base<T> *node) {
    if (node->right) return rb_tree_min(node->right);

    while (!rb_tree_node_is_lchild(node)) {
        node = node->parent;
    }
    return node->parent;
}

template <typename T>
void rb_tree_rotate_left(rb_tree_node_base<T> *node, rb_tree_node_base<T> *&root) {
    rb_tree_node_base<T> *right = node->right;
    node->right = right->left;

    if (right->left) {
        right->left->parent = node;
    }
    right->parent = node->parent;

    if (node == root) {
        root = right;
    } else if (rb_tree_node_is_lchild(node)) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left = node;
    node->parent = right;
}

template <typename T>
void rb_tree_rotate_right(rb_tree_node_base<T> *node, rb_tree_node_base<T> *&root) {
    rb_tree_node_base<T> *left = node->left;
    node->left = left->right;

    if (left->right) {
        left->right->parent = node;
    }
    left->parent = node->parent;

    if (node == root) {
        root = left;
    } else if (rb_tree_node_is_lchild(node)) {
        node->parent->left = left;
    } else {
        node->parent->right = left;
    }

    node->parent = left;
    left->right = node;
}

template <typename T>
void rb_tree_insert_rebalance(rb_tree_node_base<T> *node, rb_tree_node_base<T> *&root) {
    node->set_color_red();
    while (node != root && node->parent->is_red()) {
        if (rb_tree_node_is_lchild(node->parent)) {
            auto uncle = node->parent->parent->right;
            if (uncle && uncle->is_red()) {
                node->parent->set_color_black();
                uncle->set_color_black();
                node = node->parent->parent;
                node->set_color_red();
            } else {
                if (!rb_tree_node_is_lchild(node)) {
                    node = node->parent;
                    rb_tree_rotate_left(node, root);
                }
                node->parent->set_color_black();
                node->parent->set_color_red();
                rb_tree_rotate_right(node->parent->parent, root);
                break;
            }
        } else {
            auto uncle = node->parent->parent->left;
            if (uncle && uncle->is_red()) {
                node->parent->set_color_black();
                uncle->set_color_black();
                node = node->parent->parent;
                node->set_color_red();
            } else {
                if (rb_tree_node_is_lchild(node)) {
                    node = node->parent;
                    rb_tree_rotate_right(node, root);
                }

                node->parent->set_color_black();
                node->parent->parent->set_color_red();
                rb_tree_rotate_left(node->parent->parent, root);
                break;
            }
        }
    }
    root->set_color_black();
}

template <class T>
rb_tree_node_base<T> *rb_tree_erase_rebalance(rb_tree_node_base<T> *z,
                                              rb_tree_node_base<T> *&root,
                                              rb_tree_node_base<T> *&leftmost,
                                              rb_tree_node_base<T> *&rightmost) {
    auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);

    auto x = y->left != nullptr ? y->left : y->right;

    rb_tree_node_base<T> *xp = nullptr;

    if (y != z) {
        z->left->parent = y;
        y->left = z->left;

        if (y != z->right) {
            xp = y->parent;
            if (x != nullptr) x->parent = y->parent;

            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        } else {
            xp = y;
        }

        if (root == z)
            root = y;
        else if (rb_tree_node_is_lchild(z))
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        estd::swap(y->color, z->color);
        y = z;
    }
    else {
        xp = y->parent;
        if (x) x->parent = y->parent;

        if (root == z)
            root = x;
        else if (rb_tree_node_is_lchild(z))
            z->parent->left = x;
        else
            z->parent->right = x;

        if (leftmost == z) leftmost = x == nullptr ? xp : rb_tree_min(x);
        if (rightmost == z) rightmost = x == nullptr ? xp : rb_tree_max(x);
    }


    if (!y->is_red()) {
        while (x != root && (!x || !x->is_red())) {
            if (x == xp->left && xp->right) {
                auto brother = xp->right;
                if (brother->is_red()) {
                    brother->set_color_black();
                    xp->set_color_red();
                    rb_tree_rotate_left(xp, root);
                    brother = xp->right;
                }
                if(!brother)
                  break;
                if ((!brother->left || !brother->left->is_red()) &&
                    (!brother->right || !brother->right->is_red())) {
                    brother->set_color_red();
                    x = xp;
                    xp = xp->parent;
                } else {
                    if (!brother->right || !brother->right->is_red()) {
                        if (brother->left) brother->left->set_color_black();
                        brother->set_color_red();
                        rb_tree_rotate_right(brother, root);
                        brother = xp->right;
                    }
                    brother->color = xp->color;
                    xp->set_color_black();
                    if (brother->right) brother->right->set_color_black();
                    rb_tree_rotate_left(xp, root);
                    break;
                }
            } else if(x == xp->right && xp->left){
                auto brother = xp->left;
                if (brother->is_red()) {
                    brother->set_color_black();
                    xp->set_color_red();
                    rb_tree_rotate_right(xp, root);
                    brother = xp->left;
                }
                if(!brother)
                  break;
                if ((!brother->left || !brother->left->is_red()) &&
                    (!brother->right || !brother->right->is_red())) {
                    brother->set_color_red();
                    x = xp;
                    xp = xp->parent;
                } else {
                    if (!brother->left || !brother->left->is_red()) {
                        if (brother->right) brother->right->set_color_black();
                        brother->set_color_red();
                        rb_tree_rotate_left(brother, root);
                        brother = xp->left;
                    }
                    brother->color = xp->color;
                    xp->set_color_black();
                    if (brother->left) brother->left->set_color_black();
                    rb_tree_rotate_right(xp, root);
                    break;
                }
            }
        }
        if (x != nullptr) x->set_color_black();
    }
    return y;
}

template <typename T>
size_t distance(rb_tree_iterator<T> first, rb_tree_iterator<T> last) {
    size_t n = 0;
    while (first != last) {
        first++;
        n++;
    }
    return n;
}


template <typename T, typename Compare = estd::less<T>>
class rb_tree {
public:
    using tree_traits = rb_tree_traits<T>;
    using value_type = typename tree_traits::value_type;
    using reference = typename tree_traits::reference;
    using pointer = typename tree_traits::pointer;
    using key_type = typename tree_traits::key_type;
    using mapped_type = typename tree_traits::mapped_type;
    using node_type = typename tree_traits::node_type;
    using node_ptr = typename tree_traits::node_ptr;
    using node_base_type = typename tree_traits::base_node_type;
    using node_base_ptr = typename tree_traits::base_node_ptr;

    using const_reference = const T &;
    using rvalue_reference = T &&;
    using key_compare = Compare;
    using iterator = rb_tree_iterator<T>;
    using const_iterator = rb_tree_const_iterator<T>;
    using insert_ret = pair<iterator, bool>;
    using size_type = size_t;

public:
    rb_tree() : header_(new node_base_type(rb_tree_color::red)), count_(0) {
        header_->left = header_;
        header_->right = header_;
    }

    rb_tree(const Compare &c)
        : header_(new node_base_type(rb_tree_color::red)), count_(0), compare_(c) {
        header_->left = header_;
        header_->right = header_;
    }

    rb_tree(Compare &&c)
        : header_(new node_base_type(rb_tree_color::red)), count_(0), compare_(estd::move(c)) {
        header_->left = header_;
        header_->right = header_;
    }


    rb_tree(const rb_tree &rhs) : rb_tree(rhs.compare_) {
        for (auto &it : rhs) {
            insert_multi(it);
        }
    }
    rb_tree(rb_tree &&rhs) noexcept : rb_tree() {
        rhs.swap(*this);
    }
    rb_tree(const std::initializer_list<T> &list) : rb_tree() {
        for (auto &it : list) {
            insert_multi(it);
        }
    }

    rb_tree &operator=(rb_tree rhs) {
        rhs.swap(*this);
        return *this;
    }

    ~rb_tree() {
        clear();
        delete header_;
    }

    void swap(rb_tree &rhs) {
        estd::swap(rhs.header_, header_);
        estd::swap(rhs.count_, count_);
        estd::swap(rhs.compare_, compare_);
    }

public:
    iterator begin() {
        return min_node();
    }
    const_iterator begin() const {
        return min_node();
    }
    iterator end() {
        return header_;
    }
    const_iterator end() const {
        return header_;
    }

    bool empty() {
        return size() == 0;
    }
    size_type size() {
        return count_;
    }

    void clear() {
        if (!empty()) {
            erase_since(root());
            min_node() = header_;
            max_node() = header_;
            root() = nullptr;
            count_ = 0;
        }
    }
    const_iterator find(const key_type &key) const {
        node_base_ptr node = header_;
        node_base_ptr current = root();
        while (current) {
            if (!compare_(get_key(current), key)) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        const_iterator it(node);
        return it == end() || compare_(key, get_key(node)) ? end() : it;
    }
    iterator find(const key_type &key) {
        node_base_ptr node = header_;
        node_base_ptr current = root();
        while (current) {
            if (!compare_(get_key(current), key)) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        iterator it(node);
        return it == end() || compare_(key, get_key(node)) ? end() : it;
    }
    size_type count_multi(const key_type &key) {
        auto &&[first, last] = equal_range_multi(key);
        return distance(first, last);
    }
    size_type count_unique(const key_type &key) {
        return find(key) != end() ? 1 : 0;
    }

    template <typename... Args>
    iterator emplace_multi(Args &&...args) {
        node_ptr node = create_node(estd::forward<Args>(args)...);
        auto &&[pos, on_left] = insert_multi_pos(get_key(node));
        return insert_node_at(pos, node, on_left);
    }

    template <typename... Args>
    insert_ret emplace_unique(Args &&...args) {
        node_ptr node = create_node(estd::forward<Args>(args)...);
        auto &&[pos, success] = insert_unique_pos(get_key(node));

        if (success) {
            return {insert_node_at(pos.first, node, pos.second), success};
        }

        destory_node(node);
        return {pos.first, false};
    }


    iterator insert_multi(const_reference value) {
        return emplace_multi(value);
    }

    iterator insert_multi(rvalue_reference value) {
        return emplace_multi(estd::move(value));
    }

    template <typename It>
    void insert_multi(It first, It last) {
        while (first != last) {
            insert_multi(*first);
        }
    }

    insert_ret insert_unique(const_reference value) {
        return emplace_unique(value);
    }

    insert_ret insert_unique(rvalue_reference value) {
        return emplace_unique(estd::move(value));
    }

    template <typename It>
    void insert_unique(It first, It last) {
        while (first != last) {
            insert_unique(*first);
        }
    }

    pair<iterator, iterator> equal_range_multi(const key_type &key) {
        return {lower_bound(key), upper_bound(key)};
    }
    pair<const_iterator, const_iterator> equal_range_multi(const key_type &key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    pair<iterator, iterator> equal_range_unique(const key_type &key) {
        auto it = find(key);
        return it == end() ? make_pair(it, it) : make_pair(it++, it);
    }
    pair<const_iterator, const_iterator> equal_range_unique(const key_type &key) const{
        auto it = find(key);
        return it == end() ? make_pair(it, it) : make_pair(it++, it);
    }
    iterator erase(iterator hint) {
        auto node = hint.node;
        hint++;
        rb_tree_erase_rebalance(node, root(), min_node(), max_node());
        destory_node(node);
        count_--;
        return hint;
    }

    void erase_since(node_base_ptr node) {
        while (node) {
            erase_since(node->right);
            node_base_ptr left = node->left;
            destory_node(node);
            node = left;
        }
    }

    size_type erase_multi(const key_type &key) {
        auto &&[first, last] = equal_range_multi(key);
        size_t n = distance(first, last);
        erase(first, last);
        return n;
    }

    size_type erase_unique(const key_type &key) {
        auto it = find(key);

        if (it == end()) return 0;

        erase(it);
        return 1;
    }

    void erase(iterator first, iterator last) {
        if (first == begin() && last == end()) {
            clear();
        } else {
            while (first != last) {
                 erase(first++);
            }
        }
    }

    iterator lower_bound(const key_type &key) {
        node_base_ptr node = header_;
        node_base_ptr current = root();

        while (current) {
            if (!compare_(get_key(current), key)) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return node;
    }

    const_iterator lower_bound(const key_type &key) const {
        node_base_ptr node = header_;
        node_base_ptr current = root();

        while (current) {
            if (!compare_(get_key(current), key)) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return node;
    }

    iterator upper_bound(const key_type &key) {
        node_base_ptr node = header_;
        node_base_ptr current = root();

        while (current) {
            if (compare_(key, get_key(current))) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return node;
    }
    const_iterator upper_bound(const key_type &key) const {
        node_base_ptr node = header_;
        node_base_ptr current = root();

        while (current) {
            if (compare_(key, get_key(current))) {
                node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return node;
    }

private:
    node_base_ptr &root() {
        return header_->parent;
    }
    node_base_ptr &min_node() {
        return header_->left;
    }
    const node_base_ptr &min_node() const {
        return header_->left;
    }
    node_base_ptr &max_node() {
        return header_->right;
    }
    key_type get_key(node_base_ptr node) {
        return tree_traits::value_traits::get_key(node->get_driver_ptr()->value);
    }
    key_type get_key(iterator it) {
        return tree_traits::value_traits::get_key(*it);
    }
    pair<node_base_ptr, bool> insert_multi_pos(const key_type &key) {
        bool on_left = true;
        node_base_ptr child = root();
        node_base_ptr node = header_;

        while (child) {
            node = child;
            on_left = compare_(key, get_key(child));
            child = on_left ? node->left : node->right;
        }

        return {node, on_left};
    }
    pair<pair<node_base_ptr, bool>, bool> insert_unique_pos(const key_type &key) {
        bool on_left = true;
        node_base_ptr child = root();
        node_base_ptr node = header_;

        while (child) {
            node = child;
            on_left = compare_(key, get_key(child));
            child = on_left ? node->left : node->right;
        }

        iterator it(node);

        if (on_left) {
            if (node == header_ || it == begin()) {
                return {pair(node, on_left), true};
            } else {
                --it;
            }
        }

        if (compare_(get_key(it), key)) {
            return {pair(node, on_left), true};
        }

        return {pair(node, on_left), false};
    }
    iterator insert_value_at(node_base_ptr node, const value_type &value, bool on_left) {
        return insert_node_at(node, create_node(value), on_left);
    }

    iterator insert_node_at(node_base_ptr node, node_ptr insert_node, bool on_left) {
        insert_node->parent = node;
        if (node == header_) {
            root() = insert_node;
            min_node() = insert_node;
            max_node() = insert_node;
        } else if (on_left) {
            node->left = insert_node;
            if (min_node() == node) {
                min_node() = insert_node;
            }
        } else {
            node->right = insert_node;
            if (max_node() == node) {
                max_node() = insert_node;
            }
        }

        rb_tree_insert_rebalance(insert_node, root());

        count_++;
        return iterator(insert_node);
    }


    template <typename... Args>
    node_ptr create_node(Args &&...args) {
        return new node_type(
            rb_tree_color::red, nullptr, nullptr, nullptr, estd::forward<Args>(args)...);
    }

    void destory_node(node_base_ptr node) {
        delete node->get_driver_ptr();
    }

private:
    node_base_ptr header_;
    size_type count_;
    key_compare compare_;
};

}   // namespace estd