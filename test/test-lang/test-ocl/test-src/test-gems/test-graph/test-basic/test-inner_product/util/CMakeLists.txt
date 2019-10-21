FIND_PACKAGE(OpenCL REQUIRED)

ADD_EXECUTABLE(lide_ocl_inner_product_driver.exe
    lide_ocl_inner_product_driver.cpp
    )

TARGET_LINK_LIBRARIES(lide_ocl_inner_product_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime
    lide_ocl_actors_basic
    lide_ocl_graph_basic
    lide_ocl_edges_basic
    lide_ocl_runtime
    OpenCL::OpenCL
    )
