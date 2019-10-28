ADD_LIBRARY(lide_c_table_lookup_driver
    lide_c_table_lookup_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_table_lookup_driver
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime 
    lide_c_graph_basic
    )
