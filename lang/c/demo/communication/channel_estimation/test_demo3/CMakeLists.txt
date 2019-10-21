CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)

INCLUDE_DIRECTORIES(
    $ENV{UXLIDEC}/src/gems/actors/basic
    $ENV{UXLIDEC}/src/gems/edges/basic
    $ENV{UXLIDEC}/src/gems/actors/common
    $ENV{UXLIDEC}/src/tools/runtime
    $ENV{UXLIDEC}/src/gems/actors/communication/common
    $ENV{UXLIDEC}/src/gems/actors/communication/channel_estimation
    )

LINK_DIRECTORIES(
    $ENV{LIDEGEN}/c
    )

ADD_EXECUTABLE(lide_c_channel_estimation_driver.exe
              lide_c_channel_estimation_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_channel_estimation_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime
    lide_c_communication_common
    lide_c_communication_channel_estimation
    -lm
    )

INSTALL(TARGETS lide_c_channel_estimation_driver.exe DESTINATION .)

