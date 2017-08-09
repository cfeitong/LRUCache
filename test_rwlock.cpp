#include "rwlock.hpp"
#include "gtest/gtest.h"

#include <thread>
#include <vector>
#include <chrono>
using namespace std::chrono_literals;

TEST(RWLockTest, test_read) {
    lru::RWLock rwlock;
    int num = 0;
    std::vector<std::thread> ths;
    int id = 0;
    // 5 writers
    for (int i = 0; i < 5; i++) {
        ths.emplace_back([&]() mutable {
            std::this_thread::sleep_for(200ms * i);
            rwlock.writer_lock();
            printf("(writer)id = %d, num = %d\n", ++id, ++num);
            rwlock.writer_unlock();
        });
    }
    // 10 readers
    for (int i = 0; i < 10; i++) {
        ths.emplace_back([&]() mutable {
            std::this_thread::sleep_for(100ms * i);
            rwlock.reader_lock();
            printf("(reader)id = %d, num = %d\n", ++id, num);
            rwlock.reader_unlock();
        });
    }
    std::for_each(ths.begin(), ths.end(), [](std::thread& th) {
        th.join();
    });
}

TEST(RWLockTest, basic) {
    lru::RWLock rwlock;
    rwlock.reader_lock();
    rwlock.reader_unlock();
    rwlock.writer_lock();
    rwlock.writer_unlock();
    rwlock.reader_lock();
    rwlock.reader_unlock();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}