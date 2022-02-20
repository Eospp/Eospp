#include <gtest/gtest.h>
#include <atomic.hpp>
#include <list>
#include <thread>

TEST(ATMOIC_TEST, BASE_TEST) {
    estd::atomic<int> v(1);
    EXPECT_EQ(v.load(), 1);
    //++
    EXPECT_EQ(v++, 1);
    EXPECT_EQ(++v, 3);
    v.store(5);
    EXPECT_EQ(v.load(), 5);
    //--
    EXPECT_EQ(v--, 5);
    EXPECT_EQ(--v, 3);
    //+=
    EXPECT_EQ(v += 5, 8);
    //-=
    EXPECT_EQ(v -= 6, 2);
    //|=
    EXPECT_EQ(v |= 1, 3);
    //&=
    EXPECT_EQ(v &= 4, 0);
    //^=
    v.store(1);
    EXPECT_EQ(v ^= 12, 13);
    // fetch_add
    v.store((5));
    EXPECT_EQ(v.fetch_add(5), 5);
    EXPECT_EQ(v.load(), 10);
    // fetch_sub
    EXPECT_EQ(v.fetch_sub(5), 10);
    EXPECT_EQ(v.fetch_sub(5), 5);
    EXPECT_EQ(v.load(), 0);
    // fetch_and
    v.store(2);
    EXPECT_EQ(v.fetch_and(1), 2);
    EXPECT_EQ(v.load(), 0);
    // fetch_or
    v.store(1);
    EXPECT_EQ(v.fetch_or(2), 1);
    EXPECT_EQ(v.load(), 3);
    // fetch_xor
    v.store(1);
    EXPECT_EQ(v.fetch_xor(1), 1);
    EXPECT_EQ(v.load(), 0);
    // exchange
    v.store(10);
    EXPECT_EQ(v.exchange(20), 10);
    EXPECT_EQ(v.load(), 20);
    // compare_exchange_weak
    int value;

    do {
        if ((value = v.load()) == 0) break;
    } while (
        !v.compare_exchange_weak(value, 0, estd::memory_order_acq_rel, estd::memory_order_relaxed));

    EXPECT_EQ(v.load(), 0);

    // compare_exchange_strong
    value = v.load();
    v.compare_exchange_strong(value, 20, estd::memory_order_acq_rel, estd::memory_order_relaxed);
    EXPECT_EQ(v.load(), 20);
}
TEST(ATOMIC_TEST,MULTI_THREAD_TEST)
{

    estd::atomic<int> v(0);
    std::list<std::thread> threads;
    int times = 1000000;
    int thread_count = 4;
    for(int i = 0;i < thread_count;i++)
    {
        threads.emplace_back([&v,times]()
        {
            for(int i = 0;i < times;i++)
               v++;
        });
    }

    for(auto& thread : threads)
        thread.join();
    
    EXPECT_EQ(v.load(),thread_count * times);

}