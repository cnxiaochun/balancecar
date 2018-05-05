#include <stdint.h>
#include <string.h>
#include "pid.h"

/**
 * @brief PID初始化.
 *
 * @param v PID参数.
 * @param period PID采样周期, 单位ms.
 *
 * @param Kp 比例带, 万分之几, 1 ~ 9999
 * @param Ti 积分时间, 秒
 * @param Td 微分时间, 秒
 *
 * @return None
 */
void pid_init(PIDREG * v, uint16_t period, uint16_t Kp, uint16_t Ti, uint16_t Td) {
    float T;
    float f;

    memset(v, 0, sizeof(PIDREG));

    T = (float) period;
    v->Ref = PID_REF_RANGE;

    /* Calc Kp */
    f = T;
    f /= (float) 1000;                                  /* 秒为单位 */
    f *= (float) Kp;
    f /= (float) 10000;                                 /* 万分之几 */
    v->Kp = f;

    /* Calc Ki */
    if (Ti == 0) {
        v->Ki = (float) 0;
    }
    else {
        f = (float) Ti;
        f *= (float) 1000;                              /* 秒转成ms */
        f = T / f;                                      /* Ki = T / Ti */
        v->Ki = f;
    }

    /* Calc Kd */
    if (Td == 0) {
        v->Kd = (float) 0;
    }
    else {
        f= (float) Td;
        f *= (float) 1000;                              /* 秒转成ms */
        f /= T;                                         /* Kd = Td / T */
        v->Kd = f;
    }

    /* 最大增减速, 5% */
    v->OutMax = (float) (PID_REF_RANGE * 5 / 100);
    v->OutMin = -(float) (PID_REF_RANGE * 5 / 100);
}

/**
 * @brief PID计算.
 *
 * @param v PID参数
 * @param reverse 是否反向控制
 */
void pid_calc(PIDREG * v, uint8_t reverse) {
    // Compute the error
    if (reverse) {                                      /* Reverse control */
        v->Err = v->Fdb - v->Ref;
    }
    else {
        v->Err = v->Ref - v->Fdb;
    }
    
    // Compute the proportional output
    v->Up = v->Kp * v->Err;

    // Compute the integral output
    v->Ui += v->Ki * v->Up + v->Kc * v->SatErr;

    // Compute the derivative output
    v->Ud = v->Kd * (v->Up - v->Up1);

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;     
    
    // Saturate the output
    if (v->OutPreSat > v->OutMax)                   
        v->Out = v->OutMax;
    else if (v->OutPreSat < v->OutMin)
        v->Out = v->OutMin;  
    else
        v->Out = v->OutPreSat;                   
    
    // Compute the saturate difference
    v->SatErr = v->Out - v->OutPreSat;     
    
    // Update the previous proportional output 
    v->Up1 = v->Up; 
}

