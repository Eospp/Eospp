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
    //copy construtor
    estd::any v2 = 2;
    estd::any v3(v2);
    p      = estd::any_cast<int>(v3);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,2);
    //move construtor
    estd::any v4(estd::move(v3));
    EXPECT_TRUE(v3.empty());
    p      = estd::any_cast<int>(v4);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,2);
    //copy opeator=
    estd::any v5 = v4;
    p      = estd::any_cast<int>(v5);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,2);
    EXPECT_TRUE(v4);
    //move operator=
    estd::any v6 = estd::move(v5);
    EXPECT_FALSE(v5);
    p      = estd::any_cast<int>(v6);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p,2);
}