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

ADD_EXECUTABLE(lide_c_matrix_multiplication_actor_driver
    test/test-actors/test-matrix-multiplication/util/lide_c_matrix_multiplication_actor_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_multiplication_actor_driver
    ${matrix_libs}
    ${common_libs}
    )

ADD_EXECUTABLE(lide_c_matrix_transpose_actor_driver
    test/test-actors/test-matrix-transpose/util/lide_c_matrix_transpose_actor_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_transpose_actor_driver
    ${matrix_libs}
    ${common_libs}
    )

ADD_EXECUTABLE(lide_c_matrix_graph_driver
    test/test-graph/util/lide_c_matrix_graph_driver.c
    )
TARGET_LINK_LIBRARIES(lide_c_matrix_graph_driver
    ${matrix_libs}
    ${common_libs}
    )

