#include <gtest/gtest.h>
#include <memory.hpp>
#include <thread>
#include <list>

struct A
{
    A() = default;
    A(int n) : m(n){}
    int m{77};
};

struct B : A{};

TEST(UNIQUE_PTR_TEST, CONSTRUTOR_TEST) {
    // construtor
    estd::unique_ptr<int> p(new int(5));
    estd::unique_ptr<int> p1 = estd::make_unique<int>(5);
    EXPECT_FALSE(p == nullptr);
    EXPECT_FALSE(p1 == nullptr);
    EXPECT_EQ(*p, 5);
    EXPECT_EQ(*p1, 5);

    estd::unique_ptr<A> p2(new B());
    
}
TEST(UNIQUE_PTR_TEST, MOVE_CONSTRUTOR_TEST) {
    estd::unique_ptr<int> p = estd::make_unique<int>(5);
    estd::unique_ptr<int> p1(estd::move(p));
    estd::unique_ptr<A> p2 = estd::make_unique<B>();
    EXPECT_TRUE(p == nullptr);
    ASSERT_TRUE(p1 != nullptr);
    EXPECT_EQ(*p1, 5);
}

TEST(UNIQUE_PTR_TEST, MOVE_OPERATOR_TEST) {
    estd::unique_ptr<int> p = estd::make_unique<int>(5);
    estd::unique_ptr<int> p1 = estd::move(p);
    EXPECT_TRUE(p == nullptr);
    ASSERT_TRUE(p1 != nullptr);
    EXPECT_EQ(*p1, 5);
}

TEST(UNIQUE_PTR_TEST, INTERFACE_TEST) {
    estd::unique_ptr<int> p = estd::make_unique<int>(5);

    *p = 10;
    EXPECT_EQ(*p, 10);

    int *p2 = p.release();

    EXPECT_TRUE(p == nullptr);
    ASSERT_TRUE(p2 != nullptr);
    EXPECT_TRUE(*p2 == 10);
    delete p2;

    p.reset(new int(77));
    ASSERT_TRUE(p != nullptr);
    EXPECT_TRUE(*p == 77);

    p.reset();
    EXPECT_TRUE(p == nullptr);

    estd::unique_ptr<int> p3 = estd::make_unique<int>(10);

    EXPECT_FALSE(p3 == p);
    EXPECT_TRUE(p3 > p);

    p.swap(p3);

    ASSERT_TRUE(p3 == nullptr);
    EXPECT_TRUE(p != nullptr);
    EXPECT_TRUE(*p == 10);
}
TEST(UNIQUE_PTR_TEST, ARRAY_TEST) {
    // Array
    estd::unique_ptr<int[]> p = estd::make_unique<int[]>(5);

    for (int i = 0; i < 5; i++) p[i] = i;

    EXPECT_TRUE(p[0] == 0);
    EXPECT_TRUE(p[4] == 4);

    EXPECT_TRUE((estd::is_same_v<estd::default_deleter<int[]> &, decltype(p.get_deleter())>));
}


TEST(SHARED_PTR_TEST, CONSTRUTOR_TEST) {
    estd::shared_ptr<int> p = estd::make_shared<int>(5);
    ASSERT_TRUE(p != nullptr);
    EXPECT_TRUE(*p == 5);

    estd::shared_ptr<int> p1;
    ASSERT_TRUE(p1 == nullptr);

    bool flag = false;
    {
        estd::shared_ptr<int> p2(new int(5), [&flag](int *p) {
            flag = true;
            delete p;
        });

        ASSERT_TRUE(p2 != nullptr);
        EXPECT_TRUE(*p2 == 5);
    }

    EXPECT_TRUE(flag);

    
}

TEST(SHARED_PTR_TEST,COPY_CONSTRUTOR_TEST)
{
    estd::shared_ptr<int> p = estd::make_shared<int>(88);
    ASSERT_TRUE(p != nullptr);
    estd::shared_ptr<int> p1(p);
    ASSERT_TRUE(p1 != nullptr);
    EXPECT_TRUE(*p1 == 88);

    estd::shared_ptr<A> p3 = estd::make_shared<B>();
    ASSERT_TRUE(p3 != nullptr);
    estd::shared_ptr<A> p4 = p3;
    ASSERT_TRUE(p4 != nullptr);
}


