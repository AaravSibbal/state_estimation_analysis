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
    }else{
        zoro_print_header(&header_buffer);
    }



    fclose(fptr);
    return 0;

}