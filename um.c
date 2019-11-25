#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <bitpack.h>
#include "segment.h"
#include "seq.h"
#include "fmt.h"
#include <stdint.h>
#include "um_run.h"

#define HINT 100

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Incompatable commandline args\n");
    }
    assert(argc == 2);

    FILE * counter = fopen(argv[1], "r");
    FILE * program = fopen(argv[1], "r");
    if(program == NULL) {
        fprintf(stderr, "could not open file\n");
    }

    int num_bytes;

    while(fgetc(counter) != EOF){
        num_bytes++;
    }
    int num_words = num_bytes / 4;
    uint32_t *segment_zero = calloc(num_words, sizeof(uint32_t));
    int curr_char = 0;
    uint32_t word = 0;
    /*populates segment 0 with words read in*/
    for(int i = 0; i < num_words; i++){
        /* reads in byte by byte, packing each into word */
        for(int i = 0; i < 4; i++) {
            curr_char = getc(program);
            if(curr_char == EOF) {
                break;
            }
            word = Bitpack_newu(word, 8, 24-i*8, curr_char);
        }
        segment_zero[i] = word;

    }
    if(program != NULL) {
        fclose(program);
    }
    if(counter != NULL) {
        fclose(counter);
    }
    run_prog(segment_zero, num_words);
    return 0;
}
