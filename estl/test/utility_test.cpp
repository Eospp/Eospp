#include <utility.hpp>
#include <gtest/gtest.h>

TEST(UTILITY_TEST,BASE_TEST)
{
    int v = 5;
    
    EXPECT_EQ(typeid(decltype(estd::move(v))),typeid(int&&));
    EXPECT_EQ(typeid(decltype(estd::forward<int&>(v))),typeid(int&));
    EXPECT_EQ(typeid(decltype(estd::forward<int&&>(v))),typeid(int&&));

    int lhs = 1;
    int rhs = 2;
    
    estd::swap(lhs,rhs);
    EXPECT_EQ(lhs,2);
    EXPECT_EQ(rhs,1);

}