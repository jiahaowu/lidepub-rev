SET(source_list
    lide_c_add.c
    lide_c_block_add.c
    lide_c_file_sink.c
    lide_c_file_source.c
    lide_c_inner_product.c
    lide_c_switch.c
    lide_c_table_lookup.c
    )

ADD_LIBRARY(lide_c_actors_basic ${source_list})

INSTALL(TARGETS lide_c_actors_basic DESTINATION lidegen/c)
