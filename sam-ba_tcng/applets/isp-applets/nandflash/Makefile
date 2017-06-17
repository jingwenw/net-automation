# ----------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support  -  ROUSSET  -
# ----------------------------------------------------------------------------
# Copyright (c) 2007, Atmel Corporation
#
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
# 
# - Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the disclaimer below in the documentation and/or
# other materials provided with the distribution. 
# 
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission. 
# 
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ---------------------------------------------------------------------------- 

# 	Makefile for compiling isp-project nandflash applet

#-------------------------------------------------------------------------------
#		User-modifiable options
#-------------------------------------------------------------------------------

# Chip & board used for compilation
# (can be overriden by adding CHIP=chip and BOARD=board to the command-line)
CHIP  = at91sam9260
BOARD = at91sam9260-ek

# Optimization level, put in comment for debugging
OPTIMIZATION = -Os

# For chip with few memory, set DYN_TRACES to 0 
ifeq ($(DYN_TRACES),1)
DYNAMIC_TRACE_LEVEL = -DDYNAMIC_TRACE_LEVEL
else
DYNAMIC_TRACE_LEVEL = -DNOTRACE
endif

# install directory
ifeq ($(CHIP),at91cap9)
BOARD_DIR = $(BOARD)
else
BOARD_DIR = $(CHIP)-ek
endif

ifdef INST_PREFIX
INSTALLDIR = "$(INST_PREFIX)/$(BOARD_DIR)/"
else
INSTALLDIR = "../lib/$(BOARD_DIR)/"
endif

# AT91 library directory
AT91LIB = ../../at91lib

# Output file basename
OUTPUT = isp-nandflash-$(CHIP)

# Output directories
BIN = bin
OBJ = obj

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS = arm-none-eabi-

# Compilation tools
CC = $(CROSS)gcc
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
OBJCOPY = $(CROSS)objcopy

# Flags
INCLUDES = -I$(AT91LIB)/boards/$(BOARD) -I$(AT91LIB)/peripherals -I$(AT91LIB)/memories
INCLUDES += -I$(AT91LIB)/components -I$(AT91LIB)

CFLAGS = -Wall -mlong-calls -ffunction-sections
CFLAGS += -g $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) $(DYNAMIC_TRACE_LEVEL)
ASFLAGS = -g $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) -D__ASSEMBLY__
LDFLAGS = -g $(OPTIMIZATION) -nostartfiles -Wl,--gc-sections

#-------------------------------------------------------------------------------
#		Files
#-------------------------------------------------------------------------------

# Directories where source files can be found
ISP = ..
PERIPH = $(AT91LIB)/peripherals
MEMORIES = $(AT91LIB)/memories
BOARDS = $(AT91LIB)/boards
UTILITY = $(AT91LIB)/utility

VPATH += $(ISP)/common
VPATH += $(MEMORIES)/nandflash
VPATH += $(UTILITY)
VPATH += $(PERIPH)/dbgu $(PERIPH)/pio $(PERIPH)/aic
VPATH += $(BOARDS)/$(BOARD) $(BOARDS)/$(BOARD)/$(CHIP)

# Objects built from C source files
C_OBJECTS = main.o
C_OBJECTS += RawNandFlash.o EccNandFlash.o SkipBlockNandFlash.o
C_OBJECTS += NandFlashModel.o NandFlashModelList.o NandSpareScheme.o
C_OBJECTS += math.o stdio.o hamming.o
C_OBJECTS += dbgu.o pio.o aic.o
C_OBJECTS += board_memories.o board_lowlevel.o

# Objects built from Assembly source files
ASM_OBJECTS = isp_cstartup.o

# Append OBJ and BIN directories to filenames
C_OBJECTS := $(addprefix $(OBJ)/, $(C_OBJECTS))
ASM_OBJECTS := $(addprefix $(OBJ)/, $(ASM_OBJECTS))
OUTPUT := $(BIN)/$(OUTPUT)

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------

all: $(BIN) $(OBJ) $(MEMORY) install

$(BIN) $(OBJ):
	@mkdir $@

$(MEMORY): $(ASM_OBJECTS) $(C_OBJECTS)
	$(CC) $(LDFLAGS) -T"$(AT91LIB)/boards/$(BOARD)/$(CHIP)/$@.lds" -o $(OUTPUT).elf $^
	$(OBJCOPY) -O binary $(OUTPUT).elf $(OUTPUT).bin
	$(SIZE) $(ASM_OBJECTS) $(C_OBJECTS) $(OUTPUT).elf

$(C_OBJECTS): $(OBJ)/%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(ASM_OBJECTS): $(OBJ)/%.o: %.S Makefile
	$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJ)/*.o $(BIN)/*.bin $(BIN)/*.elf $(BIN)/*.txt

install:
	mkdir -p $(INSTALLDIR)
	cp $(OUTPUT).bin $(INSTALLDIR)
	
