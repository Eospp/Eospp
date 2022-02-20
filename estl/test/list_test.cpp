#include <gtest/gtest.h>
#include <list.hpp>
#include "class.hpp"

TEST(LIST_TEST,CONSTRUCTOR_TEST)
{
    //empty constructor
    estd::list<int> empty_list;


    //initializer_list constructor
    estd::list<int> L{1,2,3,4,5};
    EXPECT_FALSE(L.empty());
    EXPECT_EQ(L.size(),5);

    auto it = L.begin();
    for(int i = 1;i <= 5;i++,it++)
      EXPECT_EQ(*it,i);


    //copy constructor
    estd::list<int> copy_list(L);
    EXPECT_FALSE(copy_list.empty());
    EXPECT_EQ(copy_list.size(),5);
    
    it = copy_list.begin();
    for(int i = 1;i <= 5;i++,it++)
      EXPECT_EQ(*it,i);


    //move constructor
    estd::list<int> move_list(estd::move(L));

    EXPECT_FALSE(move_list.empty());
    EXPECT_EQ(move_list.size(),5);
    
    it = move_list.begin();
    for(int i = 1;i <= 5;i++,it++)
      EXPECT_EQ(*it,i);

    EXPECT_TRUE(L.empty());
    EXPECT_EQ(L.size(),0);


    //copy operator=
    estd::list<int> copy_operator_list;
    copy_operator_list = move_list;

    it = copy_operator_list.begin();
    for(int i = 1;i <= 5;i++,it++)
      EXPECT_EQ(*it,i);

    EXPECT_FALSE(copy_operator_list.empty());
    EXPECT_EQ(copy_operator_list.size(),5);
    EXPECT_FALSE(move_list.empty());
    EXPECT_EQ(move_list.size(),5);


    //move operator=
    estd::list<int> move_operator_list;
    move_operator_list = estd::move(move_list);

    it = move_operator_list.begin();
    for(int i = 1;i <= 5;i++,it++)
      EXPECT_EQ(*it,i);

    EXPECT_TRUE(move_list.empty());
    EXPECT_EQ(move_list.size(),0);
    EXPECT_FALSE(move_operator_list.empty());
    EXPECT_EQ(move_operator_list.size(),5);
}

TEST(LIST_TEST,OBJECT_TEST)
{
    estd::list<estd::test::A> L;

    L.emplace_back(5);

    EXPECT_EQ(L.size(),1);
    EXPECT_FALSE(L.empty());
    EXPECT_EQ(*L.front().value,5);


    estd::test::A obj(77);

    L.push_front(obj);
    EXPECT_EQ(*L.front().value,77);
    EXPECT_FALSE(obj.value == nullptr);

    *obj.value = 99;
    L.push_front(estd::move(obj));
    EXPECT_EQ(*L.front().value,99);
    EXPECT_EQ(obj.value,nullptr);

    obj = estd::test::A(88);
    L.push_back(estd::move(obj));
    EXPECT_EQ(*L.back().value,88);
    EXPECT_EQ(obj.value,nullptr);

    obj = estd::test::A(111);
    L.push_back(obj);
    EXPECT_EQ(*L.back().value,111);
    EXPECT_FALSE(obj.value == nullptr);
}

TEST(LIST_TEST,INTERFACE_TEST)
{
    //push_front
    estd::list<int> L;
    for(int i = 0;i < 100000;i++)
    {
        L.push_front(i);
        EXPECT_EQ(L.front(),i);
    }
    //clear
    L.clear();
    EXPECT_TRUE(L.empty());
    EXPECT_TRUE(L.begin() == L.end());
    //push_back    
    for(int i = 0;i < 100000; i++)
    {
        L.push_back(i);
        EXPECT_EQ(L.back(),i);
    }
    L.clear();
    //emplace_front
    for(int i = 0;i < 100000;i++)
    {
        L.emplace_front(i);
        EXPECT_EQ(L.front(),i);
    }
    L.clear();
    //emplace_back
    for(int i = 0;i < 100000;i++)
    {
        L.emplace_back(i);
        EXPECT_EQ(L.back(),i);
    }
    //pop_bcak
    for(int i = 99999;i >= 1;i--)
    {
        L.pop_back();
        EXPECT_EQ(L.back(),i - 1);
    }
    L.pop_back();

    for(int i = 0;i < 100000;i++)
    {
        L.push_back(i);
    }
    //pop_front
    for(int i = 1;i < 100000;i++)
    {
        L.pop_front();
        EXPECT_EQ(L.front(),i);
    }
    L.clear();
    //insert
    L.push_back(1);
    auto it = L.insert(L.begin(),2);
    EXPECT_EQ(*it,2);
    EXPECT_EQ(L.front(),2);

    it = L.insert(L.end(),3);
    EXPECT_EQ(*it,3);
    EXPECT_EQ(L.back(),3);
    
    //erase
    it = L.erase(it);
    EXPECT_EQ(L.back(),1);
    EXPECT_TRUE(it == L.end());

    it = L.erase(L.begin());
    EXPECT_EQ(*it,1);


    L.clear();

    for(int i = 0;i < 9;i++)
       L.push_back(i);

    auto first = ++L.begin();
    auto end   = --L.end();

    L.erase(first,end);
    EXPECT_EQ(L.size(),2);
    EXPECT_EQ(L.front(),0);
    EXPECT_EQ(L.back(),8);

    L.erase(L.begin(),L.end());
    EXPECT_TRUE(L.empty());
}