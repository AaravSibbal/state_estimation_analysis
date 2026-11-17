#ifndef ZORO_H
#define ZORO_H

#include <stdint.h>

typedef struct {
    char magic[4];
    uint8_t version;
    uint32_t n_samples;
    double sample_rate_hz;
}zoro_header_t;

typedef struct{
    double accel[3];
    double gyro[3];
    double mag[3];
    double ref_quat[4];
}zoro_record_t;

typedef enum{
    ZR_NO_ERROR,
    ZR_WRONG_MAGIC,
    ZR_NULL_HEADER,
    ZR_WRONG_VERSION
}zoro_error_t;

zoro_error_t zoro_verify_header(zoro_header_t *header);
void zoro_print_header(zoro_header_t *header);
void zoro_print_error(zoro_error_t err);

#endif