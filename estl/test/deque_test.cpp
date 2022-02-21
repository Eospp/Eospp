#include <gtest/gtest.h>
#include <deque.hpp>
#include "class.hpp"

TEST(DEQUE_TEST,CONSTRUCTOR_TEST)
{
    //constructor
    estd::deque<int> deque;
    int n = 1000;
    for(int i = 0;i < n;i++)
        deque.push_back(i);

    EXPECT_EQ(deque.size(),n);
    
    int i = 0;
    for(auto it : deque)
      EXPECT_EQ(it,i++);
    
    //copy constructor
    estd::deque<int> copy_deque(deque);
    EXPECT_EQ(copy_deque.size(),n);
    
    i = 0;
    for(auto it : copy_deque)
       EXPECT_EQ(it,i++);

    //move constructor
    estd::deque<int> move_deque(estd::move(copy_deque));
    i = 0;
    
    for(auto it : move_deque)
       EXPECT_EQ(it,i++);

    EXPECT_EQ(move_deque.size(),n);
    EXPECT_TRUE(copy_deque.empty());

    //copy operator=
    estd::deque<int> copy_operator_deque;
    copy_operator_deque = move_deque;

    i = 0;
    for(auto it : move_deque)
       EXPECT_EQ(it,i++);

    EXPECT_EQ(copy_operator_deque.size(),n);
    EXPECT_FALSE(move_deque.empty());

    //move operator=
    estd::deque<int> move_operator_deque;
    move_operator_deque = estd::move(copy_operator_deque);

    i = 0;
    for(auto it : move_deque)
       EXPECT_EQ(it,i++);
    
    EXPECT_EQ(move_operator_deque.size(),n);
    EXPECT_TRUE(copy_operator_deque.empty());

}

TEST(DEQUE_TEST,OBJECT_TEST)
{
    estd::deque<estd::test::A> deque;

    estd::test::A obj(77);

    //emplace_back
    deque.emplace_back(10);
    EXPECT_EQ(*deque.back(),10);
    EXPECT_EQ(deque.size(),1);
    EXPECT_FALSE(deque.empty());

    deque.emplace_back(obj);
    EXPECT_EQ(*deque.back(),77);
    EXPECT_EQ(deque.size(),2);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(obj.value == nullptr);
    
    *obj = 88;

    deque.emplace_back(estd::move(obj));
    EXPECT_EQ(*deque.back(),88);
    EXPECT_EQ(deque.size(),3);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(obj.value == nullptr);

    //swap
    estd::deque<estd::test::A>().swap(deque);
    
    ASSERT_TRUE(deque.empty());

    obj = estd::test::A(77);
    //emplace_front
    deque.emplace_front(10);
    EXPECT_EQ(*deque.front(),10);
    EXPECT_EQ(deque.size(),1);
    EXPECT_FALSE(deque.empty());

    deque.emplace_front(obj);
    EXPECT_EQ(*deque.front(),77);
    EXPECT_EQ(deque.size(),2);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(obj.value == nullptr);
    
    *obj = 88;

    deque.emplace_front(estd::move(obj));
    EXPECT_EQ(*deque.front(),88);
    EXPECT_EQ(deque.size(),3);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(obj.value == nullptr);

    estd::deque<estd::test::A>().swap(deque);

    obj = estd::test::A(77);
    //push_front
    deque.push_front(10);
    EXPECT_EQ(*deque.front(),10);
    EXPECT_EQ(deque.size(),1);
    EXPECT_FALSE(deque.empty());

    deque.push_front(obj);
    EXPECT_EQ(*deque.front(),77);
    EXPECT_EQ(deque.size(),2);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(obj.value == nullptr);
    
    *obj = 88;

    deque.push_front(estd::move(obj));
    EXPECT_EQ(*deque.front(),88);
    EXPECT_EQ(deque.size(),3);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(obj.value == nullptr);

    estd::deque<estd::test::A>().swap(deque);

    obj = estd::test::A(77);
    //push_bcak
    deque.push_back(10);
    EXPECT_EQ(*deque.back(),10);
    EXPECT_EQ(deque.size(),1);
    EXPECT_FALSE(deque.empty());

    deque.push_back(obj);
    EXPECT_EQ(*deque.back(),77);
    EXPECT_EQ(deque.size(),2);
    EXPECT_FALSE(deque.empty());
    EXPECT_FALSE(obj.value == nullptr);
    
    *obj = 88;

    deque.push_back(estd::move(obj));
    EXPECT_EQ(*deque.back(),88);
    EXPECT_EQ(deque.size(),3);
    EXPECT_FALSE(deque.empty());
    EXPECT_TRUE(obj.value == nullptr);
}

TEST(DEQUE_TEST,INTERFACE_TEST)
{
    estd::deque<int> deque;
    
    for(int i = 0;i < 10;i++)
      deque.push_back(i);

    //operator[]
    for(int i = 0;i < 10;i++)
      EXPECT_EQ(deque[i],i);

    for(int i = 0;i < 10;i++){
       deque[i] = 10 - i;
       EXPECT_EQ(deque[i],10 - i);
    }
    
    deque.clear();
    EXPECT_TRUE(deque.empty());
    
    //iterator
    for(int i = 0; i < 10;i++)
        deque.push_back(i);

    auto it = deque.begin();

    EXPECT_EQ(*it,0);
    *it = 5;
    EXPECT_EQ(*it,5);
    EXPECT_EQ(deque.front(),5);

    *it = 0;

    int i = 0;

    for(it = deque.begin(); it != deque.end(); it++)
    {
        EXPECT_EQ(*it,i++);
    }
    it = deque.begin();
    //operator+
    EXPECT_EQ(*(it + 1),1);
    
    //operator+=
    it += 2;
    EXPECT_EQ(*(it),2);

    //operator-
    EXPECT_EQ(*(it - 1),1);

    //operator-=
    it -= 2;
    EXPECT_EQ(*(it),0);

    //operator-(rhs)
    EXPECT_EQ(deque.end() - deque.begin(),deque.size());

    //swap

    estd::deque<int> rhs{4,5,6};
    estd::deque<int> lhs{1,2,3};

    for(int i = 0;i < 3; i++)
    {
        EXPECT_EQ(rhs[i],4 + i);
        EXPECT_EQ(lhs[i],1 + i);
    }

    EXPECT_EQ(rhs.size(),3);
    EXPECT_EQ(lhs.size(),3);

    rhs.swap(lhs);


    for(int i = 0;i < 3; i++)
    {
        EXPECT_EQ(rhs[i],1 + i);
        EXPECT_EQ(lhs[i],4 + i);
    }
}