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

#include "lide_ocl_inner_product_graph.h"
#include "lide_ocl_gpu.h"
#include "lide_ocl_util.h"


/* 
    Usage: lide_ocl_inner_product_driver m_file x_file y_file out_file
*/
int driver(int argc, char **argv) {
    char *m_file = NULL; 
    char *x_file = NULL; 
    char *y_file = NULL; 
    char *out_file = NULL; 
    
    int i = 0;
    int arg_count = 5;
    lide_ocl_graph_context_type *graph_context;
    lide_ocl_gpu_pointer gpu;
    bool err;
    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "lide_ocl_inner_product_driver error: arg count");
        exit(1);
    }   
    
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
    
    
    /* Open the input and output file(s). */
    i = 1;
    m_file = argv[i++]; 
    x_file = argv[i++]; 
    y_file = argv[i++]; 
    out_file = argv[i++]; 

    graph_context = (lide_ocl_graph_context_type *)
                    lide_ocl_inner_product_graph_new(m_file, x_file, y_file, 
                    out_file, gpu);

    /* Execute the graph. */
    lide_ocl_util_simple_scheduler(graph_context->actors, 
                    graph_context->actor_count, graph_context->descriptors);

    /* Normal termination. */
    lide_ocl_inner_product_graph_terminate(
            (lide_ocl_inner_product_graph_context_type *)graph_context);
    return 0;
}
