#ifndef _lide_ocl_kernel_h
#define _lide_ocl_kernel_h
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
#include "lide_ocl_gpu.h"
#define K_MAXLENGTH 500
/*Kernel context initialization*/
struct lide_ocl_kernel_context_struct;
typedef struct _lide_ocl_kernel_struct lide_ocl_kernel_type;   

/* A pointer to GPU context */
typedef lide_ocl_kernel_type *lide_ocl_kernel_pointer;

struct _lide_ocl_kernel_struct {
    cl_program cpProgram;                   /* OpenCL program object */
    cl_kernel ckKernel;	                    /* OpenCL kernel object */
    char cPathAndName[K_MAXLENGTH];         /* OpenCL kernel file path */
    char* cSourceCL;                        /* OpenCL kernel code */
    const char* cSourceFile;                /* OpenCL source file name */
    lide_ocl_gpu_pointer gpu;               /* GPU settings */
    size_t szKernelLength;                  /* Kernel source code length */
};

/********************* 
	Public Functions
*********************/
/* Construction*/
lide_ocl_kernel_pointer lide_ocl_kernel_new(void);
/* Set and get functions for gpu settings*/
void lide_ocl_kernel_set_gpu_setting(lide_ocl_kernel_pointer kernel, 
										lide_ocl_gpu_pointer gpu);
										
lide_ocl_gpu_pointer lide_ocl_kernel_get_gpu_setting(
						lide_ocl_kernel_pointer kernel);
						
/* Set and get functions for cSourceFile settings*/
void lide_ocl_kernel_set_cSourceFile(lide_ocl_kernel_pointer kernel, 
										const char* filename);

const char* lide_ocl_kernel_get_cSourceFile(lide_ocl_kernel_pointer kernel);
										
/* Set and get functions for pathname settings*/
void lide_ocl_kernel_set_cPathAndName(lide_ocl_kernel_pointer kernel, 
                            const char* pathname, const char* filename);

char* lide_ocl_kernel_get_cPathAndName(lide_ocl_kernel_pointer kernel);

/* Load source code*/
bool lide_ocl_kernel_load_source (lide_ocl_kernel_pointer kernel, 
								const char* cPreamble);

/* Create Program with kernel source code*/
bool lide_ocl_kernel_create_program (lide_ocl_kernel_pointer kernel, 
									cl_uint count);		
						
/* Build the program */
bool lide_ocl_kernel_build_program (lide_ocl_kernel_pointer kernel, 
                                    cl_uint num_devices, 
                                    const cl_device_id *device_list, 
                                    const char *options, 
                                    void (*pfn_notify)
                                    (cl_program, void *user_data), 
                                    void *user_data);

/* Create Kernel*/
bool lide_ocl_kernel_create_kernel (lide_ocl_kernel_pointer kernel, 
                                    const char *kernel_name);

#endif
