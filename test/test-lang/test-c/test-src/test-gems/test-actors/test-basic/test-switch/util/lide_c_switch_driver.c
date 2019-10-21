/*******************************************************************************
@ddblock_begin copyright

Copyright (c) 1997-2018
Maryland DSPCAD Research Group, The University of Maryland at College Park 

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

@ddblock_end copyright
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "lide_c_fifo.h"
#include "lide_c_file_source.h"
#include "lide_c_file_sink.h"
#include "lide_c_switch.h"
#include "lide_c_util.h"

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_SOURCE            0
#define ACTOR_SOURCE_CONTROL    1 
#define ACTOR_SWITCH            2 
#define ACTOR_SINK0             3 
#define ACTOR_SINK1             4

/* The total number of actors in the application. */
#define ACTOR_COUNT             5

/* The total number of fifos in the application. */
#define FIFO_COUNT              4

/* An enumeration of the edges (fifos) in the application graph. */
#define FIFO_SOURCE_SWITCH      0 
#define FIFO_CONTROL_SWITCH     1 
#define FIFO_SINK0_SWITCH       2 
#define FIFO_SINK1_SWITCH       3

/* 
    Usage: lide_c_switch_driver.exe source_file control_file out0_file
            out1_file
*/

int main(int argc, char **argv) {
    char *source_file = NULL; 
    char *source_control_file = NULL; 
    char *out0_file = NULL; 
    char *out1_file = NULL; 
    lide_c_actor_context_type *actors[ACTOR_COUNT];
    lide_c_fifo_pointer *fifos;

    int token_size = 0;
    int i = 0;
    int arg_count = 5;
    
    /* actor descriptors (for diagnostic output) */
    char *descriptors[ACTOR_COUNT] = {"source", "control", "switch", "sink0",
            "sink1"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "driver.exe error: arg count");
        exit(1);
    }   

    /* Open the input and output file(s). */
    i = 1;
    source_file = argv[i++]; 
    source_control_file = argv[i++]; 
    out0_file = argv[i++]; 
    out1_file = argv[i++]; 

    /* Create the buffers. */
    fifos = lide_c_util_malloc(sizeof(lide_c_fifo_pointer) * FIFO_COUNT);
    token_size = sizeof(int);
    for (i = 0; i < FIFO_COUNT; i++) {
        fifos[i] = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    }

    /* Create and connect the actors. */
    i = 0;
    actors[ACTOR_SOURCE] = (lide_c_actor_context_type *)
            (lide_c_file_source_new(source_file, fifos[FIFO_SOURCE_SWITCH]));

    actors[ACTOR_SOURCE_CONTROL] = (lide_c_actor_context_type *)
            (lide_c_file_source_new(source_control_file, 
            fifos[FIFO_CONTROL_SWITCH]));

    actors[ACTOR_SWITCH] = (lide_c_actor_context_type *)
            (lide_c_switch_new(fifos[FIFO_SOURCE_SWITCH],
            fifos[FIFO_CONTROL_SWITCH], fifos[FIFO_SINK0_SWITCH], 
            fifos[FIFO_SINK1_SWITCH]));

    actors[ACTOR_SINK0] = (lide_c_actor_context_type *)
            (lide_c_file_sink_new(out0_file, fifos[FIFO_SINK0_SWITCH]));

    actors[ACTOR_SINK1] = (lide_c_actor_context_type *)
            (lide_c_file_sink_new(out1_file, fifos[FIFO_SINK1_SWITCH]));


    /* Execute the graph. */
    lide_c_util_simple_scheduler(actors, ACTOR_COUNT, descriptors);

    /* Normal termination. */
    return 0;
}
