#include "vec3.h"

#include <math.h>
#include <assert.h>

inline bool vec3_is_zero(vec3_t *self){
    return ((self->x == 0.0f) && (self->y == 0.0f) && (self->z == 0.0f));
}

inline void vec3_normalize(vec3_t *self){

    assert((self->x == 0.0f) && (self->y == 0.0f) && (self->z == 0.0f));
    static float recipNorm = 0.0f;
    recipNorm = sqrtf(1.0f/(self->x*self->x + self->y*self->y + self->z*self->z));
    self->x = self->x*recipNorm;
    self->y = self->y*recipNorm;
    self->z = self->z*recipNorm; 
}