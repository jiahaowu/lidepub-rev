ADD_EXECUTABLE(lide_c_add_driver
    lide_c_add_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_add_driver
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime 
    lide_c_graph_basic
    )


