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
#include "lide_ocl_gpu.h"

lide_ocl_gpu_pointer lide_ocl_gpu_new(void){
    lide_ocl_gpu_pointer gpu;
    gpu = (lide_ocl_gpu_pointer) malloc (sizeof(lide_ocl_gpu_type));
    if (gpu == NULL)
        return NULL;
    gpu->szGlobalWorkSize = 1;
    gpu->szLocalWorkSize = 1;
    gpu->ciErr1 = 0;
    gpu->ciErr2 = 0;
    return gpu;
}


/* Get platform ID*/
bool lide_ocl__gpu_get_platformID(lide_ocl_gpu_pointer gpu, cl_uint num_entries, 
                                    cl_uint *num_platforms)
{
    gpu->ciErr1 = clGetPlatformIDs(num_entries, &gpu->cpPlatform, num_platforms);
    if (gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clGetPlatformID\n");
        if (gpu->ciErr1 == CL_INVALID_VALUE)
            fprintf (stderr, "CL_INVALID_VALUE error\n");
        else
            fprintf (stderr, "Unknown error\n");
        lide_ocl_gpu_cleanup(gpu);
        return false;
    }
    return true;
                                    
}

/* Get device*/
bool lide_ocl__gpu_get_deviceID(lide_ocl_gpu_pointer gpu, 
                                cl_device_type device_type, cl_uint num_entries, 
                                cl_uint *num_devices)
{
    gpu->ciErr1 = clGetDeviceIDs(gpu->cpPlatform, device_type, num_entries, 
                                &gpu->cdDevice, num_devices);
    if (gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clGetDeviceIDs\n");
        if (gpu->ciErr1 == CL_INVALID_PLATFORM)
            fprintf(stderr, " platform is not valid\n");
        if (gpu->ciErr1 == CL_INVALID_DEVICE_TYPE)
            fprintf(stderr, " device_type is not valid\n");
        if (gpu->ciErr1 == CL_INVALID_VALUE)
            fprintf(stderr, " num_entries and device_type are not matched\n");
        if (gpu->ciErr1 == CL_DEVICE_NOT_FOUND)
            fprintf(stderr, " device_type is not matched with OpenCL device\n");		
        lide_ocl_gpu_cleanup(gpu);
        return false;
    }
    return true;			
}

/* Create Context of GPU*/
bool lide_ocl_gpu_context_new(lide_ocl_gpu_pointer gpu, 
                                cl_context_properties *properties, 
                                cl_uint num_devices, 
                                void *pfn_notify(const char *errinfo, 
                                                const void *private_info, 
                                                size_t cb, void *user_data), 
                                void *user_data)
{
    gpu->cxGPUContext = clCreateContext(properties, num_devices, 
                                        &gpu->cdDevice, NULL, user_data, 
                                        &gpu->ciErr1);										
                                        
    if (gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateContext\n");
        if (gpu->ciErr1 == CL_INVALID_PLATFORM)
            fprintf(stderr, "Proerty and platform are not matched\n");
        if (gpu->ciErr1 == CL_INVALID_VALUE)
            fprintf(stderr, "Something not proper in the arguments\n");
        if (gpu->ciErr1 == CL_INVALID_DEVICE)
            fprintf(stderr, "Invalid device\n");
        if (gpu->ciErr1 == CL_DEVICE_NOT_AVAILABLE)
            fprintf(stderr, "Device not available\n");
        if (gpu->ciErr1 == CL_OUT_OF_HOST_MEMORY)
            fprintf(stderr, "Allocating resources failed\n");
        lide_ocl_gpu_cleanup(gpu);
        return false;
    }
    return true;													
}

/* Create a command-queue of GPU */
bool lide_ocl_gpu_cmdq_new(lide_ocl_gpu_pointer gpu, 
                            cl_command_queue_properties properties)
{
    gpu->cqCommandQueue = clCreateCommandQueue(gpu->cxGPUContext, 
                                                gpu->cdDevice, properties, 
                                                &gpu->ciErr1);
    if (gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateContext\n");
        if (gpu->ciErr1 == CL_INVALID_CONTEXT)
            fprintf(stderr, "Invalid context\n");
        if (gpu->ciErr1 == CL_INVALID_DEVICE)
            fprintf(stderr, "Invalid device\n");
        if (gpu->ciErr1 == CL_INVALID_VALUE)
            fprintf(stderr, "Something wrong specified in property\n");
        if (gpu->ciErr1 == CL_INVALID_QUEUE_PROPERTIES)
            fprintf(stderr, "Invalid property\n");
        if (gpu->ciErr1 == CL_OUT_OF_HOST_MEMORY)
            fprintf(stderr, "Allocating resources failed\n");
        lide_ocl_gpu_cleanup(gpu);
        return false;
    }
    return true;	
}

/* Set and get functions for global work size*/
void lide_ocl_gpu_set_globalworksize(lide_ocl_gpu_pointer gpu, cl_int size){
    gpu->szGlobalWorkSize = size;
    return;
}

cl_int lide_ocl_gpu_get_globalworksize(lide_ocl_gpu_pointer gpu){
    cl_int size;
    size = gpu->szGlobalWorkSize;
    return size;
}

/* Set and get functions for global work size*/
void lide_ocl_gpu_set_localworksize(lide_ocl_gpu_pointer gpu, cl_int size){
    gpu->szLocalWorkSize = size;
    return;
}

cl_int lide_ocl_gpu_get_localworksize(lide_ocl_gpu_pointer gpu){
    cl_int size;
    size = gpu->szLocalWorkSize;
    return size;
}

/* Termination*/
void lide_ocl_gpu_cleanup (lide_ocl_gpu_pointer gpu){
    if(gpu->cqCommandQueue)clReleaseCommandQueue(gpu->cqCommandQueue);
    if(gpu->cxGPUContext)clReleaseContext(gpu->cxGPUContext);
    free(gpu);
    return;
}
