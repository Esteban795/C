#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "bitpacking.h"
#include "stack.h"

typedef uint32_t cw_t;
typedef uint8_t byte_t;

#define CW_MAX_WIDTH 16
#define DICTFULL (1u << CW_MAX_WIDTH)
#define BYTE_MASK  ((1 << BYTE_WIDTH) - 1)


const cw_t NO_ENTRY = DICTFULL;
const cw_t NULL_CW = DICTFULL;
const cw_t FIRST_CW = 0x100;
const int CW_MIN_WIDTH = 9;
const int BUFFER_WIDTH = 64;
const int BYTE_WIDTH = 8;


int VERBOSITY = 0;

struct dict_entry_t {
    cw_t pointer;
    uint8_t byte;
};

typedef struct dict_entry_t dict_entry_t;

struct dict_t {
    cw_t next_available_cw;
    int cw_width;
    dict_entry_t data[DICTFULL];
};

struct dict_t dict;

cw_t inverse_table[DICTFULL][256];


void initialize_dictionary(void){
    dict.next_available_cw = FIRST_CW;
    dict.cw_width = CW_MIN_WIDTH;
    for (cw_t i = 0; i < FIRST_CW;i++){
        dict.data[i].pointer = NULL_CW;
        dict.data[i].byte = i;
    }
}

cw_t lookup(cw_t cw,byte_t byte){
    cw_t code = inverse_table[cw][byte];
    if (code >= dict.next_available_cw) return NO_ENTRY;
    dict_entry_t d_entry = dict.data[code];
    if (d_entry.pointer == cw && d_entry.byte == byte) return code;
    return NO_ENTRY;
}

void build_entry(cw_t cw, byte_t byte){
    cw_t next = dict.next_available_cw;
    if (next == DICTFULL) return;
    dict.data[next].pointer = cw;
    dict.data[next].byte = byte;
    inverse_table[cw][byte] = next;
    dict.next_available_cw++;
}


void mock_compress(FILE* in_file,FILE* out_file){
    cw_t current_cw = NULL_CW;
    int byte = getc(in_file);
    while (byte != EOF){
        if (current_cw == NULL_CW){
            current_cw = byte;
        }
        else {
            cw_t new_cw = lookup(current_cw,(uint8_t) byte);
            if (new_cw == NO_ENTRY) {
                fprintf(out_file,"%d ",current_cw);
                build_entry(current_cw,(uint8_t) byte);
                current_cw = (cw_t) byte;
            } else {
                current_cw = new_cw;
            }
        }
        byte = getc(in_file);
    }
    fprintf(out_file,"%d",current_cw);
}


void compress(void){}
void decompress_binary(void){}
void decompress_text(void){}

typedef struct stack stack;

byte_t decode_cw(FILE* fp, cw_t cw, stack* s){
    assert(cw < dict.next_available_cw);
    dict_entry_t d_entry = dict.data[cw];
    while (d_entry.pointer != NULL_CW){
        stack_push(s,d_entry.byte);
        d_entry = dict.data[d_entry.pointer];
    }
    stack_push(s,d_entry.byte);
    while (stack_size(s) > 0){
        putc(stack_pop(s),fp);
    }
    return d_entry.byte;
}

byte_t get_first_byte(cw_t cw){
    dict_entry_t d_entry = dict.data[cw];
    while (d_entry.pointer != NULL_CW){
        d_entry = dict.data[d_entry.pointer];
    }
    return d_entry.byte;
}

void mock_decompress(FILE* in_file,FILE* out_file){
    stack* s = stack_new(DICTFULL);
    cw_t previous_cw;
    cw_t current_cw;
    if (fscanf(in_file,"%d",&previous_cw) == 1){
        decode_cw(out_file,previous_cw,s);
    }
    while (fscanf(in_file,"%d",&current_cw) == 1){
        if (current_cw < dict.next_available_cw){ //cas facile
            byte_t byte = decode_cw(out_file,current_cw,s);
            build_entry(previous_cw,byte);
        } else { //cas qui nous emmerde
            byte_t byte = get_first_byte(previous_cw);
            build_entry(previous_cw,byte);
            decode_cw(out_file,current_cw,s);
        }
        previous_cw = current_cw;
    }
    stack_free(s);
}

void output_bits(bit_file* bf,uint64_t data,int width,bool flush){
    data &= (1 << width) - 1;
    bf->buffer |= (data << bf->buffer_length);
    bf->buffer_length += width;
    while (bf->buffer_length >= BYTE_WIDTH){
        byte_t to_write = bf->buffer & BYTE_MASK;
        putc(to_write,bf->fp);
        bf->buffer >>= BYTE_WIDTH;
        bf->buffer_length -= BYTE_WIDTH;
    }
    if (bf->buffer_length > 0 && flush){
        byte_t to_write = bf->buffer & BYTE_MASK;
        putc(to_write,bf->fp);
        bf->buffer = 0;
        bf->buffer_length = 0;
    }

}

uint64_t input_bits(bit_file* bf,int width,bool *eof){
    int byte = getc(bf->fp);
    int offset = bf->buffer_length;
    while (byte != EOF && bf->buffer_length - offset < width){
        bf->buffer |= (byte & BYTE_MASK) << offset;
        bf->buffer_length += BYTE_WIDTH;
        byte = getc(bf->fp);
    }
    if (byte == EOF){
        *eof = true;
        return 0;
    }


}


int main(int argc, char* argv[]){
    FILE* in = stdin;
    FILE* out = stdout;
    if (argc < 2 || argc > 4 || strlen(argv[1]) != 1) return (EXIT_FAILURE);
    if (argc >= 3) in = fopen(argv[2],"rb");
    if (argc >= 4) out = fopen(argv[3],"wb");
    initialize_dictionary();
    char what_to_do = argv[1][0];
    if (what_to_do == 'c') compress();
    if (what_to_do == 'C') mock_compress(in,out);
    if (what_to_do == 'd') decompress_binary();
    if (what_to_do == 'D') mock_decompress(in,out);
    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}
