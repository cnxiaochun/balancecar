
OUT_PATH :=
OUT_FILE :=
C_SRCS := 
C_INCS :=
C_DEPS := 
OBJS := 

-include platform.mk
-include objects.mk

OBJS:=$(patsubst %.c, $(OUT_PATH)/%.o, $(filter %.c, $(C_SRCS)))  
# OBJS:=$(C_SRCS:.c=.o)  
C_DEPS:=$(OBJS:.o=.d) 

## -MMD 可以用于自动生成头文件依赖关系  
$(OUT_PATH)/%.o : %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	$(shell mkdir -p $(dir $@))
	$(MAKE_C_COMPILER) $(MAKE_C_FLAGS_RELEASE) $(MAKE_C_FLAGS) $(C_INCS) $(MAKE_CC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MT"$(@:%.o=%.d)" -c "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: clean build

# Build Target
build: $(OUT_FILE).elf $(OUT_FILE).bin $(OUT_FILE).siz

# Tool invocations
$(OUT_FILE).elf: $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	$(MAKE_C_COMPILER) $(MAKE_C_FLAGS_RELEASE) $(MAKE_C_FLAGS) $(MAKE_EXE_LINKER_FLAGS_RELEASE) $(MAKE_EXE_LINKER_FLAGS) -Wl,-Map,"$(OUT_FILE).map" -o "$(OUT_FILE).elf" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

$(OUT_FILE).bin: $(OUT_FILE).elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	$(MAKE_OBJCOPY) -O binary "$(OUT_FILE).elf" "$(OUT_FILE).bin"
	@echo 'Finished building: $@'
	@echo ' '

$(OUT_FILE).siz: $(OUT_FILE).elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	$(MAKE_SIZE) --format=berkeley "$(OUT_FILE).elf"
	@echo 'Finished building: $@'
	@echo ' '

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS) $(OUT_FILE).elf $(OUT_FILE).bin $(OUT_FILE).siz $(OUT_FILE).map
	-@echo ' '

.PHONY: all build clean dependents


