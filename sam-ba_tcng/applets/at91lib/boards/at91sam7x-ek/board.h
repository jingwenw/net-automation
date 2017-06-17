/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  SDC  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation

 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
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
/// Definition and functions for using AT91SAM7X-related features, such
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
/// Definition of AT91SAM7X-EK characteristics, AT91SAM7X-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM7X-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM7X-EK - PIO definitions".
/// -# Several USB definitions are included here (see "SAM7X-EK - USB device").
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam7x128)
    #include "at91sam7x128/AT91SAM7X128.h"
#elif defined(at91sam7x256)
    #include "at91sam7x256/AT91SAM7X256.h"
#elif defined(at91sam7x512)
    #include "at91sam7x512/AT91SAM7X512.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7X-EK - Board Description"
/// This page lists several definition related to the board description
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME "AT91SAM7X-EK"

/// Device family
#define at91sam7x

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7X-EK - Operating frequencies"
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
/// \page "SAM7X-EK - USB device"
/// This page lists constants describing several characteristics (controller
/// type, D+ pull-up type, etc.) of the USB device controller of the chip/board.
/// 
/// !Constants
/// - BOARD_USB_UDP
/// - BOARD_USB_PULLUP_ALWAYSON
/// - BOARD_USB_NUMENDPOINTS
/// - BOARD_USB_ENDPOINTS_MAXPACKETSIZE
/// - BOARD_USB_ENDPOINTS_BANKS

/// Chip has a UDP controller.
#define BOARD_USB_UDP

/// Indicates the D+ pull-up is always connected.
#define BOARD_USB_PULLUP_ALWAYSON

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS          6

/// Returns the maximum packet size of the given endpoint.
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)  (((i == 4) || (i == 5)) ? 256 : ((i == 0) ? 8 : 64))

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)          (((i == 0) || (i == 3)) ? 1 : 2)


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7X-EK - PIO definitions"
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
/// - PIN_LED_2
/// - PIN_LED_3
/// - PINS_LEDS
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
/// - LED_DS1
/// - LED_DS2
/// - LED_DS3
/// - LED_DS4
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

/// List of all DBGU pin definitions.
#define PINS_DBGU  {0x18000000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0  {1 << 19, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1  {1 << 20, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #2 pin definition.
#define PIN_LED_2  {1 << 21, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #3 pin definition.
#define PIN_LED_3  {1 << 22, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// List of all LEDs pin definitions.
#define PINS_LEDS  PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3
/// LED DS1 index.
#define LED_DS1      0
/// LED DS2 index.
#define LED_DS2      1
/// LED DS3 index.
#define LED_DS3      2
/// LED DS3 index.
#define LED_DS4      3

/// Push button #0 definition.
#define PIN_PUSHBUTTON_1    {1 << 23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #1 definition.
#define PIN_PUSHBUTTON_2    {1 << 24, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #2 definition
#define PIN_PUSHBUTTON_3    {1 << 21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #3 definition.
#define PIN_PUSHBUTTON_4    {1 << 22, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
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

/// USART0 RXD pin definition.
#define PIN_USART0_RXD  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 TXD pin definition.
#define PIN_USART0_TXD  {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO   {1 << 16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI   {1 << 17, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK   {1 << 18, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI0       PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0  {1 << 12, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 chip select 1 pin definition.
#define PIN_SPI0_NPCS1  {1 << 13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7X-EK - External components"
/// This page lists definitions related to external on-board components.
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
#define BOARD_AT45_A_NPCS             1
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI0_NPCS1

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_B_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_B_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_B_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_PINS         PINS_SPI0
/// Serialflash SPI number.
#define BOARD_AT26_A_SPI              0
/// Chip select connected to the serialflash.
#define BOARD_AT26_A_NPCS             1
/// Chip select pin connected to the serialflash.
#define BOARD_AT26_A_NPCS_PIN         PIN_SPI0_NPCS1


//------------------------------------------------------------------------------
/// \page "SAM7X-EK - Embedded Flash Controller definitions"
/// This page contains definitions about the EFC. 
///
/// !EFC
/// - BOARD_FLASH_EFC
/// - BOARD_EFC_WAITSTATES
/// - BOARD_EFC_NUM_CYCLES

/// Indicates that chip has an normal EFC.
#define BOARD_FLASH_EFC

/// Embedded Flash controller base address 
#if defined(AT91C_BASE_EFC1)
    #define BOARD_DOUBLE_EFC     AT91C_BASE_EFC0, AT91C_BASE_EFC1
#else
    #if defined(AT91C_BASE_EFC0)    
        #define BOARD_DOUBLE_EFC AT91C_BASE_EFC0
    #else
        #define BOARD_DOUBLE_EFC AT91C_BASE_EFC
    #endif      
#endif

/// Flash Wait state cycles.
#define BOARD_EFC_WAITSTATES  1
#define BOARD_EFC_NUM_CYCLES  0x48


//------------------------------------------------------------------------------
/// \page "SAM7X-EK - ROM code related definitions"
/// This page contains information about ROM code. 
///
/// !ROM_CODE
/// - BOARD_ROMCODE_IN_RAM_SIZE

/// Size of ROM code executiong in internal SRAM
#define BOARD_ROMCODE_IN_RAM_SIZE 0x2000


//------------------------------------------------------------------------------
#endif //#ifndef BOARD_H

