#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdint.h>

typedef enum filters{
    MAHONY = 0,
    MADGWICK = 1
}filter_t;



typedef struct{
    uint32_t number;
    
}trial_t;


#endif