#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>

typedef struct{
    float x;
    float y;
    float z;
}vec3_t;

bool vec3_is_zero(vec3_t *self);
void vec3_normalize(vec3_t *self);

#endif