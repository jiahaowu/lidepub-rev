SET(source_list
    lide_c_inner_product_graph.c
    )

ADD_LIBRARY(lide_c_graph_basic ${source_list})
TARGET_LINK_LIBRARIES(lide_c_graph_basic
        lide_c_actors_basic
        lide_c_edges_basic
        lide_c_runtime
        )

INSTALL(TARGETS lide_c_graph_basic DESTINATION lidegen/c)
