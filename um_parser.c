/* Daniel Alderman and Philip Miljanic
    Comp 40, HW6 - UM
    11/15/2019
    um_parser.c
    This is the implementation for um_parser.c which handles parsing words.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitpack.h"
#include "fmt.h"
#include "assert.h"


uint32_t* parse(uint32_t word){

    uint32_t * ret_val = malloc(sizeof(uint32_t)*4);
    assert(ret_val);
    uint32_t opcode = Bitpack_getu(word, 4, 28);
    uint32_t c = Bitpack_getu(word, 3, 0);
    uint32_t b = Bitpack_getu(word, 3, 3);
    uint32_t a = Bitpack_getu(word, 3, 6);
    uint32_t val = Bitpack_getu(word, 25, 0);
    uint32_t val_reg = Bitpack_getu(word, 3, 25);

    for(int i = 0; i < 4; i++) {
        ret_val[i] = 0;
    }
    ret_val[0] = opcode;
    /* halt */
    if (opcode == 7) {
        return ret_val;
    }
    /* input/output/unmap_segment/*/
    else if (opcode > 8 && opcode < 12) {
        ret_val[3] = c;
    }
    /* Load Program & map */
    else if (opcode == 12 || opcode == 8) {
        ret_val[3] = c;
        ret_val[2] = b;
    }
    /* Load Val */
    else if (opcode == 13) {
        ret_val[1] = val_reg;
        ret_val[3] = val;
    }
    else {
        ret_val[1] = a;
        ret_val[2] = b;
        ret_val[3] = c;
    }
    return ret_val;
}
