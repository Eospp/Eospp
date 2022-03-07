#include <gtest/gtest.h>
#include <rb_tree.hpp>

TEST(RB_TREE_TEST,BASE_TEST){
    estd::rb_tree<int> tree;
    tree.insert_unique(2);
    auto it = tree.find(2);
    ASSERT_TRUE(it != tree.end());
    EXPECT_TRUE(*it == 2);
    {
        auto &&[pos,ret] = tree.insert_unique(2);
        EXPECT_FALSE(ret);
    }
    {
        tree.insert_multi(2);
        EXPECT_TRUE(tree.size() == 2);
        for(auto it : tree){
            EXPECT_EQ(it,2);
        }

    }
}