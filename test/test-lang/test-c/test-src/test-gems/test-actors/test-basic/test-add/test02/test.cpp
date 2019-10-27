#include "../util/lide_c_add_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_add_02) {
    char* argv[] = {nullptr, (char*)"x.txt", (char*)"y.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(4, argv));
}