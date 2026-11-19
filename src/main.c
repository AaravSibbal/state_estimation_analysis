#include <stdio.h>
#include "zoro.h"

int main(void){
    printf("something\n");

    FILE *fptr = fopen("data/01_undisturbed_slow_rotation_A(2).zoro", "rb");
    if(fptr == NULL){
        printf("Could not open file\n");
        return 1;
    }

    zoro_header_t header_buffer;

    zoro_error_t err = zoro_pupolate_header(&header_buffer, fptr);
    if(err != ZR_NO_ERROR){
        zoro_print_error(err);
        fclose(fptr);
        return 1;
    }

    zoro_error_t zr_err = zoro_verify_header(&header_buffer);
    if(zr_err != ZR_NO_ERROR){
        zoro_print_error(zr_err);
        zoro_print_header(&header_buffer);
        fclose(fptr);
        return 1;
    }else{
        zoro_print_header(&header_buffer);
    }

    // zoro_record_t re`cord_buffer;

    // for(uint32_t i=0; i<header_buffer.n_samples; i++){
    //     zr_err = zoro_populate_record(&record_buffer, fptr);
    //     if(zr_err == ZR_NO_ERROR){
    //         fprintf(stderr, "%d)",i);
    //         zoro_print_record(&record_buffer);
    //     }
    //     else{
    //         zoro_print_error(err);
    //         zoro_print_record(&record_buffer);
    //         fclose(fptr);
    //         return 1;
    //     }
    // }

    fclose(fptr);
    return 0;

}