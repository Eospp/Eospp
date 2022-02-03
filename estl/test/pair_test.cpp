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

    

}