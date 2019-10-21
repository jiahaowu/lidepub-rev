SET(source_list
    lide_ocl_fifo.cpp
    )

ADD_LIBRARY(lide_ocl_edges_basic
    ${source_list})

INSTALL(TARGETS lide_ocl_edges_basic DESTINATION lidegen/ocl)
