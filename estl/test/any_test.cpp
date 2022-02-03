#include <gtest/gtest.h>
#include <any.hpp>


TEST(ANY_TEST,BASE_TEST)
{
    estd::any v = 1;
    int *p      = estd::any_cast<int>(v);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,1);
    double *d   = estd::any_cast<double>(v);
    ASSERT_TRUE(d == nullptr);
    EXPECT_FALSE(v.empty());
    v.clear();
    EXPECT_TRUE(v.empty());
    v = 2.2;
    d = estd::any_cast<double>(v);
    ASSERT_TRUE(d != nullptr);
    EXPECT_EQ(*d,2.2);
}