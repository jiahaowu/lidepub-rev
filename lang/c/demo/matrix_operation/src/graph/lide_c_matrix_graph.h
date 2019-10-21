#ifndef _lide_c_multiply_graph_h
#define _lide_c_multiply_graph_h

/*******************************************************************************
@ddblock_begin copyright

Copyright (c) 1997-2016
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
#include "lide_c_basic.h"
#include "lide_c_actor.h"
#include "lide_c_file_source.h"
#include "lide_c_file_sink.h"
#include "lide_c_fifo.h"
#include "lide_c_graph.h"
#include "lide_c_matrix_multiplication.h"
#include "lide_c_matrix_transpose.h"
#include "lide_c_util.h"


#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */

#define ACTOR_XSOURCE           0
#define ACTOR_YSOURCE           1
#define ACTOR_MATRIX_MULTIPLY   2
#define ACTOR_MATRIX_TRANSPOSE  3
#define ACTOR_SINK              4

#define FIFO_XSRC_MULTI      0
#define FIFO_YSRC_MULTI      1
#define FIFO_MULTI_TRANS     2
#define FIFO_TRANS_SNK       3
/* The total number of actors in the application. */
#define ACTOR_COUNT     5
#define FIFO_COUNT      4

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/
/* Structure and pointer types associated with add objects. */
struct _lide_c_matrix_graph_context_struct;
typedef struct _lide_c_matrix_graph_context_struct 
        lide_c_matrix_graph_context_type;
        
/* Structure and pointer types associated with add objects. */
/*
struct _lide_c_matrix_graph_context_struct {
 
    lide_c_actor_context_type **actors;
    lide_c_fifo_pointer *fifos;
    int actor_count;
    int fifo_count;
    char **descriptors;
    char *x_file; 
    char *y_file; 
    char *out_file;

};
typedef struct _lide_c_matrix_graph_context_struct 
        lide_c_matrix_graph_context_type;
*/
/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

lide_c_matrix_graph_context_type *lide_c_matrix_graph_new(
        char *x_file, char *y_file, char *out_file, int m, int n, int p);

void lide_c_matrix_graph_terminate(
        lide_c_matrix_graph_context_type *context);

#endif
