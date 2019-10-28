ADD_EXECUTABLE(test_switch_02 test.cpp)

TARGET_LINK_LIBRARIES(test_switch_02  
        gtest 
        gtest_main
        lide_c_switch_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        test_switch_02
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_switch_02
    )