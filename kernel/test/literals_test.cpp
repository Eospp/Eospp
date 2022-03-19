#include <core/memory_literals.hpp>
#include <gtest/gtest.h>

TEST(LITERALS_TEST,BASE_TEST){
    using namespace eospp::memory;
    EXPECT_EQ(1_KB,1024);
    EXPECT_EQ(1_MB,1024 * 1024);
    EXPECT_EQ(1_GB,1024 * 1024 * 1024);
    EXPECT_EQ(2_KB,2ul * 1024);
    EXPECT_EQ(2_MB,2ul * 1024 * 1024);
    EXPECT_EQ(2_GB,2ul * 1024 * 1024 * 1024);

}