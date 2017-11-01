################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DigitDetector.cpp \
../src/TwoLayerNNFaster.cpp \
../src/main.cpp \
../src/util.cpp 

OBJS += \
./src/DigitDetector.o \
./src/TwoLayerNNFaster.o \
./src/main.o \
./src/util.o 

CPP_DEPS += \
./src/DigitDetector.d \
./src/TwoLayerNNFaster.d \
./src/main.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -std=c++11
	@echo 'Finished building: $<'
	@echo ' '


