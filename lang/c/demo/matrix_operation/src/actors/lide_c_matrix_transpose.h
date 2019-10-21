#ifndef _lide_c_matrix_transpose_h
#define _lide_c_matrix_transpose_h

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
#include "lide_c_actor.h"
#include "lide_c_fifo.h"

/* Actor modes */

#define LIDE_C_MATRIX_TRANSPOSE_MODE_ERROR     0 
#define LIDE_C_MATRIX_TRANSPOSE_MODE_PROCESS   1 

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/

/* Structure and pointer types associated with matrix_transpose objects. */
struct _lide_c_matrix_transpose_context_struct;
typedef struct _lide_c_matrix_transpose_context_struct 
        lide_c_matrix_transpose_context_type;

/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

lide_c_matrix_transpose_context_type *lide_c_matrix_transpose_new(
        lide_c_fifo_pointer in1, lide_c_fifo_pointer out, int m, int n);

boolean lide_c_matrix_transpose_enable(
        lide_c_matrix_transpose_context_type *context); 

void lide_c_matrix_transpose_invoke(
        lide_c_matrix_transpose_context_type *context); 

void lide_c_matrix_transpose_terminate(
        lide_c_matrix_transpose_context_type *context);
        
/*Set and get func for parameter*/
int lide_c_matrix_transpose_get_m(
        lide_c_matrix_transpose_context_type *context);
        
void lide_c_matrix_transpose_set_m(
        lide_c_matrix_transpose_context_type *context, int m);
       
int lide_c_matrix_transpose_get_n(
        lide_c_matrix_transpose_context_type *context);

void lide_c_matrix_transpose_set_n(
        lide_c_matrix_transpose_context_type *context, int n);

#endif
