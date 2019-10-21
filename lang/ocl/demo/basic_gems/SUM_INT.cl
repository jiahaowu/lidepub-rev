/******************************************************************************
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
******************************************************************************/

/* OpenCL Kernel Function for summation of a vector */
__kernel void SUM_INT(__global const int* x, __global int* scratch,
                        __global int* result, int length)
{
    
    /* get index into global data array */
    int iGID = get_global_id(0);
    int iLID = get_local_id(0);
    int offset = get_local_size(0);
    
    /* 
        bound check (equivalent to the limit on a 'for' loop for 
        standard/serial C code
    */
    if (iGID >= length)
    {   
        scratch[iLID] = 0;
    }else {
        scratch[iLID] = x[iGID];
    }
    barrier (CLK_LOCAL_MEM_FENCE);
    for ( offset = offset/2; offset>0; offset >>= 1 ){
        if (iLID < offset){
            scratch[iLID] = scratch[iLID] + scratch[iLID+offset];
        }
        barrier (CLK_LOCAL_MEM_FENCE);
    }
    if (iLID == 0){
        *result = scratch[0];
    }
}
