/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_run.h
    This is the interface for um_run.c which executes each word
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct Segment_T {
    uint32_t *array;
    int length;
} *Segment_T;

Segment_T Segment_new(int size);

void Segment_free(Segment_T seg);

uint32_t Segment_get(Segment_T seg, int i);

int Segment_length(Segment_T seg);

uint32_t Segment_put(Segment_T seg, int index, uint32_t elem);

#endif