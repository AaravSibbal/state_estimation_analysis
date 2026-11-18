#ifndef QUATERNION_H
#define QUATERNION_H

#include <assert.h>
#include <cmath>
#include <math.h>
typedef struct{
    float w;
    float x;
    float y;
    float z;
}Quaternion_t;


// hamilton multiplication i.e. q1 * q2 and not q2*q1
static inline Quaternion_t quat_multiply(Quaternion_t *q1, 
Quaternion_t* q2){
    Quaternion_t result;
    result.w = (q1->w*q2->w)-(q1->x*q2->x)-(q1->y*q2->y)-(q1->z*q2->z);
    result.x = (q1->w*q2->x)+(q1->x*q2->w)+(q1->y*q2->z)-(q1->z*q2->y);
    result.y = (q1->w*q2->y)-(q1->x*q2->z)+(q1->y*q2->w)+(q1->z*q2->x);
    result.z = (q1->w*q2->z)+(q1->x*q2->y)-(q1->y*q2->x)+(q1->z*q2->w);
    return result;
}

static inline Quaternion_t quat_inverse(Quaternion_t *q){
    Quaternion_t result;
    float q_sqr = 1/(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    assert(q_sqr != 0);
    float q_sqr_inverse = 1/q_sqr;
    result.w = q->w*q_sqr_inverse;
    result.x = (-1*q->x)*q_sqr_inverse;
    result.y = (-1*q->y)*q_sqr_inverse;
    result.z = (-1*q->z)*q_sqr_inverse;

    return result;
}

static inline float quat_calc_angle_error(Quaternion_t * reference, 
Quaternion_t *estimate){
    Quaternion_t result;
    result = quat_inverse(reference);
    result = quat_multiply(&result, estimate);
    float error = 2.0f*atan2f(
        sqrtf(result.x*result.x + result.y*result.y + result.z*result.z), 
        fabsf(result.w)
    );

    return error;   
}

#endif