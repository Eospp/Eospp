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
}