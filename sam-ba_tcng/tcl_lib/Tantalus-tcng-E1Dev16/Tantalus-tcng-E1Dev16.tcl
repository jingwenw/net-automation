#  ----------------------------------------------------------------------------
#          ATMEL Microcontroller Software Support
#  ----------------------------------------------------------------------------
#  Copyright (c) 2008, Atmel Corporation
#  Copyroght (c) 2010, Tantalus Corporation
#
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of source code must retain the above copyright notice,
#  this list of conditions and the disclaimer below.
#
#  Atmel's name may not be used to endorse or promote products derived from
#  this software without specific prior written permission. 
#
#  DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
#  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
#  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  ----------------------------------------------------------------------------

if { [ catch { source "$libPath(extLib)/common/GENERIC.tcl"} errMsg] } {
    messageDialg  error.gif "Common library file not found:\n$errMsg" "File not found" ok
    exit
}
set AT91C_IRAM_SIZE	 0x00004000

namespace eval BOARD {
    variable sramSize         $AT91C_IRAM_SIZE
    variable maxBootSize      [expr 28 * 1024]
}

################################################################################
## PROCEDURES FOR COMPATIBILITY WITH OLDER SAM-BA VERSIONS AND USER DEFINED
################################################################################
if { [ catch { source "$libPath(extLib)/common/FUNCTIONS.tcl"} errMsg] } {
    messageDialg  error.gif "Function file not found:\n$errMsg" "File not found" ok
    exit
}

array set memoryAlgo {
    "SRAM"                    "::tantalus_e1_dev16_sram"
    "SDRAM"                   "::tantalus_e1_dev16_sdram"
    "DataFlash AT45DB/DCB"    "::tantalus_e1_dev16_dataflash"  
    "SerialFlash CS0 AT26"    "::tantalus_e1_dev16_serialflash"
    "SerialFlash CS1 AT26"    "::tantalus_e1_dev16_serialflash1"
    "Peripheral"              "::tantalus_e1_dev16_peripheral"
    "ROM"                     "::tantalus_e1_dev16_rom"
    "REMAP"                   "::tantalus_e1_dev16_remap"
}

################################################################################
## SRAM
################################################################################
array set tantalus_e1_dev16_sram {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x00100000
    dftSize     0x8000
    dftSend     "RAM::sendFile"
    dftReceive  "RAM::receiveFile"
    dftScripts  ""
}

################################################################################
## SDRAM
################################################################################
array set tantalus_e1_dev16_sdram {
    dftDisplay  1
    dftDefault  0
    dftAddress  "0x70000000"
    dftSize     "$GENERIC::memorySize"
    dftSend     "RAM::sendFile"
    dftReceive  "RAM::receiveFile"
    dftScripts  "::tantalus_e1_dev16_sdram_scripts"
}

# Vdd Memory 1.8V = 0 / Vdd Memory 3.3V = 1
set BOARD::vddmem 0  
# External SDRAM = 0 / External DDR-SDRAM = 1
set BOARD::ramType 0  

set RAM::appletAddr       0x101000
set RAM::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-extram-tcnge116mb.bin"
array set tantalus_e1_dev16_sdram_scripts {
    "Enable Sdram"   "GENERIC::Init $RAM::appletAddr $RAM::appletFileName [list $::target(comType) $GENERIC::traceLevel $BOARD::vddmem $BOARD::ramType]"
}

# Initialize SDRAMC
if {[catch {GENERIC::Init $RAM::appletAddr $RAM::appletFileName [list $::target(comType) $GENERIC::traceLevel $BOARD::vddmem $BOARD::ramType]} dummy_err] } {
        if {$commandLineMode == 0} {
            messageDialg error.gif "Failed to initialize external RAM" "External RAM init." ok
        }
        puts "-E- Error during external RAM initialization"
        # Close link
        TCL_Close $target(handle)
        exit
} else {
        puts "-I- External RAM initialized"
}

################################################################################
## DATAFLASH - Leave this even though we have no dataflash support as removing
## it causes the script to fail.
################################################################################

array set tantalus_e1_dev16_dataflash {
    dftDisplay  1
    dftDefault  1
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::tantalus_e1_dev16_dataflash_scripts"
}

array set tantalus_e1_dev16_dataflash_scripts {
    "Do Not USE"                        "DATAFLASH::Init 0"
    "Send Boot File"                                     "GENERIC::SendBootFileGUI"
    "Erase All"                                          "DATAFLASH::EraseAll"
    "Set DF in Power-Of-2 Page Size mode (Binary mode)"  "DATAFLASH::BinaryPage"
}

set DATAFLASH::appletAddr       0x70000000
set DATAFLASH::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-dataflash-at91cap9.bin"

################################################################################
## SERIALFLASH - ChipSelect 0
################################################################################
array set tantalus_e1_dev16_serialflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::tantalus_e1_dev16_serialflash_scripts"
}

array set tantalus_e1_dev16_serialflash_scripts {
    "Enable Serialflash (SPI0 NCS0)"  "SERIALFLASH::Init 0"
    "Send Boot File"                  "GENERIC::SendBootFileGUI"
    "Erase All"                       "SERIALFLASH::EraseAll"
}

set SERIALFLASH::appletAddr       0x70000000
set SERIALFLASH::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-serialflash-tcnge116mb.bin.cs0"

################################################################################
## SERIALFLASH1 - ChipSelect 1
################################################################################
array set tantalus_e1_dev16_serialflash1 {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::tantalus_e1_dev16_serialflash1_scripts"
}

array set tantalus_e1_dev16_serialflash1_scripts {
    "Enable Serialflash (SPI0 NCS1)"  "SERIALFLASH1::Init 0"
    "Send Boot File"                  "GENERIC::SendBootFileGUI"
    "Erase All"                       "SERIALFLASH1::EraseAll"
}

set SERIALFLASH1::appletAddr       0x70000000
set SERIALFLASH1::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-serialflash-tcnge116mb.bin.cs1"


array set tantalus_e1_dev16_peripheral {
    dftDisplay  0
    dftDefault  0
    dftAddress  0xFF000000
    dftSize     0x10000000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set tantalus_e1_dev16_rom {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x400000
    dftSize     0x8000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set tantalus_e1_dev16_remap {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x00000000
    dftSize     0x10000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

