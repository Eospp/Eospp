#include <gtest/gtest.h>
#include <queue.hpp>
#include "class.hpp"
TEST(QUEUE_TEST,BASE_TEST)
{

    estd::queue<int> q;
    int n = 100000;
    for(int i = 0;i < n; i++){
        q.push(i);
        EXPECT_EQ(q.size(),i + 1);
    }
    
    for(int i = 0;i < n; i++){
        EXPECT_EQ(q.front(),i);
        q.pop();
    }
    
    EXPECT_TRUE(q.empty());

    for(int i = 0;i < n; i++){
        q.push(i);
    }

    //copy constructor

    estd::queue<int> copy_queue(q);
    EXPECT_EQ(copy_queue.size(),n);
    EXPECT_FALSE(copy_queue.empty());

    EXPECT_EQ(q.size(),n);
    EXPECT_FALSE(q.empty());

    for(int i = 0;i < n; i++){
        EXPECT_EQ(copy_queue.front(),i);
        copy_queue.pop();
    }

    
    //move constructor
    estd::queue<int> move_queue(estd::move(q));

    EXPECT_EQ(move_queue.size(),n);
    EXPECT_FALSE(move_queue.empty());

    EXPECT_EQ(q.size(),0);
    EXPECT_TRUE(q.empty());

    for(int i = 0;i < n; i++){
        q.push(i);
        EXPECT_EQ(move_queue.front(),i);
        move_queue.pop();
    }

    
    //copy operator=
    estd::queue<int> copy_operator_queue;
    copy_operator_queue = q;

    EXPECT_EQ(copy_operator_queue.size(),n);
    EXPECT_FALSE(copy_operator_queue.empty());

    EXPECT_EQ(q.size(),n);
    EXPECT_FALSE(q.empty());

    for(int i = 0;i < n; i++){
        EXPECT_EQ(copy_operator_queue.front(),i);
        copy_operator_queue.pop();
    }

    //move operator=
    estd::queue<int>  move_operator_queue;
    move_operator_queue = estd::move(q);

    EXPECT_EQ(move_operator_queue.size(),n);
    EXPECT_FALSE(move_operator_queue.empty());

    EXPECT_EQ(q.size(),0);
    EXPECT_TRUE(q.empty());


    //swap

    estd::queue<int> swap_queue;
    swap_queue.swap(move_operator_queue);

    EXPECT_TRUE(move_operator_queue.empty());
    EXPECT_EQ(move_operator_queue.size(),0);

    EXPECT_FALSE(swap_queue.empty());
    EXPECT_EQ(swap_queue.size(),n);

}