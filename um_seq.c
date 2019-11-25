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

#define HINT 1
 
/* struct that stores our two data structures that create our segmented memory
    Seq_T segments is a sequence of sequences, Seq_T is a sequence of ints that
    store the indices of segments that were mapped but are now unmapped*/
struct um_seq {
    Seq_T unmapped;
    Seq_T segments;
};

/*
 * Name: seg_free_helper
 * Parameters: Seq_T curr which is the sequence to be freed
 * Does: Loops through curr while it is not empy and frees every non-null elem
 * Returns: nothing
*/
void seg_free_helper(Seq_T curr)
{
    assert(curr != NULL);
    void * temp2 = NULL;
    while(Seq_length(curr) > 0) {
        temp2 = Seq_remhi(curr);
        if (temp2 != NULL) {
            free(temp2);
            temp2 = NULL;
        }
    }
    Seq_free(&curr);
}


/*
 * Name: segment_get
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, int index which is which
 *             specifies which segment to return
 * Does: accesses and returns the segment stored at int index
 * Returns: the segment stored at int index
*/
void* segment_get(void* curr, int index)
{
    assert(curr != NULL);
    um_seq obj = (um_seq) curr;
    Seq_T segment = Seq_get(obj->segments, index);
    return (void*)segment;
}

uint32_t segment_get_val(void* curr, uint32_t b, uint32_t c)
{
    assert(curr != NULL);
    Seq_T curr_seg = segment_get(curr,b);
    return *(uint32_t*)Seq_get(curr_seg, c);
}


void segment_set_val(void*curr, uint32_t value, uint32_t a, uint32_t b)
{
    assert(curr != NULL);
    Seq_T curr_seg = segment_get(curr,a);
    uint32_t * newval = malloc(sizeof(uint32_t));
    *newval = value;
    uint32_t *oldval = Seq_put(curr_seg, b, newval);
    assert(oldval != NULL);
    free(oldval);
}


void segment_set_start(void* curr, int index)
{
    assert(curr != NULL);

    um_seq obj = (um_seq) curr;
    Seq_T dup1 = Seq_get(obj->segments, index);
    Seq_T dup2 = Seq_new(HINT);
    /* Allocates entirely new sequence */
    for(int i = 0; i < Seq_length(dup1); i++) {
        uint32_t * curr_elt = calloc(1, sizeof(uint32_t));
        assert(curr_elt);
        *curr_elt = *(uint32_t*)Seq_get(dup1, i);
        Seq_addhi(dup2,curr_elt);
    }
    Seq_T seg_zero = Seq_put(obj->segments, 0, dup2);
    seg_free_helper(seg_zero);
}

/*
 * Name: unmapped_get
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence
 * Does: Pops off the top of the unmapped sequence if it is not empty, if it
 *       is empty it sets the return value to -1.
 * Returns: Either -1 if the unmapped sequence was empty, or the popped value
*/
int unmapped_get(void* curr)
{
    assert(curr != NULL);

    um_seq obj = (um_seq) curr;

    if (Seq_length(obj->unmapped) > 0) {
        int * ret_ptr = (int*)Seq_remhi(obj->unmapped);
        int ret_val = *ret_ptr;
        free(ret_ptr);
        return ret_val;
    }
    else {
        return -1;
    }
}

uint32_t segment_add(void* curr, int length)
{
    assert(curr != NULL);

    um_seq obj = (um_seq) curr;
    Seq_T new_seq = Seq_new(HINT);

    for(int i = 0; i < length; i++) {
        uint32_t * ptr = calloc(1, sizeof(uint32_t));
        assert(ptr);
        Seq_addhi(new_seq,(void*)ptr);
    }

    int index = unmapped_get(curr);
    /* checks if there is an unmapped index, then maps there if available */
    if(index == -1) {
        Seq_addhi(obj->segments, new_seq);
        index = (Seq_length(obj->segments)) - 1;
    }
   else {
        void* old_seg = Seq_put(obj->segments, index, new_seq);
        assert(old_seg == NULL);
    }

    return index;
}


int get_seg_length(void* curr, int index)
{
    assert(curr);
    um_seq obj = (um_seq) curr;
    Seq_T curr_segment = Seq_get(obj->segments, index);
    assert(curr_segment);
    return Seq_length(curr_segment);
}


void unmapped_add(void*curr, int index)
{
    assert(curr != NULL);

    um_seq obj = (um_seq) curr;
    Seq_T temp = Seq_put(obj->segments, index, NULL);
    assert(temp != NULL);
    seg_free_helper(temp);


    int * i_p = calloc(1, sizeof(int));
    assert(i_p != NULL);
    *i_p = index;
    Seq_addhi(obj->unmapped, i_p);
}

void* initialize(Seq_T seg_zero)
{
    assert(seg_zero != NULL);

    um_seq global = calloc(1, sizeof(struct um_seq));
    assert(global);

    global->unmapped = Seq_new(HINT);
    global->segments = Seq_new(HINT);

    Seq_addhi(global->segments, (void*)seg_zero);

    return global;
}


void free_segs(void* curr)
{
    assert(curr != NULL);
    um_seq obj = (um_seq) curr;


    /* Frees the unmapped sequence */
    assert(obj->unmapped != NULL);
    while(Seq_length(obj->unmapped) > 0) {
        void * temp2 = Seq_remhi(obj->unmapped);
        if(temp2 != NULL) {
            free(temp2);
        }
    }
    Seq_free(&(obj->unmapped));

    /* Frees the segment sequence */
    assert(obj->segments != NULL);
    while(Seq_length(obj->segments) > 0) {
        void* temp = Seq_remhi(obj->segments);
        if(temp != NULL) {
            Seq_T curr2 = (Seq_T) temp;
            assert(curr2);
            seg_free_helper(curr2);
        }
    }

    Seq_free(&obj->segments);
    if(obj != NULL) {
        free(obj);
    }
    curr = NULL;
}
