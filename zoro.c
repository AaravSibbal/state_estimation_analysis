#include "zoro.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static const char MAGIC[5] = "ZORO";
static const uint8_t VERSION = 1;

static zoro_error_t zoro_read_from_file(void *buffer, FILE *file_ptr, size_t buffer_size){
    assert(buffer != NULL);
    assert(file_ptr != NULL);

    size_t read_amount = fread(
        buffer,
        sizeof(uint8_t),
        buffer_size,
        file_ptr
    );

    if(read_amount == buffer_size){
        return ZR_NO_ERROR;
    }

    if(ferror(file_ptr) != 0){
        return ZR_IO_ERROR;
    }
    if(feof(file_ptr) != 0){
        return ZR_EOF;
    }

    return ZR_UNEXPECTED_SHORT_READ;

}

zoro_error_t zoro_pupolate_header(zoro_header_t *header_buffer, FILE *file_ptr){
    assert(header_buffer != NULL);
    assert(file_ptr != NULL);
    zoro_error_t err = zoro_read_from_file(
        header_buffer->magic, 
        file_ptr, 
        sizeof(header_buffer->magic)
    );

    if(err != ZR_NO_ERROR){
        return err;
    }

    err = zoro_read_from_file(
        &header_buffer->version, 
        file_ptr, 
        sizeof(header_buffer->version)
    );

    if(err != ZR_NO_ERROR){
        return err;
    }

    err = zoro_read_from_file(
        &header_buffer->n_samples, 
        file_ptr, 
        sizeof(header_buffer->n_samples)
    );

    if(err != ZR_NO_ERROR){
        return err;
    }

    err = zoro_read_from_file(
        &header_buffer->sample_rate_hz, 
        file_ptr, 
        sizeof(header_buffer->sample_rate_hz)
    );

    return err;
}


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
    fprintf(stderr, "Zoro Header:------------------\n");
    fprintf(stderr, "Magic: %s\n", header->magic);
    fprintf(stderr, "Version: %u\n", header->version);
    fprintf(stderr, "Samples: %u\n",header->n_samples);
    fprintf(stderr, "Sample Rate: %f\n", header->sample_rate_hz);
    fprintf(stderr, "------------------------------\n");
}

void zoro_print_error(zoro_error_t err){
    switch (err) {
        case ZR_NO_ERROR: // we don't print when no error
            break;
        case ZR_WRONG_MAGIC:
            fprintf(stderr, "zoro: wrong magic, not a .zoro file\n");
            break;
        case ZR_NULL_HEADER:
            fprintf(stderr, "zoro: header pointer was NULL\n");
            break;
        case ZR_WRONG_VERSION:
            fprintf(stderr, "zoro: unsupported version (expected 1)\n");
            break;
        case ZR_EOF:
            fprintf(stderr, "zoro: unexpected end of file (file truncated?)\n");
            break;
        case ZR_IO_ERROR:
            fprintf(stderr, "zoro: I/O error while reading file\n");
            break;
        case ZR_UNEXPECTED_SHORT_READ:
            fprintf(stderr, "zoro: short read with no EOF or error flag set\n");
            break;
        default:
            fprintf(stderr, "zoro: unknown error code %d\n", (int)err);
            break;
    }
}