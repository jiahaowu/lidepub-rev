/****************************************************************************
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
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


#include "lide_ocl_fifo.h"
#include "lide_ocl_file_source.h"
#include "lide_ocl_file_sink.h"
#include "lide_ocl_inner_product.h"
#include "lide_ocl_util.h"
#include "lide_ocl_gpu.h"


#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_MSOURCE           0
#define ACTOR_XSOURCE           1
#define ACTOR_YSOURCE           2
#define ACTOR_INNER_PRODUCT     3
#define ACTOR_SINK              4

/* The total number of actors in the application. */
#define ACTOR_COUNT     5

/* 
    Usage: lide_ocl_inner_product_driver m_file x_file y_file out_file
*/
int main(int argc, char **argv) {
    char *m_file = NULL; 
    char *x_file = NULL; 
    char *y_file = NULL; 
    char *out_file = NULL; 
    lide_ocl_actor_context_type *actors[ACTOR_COUNT];
    lide_ocl_gpu_pointer gpu;
    bool err;
    
    /* Connectivity: fifo1: (m, product), fifo2: (x, product); 
       fifo3: (y, product); fifo4: (product, out) 
    */
    lide_ocl_fifo_pointer   fifo1 = NULL, fifo2 = NULL, fifo3 = NULL, 
                            fifo4 = NULL;

    int token_size = 0;
    int i = 0;
    int arg_count = 5;
    
    /*GPU initialization*/
    printf("Initialize GPU\n");
    gpu = lide_ocl_gpu_new();
    if (gpu == NULL){
        fprintf(stderr, "fail to create gpu context\n");
        exit(1);
    }
        
    /*Get an OpenCL platform*/
    err = lide_ocl__gpu_get_platformID(gpu, 1, NULL);
    if(err != true){
        fprintf(stderr, "Failed to get GPU platform ID.\n");
        exit (-1);
    }
    /*Get the devices*/
    err = lide_ocl__gpu_get_deviceID(gpu, CL_DEVICE_TYPE_GPU, 1, NULL); 
    if(err != true){
        fprintf(stderr, "Failed to get GPU device ID.\n");
        exit (-1);
    }
    /*Create the context*/
    err = lide_ocl_gpu_context_new(gpu, 0, 1, NULL, NULL);
    if(err != true){
        fprintf(stderr, "Failed to create context.\n");
        exit (-1);
    }    
    /* Create a command-queue*/
    err = lide_ocl_gpu_cmdq_new(gpu, 0);
    if(err != true){
        fprintf(stderr, "Failed to create command queue.\n");
        exit (-1);
    } 
    printf("Initialize GPU finishes\n");
    /* actor descriptors (for diagnostic output) */
    const char *descriptors[ACTOR_COUNT] = {"msource", "xsource", "ysource", 
            "inner_product", "sink"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "driver error: arg count");
        exit(1);
    }   

    /* Open the input and output file(s). */
    i = 1;
    m_file = argv[i++]; 
    x_file = argv[i++]; 
    y_file = argv[i++]; 
    out_file = argv[i++]; 

    /* Create the buffers. */
    token_size = sizeof(int);
    fifo1 = lide_ocl_fifo_new(BUFFER_CAPACITY, token_size);
    fifo2 = lide_ocl_fifo_new(BUFFER_CAPACITY, token_size);
    fifo3 = lide_ocl_fifo_new(BUFFER_CAPACITY, token_size);
    fifo4 = lide_ocl_fifo_new(BUFFER_CAPACITY, token_size);

    /* Create and connect the actors. */
    i = 0;
    actors[ACTOR_MSOURCE] = (lide_ocl_actor_context_type *)
            (lide_ocl_file_source_new(m_file, fifo1));
    actors[ACTOR_XSOURCE] = (lide_ocl_actor_context_type *)
            (lide_ocl_file_source_new(x_file, fifo2));
    actors[ACTOR_YSOURCE] = (lide_ocl_actor_context_type *)
            (lide_ocl_file_source_new(y_file, fifo3));
    actors[ACTOR_INNER_PRODUCT] = (lide_ocl_actor_context_type *)
            (lide_ocl_inner_product_new(fifo1, fifo2, fifo3, fifo4, gpu, 1));
    actors[ACTOR_SINK] = (lide_ocl_actor_context_type *)
            (lide_ocl_file_sink_new(out_file, fifo4));

    /* Execute the graph. */
    lide_ocl_util_simple_scheduler(actors, ACTOR_COUNT, (char **)descriptors);

    /* Normal termination. */
    /* FIFO termination*/
    lide_ocl_fifo_free(fifo1);
    lide_ocl_fifo_free(fifo2);
    lide_ocl_fifo_free(fifo3);
    lide_ocl_fifo_free(fifo4);
    
    /* Actor termination */
    lide_ocl_file_source_terminate(
        (lide_ocl_file_source_context_type *)(actors[ACTOR_MSOURCE]));
    lide_ocl_file_source_terminate(
        (lide_ocl_file_source_context_type *)(actors[ACTOR_XSOURCE]));
    lide_ocl_file_source_terminate(
        (lide_ocl_file_source_context_type *)(actors[ACTOR_YSOURCE]));    
    lide_ocl_inner_product_terminate(
        (lide_ocl_inner_product_context_type *)(actors[ACTOR_INNER_PRODUCT]));         
    lide_ocl_file_sink_terminate(
        (lide_ocl_file_sink_context_type *)(actors[ACTOR_SINK]));
        
    /* GPU termination */
    lide_ocl_gpu_cleanup(gpu);
    return 0;
}
