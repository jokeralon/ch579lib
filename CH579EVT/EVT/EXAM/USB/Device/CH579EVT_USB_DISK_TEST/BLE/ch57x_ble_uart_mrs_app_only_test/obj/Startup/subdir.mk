################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
C:/Users/alon/Desktop/github/ch579lib/CH579EVT/EVT/EXAM/USB/Device/CH579EVT_USB_DISK_TEST/SRC/Startup/startup_ch579.S 

OBJS += \
./Startup/startup_ch579.o 

S_UPPER_DEPS += \
./Startup/startup_ch579.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_ch579.o: C:/Users/alon/Desktop/github/ch579lib/CH579EVT/EVT/EXAM/USB/Device/CH579EVT_USB_DISK_TEST/SRC/Startup/startup_ch579.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mthumb-interwork -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -x assembler-with-cpp -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\Startup" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\BLE\LIB" -I"C:\Users\alon\Desktop\github\ch579lib\CH579EVT\EVT\EXAM\USB\Device\CH579EVT_USB_DISK_TEST\SRC\StdPeriphDriver\inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


