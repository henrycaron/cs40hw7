/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_seq.h
    This is the interface for um_seq.c which handles segmented memory.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef UM_SEQ
#define UM_SEQ

typedef struct um_seq *um_seq;

/*
 * Name: segment_set_start
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, and int index which is the
 *             index of the segment to be copied and placed in segment 0
 * Does: Copies the segment stored at int index into a new segment. Frees the
 *       the current segment 0, and replaces it with the new copied segment.
 * Returns: nothing
*/
void segment_set_start(um_seq seg_mem, int index);

/*
 * Name: segment_add
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, and int length which is
 *             how many words the new segment will contain.
 * Does: Allocates memory for a new segment that is length long, and then
 *       inserts the new segment at the back of the segmented memory sequence
 *       or at the index popped from unmapped (depends if unmapped is empty)
 * Returns: the index where the new segment was inserted
*/
int segment_add(um_seq seg_mem, int length);

/*
 * Name: segment_get_val
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, uint32_t b which specifies
 *             which segment to modify, uint32_t c which specifices which word
 * Does: accesses and returns the word stored at segment b, word c
 * Returns: the word store at segment b, word c.
*/
uint32_t segment_get_val(um_seq seg_mem, uint32_t b, uint32_t c);

/*
 * Name: segment_set_val
 * Parameters: void* curr which is a um_seq struct that holds our segments
 *             sequence and our unmapped sequence, uint32_t value which is the
 *             value the word will be set to, uint32_t b which specifies which
 *             segment to modify, uint32_t c which specifices which word
 * Does: sets the value at segment b, word c, to value
 * Returns: nothing
*/
void segment_set_val(um_seq seg_mem, uint32_t value, uint32_t b, uint32_t c);

/*
 * Name: get_seg_length
 * Parameters: void* curr -- a um_seq struct that holds our segments
 *             sequence and our unmapped sequence.
 * Does: Returns the length of the segments sequence of a provided um_seq.
 * Returns: int -- the length of the segments sequence
*/
int get_seg_length(um_seq seg_mem, int index);

/*
 * Name: unmapped_add
 * Parameters: void* curr -- a um_seq struct that holds our segments
 *             sequence and our unmapped sequence. int index which is the
 *             index to our sequence, in segmented memory, to be unmapped
 * Does: Frees the sequence stored at int index, sets it to null, and pushes
 *       that index value onto the unmapped sequence.
 * Returns: void
*/
void unmapped_add(um_seq seg_mem, int index);

/*
 * Name: initialize
 * Parameters: Seq_T seg_zero -- the sequence to be stored in index 0, ie.
 *             segment 0
 * Does: Initializes an um_seq struct that contains a sequence of sequences in
 *       segments, where index 0 of the segment sequence is segment 0, and a
 *       sequence of unmapped indices
 * Returns: void * global -- a pointer to our new um_seq struct
*/
void * initialize(uint32_t* seg_zero, int seg_zero_size);

/*
 * Name: free_segs
 * Parameters: void* curr -- a pointer to our um_seq struct
 * Does: Frees the unmapped and segment sequences found in the um_seq struct
 *       passed in
 * Returns: void
*/
void free_segs(um_seq seg_mem);

#endif
