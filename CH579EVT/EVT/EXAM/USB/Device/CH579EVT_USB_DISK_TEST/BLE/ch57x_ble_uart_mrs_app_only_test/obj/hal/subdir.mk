################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/hal_flash.c \
../hal/hal_device.c \
../hal/hal_spi.c 

OBJS += \
./hal/hal_flash.o \
./hal/hal_device.o \
./hal/hal_spi.o 

C_DEPS += \
./hal/hal_flash.d \
./hal/hal_device.d \
./hal/hal_spi.d 


# Each subdirectory must supply rules for building sources it contributes
hal/%.o: ../hal/%.c
	@	@	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mthumb-interwork -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=Debug_UART0 -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\CMSIS\Include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\app\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\LIB" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\Startup" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\StdPeriphDriver\inc" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\hal\include" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\ch57x_ble_uart_mrs_app_only_test\bsp\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

