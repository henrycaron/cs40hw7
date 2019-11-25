/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_parser.h
    This is the interface for um_parser.c which handles parsing words
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef UM_PARSER
#define UM_PARSER

/*
 * Name: parse
 * Parameters: uint32_t word which is the bitpacked instruction we will parese
 * Does: parses the word using bitpack.h and then returns an array of integers
        that hold the correct register values. What and how many values are
        stored depends on the opcode. the 0th index always stores the opcode
 * Returns: integer array containing all the information needed for that
            particular instruction. So could have 4 elements or 1, and could
            be registers, or a value itslef (in the case of load val).
 */
uint32_t* parse(uint32_t word, uint32_t *instruction);

#endif
