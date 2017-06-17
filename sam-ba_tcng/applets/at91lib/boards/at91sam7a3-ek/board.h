/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
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
/// Definition and functions for using AT91SAM7A3-related features, such
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
/// Definition of AT91SAM7A3-EK characteristics, AT91SAM7A3-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM7A3-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM7A3-EK - PIO definitions".
/// -# Several USB definitions are included here (see "SAM7A3-EK - USB device").
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam7a3)
    #include "at91sam7a3/AT91SAM7A3.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - Board Description"
/// This page lists several definition related to the board description.
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME "AT91SAM7A3-EK"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MAINOSC
/// - BOARD_MCK

/// Frequency of the board main oscillator.
#define BOARD_MAINOSC           18432000

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               48000000
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - USB device"
/// This page lists constants describing several characteristics (controller
/// type, D+ pull-up type, etc.) of the USB device controller of the chip/board.
/// 
/// !Constants
/// - BOARD_USB_UDP
/// - BOARD_USB_PULLUP_EXTERNAL
/// - BOARD_USB_NUMENDPOINTS
/// - BOARD_USB_ENDPOINTS_MAXPACKETSIZE
/// - BOARD_USB_ENDPOINTS_BANKS

/// Chip has a UDP controller.
#define BOARD_USB_UDP

/// Indicates the D+ pull-up is external.
#define BOARD_USB_PULLUP_EXTERNAL

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS          6

