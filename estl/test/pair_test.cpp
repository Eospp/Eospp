#include <pair.hpp>
#include <gtest/gtest.h>


TEST(PAIR_TEST,BAST_TEST)
{
    estd::pair<int,int> p = estd::make_pair(1,2);
    EXPECT_EQ(p.first,1);
    EXPECT_EQ(p.second,2);

    auto p1 = p;
    EXPECT_EQ(p1.first,1);
    EXPECT_EQ(p1.second,2);

    EXPECT_EQ(p,p1);
    
    auto p3 = estd::make_pair(1,3);
    auto p4 = estd::make_pair(2,3);

    EXPECT_FALSE(p3 == p4);
    EXPECT_TRUE (p3 <  p4);
    EXPECT_TRUE (p3 <= p4);
    EXPECT_FALSE(p3 >  p4);
    EXPECT_FALSE(p3 >= p4);

    EXPECT_FALSE(p1 == p3);
    EXPECT_TRUE (p1 <  p3);
    EXPECT_TRUE (p1 <= p3);    
    EXPECT_FALSE(p1 >  p3);
    EXPECT_FALSE(p1 >= p3);

    //swap
    auto p5 = estd::make_pair(66,88);
    estd::swap(p3,p5);
    EXPECT_EQ(p3.first,66);
    EXPECT_EQ(p3.second,88);
    EXPECT_EQ(p5.first,1);
    EXPECT_EQ(p5.second,3);

}