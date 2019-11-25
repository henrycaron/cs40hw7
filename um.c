#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <bitpack.h>
#include "seq.h"
#include "fmt.h"
#include <stdint.h>
#include "um_run.h"

#define HINT 1
 
int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Incompatable commandline args\n");
    }
    assert(argc == 2);

    FILE * file = fopen(argv[1], "r");
    if(file == NULL) {
        fprintf(stderr, "could not open file\n");
    }
    assert(file);
    Seq_T segment_zero = Seq_new(HINT);
    int curr_char = 0;
    /*populates segment 0 with words read in*/
    while(curr_char != EOF) {
        uint32_t * word = calloc(1, sizeof(uint32_t));
        assert(word);
        /* reads in byte by byte, packing each into word */
        for(int i = 0; i < 4; i++) {
            curr_char = getc(file);
            if(curr_char == EOF) {
                break;
            }
            *word = Bitpack_newu(*word, 8, 24-i*8, curr_char);
        }
        Seq_addhi(segment_zero, word);
    }
    if(file != NULL) {
        fclose(file);
    }
    run_prog(segment_zero);
    return 0;
}
