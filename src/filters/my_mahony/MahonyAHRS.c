//=====================================================================================================
// MahonyAHRS.c
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

//---------------------------------------------------------------------------------------------------
// Header files

#include "MahonyAHRS.h"
#include <math.h>

//---------------------------------------------------------------------------------------------------
// Definitions

#define sampleFreq	512.0f			// sample frequency in Hz
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

//---------------------------------------------------------------------------------------------------
// Variable definitions

// volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
// volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
// volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
// volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki

//---------------------------------------------------------------------------------------------------
// Function declarations

float invSqrt(float x);

//====================================================================================================
// Functions


void mahony_init(mahony_t *self, float Kp, float Ki, float sample_freq){
	self->twoKp = 2.0f*Kp;
	self->twoKi = 2.0f*Ki;
	self->sample_freq = sample_freq;
	quat_set_to_identity(self->est); // sets the quat to [1, 0, 0, 0]
	self->integralFBx = 0;
	self->integralFBy = 0;
	self->integralFBz = 0;
}

//---------------------------------------------------------------------------------------------------
// AHRS algorithm update

void MahonyAHRSupdate(mahony_t *self, vec3_t *gyro, vec3_t *accel, vec3_t *mag){
	float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	float q0, q1, q2, q3;
	
	q0 = self->est->w;
	q1 = self->est->x;
	q2 = self->est->y;
	q3 = self->est->y;

	if(vec3_is_zero(mag)) {
		MahonyAHRSupdateIMU(self, gyro, accel);
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!vec3_is_zero(accel)) {

		// Normalise accelerometer measurement
		vec3_normalize(accel);
		
		// Normalise magnetometer measurement
		vec3_normalize(mag);

        // Auxiliary variables to avoid repeated arithmetic
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;   

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mag->x * (0.5f - q2q2 - q3q3) + mag->y * (q1q2 - q0q3) + mag->z * (q1q3 + q0q2));
        hy = 2.0f * (mag->x * (q1q2 + q0q3) + mag->y * (0.5f - q1q1 - q3q3) + mag->z * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mag->x * (q1q3 - q0q2) + mag->y * (q2q3 + q0q1) + mag->z * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  
	
		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (accel->y * halfvz - accel->z * halfvy) + (mag->y * halfwz - mag->z * halfwy);
		halfey = (accel->z * halfvx - accel->x * halfvz) + (mag->z * halfwx - mag->x * halfwz);
		halfez = (accel->x * halfvy - accel->y * halfvx) + (mag->x * halfwy - mag->y * halfwx);

		// Compute and apply integral feedback if enabled
		if(self->twoKi > 0.0f) {
			self->integralFBx += self->twoKi * halfex * (1.0f / self->sample_freq);	// integral error scaled by Ki
			self->integralFBy += self->twoKi * halfey * (1.0f / self->sample_freq);
			self->integralFBz += self->twoKi * halfez * (1.0f / sampleFreq);
			gyro->x += self->integralFBx;	// apply integral feedback
			gyro->y += self->integralFBy;
			gyro->z += self->integralFBz;
		}
		else {
			self->integralFBx = 0.0f;	// prevent integral windup
			self->integralFBy = 0.0f;
			self->integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gyro->x += self->twoKp * halfex;
		gyro->y += self->twoKp * halfey;
		gyro->z += self->twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gyro->x *= (0.5f * (1.0f / self->sample_freq));		// pre-multiply common factors
	gyro->y *= (0.5f * (1.0f / self->sample_freq));
	gyro->z *= (0.5f * (1.0f / self->sample_freq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gyro->x - qc * gyro->y - q3 * gyro->z);
	q1 += (qa * gyro->x + qc * gyro->z - q3 * gyro->y);
	q2 += (qa * gyro->y - qb * gyro->z + q3 * gyro->x);
	q3 += (qa * gyro->z + qb * gyro->y - qc * gyro->x); 
	
	// Normalise quaternion
	self->est->w = q0;
	self->est->x = q1;
	self->est->y = q2;
	self->est->z = q3;

	quat_normalize(self->est);
}
//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MahonyAHRSupdateIMU(mahony_t *self, vec3_t *gyro, vec3_t *accel) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	float q0, q1, q2, q3;

	q0 = self->est->w;
	q1 = self->est->x;
	q2 = self->est->y;
	q3 = self->est->y;


	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!vec3_is_zero(accel)) {

		// Normalise accelerometer measurement
		vec3_normalize(accel);

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
	
		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (accel->y * halfvz - accel->z * halfvy);
		halfey = (accel->z * halfvx - accel->x * halfvz);
		halfez = (accel->x * halfvy - accel->y * halfvx);

		// Compute and apply integral feedback if enabled
		if(self->twoKi > 0.0f) {
			self->integralFBx += self->twoKi * halfex * (1.0f / self->sample_freq);	// integral error scaled by Ki
			self->integralFBy += self->twoKi * halfey * (1.0f / self->sample_freq);
			self->integralFBz += self->twoKi * halfez * (1.0f / self->sample_freq);
			gyro->x += self->integralFBx;	// apply integral feedback
			gyro->y += self->integralFBy;
			gyro->z += self->integralFBz;
		}
		else {
			self->integralFBx = 0.0f;	// prevent integral windup
			self->integralFBy = 0.0f;
			self->integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gyro->x += self->twoKp * halfex;
		gyro->y += self->twoKp * halfey;
		gyro->z += self->twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gyro->x *= (0.5f * (1.0f / self->sample_freq));		// pre-multiply common factors
	gyro->y *= (0.5f * (1.0f / self->sample_freq));
	gyro->z *= (0.5f * (1.0f / self->sample_freq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gyro->x - qc * gyro->y - q3 * gyro->z);
	q1 += (qa * gyro->x + qc * gyro->z - q3 * gyro->y);
	q2 += (qa * gyro->y - qb * gyro->z + q3 * gyro->x);
	q3 += (qa * gyro->z + qb * gyro->y - qc * gyro->x); 
	
	// Normalise quaternion
	self->est->w = q0;
	self->est->x = q1;
	self->est->y = q2;
	self->est->z = q3;

	quat_normalize(self->est);
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
	return 1/sqrtf(x);
}

//====================================================================================================
// END OF CODE
//====================================================================================================
