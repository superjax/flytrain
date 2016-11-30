###############################################################################
# Generic Makefile Template for C/C++ for use with STM32 Microcontrollers
#
# Copyright (c) 2016 - James Jackson
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of the <organization> nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

TARGET	?= i2csniff

DEBUG ?= GDB

SERIAL_DEVICE ?= /dev/ttyUSB0

#################################
# GNU ARM Embedded Toolchain
#################################
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
A2L=arm-none-eabi-addr2line

#################################
# Working directories
#################################
BREEZY_DIR = ../../../
ROOT		 = .
SRC_DIR		 = $(ROOT)
CMSIS_DIR	 = $(BREEZY_DIR)/Drivers/CMSIS
HAL_DIR  	 = $(BREEZY_DIR)/Drivers/STM32F1xx_HAL_Driver
OBJECT_DIR	 = obj
BIN_DIR		 = obj




#################################
# Source Files
#################################
ASOURCES = $(CMSIS_DIR)/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s

VPATH		:= $(SRC_DIR):$(SRC_DIR)/startups

# Search path and source files for the CMSIS sources
VPATH		:= $(VPATH):$(CMSIS_DIR)/Device/ST/STM32F1xx/Source/Templates
CMSIS_SRC	 = $(CMSIS_DIR)/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c

# Search path and source files for the ST HAL library
HAL_PREFIX = $(HAL_DIR)/Src/stm32f1xx_hal
VPATH   := $(VPATH):$(HAL_DIR)/Src
HAL_SRC	 = $(HAL_PREFIX).c \
	   $(HAL_PREFIX)_cortex.c \
	   $(HAL_PREFIX)_dma.c \
	   $(HAL_PREFIX)_gpio.c \
	   $(HAL_PREFIX)_pwr.c \
	   $(HAL_PREFIX)_pwr.c \
	   $(HAL_PREFIX)_rcc.c \
	   $(HAL_PREFIX)_rcc_ex.c



CSOURCES =  $(CMSIS_SRC) \
	    $(HAL_SRC) \
	    $(ROOT)/stm32f1xx_it.c

CXXSOURCES = $(ROOT)/main.cpp

INCLUDE_DIRS	 = $(SRC_DIR) \
		   $(BREEZY_DIR) \
		   $(HAL_DIR)/Inc \
		   $(CMSIS_DIR)/Include \
		   $(CMSIS_DIR)/Device/ST/STM32F1xx/Include

#################################
# Object List
#################################
OBJECTS=$(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(ASOURCES))))
OBJECTS+=$(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(CSOURCES))))
OBJECTS+=$(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(CXXSOURCES))))

#################################
# Target Output Files
#################################
TARGET_ELF=$(BIN_DIR)/$(TARGET).elf
TARGET_HEX=$(BIN_DIR)/$(TARGET).hex

#################################
# Debug Config
#################################
ifeq ($(DEBUG), GDB)
DEBUG_FLAGS = -ggdb3
OPTIMIZE = -Og
else
OPTIMIZE = -Os
LTO_FLAGS = -flto -fuse-linker-plugin $(OPTIMIZE)
endif


#################################
# Flags
#################################
MCFLAGS= -mcpu=cortex-m3 -mthumb
COMPILER_FLAGS = -fno-common -fdata-sections -ffunction-sections
LINKER_FLAGS =
DEFS=-DSTM32F103xB -D__CORTEX_M3 -DWORDS_STACK_SIZE=200 -DSTM32F10X_MD -DUSE_HAL_DRIVER
CFLAGS=-c $(MCFLAGS) $(DEBUG) $(DEFS) $(OPTIMIZE) $(DEBUG_FLAGS) $(COMPILER_FLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -std=c99
CXXFLAGS=-c $(MCFLAGS) $(DEFS) $(OPTIMIZE) $(DEBUG_FLAGS) $(COMPILER_FLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -std=c++11
CXXFLAGS+=-U__STRICT_ANSI__
LDSCRIPT=$(CMSIS_DIR)/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F103XB_FLASH.ld
LDFLAGS =-T $(LDSCRIPT) $(MCFLAGS) -lm -lc --specs=nosys.specs $(DEBUG_FLAGS) -static

#################################
# Build
#################################
$(TARGET_HEX): $(TARGET_ELF)
	$(CP) -O ihex --set-start 0x8000000 $< $@

$(TARGET_ELF): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	$(SIZE) $(TARGET_ELF)

$(OBJECT_DIR)/$(TARGET)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CXX) -c -o $@ $(CXXFLAGS) $<

$(OBJECT_DIR)/$(TARGET)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(CFLAGS) $<

$(OBJECT_DIR)/$(TARGET)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CXX) -c -o $@ $(CFLAGS) $<


#################################
# Recipes
#################################
.PHONY: all flash clean

clean:
	rm -f $(OBJECTS) $(TARGET_ELF) $(TARGET_HEX) $(BIN_DIR)/output.map

flash: $(TARGET_HEX)
	stty -F $(SERIAL_DEVICE) raw speed 921600 -crtscts cs8 -parenb -cstopb -ixon
	stm32flash -w $(TARGET_HEX) -v -g 0x0 -b 921600 $(SERIAL_DEVICE)


