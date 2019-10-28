#include "../util/lide_c_src_snk_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_src_snk_01) {
    char* argv[] = {nullptr, (char*)"input.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(3, argv));
}