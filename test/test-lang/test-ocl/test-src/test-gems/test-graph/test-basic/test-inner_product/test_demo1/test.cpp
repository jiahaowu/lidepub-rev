#include "../util/lide_ocl_inner_product_driver.h"
#include <gtest/gtest.h>

TEST(lide_ocl, test_inner_product_01) {
    char* argv[] = {nullptr, (char*)"m.txt", (char*)"x.txt", (char*)"y.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(5, argv));
}