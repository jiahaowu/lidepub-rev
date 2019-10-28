FIND_PACKAGE(OpenCL QUIET)

if(OpenCL_FOUND)
    add_subdirectory(test-src/test-gems/test-graph/test-basic/test-inner_product)
endif(OpenCL_FOUND)
