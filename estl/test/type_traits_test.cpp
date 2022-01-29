#include <gtest/gtest.h>
#include <type_traits.hpp>

TEST(TYPE_TRAITS,IS_TEST)
{
    //true_type
    EXPECT_TRUE (estd::true_type::value);
    //false_type
    EXPECT_FALSE(estd::false_type::value);
    //is_pointer
    EXPECT_FALSE(estd::is_pointer<int>::value);
    EXPECT_TRUE (estd::is_pointer<int*>::value);
    EXPECT_TRUE (estd::is_pointer<int**>::value);
    //is_array
    EXPECT_FALSE(estd::is_array<int>::value);
    EXPECT_FALSE(estd::is_array<int*>::value);
    EXPECT_TRUE (estd::is_array<int[5]>::value);
    EXPECT_TRUE (estd::is_array<int[2][5]>::value);
    //is_const
    EXPECT_FALSE(estd::is_const<volatile int>::value);
    EXPECT_FALSE(estd::is_const<int>::value);
    EXPECT_TRUE (estd::is_const<const int>::value);
    EXPECT_TRUE (estd::is_const<const volatile int>::value);
    //is_volatile
    EXPECT_FALSE(estd::is_volatile<int>::value);
    EXPECT_FALSE(estd::is_volatile<const int>::value);
    EXPECT_TRUE (estd::is_volatile<volatile int>::value);
    EXPECT_TRUE (estd::is_volatile<const volatile int>::value);
    //is_reference
    EXPECT_FALSE(estd::is_reference<int>::value);
    EXPECT_TRUE (estd::is_reference<int&>::value);
    EXPECT_TRUE (estd::is_reference<int&&>::value);
    EXPECT_FALSE(estd::is_rvalue_reference<int>::value);
    EXPECT_FALSE(estd::is_rvalue_reference<int&>::value);
    EXPECT_TRUE (estd::is_rvalue_reference<int&&>::value);
    EXPECT_FALSE(estd::is_lvalue_reference<int>::value);
    EXPECT_FALSE(estd::is_lvalue_reference<int&&>::value);
    EXPECT_TRUE (estd::is_lvalue_reference<int&>::value);
    //is_same
    bool value = estd::is_same<int,double>::value;
    EXPECT_FALSE(value);
         value = estd::is_same<int,const int>::value;
    EXPECT_TRUE(value);
}

TEST(TYPE_TRAITS,ADD_TYPE_TEST)
{
    
}

TEST(TYPE_TRAITS,REMOVE_TYPE_TEST)
{

}