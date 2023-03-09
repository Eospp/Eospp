#include <gtest/gtest.h>
#include <pair.hpp>
#include <type_traits.hpp>
TEST(TYPE_TRAITS, IS_TEST) {
    // true_type
    EXPECT_TRUE(estd::true_type::value);
    // false_type
    EXPECT_FALSE(estd::false_type::value);
    // is_pointer
    EXPECT_FALSE(estd::is_pointer_v<int>);
    EXPECT_TRUE(estd::is_pointer_v<int*>);
    EXPECT_TRUE(estd::is_pointer_v<int**>);
    // is_array
    EXPECT_FALSE(estd::is_array_v<int>);
    EXPECT_FALSE(estd::is_array_v<int*>);
    EXPECT_TRUE(estd::is_array_v<int[5]>);
    EXPECT_TRUE(estd::is_array_v<int[2][5]>);
    // is_const
    EXPECT_FALSE(estd::is_const_v<volatile int>);
    EXPECT_FALSE(estd::is_const_v<int>);
    EXPECT_TRUE(estd::is_const_v<const int>);
    EXPECT_TRUE(estd::is_const_v<const volatile int>);
    // is_volatile
    EXPECT_FALSE(estd::is_volatile_v<int>);
    EXPECT_FALSE(estd::is_volatile_v<const int>);
    EXPECT_TRUE(estd::is_volatile_v<volatile int>);
    EXPECT_TRUE(estd::is_volatile_v<const volatile int>);
    // is_reference
    EXPECT_FALSE(estd::is_reference_v<int>);
    EXPECT_TRUE(estd::is_reference_v<int&>);
    EXPECT_TRUE(estd::is_reference_v<int&&>);
    EXPECT_FALSE(estd::is_rvalue_reference_v<int>);
    EXPECT_FALSE(estd::is_rvalue_reference_v<int&>);
    EXPECT_TRUE(estd::is_rvalue_reference_v<int&&>);
    EXPECT_FALSE(estd::is_lvalue_reference_v<int>);
    EXPECT_FALSE(estd::is_lvalue_reference_v<int&&>);
    EXPECT_TRUE(estd::is_lvalue_reference_v<int&>);
    // is_same
    EXPECT_FALSE((estd::is_same_v<int, double>));
    EXPECT_FALSE((estd::is_same_v<int, const int>));
    EXPECT_FALSE((estd::is_same_v<int, int*>));
    EXPECT_FALSE((estd::is_same_v<int, int&>));
    EXPECT_TRUE((estd::is_same_v<int, int>));
    // is_function
    auto func = [](int a, int b) { return a + b; };
    EXPECT_FALSE(estd::is_function_v<int*>);
    EXPECT_FALSE(estd::is_function_v<decltype(func)>);
    //TODO: ci compile error
    //EXPECT_TRUE(estd::is_function_v<decltype(printf)>);
    EXPECT_FALSE(estd::is_function_v<std::function<void(int)>>);
    // is_member_pointer
    EXPECT_FALSE(estd::is_member_pointer_v<int*>);
    struct A {
        A(int) {}
        A(int, int) {}
        void func() {}
        A& operator=(int) {
            return *this;
        }
        A& operator=(double) {
            return *this;
        }
    };
    EXPECT_TRUE(estd::is_member_pointer_v<int(A::*)>);
    // is_member_function_pointer
    EXPECT_FALSE(estd::is_member_function_pointer_v<void (*)(int, int)>);
    EXPECT_TRUE(estd::is_member_function_pointer_v<void (A::*)(int, int)>);
    EXPECT_TRUE(estd::is_member_function_pointer_v<decltype(&A::func)>);
    // is_convertible
    EXPECT_TRUE((estd::is_convertible_v<A, int, int>));
    EXPECT_TRUE((estd::is_convertible_v<A, int>));
    EXPECT_FALSE((estd::is_convertible_v<A, int*>));
    EXPECT_FALSE((estd::is_convertible_v<A, int, int, double>));
    EXPECT_TRUE((estd::is_convertible_v<int, double>));
    EXPECT_TRUE((estd::is_convertible_v<int, char>));
    // is_void
    EXPECT_FALSE(estd::is_void_v<int>);
    EXPECT_TRUE(estd::is_void_v<void>);
    EXPECT_TRUE(estd::is_void_v<const void>);
    EXPECT_TRUE(estd::is_void_v<const volatile void>);
    // is_integral
    EXPECT_TRUE(estd::is_integral_v<int>);
    EXPECT_TRUE(estd::is_integral_v<const int>);
    EXPECT_TRUE(estd::is_integral_v<estd::uint64_t>);
    EXPECT_FALSE(estd::is_integral_v<double>);
    EXPECT_FALSE(estd::is_integral_v<A>);
    // is_float
    EXPECT_TRUE(estd::is_float_v<float>);
    EXPECT_TRUE(estd::is_float_v<const double>);
    EXPECT_FALSE(estd::is_float_v<A>);
    // is_union
    union B {};
    EXPECT_FALSE(estd::is_union_v<int>);
    EXPECT_FALSE(estd::is_union_v<double>);
    EXPECT_TRUE(estd::is_union_v<B>);
    // is_class
    EXPECT_FALSE(estd::is_class_v<int>);
    EXPECT_FALSE(estd::is_class_v<B>);
    EXPECT_TRUE(estd::is_class_v<A>);
    // is_enum
    enum C {};
    EXPECT_FALSE(estd::is_enum_v<int>);
    EXPECT_FALSE(estd::is_enum_v<A>);
    EXPECT_FALSE(estd::is_enum_v<B>);
    EXPECT_TRUE(estd::is_enum_v<C>);
    // is_empty
    EXPECT_TRUE(estd::is_empty_v<A>);
    EXPECT_FALSE((estd::is_empty_v<estd::pair<int, int>>));
    // is_final
    class D final {};
    EXPECT_FALSE(estd::is_final_v<A>);
    EXPECT_TRUE(estd::is_final_v<D>);
    // is_abstract
    class E {
        virtual void func() = 0;
    };
    EXPECT_TRUE(estd::is_abstract_v<E>);
    EXPECT_FALSE(estd::is_abstract_v<A>);
    // is_assignable
    EXPECT_FALSE((estd::is_assignable_v<E, D>));
    EXPECT_TRUE((estd::is_assignable_v<A, int>));
    EXPECT_TRUE((estd::is_assignable_v<A, double>));
    //is_base_of
    struct driver : A{};
    EXPECT_TRUE((estd::is_base_of_v<A,driver>));
    EXPECT_FALSE((estd::is_base_of_v<int,A>));
}

