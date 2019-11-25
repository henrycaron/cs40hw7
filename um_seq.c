/**************************************************************
* Comp40, hw6
* Daniel Alderman and Philip Miljanic
* dalder01, pmilja01
* um_seq.c
* This is the implementation of um_seq.h, which stores the memory.
**************************************************************/
#include "um_seq.h"
#include "assert.h"
#include <stdint.h>

#define HINT 100

/* struct that stores our two data structures that create our segmented memory
    Seq_T segments is a sequence of sequences, Seq_T is a sequence of ints that
    store the indices of segments that were mapped but are now unmapped*/

struct um_seq {
    uint32_t ** memory; // array of arrays of uint32_t (memory)

    int * seg_length;
    int mem_curr_size;
    int mem_length;
    int * unmapped;     // array of unmapped indices to be reused
    int unmapped_curr_size;
    int unmapped_length;
};

static int rem(um_seq seg_mem)
{
    int return_val = seg_mem -> unmapped[seg_mem -> unmapped_curr_size - 1];
    seg_mem -> unmapped[seg_mem -> unmapped_curr_size -1] = 0;
    seg_mem -> unmapped_curr_size = seg_mem -> unmapped_curr_size - 1;
    return return_val;
}

static void add_segment(um_seq seg_mem, uint32_t *new_seg, int length)
{
    int mem_length = seg_mem -> mem_length;
    if (seg_mem -> mem_curr_size >= mem_length) {
        seg_mem -> memory = (uint32_t **) realloc(seg_mem -> memory, sizeof(uint32_t *) * mem_length * 2);
        seg_mem -> seg_length = (int *)realloc(seg_mem -> seg_length, sizeof(int) * mem_length * 2);
        // uint32_t **new_seg_mem = calloc(mem_length * 2, sizeof(uint32_t));
        // int *new_seg_length = calloc(mem_length * 2, sizeof(int));
        // for(int i = 0; i < mem_length; i++){
        //     new_seg_mem[i] = seg_mem -> memory[i];
        //     new_seg_length[i] = seg_mem -> seg_length[i];
        // }
        //
        // free(seg_mem -> memory);
        // free(seg_mem -> seg_length);
        // seg_mem -> memory = new_seg_mem;
        // seg_mem -> seg_length = new_seg_length;
        seg_mem -> mem_length = mem_length * 2;
    }

    seg_mem -> memory[seg_mem -> mem_curr_size] = new_seg;
    seg_mem -> seg_length[seg_mem -> mem_curr_size] = length;
    seg_mem -> mem_curr_size = seg_mem -> mem_curr_size + 1;
}

static void add_index(um_seq seg_mem, uint32_t index)
{
    int unm_len = seg_mem -> unmapped_length;
    if (seg_mem -> unmapped_curr_size >= unm_len) {
        int *new_unmapped = malloc(sizeof(int) * unm_len * 2);
        for(int i = 0; i < unm_len; i++){
            new_unmapped[i] = seg_mem -> unmapped[i];
        }
        free(seg_mem -> unmapped);
        seg_mem -> unmapped = new_unmapped;
        seg_mem -> unmapped_length = unm_len * 2;
    }
    seg_mem -> unmapped[seg_mem -> unmapped_curr_size] = index;
    seg_mem -> unmapped_curr_size = seg_mem -> unmapped_curr_size + 1;
}

/*
 * Name: segment_get
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, int index which is which
 *             specifies which segment to return
 * Does: accesses and returns the segment stored at int index
 * Returns: the segment stored at int index
*/
uint32_t* segment_get(um_seq seg_mem, int index)
{
    assert(seg_mem != NULL);
    uint32_t *segment = seg_mem -> memory[index];
    return segment;
}

uint32_t segment_get_val(um_seq seg_mem, uint32_t b, uint32_t c)
{
    assert(seg_mem != NULL);
    // printf("segment id: %u\nsegment index: %u\n", b, c);
    return seg_mem -> memory[b][c];
}


void segment_set_val(um_seq seg_mem, uint32_t value, uint32_t a, uint32_t b)
{
    assert(seg_mem != NULL);
    seg_mem -> memory[a][b] = value;
}


void segment_set_start(um_seq seg_mem, int index)
{
    assert(seg_mem != NULL);

    int length = seg_mem -> seg_length[index];

    uint32_t * dup = malloc(sizeof(uint32_t) * length);

    for(int i = 0; i < length; i++) {
        dup[i] = seg_mem -> memory[index][i];
    }

    uint32_t * seg_zero = seg_mem -> memory[0];
    free(seg_zero);
    seg_mem -> memory[0] = dup;
    seg_mem -> seg_length[0] = length;
    seg_mem -> seg_length[index] = 0;
}

/*
 * Name: unmapped_get
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence
 * Does: Pops off the top of the unmapped sequence if it is not empty, if it
 *       is empty it sets the return value to -1.
 * Returns: Either -1 if the unmapped sequence was empty, or the popped value
*/
//unmapped_get

int segment_add(um_seq seg_mem, int length)
{
    assert(seg_mem != NULL);

    uint32_t* new_seg = calloc(length, sizeof(uint32_t));

    int index;
    if (seg_mem -> unmapped_curr_size > 0) {
        index = rem(seg_mem);
        seg_mem -> memory[index] = new_seg;
        seg_mem -> seg_length[index] = length;
    }
    else {
        add_segment(seg_mem, new_seg, length);
        index = (seg_mem -> mem_curr_size) - 1;
    }

    // printf("new index: %d\n", index);
    return index;
}


int get_seg_length(um_seq seg_mem, int index)
{
    assert(seg_mem);
    // printf("seg length of segment %d: %d\n", index, seg_mem -> seg_length[index]);
    return seg_mem -> seg_length[index];
}


void unmapped_add(um_seq seg_mem, int index)
{
    assert(seg_mem != NULL);

    free((seg_mem -> memory[index]));
    seg_mem -> memory[index] = NULL;

    add_index(seg_mem, index);
}

void * initialize(uint32_t* seg_zero, int seg_zero_size)
{
    assert(seg_zero != NULL);

    um_seq seg_mem = malloc(sizeof(struct um_seq));
    assert(seg_mem);

    seg_mem -> unmapped = calloc(20, sizeof(int));
    seg_mem -> memory = calloc(20, sizeof(uint32_t *));
    seg_mem -> memory[0] = seg_zero;
    // printf("0;0: %u\n", seg_mem -> memory[0][0]);
    seg_mem -> unmapped_curr_size = 0;
    seg_mem -> unmapped_length = 20;

    seg_mem -> seg_length = calloc(20, sizeof(int));
    seg_mem -> mem_curr_size = 1;
    seg_mem -> mem_length = 20;

    seg_mem -> seg_length[0] = seg_zero_size;


    return (void *)seg_mem;
}


void free_segs(um_seq seg_mem)
{
    (void) seg_mem;
    assert(seg_mem != NULL);

    /* Frees the unmapped sequence */
    assert(seg_mem -> unmapped != NULL);
    free(seg_mem -> unmapped);

    /* Frees the seg_length array */
    assert(seg_mem -> seg_length != 0);
    free(seg_mem -> seg_length);

    /* Frees the segment sequence */
    assert(seg_mem -> memory != NULL);
    for(int i = 0; i < seg_mem -> mem_curr_size; i++) {
        if(seg_mem -> memory[i] != NULL){
            free(seg_mem -> memory[i]);
        }
    }
    free(seg_mem -> memory);
    free(seg_mem);

}
