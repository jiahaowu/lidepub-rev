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

#include "lide_ocl_file_source.h"
#include "lide_ocl_util.h"

struct _lide_ocl_file_source_context_struct {
#include "lide_ocl_actor_context_type_common.h"
    char *file; 
    FILE *fp;
    lide_ocl_fifo_pointer out;
};


lide_ocl_file_source_context_type *lide_ocl_file_source_new(char *file,
        lide_ocl_fifo_pointer out) {

    lide_ocl_file_source_context_type *context = NULL;

	context = (lide_ocl_file_source_context_type *)lide_ocl_util_malloc(sizeof(lide_ocl_file_source_context_type));
    context->enable =
            (lide_ocl_actor_enable_function_type)lide_ocl_file_source_enable;
    context->invoke =
            (lide_ocl_actor_invoke_function_type)lide_ocl_file_source_invoke;
    context->file = file;
    context->fp = lide_ocl_util_fopen((const char *)context->file, "r");
    context->mode = LIDE_OCL_FILE_SOURCE_MODE_WRITE;
    context->out = out;
    return context;
}

boolean lide_ocl_file_source_enable(lide_ocl_file_source_context_type *context) {
    boolean result = FALSE;

    switch (context->mode) {
    case LIDE_OCL_FILE_SOURCE_MODE_WRITE:
        result = (lide_ocl_fifo_population(context->out) < 
                lide_ocl_fifo_capacity(context->out));
        break;
    case LIDE_OCL_FILE_SOURCE_MODE_INACTIVE:
        result = FALSE;
        break;
    default:
        result = FALSE;
        break;
    }
    return result;
}

void lide_ocl_file_source_invoke(lide_ocl_file_source_context_type *context) {
    int value = 0;
    switch (context->mode) {
    case LIDE_OCL_FILE_SOURCE_MODE_WRITE:
        if (fscanf(context->fp, "%d", &value) != 1) { 
            /* End of input */
            context->mode = LIDE_OCL_FILE_SOURCE_MODE_INACTIVE;
            return;
        } 
        lide_ocl_fifo_write(context->out, &value);
        context->mode = LIDE_OCL_FILE_SOURCE_MODE_WRITE;
        break;
    case LIDE_OCL_FILE_SOURCE_MODE_INACTIVE:
        context->mode = LIDE_OCL_FILE_SOURCE_MODE_INACTIVE;
        break;
    default:
        context->mode = LIDE_OCL_FILE_SOURCE_MODE_INACTIVE;
        break;
    }
}

void lide_ocl_file_source_terminate(lide_ocl_file_source_context_type *context) {
    fclose(context->fp);
    free(context);
}

