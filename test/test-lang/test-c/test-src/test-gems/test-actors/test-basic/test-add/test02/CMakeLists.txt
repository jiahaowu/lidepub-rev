ADD_EXECUTABLE(test_add_02 test.cpp)

TARGET_LINK_LIBRARIES(test_add_02  
        gtest 
        gtest_main
        test_add_driver
    )

ADD_TEST(
    NAME 
        test_add_02
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_add_02
    )