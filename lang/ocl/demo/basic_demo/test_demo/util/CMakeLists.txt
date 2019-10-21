CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB CXX)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)
FIND_PACKAGE(OpenCL REQUIRED)

INCLUDE_DIRECTORIES(
    $ENV{UXLIDEC}/src/gems/actors/basic
    $ENV{UXLIDEC}/src/gems/edges/basic
    $ENV{UXLIDEC}/src/gems/actors/common
    $ENV{UXLIDEC}/src/tools/runtime
    $ENV{UXLIDEOCL}/src/gems/actors/basic
    $ENV{UXLIDEOCL}/src/gems/edges/basic
    $ENV{UXLIDEOCL}/src/gems/actors/common
    $ENV{UXLIDEOCL}/src/tools/runtime
    )

LINK_DIRECTORIES(
    $ENV{LIDEGEN}/c
    $ENV{LIDEGEN}/ocl
    )

ADD_EXECUTABLE(lide_ocl_inner_product_driver.exe
    lide_ocl_inner_product_driver.cpp
    )

TARGET_LINK_LIBRARIES(lide_ocl_inner_product_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime
    lide_ocl_actors_basic
    lide_ocl_edges_basic
    lide_ocl_runtime
    OpenCL::OpenCL
    )

INSTALL(TARGETS lide_ocl_inner_product_driver.exe DESTINATION .)

