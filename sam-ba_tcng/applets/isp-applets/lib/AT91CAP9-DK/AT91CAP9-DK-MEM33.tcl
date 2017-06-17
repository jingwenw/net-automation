#  ----------------------------------------------------------------------------
#          ATMEL Microcontroller Software Support
#  ----------------------------------------------------------------------------
#  Copyright (c) 2008, Atmel Corporation
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
    "SRAM"                    "::at91cap9_dk_sram"
    "SDRAM"                   "::at91cap9_dk_sdram"
    "DataFlash AT45DB/DCB"    "::at91cap9_dk_dataflash"  
    "NandFlash"               "::at91cap9_dk_nandflash"    
    "NorFlash"                "::at91cap9_dk_norflash" 
    "NorFlash Map"            "::at91cap9_dk_norflash_map" 
    "Peripheral"              "::at91cap9_dk_peripheral"
    "ROM"                     "::at91cap9_dk_rom"
    "REMAP"                   "::at91cap9_dk_remap"
    "LCD"                     "::at91cap9_dk_lcd"    
}

################################################################################
## SRAM
################################################################################
array set at91cap9_dk_sram {
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
array set at91cap9_dk_sdram {
    dftDisplay  1
    dftDefault  0
    dftAddress  "0x70000000"
    dftSize     "$GENERIC::memorySize"
    dftSend     "RAM::sendFile"
    dftReceive  "RAM::receiveFile"
    dftScripts  "::at91cap9_dk_sdram_scripts"
}

# Vdd Memory 1.8V = 0 / Vdd Memory 3.3V = 1
set BOARD::vddmem 1  
# External SDRAM = 0 / External DDR-SDRAM = 1
set BOARD::ramType 0  

set RAM::appletAddr       0x101000
set RAM::appletFileName   "$libPath(extLib)/AT91CAP9-DK/isp-extram-at91cap9.bin"
array set at91cap9_dk_sdram_scripts {
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
## DATAFLASH
################################################################################

array set at91cap9_dk_dataflash {
    dftDisplay  1
    dftDefault  1
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91cap9_dk_dataflash_scripts"
}

array set at91cap9_dk_dataflash_scripts {
    "Enable Dataflash (SPI0 CS0)"                        "DATAFLASH::Init 0"
    "Send Boot File"                                     "GENERIC::SendBootFileGUI"
    "Erase All"                                          "DATAFLASH::EraseAll"
    "Set DF in Power-Of-2 Page Size mode (Binary mode)"  "DATAFLASH::BinaryPage"
}

set DATAFLASH::appletAddr       0x70000000
set DATAFLASH::appletFileName   "$libPath(extLib)/AT91CAP9-DK/isp-dataflash-at91cap9.bin"

################################################################################
## NANDFLASH
################################################################################
array set at91cap9_dk_nandflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91cap9_dk_nandflash_scripts"
}

array set at91cap9_dk_nandflash_scripts {
    "Enable NandFlash"    "NANDFLASH::Init"
    "Send Boot File"      "GENERIC::SendBootFileGUI"
    "Erase All"           "GENERIC::EraseAll"
    "Scrub NandFlash"     "GENERIC::EraseAll $NANDFLASH::scrubErase"
}
set NANDFLASH::appletAddr       0x70000000
set NANDFLASH::appletFileName   "$libPath(extLib)/AT91CAP9-DK/isp-nandflash-at91cap9.bin"

#TCL_Write_Int $target(handle) 0x4000000 0xfffff410
#TCL_Write_Int $target(handle) 0x4000000 0xfffff430

################################################################################
## NORFLASH
################################################################################
array set at91cap9_dk_norflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91cap9_dk_norflash_scripts"
}

array set at91cap9_dk_norflash_scripts {
    "Enable NorFlash "    "NORFLASH::Init"
    "Erase All"           "NORFLASH::EraseAll"
}

set NORFLASH::appletAddr       0x70000000
set NORFLASH::appletFileName   "$libPath(extLib)/AT91CAP9-DK/isp-norflash-at91cap9.bin"

################################################################################
array set at91cap9_dk_norflash_map {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x10000000
    dftSize     0x10000000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91cap9_dk_peripheral {
    dftDisplay  0
    dftDefault  0
    dftAddress  0xFF000000
    dftSize     0x10000000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91cap9_dk_rom {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x400000
    dftSize     0x8000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91cap9_dk_remap {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x00000000
    dftSize     0x10000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91cap9_dk_lcd {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x500000
    dftSize     0x8000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

