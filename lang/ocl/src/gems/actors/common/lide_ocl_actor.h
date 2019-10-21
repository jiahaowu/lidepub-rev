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
#ifndef _lide_ocl_actor_h
#define _lide_ocl_actor_h

#include "lide_ocl_basic.h"

struct lide_ocl_actor_context_struct;

/*****************************************************************************
A pointer to a "lide_ocl_actor_invoke_function", which is a function that invokes
an actor with a given context.
*****************************************************************************/
typedef void (*lide_ocl_actor_invoke_function_type) 
        (struct lide_ocl_actor_context_struct *context);

/*****************************************************************************
A pointer to a "lide_ocl_actor_enable_function", which is a function that enables
an actor with a given context.
*****************************************************************************/
typedef boolean (*lide_ocl_actor_enable_function_type) 
        (struct lide_ocl_actor_context_struct *context);

typedef struct lide_ocl_actor_context_struct {
#include "lide_ocl_actor_context_type_common.h"
} lide_ocl_actor_context_type;

#endif
