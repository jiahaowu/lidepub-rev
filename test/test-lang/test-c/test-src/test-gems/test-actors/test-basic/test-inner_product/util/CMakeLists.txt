ADD_EXECUTABLE(lide_c_inner_product_driver.exe
    lide_c_inner_product_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_inner_product_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime 
    lide_c_graph_basic
    )
