ADD_EXECUTABLE(test_src_snk_01 test.cpp)

TARGET_LINK_LIBRARIES(test_src_snk_01  
        gtest 
        gtest_main
        lide_c_src_snk_driver
        Threads::Threads
    )

ADD_TEST(
    NAME 
        test_src_snk_01
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND 
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_src_snk_01
    )