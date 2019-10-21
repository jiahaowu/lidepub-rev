#ifndef _lide_ocl_inner_product_h
#define _lide_ocl_inner_product_h

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

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "lide_ocl_actor.h"
#include "lide_ocl_fifo.h"
#include "lide_ocl_gpu.h"

/*******************************************************************************
This inner product actor consumes vector length (the number of elements in each
vector) and vectors and computes the inner product. There are three modes
associated with this actor. At the STORE_LENGTH mode, this actor consumes
vector length, sets the length variable, and sets the PROCESS mode as the next
mode. At the PROCESS mode, this actor consumes vectors based on the configured
vector length, computes the inner product, and sets the STORE_LENGTH mode as
the next mode. For capturing an error, the actor transits to an ERROR mode
where an error message is printed to stderr.
*******************************************************************************/

/* Actor modes */
#define LIDE_OCL_INNER_PRODUCT_MODE_ERROR          0
#define LIDE_OCL_INNER_PRODUCT_MODE_STORE_LENGTH   1
#define LIDE_OCL_INNER_PRODUCT_MODE_PROCESS        2 

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/

/* Structure and pointer types associated with inner product objects. */
struct _lide_ocl_inner_product_context_struct;
typedef struct _lide_ocl_inner_product_context_struct
        lide_ocl_inner_product_context_type;

/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

/*****************************************************************************
Construct function of the lide_ocl_inner_product actor. Create a new
lide_ocl_inner_product with the specified input FIFO pointer m for the
configuration of vector length, the specified input FIFO pointer x for the
first vector, the specified input FIFO pointer y for the second vector, the
specified output FIFO pointer, and the initial parameter setting for the
repeat_length.
*****************************************************************************/
lide_ocl_inner_product_context_type *lide_ocl_inner_product_new(
        lide_ocl_fifo_pointer m, lide_ocl_fifo_pointer x, 
        lide_ocl_fifo_pointer y, lide_ocl_fifo_pointer out, 
        lide_ocl_gpu_pointer gpu, int repeat_length);

/*****************************************************************************
Enable function of the lide_ocl_inner_product actor.
*****************************************************************************/
boolean lide_ocl_inner_product_enable(
                                lide_ocl_inner_product_context_type *context);

/*****************************************************************************
Invoke function of the lide_ocl_inner_product actor.
*****************************************************************************/
void lide_ocl_inner_product_invoke(
                                lide_ocl_inner_product_context_type *context);

/*****************************************************************************
Terminate function of the lide_ocl_inner_product actor.
*****************************************************************************/
void lide_ocl_inner_product_terminate(
                                lide_ocl_inner_product_context_type *context);

/*****************************************************************************
Accessory function of the lide_ocl_inner_product actor for setting the parameter
of repeat_length. The repeat_length parameter gives the number of times to
apply the current vector length setting before transitioning to the store
length mode to read another vector length. The parameter value should be
non-negative. If it is zero-valued, it means that the actor should just
continue using the current vector length setting indefinitely.
*****************************************************************************/
void lide_ocl_inner_product_set_repeat_length(
        lide_ocl_inner_product_context_type *context, int repeat_length);

/*****************************************************************************
Accessory function of the lide_ocl_inner_product actor for getting the parameter
value of repeat_length. The repeat_length parameter gives the number of times
to apply the current vector length setting before transitioning to the store
length mode to read another vector length. The parameter value should be
non-negative. If it is zero-valued, it means that the actor should just
continue using the current vector length setting indefinitely.
*****************************************************************************/
int lide_ocl_inner_product_get_repeat_length(
        lide_ocl_inner_product_context_type *context); 

#endif
