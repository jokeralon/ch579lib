################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/bsp_device.c \
../bsp/bsp_led.c \
../bsp/bsp_log.c \
../bsp/bsp_spi.c \
../bsp/bsp_uart.c 

OBJS += \
./bsp/bsp_device.o \
./bsp/bsp_led.o \
./bsp/bsp_log.o \
./bsp/bsp_spi.o \
./bsp/bsp_uart.o 

C_DEPS += \
./bsp/bsp_device.d \
./bsp/bsp_led.d \
./bsp/bsp_log.d \
./bsp/bsp_spi.d \
./bsp/bsp_uart.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o: ../bsp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mthumb-interwork -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=Debug_UART0 -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\CMSIS\Include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\app\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\LIB" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\Startup" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\StdPeriphDriver\inc" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\hal\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\bsp\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


