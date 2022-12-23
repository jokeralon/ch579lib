################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app_flash.c \
../app/app_led.c \
../app/app_uart.c \
../app/ch57x_code_flash.c \
../app/ch57x_usb_device_msc.c \
../app/diskio.c \
../app/ff.c \
../app/ffsystem.c \
../app/ffunicode.c \
../app/peripheral_main.c 

OBJS += \
./app/app_flash.o \
./app/app_led.o \
./app/app_uart.o \
./app/ch57x_code_flash.o \
./app/ch57x_usb_device_msc.o \
./app/diskio.o \
./app/ff.o \
./app/ffsystem.o \
./app/ffunicode.o \
./app/peripheral_main.o 

C_DEPS += \
./app/app_flash.d \
./app/app_led.d \
./app/app_uart.d \
./app/ch57x_code_flash.d \
./app/ch57x_usb_device_msc.d \
./app/diskio.d \
./app/ff.d \
./app/ffsystem.d \
./app/ffunicode.d \
./app/peripheral_main.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mthumb-interwork -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=Debug_UART0 -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\CMSIS\Include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\app\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\LIB" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\Startup" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\StdPeriphDriver\inc" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\hal\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\bsp\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


