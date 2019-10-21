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

#include "lide_c_util.h"
#include "lide_c_file_read.h"
#include "lide_c_file_write.h"
#include "lide_c_fifo.h"
#include "lide_c_auto_correlation_computation.h"
#include "lide_c_noise_adder.h"
#include "lide_c_matrix_inversion.h"
#include "lide_c_cross_correlation_computation.h"
#include "lide_c_coefficient_computation.h"
#include "lide_c_response_computation.h"

#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */

#define ACTOR_LIDE_C_AUTO_CORRELATION_COMPUTATION   0
#define ACTOR_LIDE_C_NOISE_ADDER                    1
#define ACTOR_LIDE_C_MATRIX_INVERSION               2
#define ACTOR_SUBCARRIER_TIME_INDEX_SOURCE          3
#define ACTOR_SUBCARRIER_FREQ_INDEX_SOURCE          4
#define ACTOR_LIDE_C_CROSS_CORRELATION_COMPUTATION  5
#define ACTOR_LIDE_C_COEFFICIENT_COMPUTATION        6
#define ACTOR_LIDE_C_RESPONSE_COMPUTATION           7
#define ACTOR_CSI_SINK                              8

/* The total number of actors in the application. */
#define ACTOR_COUNT        9

#define TYPE_INT           0
#define TYPE_FLOAT         1
#define TYPE_DOUBLE        2

/*
    Usage:
    lide_c_channel_estimation_driver.exe subcarrier_time_index.txt subcarrier_freq_index.txt output-csi.txt
*/
int main(int argc, char **argv) {
    /* Simulation settings */
    /* pilot pattern: (time_index, freq_index) */
    int pilot_count = 4;
    int pilot_index[] = {0, 1, 0, 7, 2, 3, 2, 5};
    double channel_window = 0.04;
    double doppler_window = 0;
    double noise_variance = 0.01;
    double pilot_csi[] = {1.15, -0.77, 0.52, -0.34};
    
    FILE *subcarrier_time_index_source_file = NULL;
    FILE *subcarrier_freq_index_source_file = NULL;
    FILE *csi_sink_file = NULL;

    lide_c_actor_context_type *actors[ACTOR_COUNT];

    /* Connectivity: */
    /* R: auto-correlation matrix */
    /* d: cross-correlation vector */
    lide_c_fifo_pointer fifo_R = NULL;
    lide_c_fifo_pointer fifo_R_noise = NULL;
    lide_c_fifo_pointer fifo_R_noise_inverse = NULL;
    lide_c_fifo_pointer fifo_subcarrier_time_index_source = NULL;
    lide_c_fifo_pointer fifo_subcarrier_freq_index_source = NULL;
    lide_c_fifo_pointer fifo_d = NULL;
    lide_c_fifo_pointer fifo_filter_coefficient = NULL;
    lide_c_fifo_pointer fifo_csi = NULL;

    int token_size = 0;
    int i = 0;
    int arg_count = 4;
    char *descriptors[ACTOR_COUNT] = {
            "auto_correlation_computation", 
            "noise_adder", 
            "matrix_inversion", 
            "subcarrier_time_index_source",
            "subcarrier_freq_index_source",
            "lide_c_cross_correlation_computation", 
            "coefficient_computation",
            "response_computation", 
            "csi_sink"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "lide_c_channel_estimation_driver.exe error: arg count");
        exit(1);
    }

    /* Open the input and output file(s). */
    i = 1;
    subcarrier_time_index_source_file = lide_c_util_fopen(argv[i++], "r");
    subcarrier_freq_index_source_file = lide_c_util_fopen(argv[i++], "r");
    csi_sink_file = lide_c_util_fopen(argv[i++], "w");

    /* Create the buffers. */
    token_size = sizeof(double);
    fifo_R = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo_R_noise = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo_R_noise_inverse = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo_subcarrier_time_index_source = lide_c_fifo_new(BUFFER_CAPACITY, 
            sizeof(int));
    fifo_subcarrier_freq_index_source = lide_c_fifo_new(BUFFER_CAPACITY,
            sizeof(int));
    fifo_d = lide_c_fifo_new(BUFFER_CAPACITY, token_size);    
    fifo_filter_coefficient = lide_c_fifo_new(BUFFER_CAPACITY, token_size);
    fifo_csi = lide_c_fifo_new(BUFFER_CAPACITY, token_size);

    /* Create and connect the actors. */

    /* sources */
    actors[ACTOR_SUBCARRIER_TIME_INDEX_SOURCE] = (lide_c_actor_context_type
            *)(lide_c_file_read_new(subcarrier_time_index_source_file,
            fifo_subcarrier_time_index_source, TYPE_INT, 1));
    actors[ACTOR_SUBCARRIER_FREQ_INDEX_SOURCE] = (lide_c_actor_context_type
            *)(lide_c_file_read_new(subcarrier_freq_index_source_file,
            fifo_subcarrier_freq_index_source, TYPE_INT, 1));
    
    /* sinks */
    actors[ACTOR_CSI_SINK] = (lide_c_actor_context_type
            *)(lide_c_file_write_new(csi_sink_file, fifo_csi, TYPE_DOUBLE, 1));

    /* lide_c_auto_correlation_computation */
    actors[ACTOR_LIDE_C_AUTO_CORRELATION_COMPUTATION] = (
            lide_c_actor_context_type *)(
            lide_c_auto_correlation_computation_new(pilot_count, pilot_index, 
            channel_window, doppler_window, fifo_R));

    /* lide_c_noise_adder */
    actors[ACTOR_LIDE_C_NOISE_ADDER] = (lide_c_actor_context_type
            *)(lide_c_noise_adder_new(fifo_R, pilot_count, noise_variance, 
            fifo_R_noise));

    /* lide_c_matrix_inversion */
    actors[ACTOR_LIDE_C_MATRIX_INVERSION] = (lide_c_actor_context_type
            *)(lide_c_matrix_inversion_new(fifo_R_noise, pilot_count, 
            fifo_R_noise_inverse));
    
    /* lide_c_cross_correlation_computation */
    actors[ACTOR_LIDE_C_CROSS_CORRELATION_COMPUTATION] = 
            (lide_c_actor_context_type *)(
            lide_c_cross_correlation_computation_new(
            fifo_subcarrier_time_index_source, 
            fifo_subcarrier_freq_index_source, fifo_d, pilot_count, 
            pilot_index, channel_window, doppler_window));

    /* lide_c_coefficient_computation */
    actors[ACTOR_LIDE_C_COEFFICIENT_COMPUTATION] = (lide_c_actor_context_type
            *)(lide_c_coefficient_computation_new(fifo_R_noise_inverse, 
            fifo_d, fifo_filter_coefficient, pilot_count, pilot_count, 1));

    /* lide_c_response_computation */
    actors[ACTOR_LIDE_C_RESPONSE_COMPUTATION] = (lide_c_actor_context_type
            *)(lide_c_response_computation_new(fifo_filter_coefficient, 
            pilot_csi, pilot_count, fifo_csi));


    /* Execute the graph. */
    printf("\n\n==================== Simulation Start ====================\n");
    lide_c_util_simple_scheduler(actors, ACTOR_COUNT, descriptors);
    printf("\n====================== Simulation End ====================\n\n");
    /* Normal termination. */
    return 0;
}
