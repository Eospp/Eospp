#include <gtest/gtest.h>
#include <tuple.hpp>

TEST(TUPLE_TEST,BASE_TEST)
{
    estd::tuple<int,int> t = estd::make_tuple(1,2);
    EXPECT_EQ(estd::get<0>(t),1);
    EXPECT_EQ(estd::get<1>(t),2);
    estd::get<0>(t) = 5;
    estd::get<1>(t) = 10;
    EXPECT_EQ(estd::get<0>(t),5);
    EXPECT_EQ(estd::get<1>(t),10);

    estd::tuple<int,int> t1(100,200);
    EXPECT_EQ(estd::get<0>(t1),100);
    EXPECT_EQ(estd::get<1>(t1),200);

    auto t2 = t1;
    EXPECT_EQ(estd::get<0>(t2),100);
    EXPECT_EQ(estd::get<1>(t2),200);
    estd::swap(t1,t);
    EXPECT_EQ (estd::get<0>(t1),5);
    EXPECT_EQ (estd::get<1>(t1),10);
}

class A{
public:
      A() : value(nullptr)
      {

      }
      A(int v) : value(new int(v))
      {

      }
      A(const A& rhs) : value(rhs.value ? new int(*rhs.value) : nullptr)
      {
      }

      A(A &&rhs) : value(rhs.value)
      {
          rhs.value = nullptr;
      }
      ~A()
      {
          if(value)
            delete value;
      }
      
      void swap(A& rhs)
      {
          estd::swap(value,rhs.value);
      }

      A& operator=(A rhs)
      {
         rhs.swap(*this);
         return *this;
      }
public:
      int *value;
};

TEST(TUPLE_TEST,CONSTRUCT_TEST)
{
     estd::tuple<A> t = estd::make_tuple<A>(5);
     ASSERT_TRUE(estd::get<0>(t).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t).value,5);
     //copy constructor
     estd::tuple<A> t1(t);
     ASSERT_TRUE(estd::get<0>(t1).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t1).value,5);
     EXPECT_TRUE(estd::get<0>(t).value != estd::get<0>(t1).value);
     // move constructor
     estd::tuple<A> t2(estd::move(t));
     EXPECT_EQ  (estd::get<0>(t).value,nullptr);
     ASSERT_TRUE(estd::get<0>(t2).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t2).value,5);
     // copy operator=
     estd::tuple<A> t3;
     t3 = t2;
     ASSERT_TRUE(estd::get<0>(t3).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t3).value,5);
     ASSERT_TRUE(estd::get<0>(t2).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t2).value,5);
     //move operator=
     estd::tuple<A> t4;
     t4 = estd::move(t3);
     EXPECT_EQ  (estd::get<0>(t3).value,nullptr);
     ASSERT_TRUE(estd::get<0>(t4).value != nullptr);
     EXPECT_EQ  (*estd::get<0>(t4).value,5);

}