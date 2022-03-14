#include <gtest/gtest.h>
#include <array.hpp>

TEST(ARRAY_TEST,BASE_TEST){
    
    const size_t n = 20;
    estd::array<int,n> array;
    array.fill(77);

    EXPECT_EQ(array.size(),n);
    EXPECT_FALSE(array.empty());

    for(auto it : array){
        EXPECT_EQ(it,77);
    }

    array.front() = 11;
    array.back()  = 88;
    EXPECT_EQ(array.front(),11);
    EXPECT_EQ(array.back(),88);

    array[1] = 22;
    array[2] = 33;
    EXPECT_EQ(array[1],22);
    EXPECT_EQ(array[2],33);
}