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
#include "lide_c_table_lookup.h"
#include "lide_c_util.h"

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_XSOURCE               0
#define ACTOR_TABLE_LOOKUP          1
#define ACTOR_SINK                  2

/* The total number of actors in the application. */
#define ACTOR_COUNT 3

/* 
    Usage: lide_c_table_lookup_driver m_file table_file 
            x_file out_file
*/
int main(int argc, char **argv) {
    char *x_file = NULL; 
    char *m_file = NULL;
    FILE *table_file = NULL;
    char *out_file = NULL; 
    lide_c_actor_context_type *actors[ACTOR_COUNT];
    
    /* Connectivity: fifo1: (x, table_lookup); fifo2: (table_lookup, out) */

    lide_c_fifo_pointer fifo1 = NULL, fifo2 = NULL;

    int token_size = 0;
    int table_size = 0;
    int arg_count = 5;
    int i = 0;
    
    /* actor descriptors (for diagnostic output) */
    char *descriptors[ACTOR_COUNT] = {"xsource", "table_lookup", "sink"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "driver error: arg count");
        exit(1);
    }   

    /* Open the input and output file(s). */
    i = 1;
    m_file = lide_c_util_fopen(argv[i++], "r"); 
    table_file = lide_c_util_fopen(argv[i++], "r"); 
    x_file = argv[i++]; 
    out_file = argv[i++]; 

    /* Read table size from the m file. */
    if (fscanf(m_file, "%d", &table_size) != 1) {
        fprintf(stderr, "lide_c_table_lookup_new error: integer expected");
        exit(1);
    }   

    /* Create the buffers. */
    token_size = sizeof(int);
    fifo1 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo2 = lide_c_fifo_new(BUFFER_CAPACITY, token_size);

    /* Create and connect the actors. */
    actors[ACTOR_XSOURCE] = (lide_c_actor_context_type *)
            (lide_c_file_source_new(x_file, fifo1));

    actors[ACTOR_TABLE_LOOKUP] = (lide_c_actor_context_type *)
            (lide_c_table_lookup_new(table_file, table_size, fifo1, fifo2));

    actors[ACTOR_SINK] = (lide_c_actor_context_type *)
            (lide_c_file_sink_new(out_file, fifo2));

    /* Execute the graph. */
    lide_c_util_simple_scheduler(actors, ACTOR_COUNT, descriptors);

    /* Normal termination. */
    return 0;
}
