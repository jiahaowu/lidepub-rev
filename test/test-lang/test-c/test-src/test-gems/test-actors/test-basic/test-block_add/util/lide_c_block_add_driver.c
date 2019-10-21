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
#include "lide_c_block_add.h"
#include "lide_c_util.h"

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_XSOURCE 0
#define ACTOR_YSOURCE 1
#define ACTOR_ADDER 2
#define ACTOR_SINK 3

/* The total number of actors in the application. */
#define ACTOR_COUNT 4


/* 
    Usage: lide_c_block_add_driver x_file y_file out_file
*/
int main(int argc, char **argv) {
    char *param_file = NULL;
    char *x_file = NULL; 
    char *y_file = NULL; 
    char *out_file = NULL; 
    lide_c_actor_context_type *actors[ACTOR_COUNT];
    
    /* Connectivity: fifo1: (x, adder); fifo2: (y, adder); 
       fifo3: (adder, out) 
    */
    lide_c_fifo_pointer fifo1 = NULL, fifo2 = NULL, fifo3 = NULL;

    int token_size = 0;
    int i = 0;
    int arg_count = 5;
    int block_length = 0;
    
    /* actor descriptors (for diagnostic output) */
    char *descriptors[ACTOR_COUNT] = {"xsource", "ysource", "block_add",
            "sink"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "driver error: arg count");
        exit(1);
    }   

    /* Open the parameters file(s) */
    i = 1;
    param_file = lide_c_util_fopen(argv[i++], "r"); 

    /* Open the input and output file(s). */
    x_file = argv[i++]; 
    y_file = argv[i++]; 
    out_file = argv[i++]; 

    /* Read the parameter(s) */
    if (fscanf(param_file, "%d", &block_length) != 1) {
        fprintf(stderr, "error: missing block length parameter");
        exit(1);
    }
    fclose(param_file);

    /* Create the buffers. */
    token_size = sizeof(int);
    fifo1 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo2 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo3 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);

    /* Create and connect the actors. */
    i = 0;
    actors[ACTOR_XSOURCE] = (lide_c_actor_context_type *)
            (lide_c_file_source_new(x_file, fifo1));
    actors[ACTOR_YSOURCE] = (lide_c_actor_context_type *)
            (lide_c_file_source_new(y_file, fifo2));
    actors[ACTOR_ADDER] = (lide_c_actor_context_type *)
            (lide_c_block_add_new(fifo1, fifo2, fifo3, block_length));
    actors[ACTOR_SINK] = (lide_c_actor_context_type *)
            (lide_c_file_sink_new(out_file, fifo3));

    /* Execute the graph. */
    lide_c_util_simple_scheduler(actors, ACTOR_COUNT, descriptors);
    
    /* Normal termination. */
    return 0;
}
