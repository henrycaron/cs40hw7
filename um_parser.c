/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_parser.c
    This is the implementation for um_parser.c which handles parsing words.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fmt.h"
#include "assert.h"

uint32_t get_instruction(uint32_t word, uint32_t width, uint32_t lsb)
{
    uint32_t mask = ~0;

    mask = mask >> (32 - width) << lsb;
    return (mask & word) >> lsb;

}

uint32_t *parse(uint32_t word, uint32_t *ret_val){


    for(int i = 0; i < 4; i++){
        ret_val[i] = 0;
    }

    ret_val[0] = get_instruction(word, 4, 28);
    /* halt */
    if (ret_val[0] == 7) {
        return ret_val;
    }
    /* input/output/unmap_segment/*/
    else if (ret_val[0] > 8 && ret_val[0] < 12) {
        ret_val[3] = get_instruction(word, 3, 0);
    }
    /* Load Program & map */
    else if (ret_val[0] == 12 || ret_val[0] == 8) {
        ret_val[3] = get_instruction(word, 3, 0);
        ret_val[2] = get_instruction(word, 3, 3);
    }
    /* Load Val */
    else if (ret_val[0] == 13) {
        ret_val[1] = get_instruction(word, 3, 25);
        ret_val[3] = get_instruction(word, 25, 0);
    }
    else {
        ret_val[1] = get_instruction(word, 3, 6);
        ret_val[2] = get_instruction(word, 3, 3);
        ret_val[3] = get_instruction(word, 3, 0);
    }
    return ret_val;
}
