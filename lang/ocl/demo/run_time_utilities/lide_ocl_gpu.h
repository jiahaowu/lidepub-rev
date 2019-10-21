#ifndef _lide_ocl_gpu_h
#define _lide_ocl_gpu_h
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
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

/*GPU context initialization*/
struct lide_ocl_gpu_context_struct;
typedef struct _lide_ocl_gpu_struct lide_ocl_gpu_type;   

/* A pointer to GPU context */
typedef lide_ocl_gpu_type *lide_ocl_gpu_pointer;

struct _lide_ocl_gpu_struct {
    cl_context cxGPUContext;        /* OpenCL context */
    cl_command_queue cqCommandQueue;/* OpenCL command queue */
    cl_platform_id cpPlatform;      /* OpenCL platform */
    cl_device_id cdDevice;          /* OpenCL device */
    cl_int ciErr1, ciErr2;			/* Error code var */
    size_t szGlobalWorkSize;        /* 1D var for Total # of work items */
    size_t szLocalWorkSize;		    /* 1D var for # of work items in the work group	*/
    size_t szParmDataBytes;			/* Byte size of context information */

};

/********************* 
    Public Functions
*********************/
/* Construction*/
lide_ocl_gpu_pointer lide_ocl_gpu_new(void);
/* Get platform ID*/
bool lide_ocl__gpu_get_platformID(lide_ocl_gpu_pointer gpu, cl_uint num_entries, 
                                    cl_uint *num_platforms);

/* Get device*/
bool lide_ocl__gpu_get_deviceID(lide_ocl_gpu_pointer gpu, 
                                cl_device_type device_type, cl_uint num_entries, 
                                cl_uint *num_devices);

/* Create Context of GPU*/
bool lide_ocl_gpu_context_new(lide_ocl_gpu_pointer gpu, 
                                cl_context_properties *properties, 
                                cl_uint num_devices, 
                                void *pfn_notify(const char *errinfo, 
                                const void *private_info, 
                                size_t cb, void *user_data), 
                                void *user_data);

/* Create a command-queue of GPU */
bool lide_ocl_gpu_cmdq_new(lide_ocl_gpu_pointer gpu, 
                            cl_command_queue_properties properties);

/* Set and get functions for global work size*/
void lide_ocl_gpu_set_globalworksize(lide_ocl_gpu_pointer gpu, cl_int size);

cl_int lide_ocl_gpu_get_globalworksize(lide_ocl_gpu_pointer gpu);

/* Set and get functions for global work size*/
void lide_ocl_gpu_set_localworksize(lide_ocl_gpu_pointer gpu, cl_int size);

cl_int lide_ocl_gpu_get_localworksize(lide_ocl_gpu_pointer gpu);

/* Cleanup Functions*/
void lide_ocl_gpu_cleanup(lide_ocl_gpu_pointer gpu);    

#endif
