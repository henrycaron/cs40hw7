/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_run.h
    This is the interface for um_run.c which executes each word
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include "um_seq.h"
#include "um_parser.h"


#ifndef UM_RUN
#define UM_RUN

/*
 * Name: run_prog
 * Parameters: Seq_T seg_zero -- sequence containing segment 0 and its words
 * Does: This function executes each word in the sequence and executes the
 *       program
 * Returns: void
*/
void run_prog(uint32_t *seg_zero, int seg_zero_size);

#endif
