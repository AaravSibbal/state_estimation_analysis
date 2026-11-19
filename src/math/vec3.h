#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef struct{
    float x;
    float y;
    float z;
}vec3_t;


static inline bool vec3_is_zero(vec3_t *self){
    return ((self->x == 0.0f) && (self->y == 0.0f) && (self->z == 0.0f));
}

static inline void vec3_normalize(vec3_t *self){

    assert((self->x == 0.0f) && (self->y == 0.0f) && (self->z == 0.0f));
    static float recipNorm = 0.0f;
    recipNorm = sqrtf(1.0f/(self->x*self->x + self->y*self->y + self->z*self->z));
    self->x = self->x*recipNorm;
    self->y = self->y*recipNorm;
    self->z = self->z*recipNorm; 
}
#endif