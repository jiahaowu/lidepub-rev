SET(source_list
    lide_ocl_inner_product_graph.cpp
    )

ADD_DEFINITIONS(-DINNER_PRODUCT_OCL="${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/actors/basic/")

ADD_LIBRARY(lide_ocl_graph_basic
    ${source_list}
    )
TARGET_LINK_LIBRARIES(lide_ocl_graph_basic
        lide_ocl_actors_basic
        lide_ocl_edges_basic
        lide_ocl_runtime
        )

INSTALL(TARGETS lide_ocl_graph_basic DESTINATION lidegen/ocl)