TEST(TYPE_TRAITS, ADD_TYPE_TEST) {
    EXPECT_EQ(typeid(const int), typeid(estd::add_const_t<int>));
    EXPECT_EQ(typeid(volatile int), typeid(estd::add_volatile_t<int>));
    EXPECT_EQ(typeid(const volatile int), typeid(estd::add_cv_t<int>));
    EXPECT_EQ(typeid(int&), typeid(estd::add_lvalue_reference_t<int>));
    EXPECT_EQ(typeid(int&&), typeid(estd::add_rvalue_reference_t<int>));
    EXPECT_EQ(typeid(int*), typeid(estd::add_pointer_t<int>));

    EXPECT_EQ(typeid(int), typeid(estd::conditional_t<true, int, double>));
    EXPECT_EQ(typeid(double), typeid(estd::conditional_t<false, int, double>));

    EXPECT_TRUE((estd::is_same_v<int&&, decltype(estd::declval<int>())>));
}
int func(int, int) {
    return 0;
}
TEST(TYPE_TRAITS, REMOVE_TYPE_TEST) {
    EXPECT_EQ(typeid(int), typeid(estd::remove_const_t<int const>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_volatile_t<volatile int>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_cv_t<int const volatile>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_reference_t<int&>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_reference_t<int&&>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_extent_t<int[5]>));
    EXPECT_EQ(typeid(int[5]), typeid(estd::remove_extent_t<int[2][5]>));
    EXPECT_EQ(typeid(int), typeid(estd::remove_all_extent_t<int[2][3][5]>));

    EXPECT_EQ(typeid(int*), typeid(estd::decay_t<int[2]>));
    EXPECT_EQ(typeid(int), typeid(estd::decay_t<int const volatile>));
    EXPECT_EQ(typeid(int (*)(int, int)), typeid(estd::decay_t<decltype(func)>));
}

HasMember(number);
HasType(class_type);

TEST(TYPE_TRAITS,HAS_MEMBER_TEST)
{
    class A
    {
    public:
         using class_type = A;
         
         int number;
    };
    EXPECT_TRUE(has_member_number<A>::value);
    EXPECT_TRUE(has_member_class_type<A>::value);
    
    EXPECT_FALSE((has_member_number<estd::pair<int,int>>::value));
    EXPECT_FALSE((has_member_class_type<estd::pair<int,int>>::value));

}