/// Returns the maximum packet size of the given endpoint.
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)  ((i == 0) ? 8 : ((i == 4 || i == 5) ? 512 : 64))

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)          (((i == 0) || (i == 3)) ? 1 : 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - PIO definitions"
/// This pages lists all the pio definitions contained in board.h. The constants
/// are named using the following convention: PIN_* for a constant which defines
/// a single Pin instance (but may include several PIOs sharing the same
/// controller), and PINS_* for a list of Pin instances.
///  
/// !DBGU
/// - PINS_DBGU
/// 
/// !LEDs
/// - PIN_LED_DS1
/// - PIN_LED_DS2
/// - PIN_LED_DS3
/// - PIN_LED_DS4
/// - PINS_LEDS
/// - LED_DS1
/// - LED_DS2
/// - LED_DS3
/// - LED_DS4
///
/// !Push buttons
/// - PIN_PUSHBUTTON_1
/// - PIN_PUSHBUTTON_2
/// - PIN_PUSHBUTTON_3
/// - PIN_PUSHBUTTON_4
/// - PINS_PUSHBUTTONS
/// - PUSHBUTTON_BP1
/// - PUSHBUTTON_BP2 
/// - PUSHBUTTON_BP3
/// - PUSHBUTTON_BP4
///
/// !USART0
/// - PIN_USART0_RXD
/// - PIN_USART0_TXD
/// - PIN_USART0_SCK
/// 
/// !SPI0
/// - PIN_SPI0_MISO
/// - PIN_SPI0_MOSI
/// - PIN_SPI0_SPCK
/// - PINS_SPI0
/// - PIN_SPI0_NPCS3
/// 
/// !SPI1
/// - PIN_SPI1_MISO
/// - PIN_SPI1_MOSI
/// - PIN_SPI1_SPCK
/// - PINS_SPI1
/// - PIN_SPI1_NPCS3
/// 
/// !USB
/// - PIN_USB_VBUS
/// - PIN_USB_PULLUP

/// DBGU pins (DTXD and DRXD) definitions.
#define PINS_DBGU  {0xC0000000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_DS1   {1 << 20, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_DS2   {1 << 21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #2 pin definition.
#define PIN_LED_DS3   {1 << 24, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #3 pin definition.
#define PIN_LED_DS4   {1 << 25, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of the four LED pin definitions.
#define PINS_LEDS   PIN_LED_DS1, PIN_LED_DS2, PIN_LED_DS3, PIN_LED_DS4

/// LED DS1 index.
#define LED_DS1      0
/// LED DS2 index.
#define LED_DS2      1
/// LED DS3 index.
#define LED_DS3      2
/// LED DS4 index.
#define LED_DS4      3

/// Push button #0 definition.
#define PIN_PUSHBUTTON_1    {1 << 13, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #1 definition.
#define PIN_PUSHBUTTON_2    {1 << 12, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #2 definition
#define PIN_PUSHBUTTON_3    {1 << 8, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #3 definition.
#define PIN_PUSHBUTTON_4    {1 << 9, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// List of all push button definitions.
#define PINS_PUSHBUTTONS    PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2, PIN_PUSHBUTTON_3, PIN_PUSHBUTTON_4

/// Push button #1 index.
#define PUSHBUTTON_BP1   0
/// Push button #2 index.
#define PUSHBUTTON_BP2   1
/// Push button #3 index.
#define PUSHBUTTON_BP3   2
/// Push button #4 index.
#define PUSHBUTTON_BP4   3

/// USART0 TXD pin definition.
#define PIN_USART0_RXD  {1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
#define PIN_USART0_TXD  {1 << 3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SCK0 pin definition
#define PIN_USART0_SCK  {1 << 4, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO   {1 << 15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI   {1 << 16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK   {1 << 17, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI0       PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK 
/// SPI0 chip select 3 pin definition.
#define PIN_SPI0_NPCS3  {1 << 14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI1 MISO pin definition.
#define PIN_SPI1_MISO   {1 << 8, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
/// SPI1 MOSI pin definition.
#define PIN_SPI1_MOSI   {1 << 9, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/// SPI1 SPCK pin definition.
#define PIN_SPI1_SPCK   {1 << 10, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/// List of SPI1 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI1       PIN_SPI1_MISO, PIN_SPI1_MOSI, PIN_SPI1_SPCK
/// SPI1 chip select 3 pin definition.
#define PIN_SPI1_NPCS3  {1 << 7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

/// USB pull-up control pin definition.
#define PIN_USB_PULLUP  {1 << 1, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_1, PIO_PULLUP}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91SAM7A3-EK.
/// 
/// !AT45 Dataflash Card
/// - BOARD_AT45_A_SPI_BASE
/// - BOARD_AT45_A_SPI_ID
/// - BOARD_AT45_A_SPI_PINS
/// - BOARD_AT45_A_SPI
/// - BOARD_AT45_A_NPCS
/// - BOARD_AT45_A_NPCS_PIN
///
/// !AT45 Dataflash (serial onboard DataFlash)
/// - BOARD_AT45_B_SPI_BASE
/// - BOARD_AT45_B_SPI_ID
/// - BOARD_AT45_B_SPI_PINS
/// - BOARD_AT45_B_SPI
/// - BOARD_AT45_B_NPCS
/// - BOARD_AT45_B_NPCS_PIN
///
/// !AT26 Serial Flash
/// - BOARD_AT26_A_SPI_BASE
/// - BOARD_AT26_A_SPI_ID
/// - BOARD_AT26_A_SPI_PINS
/// - BOARD_AT26_A_SPI
/// - BOARD_AT26_A_NPCS
/// - BOARD_AT26_A_NPCS_PIN

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_A_NPCS             3
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI0_NPCS3

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_BASE         AT91C_BASE_SPI1
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_ID           AT91C_ID_SPI1
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_PINS         PINS_SPI1
/// Dataflahs SPI number.
#define BOARD_AT45_B_SPI              1
/// Chip select connected to the dataflash.
#define BOARD_AT45_B_NPCS             3
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_B_NPCS_PIN         PIN_SPI1_NPCS3

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_PINS         PINS_SPI0
/// Serialflash SPI number.
#define BOARD_AT26_A_SPI              0
/// Chip select connected to the serialflash.
#define BOARD_AT26_A_NPCS             3
/// Chip select pin connected to the serialflash.
#define BOARD_AT26_A_NPCS_PIN         PIN_SPI0_NPCS3

//------------------------------------------------------------------------------
/// \page "SAM7A3-EK - Memories"
/// This page lists definitions related to internal & external on-board memories.
/// 
/// !Embedded Flash
/// - BOARD_FLASH_EFC

/// Indicates chip has an EFC.
#define BOARD_FLASH_EFC
//------------------------------------------------------------------------------

#endif //#ifndef BOARD_H


