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
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "lide_ocl_kernel.h"
#include "lide_ocl_gpu.h"

/* Function for load OpenCL source code */
char* oclLoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength);

/* Construction*/
lide_ocl_kernel_pointer lide_ocl_kernel_new(void){
    lide_ocl_kernel_pointer kernel;
    kernel = (lide_ocl_kernel_pointer) malloc (sizeof(lide_ocl_kernel_type));
    if (kernel == NULL)
        return NULL;
    kernel->cSourceCL = NULL;
    kernel->cSourceFile = NULL;
    return kernel;
}

/* Set and get functions for gpu settings*/
void lide_ocl_kernel_set_gpu_setting(lide_ocl_kernel_pointer kernel, 
                                        lide_ocl_gpu_pointer gpu){
    kernel->gpu = gpu;
    return;
}
                                        
lide_ocl_gpu_pointer lide_ocl_kernel_get_gpu_setting(
                        lide_ocl_kernel_pointer kernel){
    return kernel->gpu;
}
                        
/* Set and get functions for cSourceFile settings*/
void lide_ocl_kernel_set_cSourceFile(lide_ocl_kernel_pointer kernel, 
                                        const char* filename){
    kernel->cSourceFile = filename;
    return;
}

const char* lide_ocl_kernel_get_cSourceFile(lide_ocl_kernel_pointer kernel){
    return kernel->cSourceFile;
}
                                        
/* Set and get functions for pathname settings*/
void lide_ocl_kernel_set_cPathAndName(lide_ocl_kernel_pointer kernel, 
                                        const char* pathname, const char* filename){
    char clpath[K_MAXLENGTH];
    strcpy(clpath, pathname);
    strcat(clpath, filename);
    strcpy(kernel->cPathAndName, clpath);
    return;
}

char* lide_ocl_kernel_get_cPathAndName(lide_ocl_kernel_pointer kernel){
    return kernel->cPathAndName;
}

/* Load source functions for loading OpenCL source code*/
bool lide_ocl_kernel_load_source (lide_ocl_kernel_pointer kernel, 
                                const char* cPreamble){

    kernel->cSourceCL = oclLoadProgSource(kernel->cPathAndName, cPreamble, 
                                        &kernel->szKernelLength);
    if (kernel->cSourceCL == NULL){
        fprintf(stderr, "error in load source\n");
        return false;
    }
    return true;
}


/* Create Program with kernel source code*/
bool lide_ocl_kernel_create_program (lide_ocl_kernel_pointer kernel, 
                                    cl_uint count){

    kernel->cpProgram = clCreateProgramWithSource(kernel->gpu->cxGPUContext, 
                                count, (const char **)&kernel->cSourceCL, 
                                &kernel->szKernelLength, 
                                &kernel->gpu->ciErr1);

    if (kernel->gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateProgramWithSource   %d\n", kernel->gpu->ciErr1);
        return false;
    }
    return true;
}		
                        
/* Build the program */
bool lide_ocl_kernel_build_program (lide_ocl_kernel_pointer kernel, 
                                    cl_uint num_devices, 
                                    const cl_device_id *device_list, 
                                    const char *options, 
                                    void (*pfn_notify)
                                    (cl_program, void *user_data), 
                                    void *user_data
                                    ){
    kernel->gpu->ciErr1 = clBuildProgram(kernel->cpProgram, num_devices, 
                                        device_list, options, NULL, user_data);
    if (kernel->gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clBuildProgram\n");
        return false;
    }			
    return true;
}


/* Create Kernel*/
bool lide_ocl_kernel_create_kernel (lide_ocl_kernel_pointer kernel, 
                                    const char *kernel_name){
    kernel->ckKernel = clCreateKernel(kernel->cpProgram, kernel_name, 
                                    &kernel->gpu->ciErr1);
    if (kernel->gpu->ciErr1 != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateKernel\n");
        return false;
    }
    return true;
}

char* oclLoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
    /* locals variables for load source */
    FILE* pFileStream = NULL;
    size_t szSourceLength;
    size_t szPreambleLength;
    /* open the OpenCL source code file */
    /* Windows version */
    #ifdef _WIN32   
        if(fopen_s(&pFileStream, cFilename, "rb") != 0) 
        {       
            return NULL;
        }
    /* Linux version */
    #else           
        pFileStream = fopen(cFilename, "rb");
        if(pFileStream == 0) 
        {       
            return NULL;
        }
    #endif

    szPreambleLength = strlen(cPreamble);

    /* get the length of the source code */
    fseek(pFileStream, 0, SEEK_END); 
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET); 

    /* allocate a buffer for the source code string and read it in */
    char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1); 
    memcpy(cSourceString, cPreamble, szPreambleLength);
    if (fread((cSourceString) + szPreambleLength, szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }

    /* close the file and return the total length of the combined (preamble + source) string */
    fclose(pFileStream);
    if(szFinalLength != 0)
    {
        *szFinalLength = szSourceLength + szPreambleLength;
    }
    cSourceString[szSourceLength + szPreambleLength] = '\0';
    if (cSourceString == NULL)
        printf("null string\n");
    return cSourceString;
}
