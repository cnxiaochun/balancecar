#pragma once

#include <stdint.h>

/* PID精度 */
#define PID_REF_RANGE_INT       (100)
#define PID_REF_RANGE           ((float) PID_REF_RANGE_INT)

typedef struct {
                  float  Ref;           // Input: Reference input 
                  float  Fdb;           // Input: Feedback input 
                  float  Err;           // Variable: Error 
                  float  Kp;            // Parameter: Proportional gain
                  float  Up;            // Variable: Proportional output 
                  float  Ui;            // Variable: Integral output 
                  float  Ud;            // Variable: Derivative output    
                  float  OutPreSat;     // Variable: Pre-saturated output
                  float  OutMax;        // Parameter: Maximum output 
                  float  OutMin;        // Parameter: Minimum output
                  float  Out;           // Output: PID output 
                  float  SatErr;        // Variable: Saturated difference
                  float  Ki;            // Parameter: Integral gain
                  float  Kc;            // Parameter: Integral correction gain
                  float  Kd;            // Parameter: Derivative gain 
                  float  Up1;           // History: Previous proportional output
} PIDREG;     

void pid_init(PIDREG * v, uint16_t period, uint16_t Kp, uint16_t Ti, uint16_t Td);

void pid_calc(PIDREG *v, uint8_t reverse);

