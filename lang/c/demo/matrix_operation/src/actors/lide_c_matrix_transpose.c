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
#include <stdio.h>
#include <stdlib.h>

#include "lide_c_matrix_transpose.h"
#include "lide_c_util.h"

struct _lide_c_matrix_transpose_context_struct {
#include "lide_c_actor_context_type_common.h"

    /* Actor interface ports. */
    lide_c_fifo_pointer in1;
    lide_c_fifo_pointer out;
    
    /* Matrix data pointer*/
    int *m_in1;
    int *m_out;
    
    /* Size of matrix*/
    int m, n;
};

lide_c_matrix_transpose_context_type *lide_c_matrix_transpose_new(
        lide_c_fifo_pointer in1, lide_c_fifo_pointer out, int m, int n) {

    lide_c_matrix_transpose_context_type *context = NULL;

    context = lide_c_util_malloc(sizeof(lide_c_matrix_transpose_context_type));
    context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS;
    context->enable = (lide_c_actor_enable_function_type)
                        lide_c_matrix_transpose_enable;
    context->invoke = (lide_c_actor_invoke_function_type)
                        lide_c_matrix_transpose_invoke;
    /* FIFO info*/
    context->in1 = in1;
    context->out = out;
    /*Parameter*/

    context->m = m;
    context->n = n;
    
    context->m_in1 = NULL;
    context->m_out = NULL;
    
    if (context->m <= 0 || context->n <= 0){
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_ERROR;
    }
    

    return context;
}

boolean lide_c_matrix_transpose_enable(lide_c_matrix_transpose_context_type *context) {
    boolean result = FALSE;

    switch (context->mode) {
    case LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS:
        result = (lide_c_fifo_population(context->in1) >= 
                context->m * context->n) &&
                ((lide_c_fifo_population(context->out)  + 
                context->m * context->n )<  
                lide_c_fifo_capacity(context->out));
        break;
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*******************************************************************************
The invoke method assumes that a corresponding call to enable has
returned TRUE so that the required data is available.

*******************************************************************************/
void lide_c_matrix_transpose_invoke(lide_c_matrix_transpose_context_type *context) {
    int size_in1;
    int i, j;
    int idx1, idx2;
    switch (context->mode) {
    case LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS:
        size_in1 = context->m * context->n;
        /*Memory allocation*/
        context->m_in1 = (int *)lide_c_util_malloc(sizeof(int) * size_in1);
        context->m_out = (int *)lide_c_util_malloc(sizeof(int) * size_in1);
        /*Read input data*/
        lide_c_fifo_read_block(context->in1, context->m_in1, size_in1);
        /*Computation*/
        for (i = 0; i < context->m; i++) {
            for (j = 0; j < context->n; j++) {
                idx1 = i*context->n + j;
                idx2 = j*context->m + i;                
                context->m_out[idx2] = context->m_in1[idx1];
            }
        }
        /*Send output*/
        lide_c_fifo_write_block(context->out, context->m_out, size_in1);
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS;
        free(context->m_in1);
        free(context->m_out);
        break;
    default:
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_ERROR;
        break;
    }
}

void lide_c_matrix_transpose_terminate(lide_c_matrix_transpose_context_type *context) {
    
    free(context);
}

/*parameter set and get functions*/
int lide_c_matrix_transpose_get_m(
        lide_c_matrix_transpose_context_type *context){
    return context->m;
}
        
void lide_c_matrix_transpose_set_m(
        lide_c_matrix_transpose_context_type *context, int m){
    context->m = m;
    if(context->m > 0 && context->n > 0){
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS;
    }else{
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_ERROR;
    }
          
}
       
int lide_c_matrix_transpose_get_n(
        lide_c_matrix_transpose_context_type *context){
    return context->n;
}

void lide_c_matrix_transpose_set_n(
        lide_c_matrix_transpose_context_type *context, int n){
    context->n = n;
    if(context->m > 0 && context->n > 0){
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS;
    }else{
        context->mode = LIDE_C_MATRIX_TRANSPOSE_MODE_ERROR;
    }
          
}
 