#include <gtest/gtest.h>
#include <bitset.hpp>


TEST(BITSET_TEST,BASE_TEST){
    
    estd::bitset<5> s;
    EXPECT_EQ(s.count(),5);
    EXPECT_EQ(s[0],false);
    EXPECT_EQ(s[1],false);
    EXPECT_EQ(s[2],false);
    EXPECT_EQ(s[3],false);
    EXPECT_EQ(s[4],false);


    s.set(0);
    EXPECT_TRUE(s[0]);
    EXPECT_TRUE(s.test(0));

    s.set(1);
    EXPECT_TRUE(s[1]);
    EXPECT_TRUE(s.test(1));

    s.clear(0);
    EXPECT_FALSE(s[0]);
    EXPECT_FALSE(s.test(0));

    estd::bitset<100> s1;

    for(int i = 0;i < 100; i++){
        s1.set(i);
        EXPECT_TRUE(s1[i]);
        EXPECT_TRUE(s1.test(i));
    }

    s1.reset(false);
    
    for(int i = 0;i < 100; i++){
       EXPECT_FALSE(s1[i]);
       EXPECT_FALSE(s1.test(i));
    }

    
}