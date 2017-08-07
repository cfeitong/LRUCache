#include "lrucache.hpp"
#include "gtest/gtest.h"

int resource[200];
int access_seq[] = {4, 2, 5, 2, 3, 6, 8, 1, 1, 2, 2, 1};

TEST(LRUCacheTest, test_put) {
    for (int i = 0; i < 200; i++) {
        resource[i] = i;
    }
    lru::LRUCache cache(5);

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}