/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation

 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
 * 
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission. 
 * 
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \dir
/// !Purpose
/// 
/// Definition and functions for using AT91SAM9260-related features, such
/// has PIO pins, memories, etc.
/// 
/// !Usage
/// -# The code for booting the board is provided by board_cstartup.S and
///    board_lowlevel.c.
/// -# For using board PIOs, board characteristics (clock, etc.) and external
///    components, see board.h.
/// -# For manipulating memories (remapping, SDRAM, etc.), see board_memories.h.
//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------
/// \unit
/// !Purpose
/// 
/// Definition of AT91SAM9260-EK characteristics, AT91SAM9260-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM9260-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM9260-EK - PIO definitions".
/// -# Several USB definitions are included here (see "SAM9260-EK - USB device").
/// -# For external components definitions, see "SAM79260-EK - External components".
/// -# For memory-related definitions, see "SAM79260-EK - Memories".
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam9260)
    #include "at91sam9260/AT91SAM9260.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - Board Description"
/// This page lists several definition related to the board description.
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME              "AT91SAM9260-EK"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MAINOSC
/// - BOARD_MCK

/// Frequency of the board main oscillator.
#define BOARD_MAINOSC           18432000

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               ((18432000 * 97 / 9) / 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - USB device"
/// This page lists constants describing several characteristics (controller
/// type, D+ pull-up type, etc.) of the USB device controller of the chip/board.
/// 
/// !Constants
/// - BOARD_USB_UDP
/// - BOARD_USB_PULLUP_INTERNAL
/// - BOARD_USB_NUMENDPOINTS
/// - BOARD_USB_ENDPOINTS_MAXPACKETSIZE
/// - BOARD_USB_ENDPOINTS_BANKS

/// Chip has a UDP controller.
#define BOARD_USB_UDP

/// Indicates the D+ pull-up is internal to the USB controller.
#define BOARD_USB_PULLUP_INTERNAL

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS          6

/// Returns the maximum packet size of the given endpoint.
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)  (((i == 4) || (i == 5)) ? 512 : ((i == 0) ? 8 : 64))

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)          (((i == 0) || (i == 3)) ? 1 : 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - PIO definitions"
/// This pages lists all the pio definitions contained in board.h. The constants
/// are named using the following convention: PIN_* for a constant which defines
/// a single Pin instance (but may include several PIOs sharing the same
/// controller), and PINS_* for a list of Pin instances.
///
/// !DBGU
/// - PINS_DBGU
/// 
/// !LEDs
/// - PIN_LED_0
/// - PIN_LED_1
/// - PINS_LEDS
/// - LED_POWER
/// - LED_DS1
/// 
/// !Push buttons
/// - PIN_PUSHBUTTON_1
/// - PIN_PUSHBUTTON_2
/// - PINS_PUSHBUTTONS
/// - PUSHBUTTON_BP1
/// - PUSHBUTTON_BP2
/// 
/// !USART0
/// - PIN_USART0_RXD
/// - PIN_USART0_TXD
/// 
/// !SPI0
/// - PIN_SPI0_MISO
/// - PIN_SPI0_MOSI
/// - PIN_SPI0_SPCK
/// - PINS_SPI0
/// - PIN_SPI0_NPCS0
/// - PIN_SPI0_NPCS1
/// 
/// !SSC
/// - PINS_SSC_TX
/// 
/// !USB
/// - PIN_USB_VBUS
/// 
/// !MCI
/// - PINS_MCI

/// List of all DBGU pin definitions.
#define PINS_DBGU  {(1<<14) | (1<<15), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0   {1 << 9, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1   {1 << 6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// List of all LED definitions.
#define PINS_LEDS   PIN_LED_0, PIN_LED_1
/// Power LED index.
#define LED_POWER       0
/// DS1 LED index.
#define LED_DS1         1

/// Push button #1 pin definition.
#define PIN_PUSHBUTTON_1  {1 << 30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
/// Pusb button #2 pin definition.
#define PIN_PUSHBUTTON_2  {1 << 31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
/// List of all pushbutton pin definitions.
#define PINS_PUSHBUTTONS  PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2
/// Push button #1 index.
#define PUSHBUTTON_BP1   0
/// Push button #2 index.
#define PUSHBUTTON_BP2   1

/// USART0 RXD pin definition.
#define PIN_USART0_RXD  {1 << 5, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 TXD pin definition.
#define PIN_USART0_TXD  {1 << 4, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI  {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK  {1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI0      PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK	
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0 {AT91C_PA3_SPI0_NPCS0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 chip select 1 pin definition.
#define PIN_SPI0_NPCS1 {AT91C_PC11_SPI0_NPCS1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_PULLUP}

/// SSC transmitter pins definition.
#define PIN_SSC0   {0x00003F0000, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/// USB VBus monitoring pin definition.
#define PIN_USB_VBUS    {1 << 5, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}

/// List of MCI pins definitions.
#define PINS_MCI  {0x0000003B, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}, \
                  {1 << 8, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91SAM9260-EK.
/// 
/// !AT45 Dataflash Card (A)
/// - BOARD_AT45_A_SPI_BASE
/// - BOARD_AT45_A_SPI_ID
/// - BOARD_AT45_A_SPI_PINS
/// - BOARD_AT45_A_SPI
/// - BOARD_AT45_A_NPCS
/// - BOARD_AT45_A_NPCS_PIN
/// 
/// !AT45 Dataflash (B)
/// - BOARD_AT45_B_SPI_BASE
/// - BOARD_AT45_B_SPI_ID
/// - BOARD_AT45_B_SPI_PINS
/// - BOARD_AT45_B_SPI
/// - BOARD_AT45_B_NPCS
/// - BOARD_AT45_B_NPCS_PIN
/// 
/// - BOARD_AT45_SPI_BASE
/// - BOARD_AT45_SPI_ID
/// - BOARD_AT45_SPI_PIN
/// - BOARD_AT45_NPCS
/// - BOARD_AT45_NPCS_PIN
///
/// !SD Card
/// - BOARD_SD_MCI_BASE
/// - BOARD_SD_MCI_ID
/// - BOARD_SD_PINS
/// - BOARD_SD_SLOT

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_A_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_B_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_B_NPCS             1
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_B_NPCS_PIN         PIN_SPI0_NPCS1

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_PINS         PINS_SPI0
/// Serialflash SPI number.
#define BOARD_AT26_A_SPI              0
/// Chip select connected to the serialflash.
#define BOARD_AT26_A_NPCS             0
/// Chip select pin connected to the serialflash.
#define BOARD_AT26_A_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of SSC peripheral connected to the dataflash.
#define BOARD_AT45_SSC_BASE         AT91C_BASE_SSC0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_SSC_ID           AT91C_ID_SSC0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_SSC_PIN          PIN_SSC0
/// Chip select connected to the dataflash.
#define BOARD_AT45_NPCS             0
/// Base address of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_BASE           AT91C_BASE_MCI
/// Peripheral identifier of the MCI connected to the SD card.
#define BOARD_SD_MCI_ID             AT91C_ID_MCI
/// MCI pins that shall be configured to access the SD card.
#define BOARD_SD_PINS               PINS_MCI
/// MCI slot to which the SD card is connected to.
#define BOARD_SD_SLOT               MCI_SD_SLOTB
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9260-EK - Memories"
/// This page lists definitions related to external on-board memories.
/// 
/// !SDRAM
/// - BOARD_SDRAM_SIZE
/// - PINS_SDRAM
///
/// !Nandflash
/// - PINS_NANDFLASH
/// - BOARD_NF_COMMAND_ADDR
/// - BOARD_NF_ADDRESS_ADDR
/// - BOARD_NF_DATA_ADDR
/// - BOARD_NF_CE_PIN
/// - BOARD_NF_RB_PIN


/// Board SDRAM size
#define BOARD_SDRAM_SIZE   0x04000000

/// List of all SDRAM pins definitions.
#define PINS_SDRAM  {0xFFFF0000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}


/// Nandflash controller peripheral pins definition.
#define PINS_NANDFLASH         BOARD_NF_CE_PIN, BOARD_NF_RB_PIN

/// Nandflash chip enable pin definition.
#define BOARD_NF_CE_PIN        {1 << 14, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// Nandflash ready/busy pin definition.
#define BOARD_NF_RB_PIN        {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
/// Address for transferring command bytes to the nandflash.
#define BOARD_NF_COMMAND_ADDR  0x40400000
/// Address for transferring address bytes to the nandflash.
#define BOARD_NF_ADDRESS_ADDR  0x40200000
/// Address for transferring data bytes to the nandflash.
#define BOARD_NF_DATA_ADDR     0x40000000


/// Address for transferring command bytes to the norflash.
#define BOARD_NORFLASH_ADDR  0x10000000

//------------------------------------------------------------------------------

#endif //#ifndef BOARD_H

