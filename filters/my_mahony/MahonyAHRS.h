//=====================================================================================================
// MahonyAHRS.h
//=====================================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h

//----------------------------------------------------------------------------------------------------
// Variable declaration

#include "../../quaternion.h"
#include "../../vec3.h"

typedef struct{
    float twoKp; // parameters set on init
    float twoKi;
    float sample_freq;
    quaternion_t *est; // state variables changes with each update
    float integralFBx;
    float integralFBy;
    float integralFBz;
}mahony_t;


// extern volatile float twoKp;			// 2 * proportional gain (Kp)
// extern volatile float twoKi;			// 2 * integral gain (Ki)
// extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame

//---------------------------------------------------------------------------------------------------
// Function declarations

void mahony_init(mahony_t *self, float Kp, float Ki, float sample_freq);
// defaults: Kp: 0.5f, Ki: 0.0f, sample_freq: 512.0f 
void mahony_init_default(mahony_t *self);

void MahonyAHRSupdate(mahony_t *self, vec3_t *gyro, vec3_t *accel, vec3_t *mag);
void MahonyAHRSupdateIMU(mahony_t *self, vec3_t *gyro, vec3_t *accel);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
