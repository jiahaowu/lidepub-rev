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
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <math.h>
#include "lide_ocl_fifo.h"
#include "lide_ocl_inner_product.h"
#include "lide_ocl_util.h"
#include "lide_ocl_gpu.h"
#include "lide_ocl_kernel.h"

/*******************************************************************************
INNER PRODUCT STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_ocl_inner_product_context_struct { 
#include "lide_ocl_actor_context_type_common.h"

    /* Vector length variable. */
    int length;  

    /* Repeat length parameter. */
    int repeat_length;

    /* Input ports. */
    lide_ocl_fifo_pointer m;
    lide_ocl_fifo_pointer x;
    lide_ocl_fifo_pointer y;

    /* Output port. */
    lide_ocl_fifo_pointer out;
    /* GPU setting*/
    lide_ocl_gpu_pointer gpu;
    lide_ocl_kernel_pointer kernel_sum;
    lide_ocl_kernel_pointer kernel_multi;
    /* Kernel Settings*/
    /* Host Buffer*/
    void *srcX, *srcY;
    /* GPU buffer*/
    cl_mem cmDevSrcX, cmDevSrcY, cmDeVDst;
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

lide_ocl_inner_product_context_type *lide_ocl_inner_product_new(
        lide_ocl_fifo_pointer m, lide_ocl_fifo_pointer x, lide_ocl_fifo_pointer y,
        lide_ocl_fifo_pointer out, lide_ocl_gpu_pointer gpu, int repeat_length) { 

    lide_ocl_inner_product_context_type *context = NULL;

    context = (lide_ocl_inner_product_context_type *)lide_ocl_util_malloc(
                sizeof(lide_ocl_inner_product_context_type));
    context->mode = LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH;
    context->enable =
            (lide_ocl_actor_enable_function_type)lide_ocl_inner_product_enable;
    context->invoke = 
            (lide_ocl_actor_invoke_function_type)lide_ocl_inner_product_invoke;
    context->length = 0;
    context->repeat_length = repeat_length;
    context->m = m;
    context->x = x;
    context->y = y;
    context->out = out;
    /* GPU construction*/
    context->kernel_multi = (lide_ocl_kernel_pointer) malloc (
                            sizeof (lide_ocl_kernel_type));
    context->kernel_sum = (lide_ocl_kernel_pointer) malloc (
                            sizeof (lide_ocl_kernel_type));
    lide_ocl_kernel_set_gpu_setting(context->kernel_multi, gpu);
    lide_ocl_kernel_set_gpu_setting(context->kernel_sum, gpu);

    lide_ocl_kernel_set_cSourceFile(context->kernel_multi, 
                                    "VECMULTI.cl");
    
    lide_ocl_kernel_set_cPathAndName(context->kernel_multi, INNER_PRODUCT_OCL, 
                                    "VECMULTI.cl");	

    context->gpu = gpu;
    
    /* GPU memory allocation*/
    context->cmDeVDst = clCreateBuffer(context->gpu->cxGPUContext, 
                        CL_MEM_WRITE_ONLY, sizeof(cl_int) * 1, NULL, 
                        &context->gpu->ciErr2);
    context->gpu->ciErr1 |= context->gpu->ciErr2;

    if (context->gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateBuffer, Line %u in file %s !!!\n\n", 
                __LINE__, __FILE__);
        lide_ocl_gpu_cleanup(context->gpu);
    }
    /* First kernel*/		
    lide_ocl_kernel_load_source (context->kernel_multi, "");	
    
    /* Create the program */
    lide_ocl_kernel_create_program(context->kernel_multi, 1);
    
    /* Build the program with 'mad' Optimization option	*/
    lide_ocl_kernel_build_program (context->kernel_multi, 0, NULL, NULL, NULL, 
                                    NULL);	
    
    /* Create the kernel */
    lide_ocl_kernel_create_kernel (context->kernel_multi, "vecmulti");
    lide_ocl_kernel_set_cSourceFile(context->kernel_sum, 
                                    "SUM_INT.cl");
    lide_ocl_kernel_set_cPathAndName(context->kernel_sum, INNER_PRODUCT_OCL, 
                                    "SUM_INT.cl");
    /* Second kernel*/	
    /* Read the OpenCL kernel in from source file */
    lide_ocl_kernel_load_source (context->kernel_sum, "");

    /* Create the program */
    lide_ocl_kernel_create_program(context->kernel_sum, 1);
    
    /* Build the program with 'mad' Optimization option */
    lide_ocl_kernel_build_program (context->kernel_sum, 0, NULL, NULL, NULL, 
                                    NULL);
    
    /* Create the kernel */
    lide_ocl_kernel_create_kernel (context->kernel_sum, "SUM_INT");	
    return context;
}

