ADD_EXECUTABLE(test_ocl_inner_product_02 test.cpp)

TARGET_LINK_LIBRARIES(test_ocl_inner_product_02  
        gtest 
        gtest_main
        lide_ocl_inner_product_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        test_ocl_inner_product_02
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_ocl_inner_product_02
    )