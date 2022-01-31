#include <gtest/gtest.h>
#include <type_traits.hpp>

TEST(TYPE_TRAITS,IS_TEST)
{
    //true_type
    EXPECT_TRUE (estd::true_type::value);
    //false_type
    EXPECT_FALSE(estd::false_type::value);
    //is_pointer
    EXPECT_FALSE(estd::is_pointer_v<int>);
    EXPECT_TRUE (estd::is_pointer_v<int*>);
    EXPECT_TRUE (estd::is_pointer_v<int**>);
    //is_array
    EXPECT_FALSE(estd::is_array_v<int>);
    EXPECT_FALSE(estd::is_array_v<int*>);
    EXPECT_TRUE (estd::is_array_v<int[5]>);
    EXPECT_TRUE (estd::is_array_v<int[2][5]>);
    //is_const
    EXPECT_FALSE(estd::is_const_v<volatile int>);
    EXPECT_FALSE(estd::is_const_v<int>);
    EXPECT_TRUE (estd::is_const_v<const int>);
    EXPECT_TRUE (estd::is_const_v<const volatile int>);
    //is_volatile
    EXPECT_FALSE(estd::is_volatile_v<int>);
    EXPECT_FALSE(estd::is_volatile_v<const int>);
    EXPECT_TRUE (estd::is_volatile_v<volatile int>);
    EXPECT_TRUE (estd::is_volatile_v<const volatile int>);
    //is_reference
    EXPECT_FALSE(estd::is_reference_v<int>);
    EXPECT_TRUE (estd::is_reference_v<int&>);
    EXPECT_TRUE (estd::is_reference_v<int&&>);
    EXPECT_FALSE(estd::is_rvalue_reference_v<int>);
    EXPECT_FALSE(estd::is_rvalue_reference_v<int&>);
    EXPECT_TRUE (estd::is_rvalue_reference_v<int&&>);
    EXPECT_FALSE(estd::is_lvalue_reference_v<int>);
    EXPECT_FALSE(estd::is_lvalue_reference_v<int&&>);
    EXPECT_TRUE (estd::is_lvalue_reference_v<int&>);
    //is_same
    EXPECT_FALSE((estd::is_same_v<int,double>));
    EXPECT_FALSE((estd::is_same_v<int,const int>));
    EXPECT_FALSE((estd::is_same_v<int,int*>));
    EXPECT_FALSE((estd::is_same_v<int,int&>));
    EXPECT_TRUE ((estd::is_same_v<int,int>));
    //is_function
    auto func = [](int a,int b){return a + b;};
    EXPECT_FALSE(estd::is_function_v<int*>);
    EXPECT_FALSE(estd::is_function_v<decltype(func)>);
    EXPECT_TRUE (estd::is_function_v<decltype(printf)>);
    EXPECT_FALSE(estd::is_function_v<std::function<void(int)>>);
    //is_member_pointer
    EXPECT_FALSE(estd::is_member_pointer_v<int*>);
    struct A
    {
        A(int ){}
        A(int,int){}
    };
    EXPECT_TRUE (estd::is_member_pointer_v<int(A::*)>);
    //is_convertible
    EXPECT_TRUE ((estd::is_convertible_v<A,int,int>));
    EXPECT_TRUE ((estd::is_convertible_v<A,int>));
    EXPECT_FALSE((estd::is_convertible_v<A,int*>));
    EXPECT_FALSE((estd::is_convertible_v<A,int,int,double>));
    EXPECT_TRUE ((estd::is_convertible_v<int,double>));
    EXPECT_TRUE ((estd::is_convertible_v<int,char>));
}

TEST(TYPE_TRAITS,ADD_TYPE_TEST)
{
    EXPECT_EQ(typeid(const int),typeid(estd::add_const_t<int>));
    EXPECT_EQ(typeid(volatile int),typeid(estd::add_volatile_t<int>));
    EXPECT_EQ(typeid(const volatile int),typeid(estd::add_cv_t<int>));
    EXPECT_EQ(typeid(int &),typeid(estd::add_lvalue_reference_t<int>));
    EXPECT_EQ(typeid(int&&),typeid(estd::add_rvalue_reference_t<int>));
    EXPECT_EQ(typeid(int*),typeid(estd::add_pointer_t<int>));

    EXPECT_EQ(typeid(int),typeid(estd::conditional_t<true,int,double>));
    EXPECT_EQ(typeid(double),typeid(estd::conditional_t<false,int,double>));
}
int func(int,int){}
TEST(TYPE_TRAITS,REMOVE_TYPE_TEST)
{
    EXPECT_EQ(typeid(int),typeid(estd::remove_const_t<int const>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_volatile_t<volatile int>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_cv_t<int const volatile>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_reference_t<int&>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_reference_t<int&&>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_extent_t<int[5]>));
    EXPECT_EQ(typeid(int[5]),typeid(estd::remove_extent_t<int[2][5]>));
    EXPECT_EQ(typeid(int),typeid(estd::remove_all_extent_t<int[2][3][5]>));

    EXPECT_EQ(typeid(int*),typeid(estd::decay_t<int[2]>));
    EXPECT_EQ(typeid(int),typeid(estd::decay_t<int const volatile>));
    EXPECT_EQ(typeid(int(*)(int,int)),typeid(estd::decay_t<decltype(func)>));
}