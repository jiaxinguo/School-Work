################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/efm32g/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EFM32G_timer_pwm_gecko.axf

# Tool invocations
EFM32G_timer_pwm_gecko.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb  --specs=nano.specs -Xlinker --gc-sections -Xlinker -Map="EFM32G_timer_pwm_gecko.map" -T "EFM32G_timer_pwm_gecko.ld" -o EFM32G_timer_pwm_gecko.axf "./src/main_timer_pwm.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_prs.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./CMSIS/efm32g/startup_gcc_efm32g.o" "./CMSIS/efm32g/system_efm32g.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: EFM32G_timer_pwm_gecko.hex'
	arm-none-eabi-objcopy -O ihex "EFM32G_timer_pwm_gecko.axf" "EFM32G_timer_pwm_gecko.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "EFM32G_timer_pwm_gecko.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EFM32G_timer_pwm_gecko.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
