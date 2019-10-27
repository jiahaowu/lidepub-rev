ADD_EXECUTABLE(test_add_01 test.cpp)

TARGET_LINK_LIBRARIES(test_add_01  
        gtest 
        gtest_main
        test_add_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        add_test_01
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_add_01
    )