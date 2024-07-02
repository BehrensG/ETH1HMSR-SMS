################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/HiSLIP/Src/HiSLIP_Either.c \
../Core/BSP/HiSLIP/Src/HiSLIP_Support.c \
../Core/BSP/HiSLIP/Src/HiSLIP_Sync.c \
../Core/BSP/HiSLIP/Src/HiSLIP_Task.c \
../Core/BSP/HiSLIP/Src/HiSLIP_aSync.c 

OBJS += \
./Core/BSP/HiSLIP/Src/HiSLIP_Either.o \
./Core/BSP/HiSLIP/Src/HiSLIP_Support.o \
./Core/BSP/HiSLIP/Src/HiSLIP_Sync.o \
./Core/BSP/HiSLIP/Src/HiSLIP_Task.o \
./Core/BSP/HiSLIP/Src/HiSLIP_aSync.o 

C_DEPS += \
./Core/BSP/HiSLIP/Src/HiSLIP_Either.d \
./Core/BSP/HiSLIP/Src/HiSLIP_Support.d \
./Core/BSP/HiSLIP/Src/HiSLIP_Sync.d \
./Core/BSP/HiSLIP/Src/HiSLIP_Task.d \
./Core/BSP/HiSLIP/Src/HiSLIP_aSync.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/HiSLIP/Src/%.o Core/BSP/HiSLIP/Src/%.su Core/BSP/HiSLIP/Src/%.cyclo: ../Core/BSP/HiSLIP/Src/%.c Core/BSP/HiSLIP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DDATA_IN_D2_SRAM -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HTTP/Inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/multicastDNS/Inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HiSLIP/Inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/Inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc/scpi" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/src/apps/http -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-HiSLIP-2f-Src

clean-Core-2f-BSP-2f-HiSLIP-2f-Src:
	-$(RM) ./Core/BSP/HiSLIP/Src/HiSLIP_Either.cyclo ./Core/BSP/HiSLIP/Src/HiSLIP_Either.d ./Core/BSP/HiSLIP/Src/HiSLIP_Either.o ./Core/BSP/HiSLIP/Src/HiSLIP_Either.su ./Core/BSP/HiSLIP/Src/HiSLIP_Support.cyclo ./Core/BSP/HiSLIP/Src/HiSLIP_Support.d ./Core/BSP/HiSLIP/Src/HiSLIP_Support.o ./Core/BSP/HiSLIP/Src/HiSLIP_Support.su ./Core/BSP/HiSLIP/Src/HiSLIP_Sync.cyclo ./Core/BSP/HiSLIP/Src/HiSLIP_Sync.d ./Core/BSP/HiSLIP/Src/HiSLIP_Sync.o ./Core/BSP/HiSLIP/Src/HiSLIP_Sync.su ./Core/BSP/HiSLIP/Src/HiSLIP_Task.cyclo ./Core/BSP/HiSLIP/Src/HiSLIP_Task.d ./Core/BSP/HiSLIP/Src/HiSLIP_Task.o ./Core/BSP/HiSLIP/Src/HiSLIP_Task.su ./Core/BSP/HiSLIP/Src/HiSLIP_aSync.cyclo ./Core/BSP/HiSLIP/Src/HiSLIP_aSync.d ./Core/BSP/HiSLIP/Src/HiSLIP_aSync.o ./Core/BSP/HiSLIP/Src/HiSLIP_aSync.su

.PHONY: clean-Core-2f-BSP-2f-HiSLIP-2f-Src

