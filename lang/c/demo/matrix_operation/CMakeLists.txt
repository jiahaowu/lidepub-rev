CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)

INCLUDE_DIRECTORIES(
    $ENV{UXLIDEC}/src/gems/actors/basic
    $ENV{UXLIDEC}/src/gems/edges/basic
    $ENV{UXLIDEC}/src/gems/actors/common
    $ENV{UXLIDEC}/src/gems/graph/common
    $ENV{UXLIDEC}/src/gems/graph/basic
    $ENV{UXLIDEC}/src/tools/runtime
    src/graph
    src/actors
    )

LINK_DIRECTORIES(
    $ENV{LIDEGEN}/c
    )

ADD_LIBRARY(lide_c_matrix_actor
    src/actors/lide_c_matrix_multiplication.c
    src/actors/lide_c_matrix_transpose.c
    )

ADD_LIBRARY(lide_c_matrix_graph
    src/graph/lide_c_matrix_graph.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_graph
    lide_c_matrix_actor
    )

SET(matrix_libs
    lide_c_matrix_actor
    lide_c_matrix_graph)

SET(common_libs
    lide_c_graph_basic
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime
    )

ADD_EXECUTABLE(lide_c_matrix_multiplication_actor_driver.exe
    test/test-actors/test-matrix-multiplication/util/lide_c_matrix_multiplication_actor_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_multiplication_actor_driver.exe
    ${matrix_libs}
    ${common_libs}
    )
INSTALL(TARGETS lide_c_matrix_multiplication_actor_driver.exe DESTINATION test/test-actors/test-matrix-multiplication/util)

ADD_EXECUTABLE(lide_c_matrix_transpose_actor_driver.exe
    test/test-actors/test-matrix-transpose/util/lide_c_matrix_transpose_actor_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_transpose_actor_driver.exe
    ${matrix_libs}
    ${common_libs}
    )
INSTALL(TARGETS lide_c_matrix_transpose_actor_driver.exe DESTINATION test/test-actors/test-matrix-transpose/util)

ADD_EXECUTABLE(lide_c_matrix_graph_driver.exe
    test/test-graph/util/lide_c_matrix_graph_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_graph_driver.exe
    ${matrix_libs}
    ${common_libs}
    )
INSTALL(TARGETS lide_c_matrix_graph_driver.exe DESTINATION test/test-graph/util)

