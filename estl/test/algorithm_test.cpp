#include <algorithm.hpp>
#include <gtest/gtest.h>

TEST(ALGORITHM_TEST,BASE_TEST){

    EXPECT_EQ(estd::min(1,2),1);
    EXPECT_EQ(estd::min(-1,2),-1);
    EXPECT_EQ(estd::max(10,12),12);
    EXPECT_EQ(estd::max(100,2),100);
}