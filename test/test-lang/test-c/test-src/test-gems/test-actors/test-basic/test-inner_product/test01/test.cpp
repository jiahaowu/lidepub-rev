#include "../util/lide_c_inner_product_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_inner_product_01) {
    char* argv[] = {nullptr, (char*)"m.txt", (char*)"x.txt", (char*)"y.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(5, argv));
}