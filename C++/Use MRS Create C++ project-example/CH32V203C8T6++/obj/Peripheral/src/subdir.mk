################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripheral/src/ch32v20x_adc.c \
../Peripheral/src/ch32v20x_bkp.c \
../Peripheral/src/ch32v20x_can.c \
../Peripheral/src/ch32v20x_crc.c \
../Peripheral/src/ch32v20x_dbgmcu.c \
../Peripheral/src/ch32v20x_dma.c \
../Peripheral/src/ch32v20x_exti.c \
../Peripheral/src/ch32v20x_flash.c \
../Peripheral/src/ch32v20x_gpio.c \
../Peripheral/src/ch32v20x_i2c.c \
../Peripheral/src/ch32v20x_iwdg.c \
../Peripheral/src/ch32v20x_misc.c \
../Peripheral/src/ch32v20x_opa.c \
../Peripheral/src/ch32v20x_pwr.c \
../Peripheral/src/ch32v20x_rcc.c \
../Peripheral/src/ch32v20x_rtc.c \
../Peripheral/src/ch32v20x_spi.c \
../Peripheral/src/ch32v20x_tim.c \
../Peripheral/src/ch32v20x_usart.c \
../Peripheral/src/ch32v20x_wwdg.c 

C_DEPS += \
./Peripheral/src/ch32v20x_adc.d \
./Peripheral/src/ch32v20x_bkp.d \
./Peripheral/src/ch32v20x_can.d \
./Peripheral/src/ch32v20x_crc.d \
./Peripheral/src/ch32v20x_dbgmcu.d \
./Peripheral/src/ch32v20x_dma.d \
./Peripheral/src/ch32v20x_exti.d \
./Peripheral/src/ch32v20x_flash.d \
./Peripheral/src/ch32v20x_gpio.d \
./Peripheral/src/ch32v20x_i2c.d \
./Peripheral/src/ch32v20x_iwdg.d \
./Peripheral/src/ch32v20x_misc.d \
./Peripheral/src/ch32v20x_opa.d \
./Peripheral/src/ch32v20x_pwr.d \
./Peripheral/src/ch32v20x_rcc.d \
./Peripheral/src/ch32v20x_rtc.d \
./Peripheral/src/ch32v20x_spi.d \
./Peripheral/src/ch32v20x_tim.d \
./Peripheral/src/ch32v20x_usart.d \
./Peripheral/src/ch32v20x_wwdg.d 

OBJS += \
./Peripheral/src/ch32v20x_adc.o \
./Peripheral/src/ch32v20x_bkp.o \
./Peripheral/src/ch32v20x_can.o \
./Peripheral/src/ch32v20x_crc.o \
./Peripheral/src/ch32v20x_dbgmcu.o \
./Peripheral/src/ch32v20x_dma.o \
./Peripheral/src/ch32v20x_exti.o \
./Peripheral/src/ch32v20x_flash.o \
./Peripheral/src/ch32v20x_gpio.o \
./Peripheral/src/ch32v20x_i2c.o \
./Peripheral/src/ch32v20x_iwdg.o \
./Peripheral/src/ch32v20x_misc.o \
./Peripheral/src/ch32v20x_opa.o \
./Peripheral/src/ch32v20x_pwr.o \
./Peripheral/src/ch32v20x_rcc.o \
./Peripheral/src/ch32v20x_rtc.o \
./Peripheral/src/ch32v20x_spi.o \
./Peripheral/src/ch32v20x_tim.o \
./Peripheral/src/ch32v20x_usart.o \
./Peripheral/src/ch32v20x_wwdg.o 


# Each subdirectory must supply rules for building sources it contributes
Peripheral/src/%.o: ../Peripheral/src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\OWNER\Desktop\v2\CH32V203C8T6++\Debug" -I"C:\Users\OWNER\Desktop\v2\CH32V203C8T6++\Core" -I"C:\Users\OWNER\Desktop\v2\CH32V203C8T6++\User" -I"C:\Users\OWNER\Desktop\v2\CH32V203C8T6++\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

