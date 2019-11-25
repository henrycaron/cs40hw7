/* Daniel Alderman and Philip Miljanic
 *    Comp 40, HW6 - UM
 *    11/15/2019
 *    um_run.c
 *    This is the implementation for um_run.c which executes each word
*/

#include <stdio.h>
#include <stdlib.h>

#include "um_run.h"
#include "um_parser.h"
#include "assert.h"
#include "um-dis.h"
#include <math.h>
 

/*
 * Name: free_prog
 * Parameters: seg_mem which holds our sgemented memory, registers which holds
 *             an array of our registers, and counter which is the curr_segment
 *             instruction we are executing.
 * Does: frees all memory associated with the um.
 * Returns: nothing
*/
void free_prog(um_seq seg_mem, uint32_t* registers, int* counter)
{
    assert(counter);
    assert(registers);
    assert(seg_mem);

    free(counter);
    free(registers);
    free_segs((void*)seg_mem);
}

/*
 * Name: condition
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: set ra equal to rb if rc is not equal to 0.
 * Returns: nothing
*/
void conditional_move(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];

    if(registers[rc] != 0) {
        registers[ra] = registers[rb];
    }
}

/*
 * Name: seg_load
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory.
 * Does: sets register ra equal to the value stored at the segment[rb]
 *       index[rc]
 * Returns: nothing
*/
void seg_load(uint32_t* instruction, uint32_t* registers, um_seq seg_mem)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    registers[ra] = segment_get_val((void*) seg_mem,
                                    registers[rb], registers[rc]);
}

/*
 * Name: seg_store
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory.
 * Does: stores value rc at segment ra, index rb
 * Returns: nothing
*/
void seg_store(uint32_t* instruction, uint32_t* registers, um_seq seg_mem)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    segment_set_val(seg_mem, registers[rc], registers[ra], registers[rb]);

}

/*
 * Name: add
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: adds the values of rb and rc together and stores the sum in ra
 * Returns: nothing
*/
void add(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];

    registers[ra] = (registers[rb] + registers[rc]);

}

/*
 * Name: multiply
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: mutliplies the values stored in rb and rc and stores product in rc
 * Returns: nothing
*/
void multiply(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    registers[ra] = (registers[rb] * registers[rc]);
}

/*
 * Name: divide
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: divides the value in rb by the val in rc and stores the result in ra
 * Returns: nothing
*/
void divide(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    registers[ra] = registers[rb] / registers[rc];

}

/*
 * Name: nand
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: does bitwise nand operation on values in rb and rc, puts results in ra
 * Returns: nothing
*/
void nand(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];

    uint32_t temp = registers[rb] & registers[rc];
    registers[ra] = ~temp;
}

/*
 * Name: halt
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory. counter which
                stores which element in seg 0 to execute next
 * Does: frees instruction, then calls free_prog to free everything else. After
         this it exits the program with exit 0.
 * Returns: nothing
*/
void halt(uint32_t* instruction, uint32_t* registers, um_seq seg_mem,
          int* counter)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);
    assert(counter);
    free(instruction);
    free_prog(seg_mem, registers, counter);
    exit(0);
}

/*
 * Name: map_seg
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory.
 * Does: maps a new segment of length, value in rc, & sets rb to the new index
 * Returns: nothing
*/
void map_seg(uint32_t* instruction, uint32_t* registers, um_seq seg_mem)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);

    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    registers[rb] = segment_add(seg_mem, registers[rc]);
}

/*
 * Name: seg_store
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory.
 * Does: unmaps the segment stored at index rc
 * Returns: nothing
*/
void unmap_seg(uint32_t* instruction, uint32_t* registers, um_seq seg_mem)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);
    uint32_t rc = instruction[3];
    unmapped_add(seg_mem, registers[rc]);
}

/*
 * Name: output
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: prints the value stored in rc to stdout. Only prints one byte
 * Returns: nothing
*/
void output(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t rc = instruction[3];
    assert(registers[rc] < 256);
    printf("%c", registers[rc]);
}

