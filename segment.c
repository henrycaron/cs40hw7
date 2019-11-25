/**************************************************************
* Comp40, hw6
* Daniel Alderman and Philip Miljanic
* dalder01, pmilja01
* um_seq.c
* This is the implementation of um_seq.h, which stores the memory.
**************************************************************/

#include "segment.h"
#include "assert.h"
#include <stdint.h>

Segment_T Segment_new(int size)
{
    Segment_T seg = malloc(sizeof(struct Segment_T));
    
    seg -> array = calloc(size, sizeof(uint32_t));
    seg -> length = size;
    
    return seg;
}

void Segment_free(Segment_T seg)
{
    free(seg -> array);
    free(seg);
}

uint32_t Segment_get(Segment_T seg, int i)
{
    return seg -> array[i];
}

int Segment_length(Segment_T seg)
{
    return seg -> length;
}

uint32_t Segment_put(Segment_T seg, int index, uint32_t elem)
{
    uint32_t prev = seg -> array [index];
    seg -> array [index] = elem;
    return prev;
}
