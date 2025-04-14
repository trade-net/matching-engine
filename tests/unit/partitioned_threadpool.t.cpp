#include <gtest/gtest.h>
#include <partitioned_threadpool.h>
#include <atomic>
#include <chrono>
#include <thread>

TEST(PartitionedThreadPoolTest, TaskExecutionTest) {
    PartitionedThreadPool pool(4);
    std::atomic<int> counter = 0;

    for (int i = 0; i < 10; ++i) {
        pool.enqueue(i % 4, [&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter++;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(counter.load(), 10);
}

TEST(PartitionedThreadPoolTest, ThreadAssignmentTest) {
    PartitionedThreadPool pool(3);
    std::atomic<int> threadHits[3] = {0, 0, 0};

    for (int i = 0; i < 9; ++i) {
        size_t threadIndex = i % 3;
        pool.enqueue(threadIndex, [&threadHits, threadIndex]() {
            threadHits[threadIndex]++;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(threadHits[0].load(), 3);
    EXPECT_EQ(threadHits[1].load(), 3);
    EXPECT_EQ(threadHits[2].load(), 3);
}

TEST(PartitionedThreadPoolTest, ShutdownTest) {
    PartitionedThreadPool* pool = new PartitionedThreadPool(2);
    std::atomic<int> counter = 0;

    for (int i = 0; i < 5; ++i) {
        pool->enqueue(i % 2, [&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter++;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(counter.load(), 5);

    delete pool; // Ensure proper shutdown
}

TEST(PartitionedThreadPoolTest, StressTest) {
    PartitionedThreadPool pool(8);
    std::atomic<int> counter = 0;

    for (int i = 0; i < 1000; ++i) {
        pool.enqueue(i % 8, [&counter]() {
            counter++;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ(counter.load(), 1000);
}
