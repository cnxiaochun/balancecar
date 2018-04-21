
RM:=rm -rf
MAKEDIR:=mkdir

TOOLS:=~/opt/gcc-arm/bin

MAKE_C_COMPILER:=$(TOOLS)/arm-none-eabi-gcc
MAKE_OBJCOPY:=$(TOOLS)/arm-none-eabi-objcopy
MAKE_SIZE:=$(TOOLS)/arm-none-eabi-size

MAKE_C_FLAGS:=-mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal

MAKE_CC_FLAGS:=-DDEBUG -DUSE_FULL_ASSERT -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast 

MAKE_EXE_LINKER_FLAGS:=-T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"ldscripts"

# -O0 None, -O1 Optimize, -O2 Optimize more, -O3 Optimize Most, -Os Optimize size, -Og Optimize for debug
# None, -g1 Minimal, -g Default, -g3 Maximum 
MAKE_C_FLAGS_RELEASE:=-O3
MAKE_C_FLAGS_DEBUG:=-O0 -g
MAKE_EXE_LINKER_FLAGS_RELEASE:=-s

