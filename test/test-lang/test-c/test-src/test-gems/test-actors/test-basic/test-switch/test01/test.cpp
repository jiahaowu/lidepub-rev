#include "../util/lide_c_switch_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_switch_01) {
    char* argv[] = {nullptr, (char*)"source.txt", (char*)"control.txt", (char*)"out0.txt",  (char*)"out1.txt"};
    ASSERT_EQ(0, driver(5, argv));
}