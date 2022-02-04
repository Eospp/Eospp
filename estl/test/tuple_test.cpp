#include <gtest/gtest.h>
#include <tuple.hpp>

TEST(TUPLE_TEST,BASE_TEST)
{
    estd::tuple<int,int> t = estd::make_tuple(1,2);
    EXPECT_EQ(estd::get<0>(t),1);
    EXPECT_EQ(estd::get<1>(t),2);
    estd::get<0>(t) = 5;
    estd::get<1>(t) = 10;
    EXPECT_EQ(estd::get<0>(t),5);
    EXPECT_EQ(estd::get<1>(t),10);

    estd::tuple<int,int> t1(100,200);
    EXPECT_EQ(estd::get<0>(t1),100);
    EXPECT_EQ(estd::get<1>(t1),200);

    auto t2 = t1;
    EXPECT_EQ(estd::get<0>(t2),100);
    EXPECT_EQ(estd::get<1>(t2),200);

}