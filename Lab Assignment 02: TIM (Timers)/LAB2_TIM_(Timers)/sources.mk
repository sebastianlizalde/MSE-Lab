# This is a makefile for our source files 
SRC_DIR = .
INCLUDE_DIR = .

#List all yout source files here
SRCS =     \
$(SRC_DIR)/main.c \
$(SRC_DIR)/stm32.startup.c \
$(SRC_DIR)/TIM_Driver.c \
$(SRC_DIR)/led.c \
$(SRC_DIR)/GPIO_stm32f4.c \
$(SRC_DIR)/Timer.c \
$(SRC_DIR)/pwm.c 


#List all your include directories here
INCLUDES = \
-I$(INCLUDE_DIR)
