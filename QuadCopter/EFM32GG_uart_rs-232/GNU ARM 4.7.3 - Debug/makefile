################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/efm32gg/subdir.mk
-include BSP/subdir.mk
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
all: EFM32GG_uart_rs-232.axf

# Tool invocations
EFM32GG_uart_rs-232.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb  --specs=nano.specs -Xlinker --gc-sections -Xlinker -Map="EFM32GG_uart_rs-232.map" -T "EFM32GG_uart_rs-232.ld" -o EFM32GG_uart_rs-232.axf "./src/main.o" "./emlib/em_cmu.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./CMSIS/efm32gg/startup_gcc_efm32gg.o" "./CMSIS/efm32gg/system_efm32gg.o" "./BSP/bsp_dk_3201.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: EFM32GG_uart_rs-232.hex'
	arm-none-eabi-objcopy -O ihex "EFM32GG_uart_rs-232.axf" "EFM32GG_uart_rs-232.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "EFM32GG_uart_rs-232.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EFM32GG_uart_rs-232.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
