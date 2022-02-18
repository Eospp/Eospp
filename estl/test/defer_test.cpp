#include <gtest/gtest.h>
#include <defer.hpp>


TEST(DEFER_TEST,BAST_TEST)
{
    bool flag = false;
    {
        defer_scope [&flag]()
        {
            flag = true;
        };
    }
    EXPECT_TRUE(flag);

    flag = false;
    {
        defer_scope [&flag]()
        {
            flag = true;
        };
    }
    
    EXPECT_TRUE(flag);
}