boolean lide_ocl_inner_product_enable(
        lide_ocl_inner_product_context_type *context) {
    boolean result = FALSE;

    switch (context->mode) {
    case LIDE_OCL_INNER_PRODUCT_MODE_ERROR:
        result = TRUE;
        break;
    case LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH:
        result = lide_ocl_fifo_population(context->m) >= 1;
        break;
    case LIDE_OCL_INNER_PRODUCT_MODE_PROCESS:
        result = (lide_ocl_fifo_population(context->x) >= context->length) &&
                (lide_ocl_fifo_population(context->y) >= context->length) &&
                ((lide_ocl_fifo_population(context->out) <
                lide_ocl_fifo_capacity(context->out)));
        break;
    default:
            result = FALSE;
            break;
    }
    return result;
}

void lide_ocl_inner_product_invoke(
                            lide_ocl_inner_product_context_type *context) {
    int i = 0;
    int x_value = 0;
    int y_value = 0;
    static int repeat_length_count = 1;
    int *x_array,*y_array;
    double exp_size, vec_size;
    int result;
    result = 0;
    switch (context->mode) {
    case LIDE_OCL_INNER_PRODUCT_MODE_ERROR:
        fprintf(stderr, "lide_ocl_inner_product: error invalid length.\n");
        context->mode = LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH;
        break;

    case LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH:
        lide_ocl_fifo_read(context->m, &(context->length));
        if (context->length <= 0) {
            context->mode = LIDE_OCL_INNER_PRODUCT_MODE_ERROR;
            return;
        }
        /* G_size and L_size set */
        exp_size = ceil(log2(double(context->length)));
        vec_size = exp2(exp_size);
        context->gpu->szGlobalWorkSize = (size_t)vec_size;
        context->gpu->szLocalWorkSize = (size_t)vec_size;  

        context->cmDevSrcX = clCreateBuffer(context->gpu->cxGPUContext, 
                        CL_MEM_READ_WRITE, 
                        sizeof(cl_int) * context->gpu->szGlobalWorkSize, 
                        NULL, &context->gpu->ciErr1);
        context->cmDevSrcY = clCreateBuffer(context->gpu->cxGPUContext, 
                        CL_MEM_READ_WRITE, 
                        sizeof(cl_int) * context->gpu->szGlobalWorkSize, 
                        NULL, &context->gpu->ciErr2);
                        
        context->gpu->ciErr1 |= context->gpu->ciErr2;
        
        context->mode = LIDE_OCL_INNER_PRODUCT_MODE_PROCESS;
        break;

    case LIDE_OCL_INNER_PRODUCT_MODE_PROCESS:
        /*Host buffer*/
        x_array = (int *)malloc(sizeof(int) * context->length);
        y_array = (int *)malloc(sizeof(int) * context->length);
        for (i = 0; i < context->length; i++) {
            lide_ocl_fifo_read(context->x, &(x_value));
            lide_ocl_fifo_read(context->y, &(y_value));
            x_array[i] = x_value;
            y_array[i] = y_value;
        }

        /*GPU part*/
        /* Set the Argument values */
        context->gpu->ciErr1 = clSetKernelArg(context->kernel_multi->ckKernel, 
                                0, sizeof(cl_mem), (void*)&context->cmDevSrcX);	
        
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_multi->ckKernel, 
                                1, sizeof(cl_mem), (void*)&context->cmDevSrcY);
        
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_multi->ckKernel, 
                                2, sizeof(cl_mem), (void*)&context->cmDevSrcX);
        
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_multi->ckKernel, 
                                3, sizeof(cl_int), (void*)&context->length);

        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            fprintf(stderr, "Error in clSetKernelArg, Line %u in file %s !\n\n", 
                    __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        /*Synchronous write data*/
        context->gpu->ciErr1 = clEnqueueWriteBuffer(
                                            context->gpu->cqCommandQueue, 
                                            context->cmDevSrcX, CL_TRUE, 0, 
                                            sizeof(cl_int) * context->length, 
                                            x_array, 0, NULL, NULL);
        context->gpu->ciErr1 |= clEnqueueWriteBuffer(
                                            context->gpu->cqCommandQueue, 
                                            context->cmDevSrcY, CL_TRUE, 0, 
                                            sizeof(cl_int) * context->length, 
                                            y_array, 0, NULL, NULL);
        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            printf("Error %d in clEnqueueWriteBuffer, Line %u in file %s !\n\n", 
                    context->gpu->ciErr1, __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        
        /*launch kernel*/
        context->gpu->ciErr1 = clEnqueueNDRangeKernel(
                                context->gpu->cqCommandQueue, 
                                context->kernel_multi->ckKernel, 1, NULL, 
                                &context->gpu->szGlobalWorkSize, 
                                &context->gpu->szLocalWorkSize, 0, NULL, NULL);
   
        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            printf("Error in clEnqueueNDRangeKernel, Line %u in file %s !\n\n",
                     __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        
        /* Set the Argument values*/
        context->gpu->ciErr1 = clSetKernelArg(context->kernel_sum->ckKernel, 
                                0, sizeof(cl_mem), (void*)&context->cmDevSrcX);
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_sum->ckKernel, 
                                1, sizeof(cl_mem), (void*)&context->cmDevSrcY);
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_sum->ckKernel, 
                                2, sizeof(cl_mem), (void*)&context->cmDeVDst);
        context->gpu->ciErr1 |= clSetKernelArg(context->kernel_sum->ckKernel, 
                                3, sizeof(cl_int), (void*)&context->length);

        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            fprintf(stderr, "Error in clSetKernelArg, Line %u in file %s!\n\n",
                     __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        /* Launch kernel*/
        context->gpu->ciErr1 = clEnqueueNDRangeKernel(
                                context->gpu->cqCommandQueue, 
                                context->kernel_sum->ckKernel, 1, NULL, 
                                &context->gpu->szGlobalWorkSize, 
                                &context->gpu->szLocalWorkSize, 0, NULL, NULL);
   
        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            printf("Error in clEnqueueNDRangeKernel, Line %u in file %s !\n\n",
                     __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        
        /* 
            Synchronous/blocking read of results, and check accumulated errors 
        */
        context->gpu->ciErr1 = clEnqueueReadBuffer(
                                                context->gpu->cqCommandQueue, 
                                                context->cmDeVDst, CL_TRUE, 0, 
                                                sizeof(cl_int) * 1, &result, 
                                                0, NULL, NULL);
        if (context->gpu->ciErr1 != CL_SUCCESS)
        {
            fprintf(stderr, "Error in clEnqueueReadBuffer, Line %u in file %s!\
                    \n\n", __LINE__, __FILE__);
            lide_ocl_gpu_cleanup(context->gpu);
        }
        
        lide_ocl_fifo_write(context->out, &result);
        if (context->repeat_length < 0) {
            context->mode = LIDE_OCL_INNER_PRODUCT_MODE_ERROR;
            return;
        }

        if (context->repeat_length == 0) {
            context->mode = LIDE_OCL_INNER_PRODUCT_MODE_PROCESS;
        } else {
            if (context->repeat_length == repeat_length_count) {
                context->mode = LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH;
                repeat_length_count = 1;
            } else {
                context->mode = LIDE_OCL_INNER_PRODUCT_MODE_PROCESS;
                repeat_length_count++;
            }
        }
        break; 

    default:
        context->mode = LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH;
        break;
    } 
    return;
}

void lide_ocl_inner_product_terminate(
        lide_ocl_inner_product_context_type *context) {
    clReleaseMemObject(context->cmDevSrcX);
    clReleaseMemObject(context->cmDevSrcY);
    clReleaseMemObject(context->cmDeVDst);
 
    clReleaseKernel(context->kernel_sum->ckKernel);
    clReleaseProgram(context->kernel_sum->cpProgram);
    free(context->kernel_sum);
    
    clReleaseKernel(context->kernel_multi->ckKernel);
    clReleaseProgram(context->kernel_multi->cpProgram);
    free(context->kernel_multi);
    
    free(context);
}

void lide_ocl_inner_product_set_repeat_length(
        lide_ocl_inner_product_context_type *context, int repeat_length) {
    context->repeat_length = repeat_length;
}

int lide_ocl_inner_product_get_repeat_length(
        lide_ocl_inner_product_context_type *context) {
    return context->repeat_length;
}

