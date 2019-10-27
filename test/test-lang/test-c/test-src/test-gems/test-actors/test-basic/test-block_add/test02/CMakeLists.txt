ADD_EXECUTABLE(test_block_add_02 test.cpp)

TARGET_LINK_LIBRARIES(test_block_add_02  
        gtest 
        gtest_main
        lide_c_block_add_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        test_block_add_02
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_block_add_02
    )