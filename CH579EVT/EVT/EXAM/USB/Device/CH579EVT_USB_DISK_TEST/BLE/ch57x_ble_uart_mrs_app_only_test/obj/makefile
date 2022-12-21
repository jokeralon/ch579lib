################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include hal/subdir.mk
-include bsp/subdir.mk
-include app/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ch57x_ble_uart_mrs_app_only_test.hex \

SECONDARY_LIST += \
ch57x_ble_uart_mrs_app_only_test.lst \

SECONDARY_SIZE += \
ch57x_ble_uart_mrs_app_only_test.siz \


# All Target
all: ch57x_ble_uart_mrs_app_only_test.elf secondary-outputs

# Tool invocations
ch57x_ble_uart_mrs_app_only_test.elf: $(OBJS) $(USER_OBJS)
	@	@	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mthumb-interwork -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -T "C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\ld\CH579.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"ch57x_ble_uart_mrs_app_only_test.map" --specs=nano.specs --specs=nosys.specs -o "ch57x_ble_uart_mrs_app_only_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
ch57x_ble_uart_mrs_app_only_test.hex: ch57x_ble_uart_mrs_app_only_test.elf
	@	arm-none-eabi-objcopy -O ihex "ch57x_ble_uart_mrs_app_only_test.elf"  "ch57x_ble_uart_mrs_app_only_test.hex"
	@	@
ch57x_ble_uart_mrs_app_only_test.lst: ch57x_ble_uart_mrs_app_only_test.elf
	@	arm-none-eabi-objdump --source --all-headers --demangle --disassemble --line-numbers --wide "ch57x_ble_uart_mrs_app_only_test.elf" > "ch57x_ble_uart_mrs_app_only_test.lst"
	@	@
ch57x_ble_uart_mrs_app_only_test.siz: ch57x_ble_uart_mrs_app_only_test.elf
	@	arm-none-eabi-size --format=berkeley "ch57x_ble_uart_mrs_app_only_test.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) ch57x_ble_uart_mrs_app_only_test.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
