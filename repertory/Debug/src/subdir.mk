################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GlobalHelper.cpp \
../src/kf1304_repertory.cpp \
../src/rep.cpp \
../src/sockethelper.cpp \
../src/sockethelper_test.cpp 

OBJS += \
./src/GlobalHelper.o \
./src/kf1304_repertory.o \
./src/rep.o \
./src/sockethelper.o \
./src/sockethelper_test.o 

CPP_DEPS += \
./src/GlobalHelper.d \
./src/kf1304_repertory.d \
./src/rep.d \
./src/sockethelper.d \
./src/sockethelper_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


