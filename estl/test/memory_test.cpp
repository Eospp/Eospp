#include <gtest/gtest.h>
#include <memory.hpp>
#include <memory>

TEST(UNIQUE_PTR_TEST,CONSTRUTOR_TEST)
{
    //construtor
    estd::unique_ptr<int> p(new int(5));
    estd::unique_ptr<int> p1 = estd::make_unique<int>(5);
    EXPECT_FALSE(p  == nullptr);
    EXPECT_FALSE(p1 == nullptr);
    EXPECT_EQ(*p,5);
    EXPECT_EQ(*p1,5);
}
TEST(UNIQUE_PTR_TEST,MOVE_CONSTUTOR_TEST)
{
    estd::unique_ptr<int> p  = estd::make_unique<int>(5);
    estd::unique_ptr<int> p1(estd::move(p));
    
}
TEST(MEMORY_TEST,UNIQUE_PTR_TEST)
{
    
    //move construtor
    {
        estd::unique_ptr<int> p  = estd::make_unique<int>(5);
        estd::unique_ptr<int> p1(estd::move(p));

        EXPECT_TRUE(p  == nullptr);
        ASSERT_TRUE(p1 != nullptr);

        EXPECT_EQ(*p1,5);
    }
    //move operator=
    {
        estd::unique_ptr<int> p  = estd::make_unique<int>(5);
        estd::unique_ptr<int> p1 = estd::move(p);

        EXPECT_TRUE(p  == nullptr);
        ASSERT_TRUE(p1 != nullptr);

        EXPECT_EQ(*p1,5);
    }
    //interface
    {
        estd::unique_ptr<int> p = estd::make_unique<int>(5);
        
        *p = 10;
        EXPECT_EQ(*p,10);

        int *p2 = p.release();
    
        EXPECT_TRUE(p   == nullptr);
        ASSERT_TRUE(p2  != nullptr);
        EXPECT_TRUE(*p2 == 10);

        p.reset(new int(77));
        ASSERT_TRUE(p   != nullptr);
        EXPECT_TRUE(*p  == 77);
        
        p.reset();
        EXPECT_TRUE(p == nullptr);

        estd::unique_ptr<int> p3 = estd::make_unique<int>(10);

        EXPECT_FALSE(p3 == p); 
        EXPECT_TRUE(p3 > p);

        p.swap(p3);

        ASSERT_TRUE(p3 == nullptr);
        EXPECT_TRUE(p  != nullptr);
        EXPECT_TRUE(*p == 10);
    }

    //Array
    {
        estd::unique_ptr<int[]> p = estd::make_unique<int[]>(5);
        
        for(int i = 0;i < 5;i++)
           p[i] = i;

        EXPECT_TRUE(p[0] == 0);
        EXPECT_TRUE(p[4] == 4);

        EXPECT_TRUE((estd::is_same_v<estd::default_deleter<int[]>&,decltype(p.get_deleter())>));
    }
    
}