TEST(SHARED_PTR_TEST,MOVE_CONSTRUTOR_TEST)
{
    estd::shared_ptr<int> p = estd::make_shared<int>(88);
    ASSERT_TRUE(p != nullptr);
    estd::shared_ptr<int> p1(estd::move(p));
    ASSERT_TRUE(p == nullptr);
    ASSERT_TRUE(p1 != nullptr);
    EXPECT_TRUE(*p1 == 88);
}


TEST(SHARED_PTR_TEST,COPY_OPERATOR_TEST)
{
    estd::shared_ptr<int> p;
    estd::shared_ptr<int> p1;
    p1 = p;
    ASSERT_TRUE(p  == nullptr);
    ASSERT_TRUE(p1 == nullptr);

    p = new int(5);

    ASSERT_TRUE(p != nullptr);
    EXPECT_TRUE(*p == 5);

    p1 = p;
    ASSERT_TRUE(p1 != nullptr);
    EXPECT_TRUE(*p1 == 5);
}

TEST(SHARED_PTR_TEST,COMPARE_OPERATOR_TEST)
{
    estd::shared_ptr<int> one(new int(0));
    estd::shared_ptr<int> two(new int(0));

    EXPECT_TRUE(one < two);
    EXPECT_TRUE(one <= two);
    EXPECT_FALSE(one > two);
    EXPECT_FALSE(one >= two);
}

TEST(SHARED_PTR_TEST,INTERFACE_TEST)
{
    estd::shared_ptr<int> p(new int(5));
    estd::shared_ptr<int> p1(new int(22));

    p1.swap(p);
    EXPECT_TRUE(*p == 22);
    EXPECT_TRUE(*p1 == 5);
    
    EXPECT_TRUE(p.unique());
    EXPECT_TRUE(p1.unique());

    p1 = p;
    EXPECT_FALSE(p.unique());
    EXPECT_EQ(p.use_count(),2);

    int *tmp = p1.get();
    ASSERT_TRUE(tmp != nullptr);
    EXPECT_TRUE(*tmp == 22);

    p1.reset();
    ASSERT_TRUE(p1 == nullptr);
    EXPECT_TRUE(p.unique());

    bool flag = false;
    {
       estd::shared_ptr<int> p2;
       p2.reset(new int(88),[&flag](int *p)
       {
           flag = true;
           delete p;
       });
       p2.reset(new int(22));
       EXPECT_TRUE(*p2 == 22);
    }
    EXPECT_TRUE(flag);


    {
        bool flag = false;

        std::list<std::thread> threads;
        
        estd::shared_ptr<int> p3(new int(22),[&flag](int *p)
        {
            flag = true;
            delete p;
        });

        for(int i = 0;i < 5; i++)
        {
            threads.emplace_back([p3]()
            {

            });
        }
        p3.reset();

        for(auto &thread : threads)
              thread.join();

        EXPECT_TRUE(flag);
    }

    estd::shared_ptr<A> p4; 
    estd::shared_ptr<B> p5 = estd::make_shared<B>();
    p4 = p5;
    ASSERT_TRUE(p4 != nullptr);
    EXPECT_EQ(p4.use_count(),2);
    EXPECT_EQ(p4->m,77);
    EXPECT_EQ((*p4).m,77);
}

TEST(WEAK_PTR_TEST,CONSTRUTOR_TEST)
{
    estd::weak_ptr<int> p;
    EXPECT_TRUE(p.expired());
    EXPECT_EQ(p.use_count(),0);
    
    auto p1 = p.lock();
    EXPECT_TRUE(p1 == nullptr);

    estd::shared_ptr<int> num = estd::make_shared<int>(5);
    p = num;
    EXPECT_FALSE(p.expired());
    EXPECT_EQ(p.use_count(),1);

    auto p2 = p.lock();
    ASSERT_TRUE(p2 != nullptr);
    EXPECT_TRUE(*p2 == 5);
    EXPECT_TRUE(p2.use_count() == 2);
}