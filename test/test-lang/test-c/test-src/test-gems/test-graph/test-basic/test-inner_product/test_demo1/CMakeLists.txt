ADD_EXECUTABLE(test_inner_product_graph_01 test.cpp)

TARGET_LINK_LIBRARIES(test_inner_product_graph_01  
        gtest 
        gtest_main
        lide_c_inner_product_graph_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        test_inner_product_graph_01
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_inner_product_graph_01
    )