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
    EXPECT_FALSE(value);
         value = estd::is_same<int,int*>::value;
    EXPECT_FALSE(value);
         value = estd::is_same<int,int&>::value;
    EXPECT_FALSE(value);
         value = estd::is_same<int,int>::value;
    EXPECT_TRUE(value);
    //is_function
    auto func = [](int a,int b){return a + b;};
    EXPECT_FALSE(estd::is_function<int*>::value);
    EXPECT_FALSE(estd::is_function<decltype(func)>::value);
    EXPECT_TRUE (estd::is_function<decltype(printf)>::value);
    EXPECT_FALSE(estd::is_function<std::function<void(int)>>::value);
    //is_member_pointer
    EXPECT_FALSE(estd::is_member_pointer<int*>::value);
    class A{};
    EXPECT_TRUE (estd::is_member_pointer<int(A::*)>::value);

}

TEST(TYPE_TRAITS,ADD_TYPE_TEST)
{
    EXPECT_TRUE(typeid(const int)          == typeid(estd::add_const_t<int>));
    EXPECT_TRUE(typeid(volatile int)       == typeid(estd::add_volatile_t<int>));
    EXPECT_TRUE(typeid(const volatile int) == typeid(estd::add_cv_t<int>));
    EXPECT_TRUE(typeid(int &)              == typeid(estd::add_lvalue_reference_t<int>));
    EXPECT_TRUE(typeid(int&&)              == typeid(estd::add_rvalue_reference_t<int>));
    EXPECT_TRUE(typeid(int*)               == typeid(estd::add_pointer_t<int>));

    EXPECT_TRUE(typeid(int)                == typeid(estd::conditional_t<true,int,double>));
    EXPECT_TRUE(typeid(double)             == typeid(estd::conditional_t<false,int,double>));
}
int func(int,int){}
TEST(TYPE_TRAITS,REMOVE_TYPE_TEST)
{
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_const_t<int const>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_volatile_t<volatile int>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_cv_t<int const volatile>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_reference_t<int&>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_reference_t<int&&>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_extent_t<int[5]>));
    EXPECT_TRUE(typeid(int[5])             == typeid(estd::remove_extent_t<int[2][5]>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::remove_all_extent_t<int[2][3][5]>));

    EXPECT_TRUE(typeid(int*)               == typeid(estd::decay_t<int[2]>));
    EXPECT_TRUE(typeid(int)                == typeid(estd::decay_t<int const volatile>));
    EXPECT_TRUE(typeid(int(*)(int,int))    == typeid(estd::decay_t<decltype(func)>));
}