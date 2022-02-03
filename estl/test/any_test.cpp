#include <gtest/gtest.h>
#include <any.hpp>

class A
{
public:
      A(int v) : m_a(v){}
      int m_a;
};

TEST(ANY_TEST,BASE_TEST)
{

    estd::any v = 1;
    int *p      = estd::any_cast<int>(v);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,1);
    //any_cast
    double *d   = estd::any_cast<double>(v);
    ASSERT_TRUE(d == nullptr);
    EXPECT_FALSE(v.empty());
    //clear & empty
    v.clear();
    EXPECT_TRUE(v.empty());
    v = 2.2;
    d = estd::any_cast<double>(v);
    ASSERT_TRUE(d != nullptr);
    EXPECT_EQ(*d,2.2);
    //emplace
    v.emplace<A>(3);
    A *a = estd::any_cast<A>(v);
    ASSERT_TRUE(a != nullptr);
    EXPECT_EQ   (a->m_a,3);
    //make_any
    estd::any v1 = estd::make_any<A>(5);
    a = estd::any_cast<A>(v1);
    ASSERT_TRUE(a != nullptr);
    EXPECT_EQ(a->m_a,5);
    //swap
    estd::swap(v,v1);
    a = estd::any_cast<A>(v);
    ASSERT_TRUE(a != nullptr);
    EXPECT_EQ(a->m_a,5);
    
}