#include <gtest/gtest.h>
#include <rb_tree.hpp>
#include "class.hpp"
TEST(RB_TREE_TEST,CONSTRUCTOR_TEST){
    //constructor
    estd::rb_tree<int> tree{1,2,3,4,5};
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(),5);
    int n = 1;
    for(auto it : tree){
        EXPECT_EQ(it,n++);
    }


    //copy_constructor
    estd::rb_tree<int> tree1(tree);
    EXPECT_FALSE(tree.empty());
    EXPECT_FALSE(tree1.empty());
    EXPECT_EQ(tree.size(),5);
    EXPECT_EQ(tree1.size(),5);
    n = 1;
    for(auto it : tree1){
        EXPECT_EQ(it,n++);
    }


    //move_constructor
    estd::rb_tree<int> tree2(estd::move(tree1));
    EXPECT_FALSE(tree2.empty());
    EXPECT_FALSE(!tree1.empty());
    EXPECT_EQ(tree2.size(),5);
    EXPECT_EQ(tree1.size(),0);
    n = 1;
    for(auto it : tree2){
        EXPECT_EQ(it,n++);
    }
    
    //copy operator=
    estd::rb_tree<int> tree3;
    tree3 = tree2;
    EXPECT_FALSE(tree3.empty());
    EXPECT_FALSE(tree2.empty());
    EXPECT_EQ(tree2.size(),5);
    EXPECT_EQ(tree3.size(),5);
    n = 1;
    for(auto it : tree3){
        EXPECT_EQ(it,n++);
    }

    //move operator=
    estd::rb_tree<int> tree4;
    tree4 = estd::move(tree3);
    EXPECT_FALSE(tree4.empty());
    EXPECT_FALSE(!tree3.empty());
    EXPECT_EQ(tree4.size(),5);
    EXPECT_EQ(tree3.size(),0);
    n = 1;
    for(auto it : tree4){
        EXPECT_EQ(it,n++);
    }

}

TEST(RB_TREE_TEST,INTERFACE_TEST){
    //ascending sort
    {
        estd::rb_tree<int,estd::less<int>> tree{1,2,3,4,5,6};
        EXPECT_EQ(tree.size(),6);
        int n = 1;
        for(auto &it : tree){
            EXPECT_EQ(it,n++);
        }
    }

    //descending sort
    {
        estd::rb_tree<int,estd::greater<int>> tree{1,2,3,4,5,6};
        int n = 6;
        for(auto &it : tree){
            EXPECT_EQ(it,n--);
        }
    }

    //insert_unique
    estd::rb_tree<int> tree;
    {
        auto &&[pos,flag] = tree.insert_unique(1);
        ASSERT_TRUE(flag);
        EXPECT_EQ(*pos,1);
    }
    {   
        auto &&[pos,flag] = tree.insert_unique(1);
        ASSERT_TRUE(!flag);
    }
    //insert_multi
    {
        for(int i = 0;i < 5;i++)
          tree.insert_multi(1);
        EXPECT_EQ(tree.size(),6);

        for(auto it : tree){
            EXPECT_EQ(it,1);
        }
    }

    tree.clear();
    EXPECT_EQ(tree.size(),0);
    EXPECT_TRUE(tree.empty());

    for(int i = 0;i < 100000; i += 2)
       tree.insert_unique(i);
    
    //find
    auto it = tree.find(0);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(*it,0);

    it = tree.find(1);
    ASSERT_TRUE(it == tree.end());

    for(int i = 0;i < 100000; i += 2){
        it = tree.find(i);
        ASSERT_TRUE(it != tree.end());
        EXPECT_EQ(*it,i);
    }

    //lower_bound & upper_bound
    it = tree.lower_bound(2);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(*it,2);

    it = tree.upper_bound(2);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(*it,4);

    it = tree.lower_bound(-1);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(*it,0);

    it = tree.upper_bound(-1);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(*it,0);

    it = tree.lower_bound(100000);
    ASSERT_TRUE(it == tree.end());

    it = tree.upper_bound(100000);
    ASSERT_TRUE(it == tree.end());

    //equal_range_unique
    { 
        auto &&[first,last] = tree.equal_range_unique(2);
        ASSERT_TRUE(first != tree.end());
        ASSERT_TRUE(last != tree.end());
        EXPECT_EQ(*first,2);
    }
    {
        auto &&[first,last] = tree.equal_range_unique(1);
        ASSERT_TRUE(first == tree.end());
        ASSERT_TRUE(last  == tree.end());
    }
    //equal_range_multi
    for(int i = 0;i < 10; i++){
        tree.insert_multi(11);
    }
    {
        auto &&[first,last] = tree.equal_range_multi(11);
        ASSERT_TRUE(first != tree.end());

        int n = 0;
        while(first != last){
            EXPECT_EQ(*first,11);
            n++;
            first++;
        } 
        EXPECT_EQ(n,10);
    }
    //erase_unique
    std::size_t n = tree.erase_unique(2);
    EXPECT_EQ(n,1);

    it = tree.find(2);
    ASSERT_TRUE(it == tree.end());

    n = tree.erase_unique(1);
    EXPECT_EQ(n,0);

    //count_unique
    n = tree.count_unique(2);
    EXPECT_EQ(n,0);

    n = tree.count_unique(6);
    EXPECT_EQ(n,1);
    
    //count_multi
    for(int i = 0;i < 10; i++)
      tree.insert_multi(2);

    n = tree.count_multi(2);
    EXPECT_EQ(n,10);

    //erase_multi
    n = tree.erase_multi(2);
    EXPECT_EQ(n,10);
    
    it = tree.find(2);
    ASSERT_TRUE(it == tree.end());

    n = tree.erase_unique(12);
    EXPECT_EQ(n,1);

    estd::rb_tree<int> tree1;
    
    for(int i = 0;i < 10000; i++){
       auto &&[pos,flag] = tree1.insert_unique(i);
       ASSERT_TRUE(flag);
    }

    for(int i = 0;i < 100; i++){
        n = tree1.erase_unique(i);
        EXPECT_EQ(n,1);

        int j = i + 1;
        for(auto it : tree1){
            EXPECT_EQ(it,j++);
        }
    }

    
}

TEST(RB_TREE_TEST,OBJECT_TEST){
    
    using namespace estd::test;
    auto compare = [](const A &lhs,const A &rhs)
    {   
        return *lhs < *rhs;
    };

    estd::rb_tree<A,decltype(compare)> tree(compare);

    for(int i = 0;i < 100;i++){
        auto &&[pos,flag] = tree.emplace_unique(i);
        ASSERT_TRUE(flag);
        EXPECT_EQ(*pos->value,i);
    }
    EXPECT_EQ(tree.size(),100);

    auto it = tree.find(0);
    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(**it,0);
    
    tree.clear();
    EXPECT_TRUE(tree.empty());


    A a(100);
    tree.insert_unique(a);

    ASSERT_TRUE(a.value != nullptr);

    it = tree.find(100);

    ASSERT_TRUE(it != tree.end());
    EXPECT_EQ(**it,100);

    tree.insert_multi(estd::move(a));
    ASSERT_TRUE(a.value == nullptr);

    size_t n = tree.count_multi(100);
    EXPECT_EQ(n,2);
}