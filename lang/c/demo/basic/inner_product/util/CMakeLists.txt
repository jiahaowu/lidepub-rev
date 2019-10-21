CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)

INCLUDE_DIRECTORIES(
    $ENV{UXLIDEC}/src/gems/actors/basic
    $ENV{UXLIDEC}/src/gems/edges/basic
    $ENV{UXLIDEC}/src/gems/actors/common
    $ENV{UXLIDEC}/src/tools/runtime
    )

LINK_DIRECTORIES(
    $ENV{LIDEGEN}/c
    )

ADD_EXECUTABLE(lide_c_inner_product_driver.exe
    lide_c_inner_product_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_inner_product_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime )

INSTALL(TARGETS lide_c_inner_product_driver.exe DESTINATION .)

