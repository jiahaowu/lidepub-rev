#include "../util/lide_c_block_add_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_block_add_03) {
    char* argv[] = {nullptr, (char*)"param.txt", (char*)"x.txt", (char*)"y.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(5, argv));
}