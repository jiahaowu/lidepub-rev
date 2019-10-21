ADD_EXECUTABLE(lide_c_communication_channel_estimation_driver.exe
    lide_c_communication_channel_estimation_driver.c
    )

TARGET_LINK_LIBRARIES(lide_c_communication_channel_estimation_driver.exe
    lide_c_actors_basic
    lide_c_edges_basic
    lide_c_runtime
    lide_c_communication_common
    lide_c_communication_channel_estimation
    -lm
    )

