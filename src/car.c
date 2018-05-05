#include "car.h"
#include "wrio.h"
#include "pwm.h"
#include "led.h"

/* 速度1档, 30% */
#define SPEED_GROUP_1       (PWM_MAX_VALUE * 30 / 100)

/* 速度2档, 60% */
#define SPEED_GROUP_2       (PWM_MAX_VALUE * 60 / 100)

/* 速度3档, 90% */
#define SPEED_GROUP_3       (PWM_MAX_VALUE * 90 / 100)

/* 转弯后增速, 0.2% */
#define SPEED_TURN_ADJUST   (PWM_MAX_VALUE * 2 / 1000)

/* 转弯模式, 0 = 大转弯, 1 = 飘移转弯 */
static int turn_mode;

/* 镜像模式启用, 此值=1, 前进变后退, 后退变前进 */
static int mirror_mode;

static uint16_t speed;

static uint16_t cspeed;

/**
 * @brief 线性转换.
 *
 * @param value 输入值.
 * @param in_min 输入下限, 必须 <= in_max
 * @param in_max 输入上限, 必须 >= in_min
 * @param outA 输出范围A
 * @param outB 输出范围B
 *
 * outB > outA, 输出线性从小到大
 * outA > outB, 输出线性从大到小
 *
 */
static uint16_t linear(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t outA, uint16_t outB) {
    if (value <= in_min) return outA;
    if (value >= in_max) return outB;
    if (outA == outB) return outA;

    int32_t result = value;
    result -= in_min;

    if (outB > outA) {
        result *= (outB - outA);
        result /= (in_max - in_min);
        result += outA;
    }
    else {
        result *= (outA - outB);
        result /= (in_max - in_min);
        result = outA - result;
    }

    return (uint16_t) result;
}

void car_init(void) {
    turn_mode = 0;
    mirror_mode = 0;
    speed = 300;
    cspeed = speed;
}

void car_switch_speed(void) {
    if (speed >= SPEED_GROUP_3) {
        speed = SPEED_GROUP_1;
        beep(1);
    }
    else if (speed >= SPEED_GROUP_2) {
        speed = SPEED_GROUP_3;
        beep(3);
    }
    else {
        speed = SPEED_GROUP_2;
        beep(2);
    }

    cspeed = speed;
}

void car_switch_turn(void) {
    switch (turn_mode) {
        case 0:
            turn_mode = 1;
            beep(2);
            break;
        default:
            turn_mode = 0;
            beep(1);
            break;
    }
}

void car_switch_mirror(void) {
    switch (mirror_mode) {
        case 0:
            mirror_mode = 1;
            beep(2);
            break;
        default:
            mirror_mode = 0;
            beep(1);
            break;
    }
}

static void LMotor_forward(void) {
    wrio_hig(WRIO_AN1);
    wrio_low(WRIO_AN2);
}

static void LMotor_backward(void) {
    wrio_hig(WRIO_AN2);
    wrio_low(WRIO_AN1);
}

static void LMotor_nothing(void) {
    wrio_low(WRIO_AN1);
    wrio_low(WRIO_AN2);
}

static void LMotor_brake(void) {
    wrio_hig(WRIO_AN1);
    wrio_hig(WRIO_AN2);
}

static void RMotor_forward(void) {
    wrio_hig(WRIO_BN1);
    wrio_low(WRIO_BN2);
}

static void RMotor_backward(void) {
    wrio_hig(WRIO_BN2);
    wrio_low(WRIO_BN1);
}

static void RMotor_brake(void) {
    wrio_hig(WRIO_BN1);
    wrio_hig(WRIO_BN2);
}

static void RMotor_nothing(void) {
    wrio_low(WRIO_BN1);
    wrio_low(WRIO_BN2);
}

/**
 * @brief 转弯后慢慢加速.
 */
static void adjust_speed(void) {
    unsigned int new_speed = (unsigned int) (cspeed + SPEED_TURN_ADJUST);
    cspeed = (uint16_t)(new_speed > speed ? speed : new_speed);
}

static void internal_forward(void) {
    LMotor_forward();
    RMotor_forward();
    adjust_speed();
    pwm_l(cspeed);
    pwm_r(cspeed);
}

static void internal_backward(void) {
    LMotor_backward();
    RMotor_backward();
    adjust_speed();
    pwm_l(cspeed);
    pwm_r(cspeed);
}

void car_forward(void) {
    if (1 == mirror_mode) {
        internal_backward();
    }
    else {
        internal_forward();
    }
}

void car_backward(void) {
    if (1 == mirror_mode) {
        internal_forward();
    }
    else {
        internal_backward();
    }
}

/**
 * @brief 计算转弯速度.
 *
 * 转弯速度要比正常速度慢一点.
 *
 * 如果速度高, 转弯的时候减的速度也多一点，最多减去30%
 */
static uint16_t turn_speed(uint16_t sd) {
    /* 30% ~ 90% 的速度, 转成 sd * 100% - sd * 70% */
    return linear(sd, SPEED_GROUP_1, SPEED_GROUP_3, sd, (uint16_t)((uint32_t) sd * 70u / 100u));
}

void car_turnleft(void) {
    cspeed = turn_speed(speed);

    if (1 == turn_mode) {                                   /* 飘移 */
        /* 原地打转 */
        LMotor_backward();
        RMotor_forward();
        pwm_l(cspeed);
        pwm_r(cspeed);
    }
    else {                                                  /* 正常转弯 */
        /* 总体保持前进, 小转弯 */
        if (1 == mirror_mode) {                             /* 镜像模式 */
            LMotor_backward();
            RMotor_backward();
            pwm_l(cspeed);
            pwm_r((uint16_t)(cspeed / 4));
        }
        else {                                              /* 非镜像模式 */
            LMotor_forward();
            RMotor_forward();
            pwm_l((uint16_t)(cspeed / 4));
            pwm_r(cspeed);
        }
    }
}

void car_turnright(void) {
    cspeed = turn_speed(speed);

    if (1 == turn_mode) {                                   /* 飘移 */
        /* 原地打转 */
        LMotor_forward();
        RMotor_backward();
        pwm_l(cspeed);
        pwm_r(cspeed);
    }
    else {                                                  /* 正常转弯 */
        /* 总体保持前进, 小转弯 */
        if (1 == mirror_mode) {                             /* 镜像模式 */
            LMotor_backward();
            RMotor_backward();
            pwm_l((uint16_t)(cspeed / 4));
            pwm_r(cspeed);
        }
        else {                                              /* 非镜像模式 */
            LMotor_forward();
            RMotor_forward();
            pwm_l(cspeed);
            pwm_r((uint16_t)(cspeed / 4));
        }
    }
}

void car_nothing(void) {
    LMotor_nothing();
    RMotor_nothing();
    pwm_l(0);
    pwm_r(0);
}

void car_brake(void) {
    LMotor_brake();
    RMotor_brake();
    pwm_l(PWM_MAX_VALUE);
    pwm_r(PWM_MAX_VALUE);
}


