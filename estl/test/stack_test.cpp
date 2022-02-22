#include <gtest/gtest.h>
#include <stack.hpp>
#include "class.hpp"
TEST(STACK_TEST,BASE_TEST)
{
    estd::stack<int> s;
    EXPECT_TRUE(s.empty());


    int n = 10000;

    for(int i = 0; i < n; i++){
        s.push(i);
        EXPECT_EQ(s.size(),i + 1);
    }
    EXPECT_FALSE(s.empty());
    
    for(int i = 0;i < n; i++){
        EXPECT_EQ(n - 1 - i,s.top());
        s.pop();
    }

    for(int i = 0;i < n; i++){
        s.push(i);
    }

    estd::stack<int> copy_stack(s);
    EXPECT_EQ(copy_stack.size(),n);
    EXPECT_EQ(s.size(),n);

    for(int i = 0;i < n; i++){
        EXPECT_EQ(n - 1 - i,copy_stack.top());
        copy_stack.pop();
    }


    estd::stack<int> move_stack(std::move(s));
    estd::stack<int> copy_operator_stack;
    copy_operator_stack = move_stack;

    EXPECT_TRUE(s.size() == 0);
    EXPECT_TRUE(s.empty());

    EXPECT_EQ(move_stack.size(),n);

    for(int i = 0;i < n; i++){
        EXPECT_EQ(n - 1 - i,move_stack.top());
        move_stack.pop();
    }
    
    EXPECT_EQ(move_stack.size(),0);
    EXPECT_EQ(copy_operator_stack.size(),n);
    

    for(int i = 0;i < n; i++){
        EXPECT_EQ(n - 1 - i,copy_operator_stack.top());
        copy_operator_stack.pop();
    }

    for(int i = 0;i < n; i++)
      s.push(i);

    EXPECT_EQ(s.size(),n);

    estd::stack<int> move_operator_stack;
    move_operator_stack = estd::move(s);

    EXPECT_TRUE(s.empty());
    EXPECT_EQ(move_operator_stack.size(),n);

    for(int i = 0;i < n; i++){
        EXPECT_EQ(n - 1 - i,move_operator_stack.top());
        move_operator_stack.pop();
    }


  
}

TEST(STACK_TEST,OBJECT_TEST)
{

    estd::stack<estd::test::A> s;

    int n = 1000;
    for(int i = 0;i < n; i++){
        auto&& obj = s.emplace(i);
        EXPECT_EQ(*obj,i);
    }

    for(int i = 0;i < n; i++){
        auto&& obj = s.top();
        EXPECT_EQ(n - 1 - i,*obj);
        s.pop();
    }

    estd::test::A obj(77);

    s.push(obj);

    EXPECT_EQ(*s.top(),77);
    ASSERT_TRUE(obj.value != nullptr);
    s.pop();

    *obj = 88;

    s.push(estd::move(obj));
    EXPECT_EQ(*s.top(),88);
    ASSERT_TRUE(obj.value == nullptr);
    
    
}
