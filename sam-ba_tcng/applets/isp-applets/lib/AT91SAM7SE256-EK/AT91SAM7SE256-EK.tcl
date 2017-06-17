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
    variable sramSize         0x8000
    variable maxBootSize      0
}

################################################################################
## PROCEDURES FOR COMPATIBILITY WITH OLDER SAM-BA VERSIONS AND USER DEFINED
################################################################################
if { [ catch { source "$libPath(extLib)/common/FUNCTIONS.tcl"} errMsg] } {
    messageDialg  error.gif "Function file not found:\n$errMsg" "File not found" ok
    exit
}

array set memoryAlgo {
    "SRAM"                    "::at91sam7se256_sram"
    "SDRAM"                   "::at91sam7se256_sdram"
    "Flash"                   "::at91sam7se256_flash"
    "DataFlash AT45DB/DCB"    "::at91sam7se256_dataflash"
    "SerialFlash AT25/AT26"   "::at91sam7se256_serialflash"
    "NandFlash"               "::at91sam7se256_nandflash"
    "NorFlash"                "::at91sam7se256_norflash"
    "NorFlash Map"            "::at91sam7se256_norflash_map"
    "Peripheral"              "::at91sam7se256_peripheral"
    "ROM"                     "::at91sam7se256_rom"
    "REMAP"                   "::at91sam7se256_remap"
}
#"NorFlash"                "::at91sam7se32_norflash"

################################################################################
## SRAM
################################################################################
array set at91sam7se256_sram {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x00200000
    dftSize     0x8000
    dftSend     "RAM::sendFile"
    dftReceive  "RAM::receiveFile"
    dftScripts  ""
}

################################################################################
## FLASH
################################################################################
array set at91sam7se256_flash {
    dftDisplay  1
    dftDefault  1
    dftAddress  0x100000
    dftSize     0x40000
    dftSend     "FLASH::SendFile"
    dftReceive  "FLASH::ReceiveFile"
    dftScripts  "::at91sam7se256_flash_scripts"
}
set FLASH::appletAddr       0x202000
set FLASH::appletFileName   "$libPath(extLib)/$target(board)/isp-flash-at91sam7se256.bin"

array set at91sam7se256_flash_scripts {
        "Erase All Flash"                      "FLASH::EraseAll"
        "Enable BrownOut Detector (GPNVM0)"    "FLASH::ScriptGPNMV 0"
        "Disable BrownOut Detector (GPNVM0)"   "FLASH::ScriptGPNMV 1"
        "Enable BrownOut Reset (GPNVM1)"       "FLASH::ScriptGPNMV 2"
        "Disable BrownOut Reset (GPNVM1)"      "FLASH::ScriptGPNMV 3"
        "Boot from Flash (GPNVM2)"             "FLASH::ScriptGPNMV 4"
        "Boot from ROM (GPNVM2)"               "FLASH::ScriptGPNMV 5"
        "Enable Security Bit"                  "FLASH::ScriptSetSecurityBit"
        "Enable Flash access"                  "FLASH::Init"
}


################################################################################
## SDRAM
################################################################################
array set at91sam7se256_sdram {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x20000000
    dftSize     "$GENERIC::memorySize"
    dftSend     "RAM::sendFile"
    dftReceive  "RAM::receiveFile"
    dftScripts  "::at91sam7se256_sdram_scripts"
}

# Vdd Memory 1.8V = 0 / Vdd Memory 3.3V = 1
set BOARD::vddmem 1  
# External SDRAM = 0 / External DDR-SDRAM = 1
set BOARD::ramType 0  

set RAM::appletAddr       0x202000
set RAM::appletFileName   "$libPath(extLib)/$target(board)/isp-extram-at91sam7se256.bin"
array set at91sam7se256_sdram_scripts {
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

# Initialize FLASH after external memory
if {[catch {FLASH::Init } dummy_err] } {
        if {$commandLineMode == 0} {
            messageDialg error.gif "Failed to initialize FLASH accesses" "FLASH init." ok
        }
        puts "-E- Error during FLASH initialization"
        # Close link
        TCL_Close $target(handle)
        exit
} else {
        puts "-I- FLASH initialized"
}

################################################################################
## DATAFLASH
################################################################################

array set at91sam7se256_dataflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91sam7se256_dataflash_scripts"
}

array set at91sam7se256_dataflash_scripts {
    "Enable Dataflash (SPI0 CS0)"                        "DATAFLASH::Init 0"
    "Set DF in Power-Of-2 Page Size mode (Binary mode)"  "DATAFLASH::BinaryPage"
    "Erase All"                                          "DATAFLASH::EraseAll"
}

set DATAFLASH::appletAddr       0x20000000
set DATAFLASH::appletFileName   "$libPath(extLib)/$target(board)/isp-dataflash-at91sam7se256.bin"

################################################################################
## SERIALFLASH
################################################################################

array set at91sam7se256_serialflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91sam7se256_serialflash_scripts"
}

array set at91sam7se256_serialflash_scripts {
    "Enable Serialflash (SPI0 CS0)"   "SERIALFLASH::Init 0"
    "Erase All"                       "SERIALFLASH::EraseAll"
}

set SERIALFLASH::appletAddr       0x20000000
set SERIALFLASH::appletFileName   "$libPath(extLib)/$target(board)/isp-serialflash-at91sam7se256.bin"

################################################################################
## NANDFLASH
################################################################################
array set at91sam7se256_nandflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91sam7se256_nandflash_scripts"
}

array set at91sam7se256_nandflash_scripts {
    "Enable NandFlash"    "NANDFLASH::Init"
    "Erase All"           "GENERIC::EraseAll"
    "Scrub NandFlash"     "GENERIC::EraseAll $NANDFLASH::scrubErase"
}
set NANDFLASH::appletAddr       0x20000000
set NANDFLASH::appletFileName   "$libPath(extLib)/$target(board)/isp-nandflash-at91sam7se256.bin"

#TCL_Write_Int $target(handle) 0x4000000 0xfffff410
#TCL_Write_Int $target(handle) 0x4000000 0xfffff430

################################################################################
## NORFLASH
################################################################################
array set at91sam7se256_norflash {
    dftDisplay  1
    dftDefault  0
    dftAddress  0x0
    dftSize     "$GENERIC::memorySize"
    dftSend     "GENERIC::SendFile"
    dftReceive  "GENERIC::ReceiveFile"
    dftScripts  "::at91sam7se256_norflash_scripts"
}

array set at91sam7se256_norflash_scripts {
    "Enable NorFlash "    "NORFLASH::Init"
    "Erase All"           "NORFLASH::EraseAll" 
}
set NORFLASH::appletAddr       0x20000000
set NORFLASH::appletFileName   "$libPath(extLib)/$target(board)/isp-norflash-at91sam7se256.bin"

################################################################################
array set at91sam7se256_norflash_map {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x10000000
    dftSize     0x10000000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91sam7se256_peripheral {
    dftDisplay  0
    dftDefault  0
    dftAddress  0xF0000000
    dftSize     0x10000000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91sam7se256_rom {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x300000
    dftSize     0x8000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}

array set at91sam7se256_remap {
    dftDisplay  0
    dftDefault  0
    dftAddress  0x00000000
    dftSize     0x8000
    dftSend     ""
    dftReceive  ""
    dftScripts  ""
}
