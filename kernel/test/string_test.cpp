#include <util/string.hpp>
#include <gtest/gtest.h>
using namespace eospp;
TEST(STRING_TEST,MEMCPY_TEST){

    {
        uint8_t operand1 = '1';
        uint8_t operand2;

        util::memcpy(&operand1,&operand2,sizeof(operand1));
        EXPECT_EQ(operand1,operand2);
    }
    {
        uint16_t operand1 = 77;
        uint16_t operand2;

        util::memcpy(&operand1,&operand2,sizeof(operand1));
        EXPECT_EQ(operand1,operand2);
    }
    
    {
        uint32_t operand1 = 77;
        uint32_t operand2;

        util::memcpy(&operand1,&operand2,sizeof(operand1));
        EXPECT_EQ(operand1,operand2);
    }
    
    {
        uint64_t operand1 = 77;
        uint64_t operand2;

        util::memcpy(&operand1,&operand2,sizeof(operand1));
        EXPECT_EQ(operand1,operand2);
    }

    char str[] = "hello world";
    char buf[1024];

    char *end = static_cast<char*>(util::memcpy(buf,str,sizeof(str)));
    std::string tmp(buf);
    EXPECT_EQ(tmp,str);
    EXPECT_EQ(end - buf,sizeof(str));

}

TEST(STRING_TEST,MEMMOVE_TEST){
    char str[] = "hello world";
    char *end = static_cast<char*>(util::memmove(str,str + 2,sizeof(str) - 2));
    std::string tmp("llo world");
    EXPECT_EQ(tmp,str);
    EXPECT_EQ(end - str,sizeof(str) - 2);

    char hello_eospp[] = "hello eospp hello";
    util::memmove(hello_eospp + 6,hello_eospp,11);
    tmp = "hello hello eospp";
    EXPECT_EQ(tmp,hello_eospp);

}

TEST(STRING_TEST,STRING_LEN_TEST){

    char str[] = "hello world";
    char empty_str[] = "";
    EXPECT_EQ(strlen(str),sizeof(str) - 1);
    EXPECT_EQ(strlen(empty_str),sizeof(empty_str) - 1);
}

TEST(STRING_TEST,STRING_COPY_TEST){
    char hello[] = "hello world";
    std::string str(hello);
    char buf[1024];
    strcpy(buf,hello);
    EXPECT_EQ(strlen(buf),sizeof(hello) - 1);
    EXPECT_EQ(str,buf);
}

TEST(STRING_TEST,STRING_SNPRINTF_TEST){
    char buf[1024];
    util::snprintf(buf,sizeof(buf),"hello %s %d","world",10);
    EXPECT_EQ(strlen(buf),14);
    std::string str("hello world 10");
    EXPECT_EQ(str,buf);

    util::snprintf(buf,sizeof(buf),"hello %.1f %d",1.2,-102);
    str = "hello 1.2 -102";
    EXPECT_EQ(str,buf);
}
