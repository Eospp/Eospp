#pragma once
#include <utility.hpp>

namespace estd {

enum rb_tree_color { red, black };



template <typename Driver>
struct rb_tree_node_base {
    using rb_tree_node_base_ptr = rb_tree_node_base*;

    Driver* get_node_ptr() const {
        return static_cast<Driver*>(this);
    }

    
public:
    rb_tree_color color;
    rb_tree_node_base_ptr left;
    rb_tree_node_base_ptr right;
    rb_tree_node_base_ptr parent;
};


template <typename T>
struct rb_tree_node : rb_tree_node_base<T> {
public:
    using rb_tree_node_base_ptr = rb_tree_node_base<T>*;
    using rb_tree_node_ptr = rb_tree_node*;
    using value_type = T;
    using reference = T&;
    using const_ference = const T&;


    value_type value;
};

template<typename T>
inline bool rb_tree_node_is_red(rb_tree_node_base<T> *node){
    return node->color == rb_tree_color::red;
}

template<typename T>
inline bool rb_tree_node_is_black(rb_tree_node_base<T> *node){
    return node->color == rb_tree_color::black;
}

template <typename T>
struct rb_tree_iterator_base {
public:
    using rb_tree_node_base_ptr = rb_tree_node_base<T>*;

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

    void decrement(){
        if(node->parent->parent == node && rb_tree_node_is_red(node)){
            node = node->right;
        }else if(node->left){
            node = node->left;
            while(node->right)
            {
                node = node->right;
            }

        }else{
            rb_tree_node_base_ptr parent = node->parent;
            while(node == parent->left){
                node = parent;
                parent = parent->parent;
            }
            node = parent;
        }


    }

    rb_tree_node_base_ptr node;
};


}   // namespace estd