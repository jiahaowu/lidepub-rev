SET(source_list
    lide_ocl_gpu.cpp
    lide_ocl_kernel.cpp
    lide_ocl_util.cpp
    )

ADD_LIBRARY(lide_ocl_runtime
    ${source_list})


INSTALL(TARGETS lide_ocl_runtime DESTINATION lidegen/ocl)
