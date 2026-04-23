# This is a makefile for our source files 
SRC_DIR = .
INCLUDE_DIR = .

# List all your source files here 
SRCS = \
$(SRC_DIR)/main.c \
$(SRC_DIR)/stm32_startup.c \
$(SRC_DIR)/GPIO_stm32f4.c \
$(SRC_DIR)/led.c \
$(SRC_DIR)/push_button.c 

# List all your include directories here
INCLUDES = \
-I$(INCLUDE_DIR)