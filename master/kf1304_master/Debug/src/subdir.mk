################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Cpp2mysql.cpp \
../src/GlobalHelper.cpp \
../src/StringHelper.cpp \
../src/kf1304_master.cpp \
../src/master.cpp \
../src/slaver.cpp 

OBJS += \
./src/Cpp2mysql.o \
./src/GlobalHelper.o \
./src/StringHelper.o \
./src/kf1304_master.o \
./src/master.o \
./src/slaver.o 

CPP_DEPS += \
./src/Cpp2mysql.d \
./src/GlobalHelper.d \
./src/StringHelper.d \
./src/kf1304_master.d \
./src/master.d \
./src/slaver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


