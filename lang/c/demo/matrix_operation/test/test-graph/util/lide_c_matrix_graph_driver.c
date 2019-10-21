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

#include "lide_c_matrix_graph.h"


/* 
    Usage: lide_c_matrix_graph_driver x_file y_file para_file out_file
*/
int main(int argc, char **argv) {
    char *x_file = NULL; 
    char *y_file = NULL; 
    char *para_file = NULL; 
    char *out_file = NULL; 
    lide_c_graph_context_type *graph;

    int i = 0;
    int arg_count = 5;
    
    int m, n, p;
    FILE *p_file;
    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "lide_c_matrix_graph_driver error: arg count");
        exit(1);
    }   

    /* Open the input and output file(s). */
    i = 1;
    x_file = argv[i++]; 
    y_file = argv[i++]; 
    para_file = argv[i++]; 
    out_file = argv[i++]; 
    
    /* Get parameter*/
    p_file = lide_c_util_fopen(para_file, "r"); 
    fscanf(p_file, "%d", &m);
    fscanf(p_file, "%d", &n);
    fscanf(p_file, "%d", &p);
    
    graph = (lide_c_graph_context_type *)lide_c_matrix_graph_new( 
            x_file, y_file, out_file, m, n, p);
    if(graph == NULL)
        printf("graph is NULL");
    /* Execute the graph. */
    lide_c_util_simple_scheduler((lide_c_actor_context_type **)(graph->actors), 
                                ACTOR_COUNT, graph->descriptors);

    /* Normal termination. */
    lide_c_matrix_graph_terminate(graph);
    return 0;
}
