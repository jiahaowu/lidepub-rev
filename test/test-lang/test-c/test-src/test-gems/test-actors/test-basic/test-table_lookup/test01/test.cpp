#include "../util/lide_c_table_lookup_driver.h"
#include <gtest/gtest.h>

TEST(lide_c, test_table_lookup_01) {
    char* argv[] = {nullptr, (char*)"m.txt", (char*)"table.txt", (char*)"x.txt", (char*)"out.txt"};
    ASSERT_EQ(0, driver(5, argv));
}