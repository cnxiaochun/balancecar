#include "stimer.h"
#include "cmsis_device.h"

#define OS_STIMER_RUN_BIT   (0x80000000UL)

static uint32_t _OS_Stimers[OS_STIMERS];

void StimerRun(uint8_t stimer_id, uint32_t time) {
    _OS_Stimers[stimer_id] = -(time);
}

void StimerBreak(uint8_t stimer_id) {
    _OS_Stimers[stimer_id] = 0UL;
}

void StimerPause(uint8_t stimer_id) {
    _OS_Stimers[stimer_id] &= ~OS_STIMER_RUN_BIT;
}

void StimerContinue(uint8_t stimer_id) {
    _OS_Stimers[stimer_id] |= OS_STIMER_RUN_BIT;
}

int StimerCheck(uint8_t stimer_id) {
    return (_OS_Stimers[stimer_id] & OS_STIMER_RUN_BIT) ? 0 : 1;
}

int StimerIsRun(uint8_t stimer_id) {
    return (_OS_Stimers[stimer_id] & OS_STIMER_RUN_BIT) ? 1 : 0;
}

void stimer_task(void) {
    /* 保证所有修改静态定时器的任务优先级不能高于此任务优先级 */
    #if OS_STIMERS > 0
    if(_OS_Stimers[0] & OS_STIMER_RUN_BIT) _OS_Stimers[0]++;
    #endif
    #if OS_STIMERS > 1
    if(_OS_Stimers[1] & OS_STIMER_RUN_BIT) _OS_Stimers[1]++;
    #endif
    #if OS_STIMERS > 2
    if(_OS_Stimers[2] & OS_STIMER_RUN_BIT) _OS_Stimers[2]++;
    #endif
    #if OS_STIMERS > 3
    if(_OS_Stimers[3] & OS_STIMER_RUN_BIT) _OS_Stimers[3]++;
    #endif
    #if OS_STIMERS > 4
    if(_OS_Stimers[4] & OS_STIMER_RUN_BIT) _OS_Stimers[4]++;
    #endif
    #if OS_STIMERS > 5
    if(_OS_Stimers[5] & OS_STIMER_RUN_BIT) _OS_Stimers[5]++;
    #endif
    #if OS_STIMERS > 6
    if(_OS_Stimers[6] & OS_STIMER_RUN_BIT) _OS_Stimers[6]++;
    #endif
    #if OS_STIMERS > 7
    if(_OS_Stimers[7] & OS_STIMER_RUN_BIT) _OS_Stimers[7]++;
    #endif
    #if OS_STIMERS > 8
    if(_OS_Stimers[8] & OS_STIMER_RUN_BIT) _OS_Stimers[8]++;
    #endif
    #if OS_STIMERS > 9
    if(_OS_Stimers[9] & OS_STIMER_RUN_BIT) _OS_Stimers[9]++;
    #endif
    #if OS_STIMERS > 10
    if(_OS_Stimers[10] & OS_STIMER_RUN_BIT) _OS_Stimers[10]++;
    #endif
    #if OS_STIMERS > 11
    if(_OS_Stimers[11] & OS_STIMER_RUN_BIT) _OS_Stimers[11]++;
    #endif
    #if OS_STIMERS > 12
    if(_OS_Stimers[12] & OS_STIMER_RUN_BIT) _OS_Stimers[12]++;
    #endif
    #if OS_STIMERS > 13
    if(_OS_Stimers[13] & OS_STIMER_RUN_BIT) _OS_Stimers[13]++;
    #endif
    #if OS_STIMERS > 14
    if(_OS_Stimers[14] & OS_STIMER_RUN_BIT) _OS_Stimers[14]++;
    #endif
    #if OS_STIMERS > 15
    if(_OS_Stimers[15] & OS_STIMER_RUN_BIT) _OS_Stimers[15]++;
    #endif
    #if OS_STIMERS > 16
    if(_OS_Stimers[16] & OS_STIMER_RUN_BIT) _OS_Stimers[16]++;
    #endif
    #if OS_STIMERS > 17
    if(_OS_Stimers[17] & OS_STIMER_RUN_BIT) _OS_Stimers[17]++;
    #endif
    #if OS_STIMERS > 18
    if(_OS_Stimers[18] & OS_STIMER_RUN_BIT) _OS_Stimers[18]++;
    #endif
    #if OS_STIMERS > 19
    if(_OS_Stimers[19] & OS_STIMER_RUN_BIT) _OS_Stimers[19]++;
    #endif
    #if OS_STIMERS > 20
		#error OS_STIMERS MAX 20
    #endif
}
