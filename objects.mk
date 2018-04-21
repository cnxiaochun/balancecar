
OUT_PATH:=Release

OUT_FILE:=$(OUT_PATH)/poise

C_INCS += \
	-I"inc" \
	-I"system/inc" \
	-I"system/inc/cmsis" \
	-I"system/inc/stm32f1-stdperiph"

C_SRCS += \
src/wrio.c \
src/led.c \
src/car.c \
src/main.c \
src/psx.c \
src/oled.c \
src/pwm.c \
src/encoder.c \
src/stimer.c \
src/usart_monitor.c 

C_SRCS += \
system/src/cmsis/system_stm32f10x.c \
system/src/cmsis/vectors_stm32f10x.c 

C_SRCS += \
system/src/cortexm/_initialize_hardware.c \
system/src/cortexm/_reset_hardware.c \
system/src/cortexm/exception_handlers.c 

C_SRCS += \
system/src/diag/Trace.c \
system/src/diag/trace_impl.c \
system/src/diag/_write.c

C_SRCS += \
system/src/newlib/_exit.c \
system/src/newlib/_sbrk.c \
system/src/newlib/_startup.c \
system/src/newlib/_syscalls.c \
system/src/newlib/assert.c 

C_SRCS += \
system/src/stm32f1-stdperiph/misc.c \
system/src/stm32f1-stdperiph/stm32f10x_gpio.c \
system/src/stm32f1-stdperiph/stm32f10x_rcc.c \
system/src/stm32f1-stdperiph/stm32f10x_tim.c \
system/src/stm32f1-stdperiph/stm32f10x_i2c.c \
system/src/stm32f1-stdperiph/stm32f10x_usart.c 

