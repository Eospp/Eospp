#include <gtest/gtest.h>
#include <functional.hpp>

int func(int a,int b)
{
    return a + b;
}

class A
{
public:
      int sub(int a,int b)
      {
          return a - b;
      }
      static int add(int a,int b)
      {
          return a + b;
      }

      int operator()(int a,int b)
      {
          return a * b;
      }
};

static int g_value = 1;

int& get_value()
{
    return g_value;
}

TEST(FUNCTIONAL_TEST,INVOKE_TEST)
{
    EXPECT_EQ(estd::invoke(&func,1,2),3);
    A a;
    EXPECT_EQ(estd::invoke(&A::sub,a,10,3),7);
    EXPECT_EQ(estd::invoke(&A::sub,&a,10,3),7);
    EXPECT_EQ(estd::invoke(&A::add,2,3),5);
    EXPECT_EQ(estd::invoke(&a,1,2),2);
    int &v = estd::invoke(&get_value);
    v = 10;
    EXPECT_EQ(g_value,10);

    auto callable = [](int a,int b){ return a + b;};
    
    EXPECT_EQ(estd::invoke(callable,1,2),3);
}

TEST(FUNCTIONAL_TEST,FUNCTION_TEST)
{
    estd::function<int(int,int)> f(&func);
    EXPECT_EQ(f(1,2),3);
    //copy construtor
    estd::function<int(int,int)> f1(f);
    EXPECT_EQ(f1(10,5),15);
    //move construtor
    estd::function<int(int,int)> f2(estd::move(f));
    EXPECT_FALSE(f);
    EXPECT_EQ(f2(1,2),3);
    //operator=
    A obj;
    f = [&obj](int a,int b)
    {
        return obj.sub(a,b);
    };
    EXPECT_EQ(f(5,2),3);
    //copy operator=
    f2 = f;
    ASSERT_TRUE(f  != false);
    ASSERT_TRUE(f2 != false);
    EXPECT_EQ(f(5,2),3);
    EXPECT_EQ(f2(5,2),3);
    //move operator=
    f2 = estd::move(f);
    ASSERT_TRUE(f  == false);
    ASSERT_TRUE(f2 != false);
    EXPECT_EQ(f2(5,2),3);
    
}