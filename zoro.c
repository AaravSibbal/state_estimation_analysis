#include "zoro.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static const char MAGIC[5] = "ZORO";
static const uint8_t VERSION = 1;

zoro_error_t zoro_verify_header(zoro_header_t *header){
    if(header == NULL){
        return ZR_NULL_HEADER;
    }

    int is_equal = memcmp(
        header->magic, 
        MAGIC,
        sizeof(char)*4
    );

    if(is_equal != 0){
        return ZR_WRONG_MAGIC;
    }

    if(header->version != VERSION){
        return ZR_WRONG_VERSION;
    }


    return ZR_NO_ERROR;
}

void zoro_print_header(zoro_header_t *header){
    printf("Zoro Header:------------------\n");
    printf("Magic: %s\n", header->magic);
    printf("Version: %u\n", header->version);
    printf("Samples: %u\n",header->n_samples);
    printf("Sample Rate: %f\n", header->sample_rate_hz);
    printf("------------------------------\n");
}

/**

typedef enum{
    ZR_NO_ERROR,
    ZR_WRONG_MAGIC,
    ZR_NULL_HEADER,
    ZR_WRONG_VERSION
}zoro_error_t;
*/
void zoro_print_error(zoro_error_t err){
    switch (err) {
        case ZR_NO_ERROR: // we don't print when no error
            break;
        case ZR_WRONG_MAGIC:
            printf("MAGIC value for the header was wrong");
            break;
        case ZR_NULL_HEADER:
            printf("Header given was a null value");
            break;
        case ZR_WRONG_VERSION:
            printf("Wrong version, this is version 1");
            break;
        default:
            printf("wrong error value");
        }
}