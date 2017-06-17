/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation

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
/// Definition and functions for using AT91SAM7L-related features, such
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
/// Definition of AT91SAM7L-EK characteristics, AT91SAM7L-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM7L-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM7L-EK - PIO definitions".
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam7l64)
    #include "at91sam7l64/AT91SAM7L64.h"
#elif defined(at91sam7l128)
    #include "at91sam7l128/AT91SAM7L128.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Board Description"
/// This page lists several definition related to the board description
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME "AT91SAM7L-EK"

/// Device family
#define at91sam7l

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MCK

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               (32768 * 915)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - IAP feature"
/// This page lists several definition related to the IAP feature
/// 
/// !Definitions
/// - IAP_FUNC_ADDR

/// IAP function address is located in ROM at SWI vector address
#define IAP_FUNC_ADDR 0x400008
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - PIO definitions"
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
/// !USART
/// - PIN_USART0_RXD
/// - PIN_USART0_TXD
/// 
/// !USB
/// - PIN_USB_VBUS
/// - PIN_USB_PULLUP
/// 
/// !Keyboard Matrix
/// - PIN_KBMATRIX_ROW_0
/// - PIN_KBMATRIX_ROW_1
/// - PIN_KBMATRIX_ROW_2
/// - PIN_KBMATRIX_ROW_3
/// - PIN_KBMATRIX_ROW_4
/// - PINS_KBMATRIX_ROWS
/// - PIN_KBMATRIX_COL_0
/// - PIN_KBMATRIX_COL_1
/// - PIN_KBMATRIX_COL_2
/// - PIN_KBMATRIX_COL_3
/// - PIN_KBMATRIX_COL_4
/// - PIN_KBMATRIX_COL_5
/// - PIN_KBMATRIX_COL_6
/// - PINS_KBMATRIX_COLS

/// DBGU pins (DTXD and DRXD) definitions.
#define PINS_DBGU   {0x00030000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0  {1 << 7, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1  {1 << 8, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #2 pin definition.
#define PIN_LED_2  {1 << 9, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of all led definitions.
#define PINS_LEDS  PIN_LED_0, PIN_LED_1, PIN_LED_2

/// LED DS1 index.
#define LED_DS1     0
/// LED DS2 index.
#define LED_DS2     1
/// Power LED index.
#define LED_POWER   2

/// Push button #1 pin definition
#define PIN_PUSHBUTTON_1 {1 << 0, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #2 pin definition
#define PIN_PUSHBUTTON_2 {1 << 1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// List of all pushbuttons definitions.
#define PINS_PUSHBUTTONS PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2

/// Keyboard column #0 pin definition.
#define PIN_KEYBOARD_COL0 {1 << 10, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}

/// Pushbutton 1 index
#define PUSHBUTTON_BP1  0
/// Pushbutton 2 index
#define PUSHBUTTON_BP2  1

/// USART0 TXD pin definition.
#define PIN_USART0_RXD  {1 << 12, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
#define PIN_USART0_TXD  {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}

/// Keyboard rows ( scanning output )
#define PIN_KBMATRIX_ROW_0  {1 <<  0, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_1  {1 <<  1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_2  {1 <<  2, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_3  {1 <<  3, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_4  {1 <<  4, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PINS_KBMATRIX_ROWS  PIN_KBMATRIX_ROW_0, PIN_KBMATRIX_ROW_1, PIN_KBMATRIX_ROW_2, \
                            PIN_KBMATRIX_ROW_3, PIN_KBMATRIX_ROW_4

/// Keyboard columns ( capture input )
#define PIN_KBMATRIX_COL_0   {1 << 10, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_1   {1 << 11, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_2   {1 << 25, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_3   {1 << 26, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_4   {1 << 27, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_5   {1 << 28, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_6   {1 << 29, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PINS_KBMATRIX_COLS   PIN_KBMATRIX_COL_0, PIN_KBMATRIX_COL_1, PIN_KBMATRIX_COL_2, \
                             PIN_KBMATRIX_COL_3, PIN_KBMATRIX_COL_4, PIN_KBMATRIX_COL_5, \
                             PIN_KBMATRIX_COL_6


//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Memories"
/// This page lists definitions related to internal memories.
///
/// !Embedded Flash
/// - BOARD_FLASH_EEFC
/// - BOARD_FLASH_IAP_ADDRESS
///
/// !Internal SRAM
/// - AT91C_ISRAM
/// - AT91C_ISRAM_SIZE

/// Indicates that chip has an Enhanced EFC.
#define BOARD_FLASH_EEFC
/// Address of the IAP function in ROM.
#define BOARD_FLASH_IAP_ADDRESS         0x400008

// Internal SRAM
#define AT91C_ISRAM          (0x002FF000) // Internal SRAM base address for contiguous access
#define AT91C_ISRAM_SIZE     (AT91C_ISRAM_1_SIZE + AT91C_ISRAM_2_SIZE) // Internal SRAM total size


//------------------------------------------------------------------------------

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

