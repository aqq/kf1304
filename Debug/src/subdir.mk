################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/kf1304.cpp \
../src/main.cpp \
../src/master.cpp \
../src/master_test.cpp \
../src/slaver.cpp 

OBJS += \
./src/kf1304.o \
./src/main.o \
./src/master.o \
./src/master_test.o \
./src/slaver.o 

CPP_DEPS += \
./src/kf1304.d \
./src/main.d \
./src/master.d \
./src/master_test.d \
./src/slaver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


