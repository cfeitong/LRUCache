#include "lrucache.hpp"
#include "gtest/gtest.h"
#include <vector>
#include <random>
#include <chrono>

TEST(LRUCacheTest, test_normal) {
    int resource[20];
    int access_seq[] = {4, 2, 5, 2, 3, 6, 8, 1, 1, 2, 2, 1};
    for (int i = 0; i < sizeof(resource) / sizeof(resource[0]); i++) {
        resource[i] = i;
    }
    lru::LRUCache<int, int*> cache(5);
    for (size_t i = 0; i < sizeof(access_seq) / sizeof(access_seq[0]); i++) {
        int* result;
        if (cache.has(access_seq[i])) {
            result = cache.get(access_seq[i]);
        } else {
            cache.put(access_seq[i], result = &resource[access_seq[i]]);
        }
        EXPECT_EQ(*result, access_seq[i]);
    }
}

TEST(LRUCacheTest, test_performace) {
    std::vector<int> resource, access_seq;
    std::random_device rd;
    std::uniform_int_distribution<int> dis(0, 100000-1);
    for (int i = 0; i < 100000; i++) {
        resource.push_back(i);
        access_seq.push_back(dis(rd));
    }

    auto start = std::chrono::system_clock::now();
    lru::LRUCache<int, std::vector<int>::const_iterator> cache(500);
    for (auto iter = access_seq.cbegin(); iter != access_seq.cend(); iter++) {
        std::vector<int>::const_iterator result;
        if (cache.has(*iter)) {
            result = cache.get(*iter);
        } else {
            cache.put(*iter, result = resource.cbegin() + *iter);
        }
        EXPECT_EQ(*result, *iter);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    // execution time should be less than 2s
    EXPECT_LT(diff.count(), 2.);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}