/*
 * Name: input
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: gets value (one byte) from stdin and stores it in register rc
 * Returns: nothing
*/
void input(uint32_t* instruction, uint32_t* registers)
{
    assert(instruction);
    assert(registers);
    uint32_t rc = instruction[3];
    registers[rc] = fgetc(stdin);
}

/*
 * Name: load_program
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory. counter which
                stores which element in seg 0 to execute next
 * Does: duplicates segment stored at register rb, sets segment 0 to the newly
        duplicated segment. It updates counter to point to the value in rc
 * Returns: nothing
*/
void load_program(uint32_t* instruction, uint32_t* registers, um_seq seg_mem,
                  int* counter)
{
    assert(instruction);
    assert(registers);
    assert(seg_mem);
    assert(counter);
    uint32_t rb = instruction[2];
    uint32_t rc = instruction[3];
    if(registers[rb] == 0) {
        *counter = (int) registers[rc];
    }
    else {
        segment_set_start(seg_mem, (int) registers[rb]);
        *counter = (int) registers[rc];
    }
}

/*
 * Name: load_val
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register
 * Does: loads the value stored in instruction[3] into register ra.
 * Returns: nothing
*/
void load_val(uint32_t * instruction, uint32_t * registers)
{
    assert(instruction);
    assert(registers);
    uint32_t ra = instruction[1];
    uint32_t val = instruction[3];
    registers[ra] = val;

}

/*
 * Name: execute
 * Parameters: instruction which is an integer array that holds the opcode
                and registers to use. registers which is a uint array that
                holds the data associated with each register. seg_mem which
                is a struct that holds our segmented memory. counter which
                stores which element in seg 0 to execute next
 * Does: calls the correct function by analyzing the opcode stored in
        instruction 0. It passes the function all the information it needs.
 * Returns: nothing
*/
void execute(um_seq seg_mem, uint32_t* instruction, uint32_t* registers,
             int* counter)
{
    assert(seg_mem);
    assert(instruction);
    assert(registers);
    assert(counter);

    switch (instruction[0]) {
        case 0: conditional_move(instruction, registers);
            break;
        case 1: seg_load(instruction, registers, seg_mem);
            break;
        case 2: seg_store(instruction, registers, seg_mem);
            break;
        case 3: add(instruction, registers);
            break;
        case 4: multiply(instruction, registers);
            break;
        case 5: divide(instruction, registers);
            break;
        case 6: nand(instruction, registers);
            break;
        case 7: halt(instruction, registers, seg_mem, counter);
            break;
        case 8: map_seg(instruction, registers, seg_mem);
            break;
        case 9: unmap_seg(instruction, registers, seg_mem);
            break;
        case 10: output(instruction, registers);
            break;
        case 11: input(instruction, registers);
            break;
        case 12: load_program(instruction, registers, seg_mem, counter);
            break;
        case 13: load_val(instruction, registers);
            break;
        default: exit(EXIT_FAILURE);
            break;
    }
}

void run_prog(Seq_T seg_zero)
{
    um_seq seg_mem = initialize(seg_zero);
    int* counter = malloc(sizeof(int));
    assert(counter);
    *counter = 0;
    uint32_t * registers = calloc(1, sizeof(uint32_t)*8);
    assert(registers);
    
    uint32_t * instruction = malloc(sizeof(uint32_t)*4);
    
    for(int i = 0; i < 4; i++){
        instruction[i] = 0;
    }
    assert(instruction);
    while(*counter < get_seg_length(seg_mem,0)) {
        instruction = parse((uint32_t)segment_get_val(seg_mem, 0,
                                                                 *counter),
                                                                 instruction);
        *counter = *counter + 1;
        execute(seg_mem, instruction, registers, counter);
    }
    free(instruction);
    free_prog(seg_mem, registers, counter);
}
