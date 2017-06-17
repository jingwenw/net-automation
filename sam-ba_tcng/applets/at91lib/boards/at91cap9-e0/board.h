/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation
 *
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
/// Definition and functions for using AT91CAP9-related features, such
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
/// Definition of AT91CAP9-DK characteristics, AT91CAP9-dependant PIOs and
/// external components interfacing.
///
/// !Usage
/// -# For operating frequency information, see "AT91CAP9-DK - Operating frequencies".
/// -# For using portable PIO definitions, see "AT91CAP9-DK - PIO definitions".
/// -# Several USB definitions are included here (see "AT91CAP9-DK - USB device").
/// -# For external components definitions, see "AT91CAP9-DK - External components".
/// -# For memory-related definitions, see "AT91CAP9-DK - Memories".
//------------------------------------------------------------------------------

#ifndef BOARD_H
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91cap9)
    #include "at91cap9/at91cap9.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - Board Description"
/// This page lists several definition related to the board description
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME "AT91CAP9-DK"
//------------------------------------------------------------------------------

#define AT91C_ID_PIOA AT91C_ID_PIOABCD
#define AT91C_ID_PIOB AT91C_ID_PIOABCD
#define AT91C_ID_PIOC AT91C_ID_PIOABCD
#define AT91C_ID_PIOD AT91C_ID_PIOABCD

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
///
/// !Definitions
/// - BOARD_MAINOSC
/// - BOARD_MCK

/// Frequency of the board main oscillator.
#define BOARD_MAINOSC           12000000

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               ((12000000*50/3)/2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - USB device"
/// This page lists constants describing several characteristics (controller
/// type, D+ pull-up type, etc.) of the USB device controller of the chip/board.
///
/// !Constants
/// - BOARD_USB_UDPHS
/// - BOARD_USB_PULLUP_INTERNAL
/// - BOARD_USB_NUMENDPOINTS
/// - BOARD_USB_ENDPOINTS_MAXPACKETSIZE
/// - BOARD_USB_ENDPOINTS_BANKS

/// Chip has a UDPHS controller.
#define BOARD_USB_UDPHS

/// Indicates the D+ pull-up is internal to the USB controller.
#define BOARD_USB_PULLUP_INTERNAL

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS                  7

/// Returns the maximum packet size of the given endpoint.
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)    ((i == 0) ? 64 : 512)
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE_FS    64

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)            ((i == 0) ? 1 : ((i == 1) || (i == 2)) ? 2 : 3)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - PIO definitions"
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
/// - PINS_LEDS
/// - LED_POWER
/// - LED_DS1
/// - LED_DS2
///
/// !Push buttons
/// - PIN_PUSHBUTTON_1
/// - PIN_PUSHBUTTON_2
/// - PINS_PUSHBUTTONS
/// - PUSHBUTTON_BP1
/// - PUSHBUTTON_BP2
///
/// !SPI
/// - PIN_SPI_MISO
/// - PIN_SPI_MOSI
/// - PIN_SPI_SPCK
/// - PINS_SPI
/// - PIN_SPI_NPCS0
///
/// !SSC
/// - PINS_SSC_TRANSMITTER
///
/// !USB
/// - PIN_USB_VBUS

/// List of all DBGU pin definitions.
//#define PIN_DBGU PINS_DBGU
#define PINS_DBGU   {(1<<30)|(1<<31), AT91C_BASE_PIOC, AT91C_ID_PIOABCD, PIO_PERIPH_A, PIO_DEFAULT}
/// LED #0 pin definition.
#define PIN_LED_0  {AT91C_PIO_PA10, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1  {AT91C_PIO_PA11, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #2 pin definition.
//#define PIN_LED_2  {AT91C_PIO_PA11, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_OUTPUT_0, PIO_DEFAULT}
/// List of all LEDs definitions.
#define PINS_LEDS  PIN_LED_0, PIN_LED_1

/// Power LED index
#define LED_POWER   0
/// DS1 LED index.
#define LED_DS1     1
/// DS2 LED index.
#define LED_DS2     2

/// Push button #1 pin definition.
#define PIN_PUSHBUTTON_1  {1 << 9, AT91C_BASE_PIOD, AT91C_ID_PIOABCD, PIO_INPUT, PIO_PULLUP}
/// Push button #2 pin definition.
#define PIN_PUSHBUTTON_2  {1 << 8, AT91C_BASE_PIOD, AT91C_ID_PIOABCD, PIO_INPUT, PIO_PULLUP}
/// List of push button pin definitions.
#define PINS_PUSHBUTTONS  PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2
/// Push button #1 index.
#define PUSHBUTTON_BP1   0
/// Push button #2 index.
#define PUSHBUTTON_BP2   1

/// Keyboard row #0 pin definition.
#define PIN_KEYBOARD_ROW0 {1 << 10, AT91C_BASE_PIOD, AT91C_ID_PIOABCD, PIO_OUTPUT_0, PIO_DEFAULT}
/// Keyboard row #2 pin definition.
#define PIN_KEYBOARD_ROW2 {1 << 12, AT91C_BASE_PIOD, AT91C_ID_PIOABCD, PIO_INPUT, PIO_PULLUP} // workaround
/// Keyboard row #3 pin definition.
#define PIN_KEYBOARD_ROW3 {1 << 13, AT91C_BASE_PIOD, AT91C_ID_PIOABCD, PIO_INPUT, PIO_PULLUP} // workaround

/// USART0 TXD pin definition.
//#define PIN_USART0_RXD  {1 << 7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
//#define PIN_USART0_TXD  {1 << 6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI MISO pin definition.
#define PIN_SPI_MISO  {AT91C_PA0_SPI0_MISO, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_PULLUP}
/// SPI MOSI pin definition.
#define PIN_SPI_MOSI  {AT91C_PA1_SPI0_MOSI, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_DEFAULT}
/// SPI SPCK pin definition.
#define PIN_SPI_SPCK  {AT91C_PA2_SPI0_SPCK, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_DEFAULT}
/// List of SPI pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI      PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK
/// SPI chip select 0 pin definition.
#define PIN_SPI0_NPCS0 {AT91C_PA5_SPI0_NPCS0, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_DEFAULT}
/// SPI chip select 1 pin definition.
#define PIN_SPI0_NPCS1 {AT91C_PA3_SPI0_NPCS1, AT91C_BASE_PIOA, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_DEFAULT}

/// SSC transmitter pins definition.
/*
#define PINS_SSC_TX  {0x00038000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SSC0_RD  {AT91C_PA16_RD0 , AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_SSC0_TD  {AT91C_PA15_TD0 , AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SSC0_TK  {AT91C_PC1_TK0 , AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SSC0_TF  {AT91C_PC0_TF0 , AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SSC0   PIN_SSC0_RD, PIN_SSC0_TD, PIN_SSC0_TK, PIN_SSC0_TF
*/

/// USB VBus monitoring pin definition.
#define PIN_USB_VBUS    {1 << 31, AT91C_BASE_PIOB, AT91C_ID_PIOABCD, PIO_INPUT, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91AT91CAP9-DK.
///
/// !AT45 Dataflash
/// - BOARD_AT45_A_SPI_BASE
/// - BOARD_AT45_A_SPI_ID
/// - BOARD_AT45_A_SPI_PINS
/// - BOARD_AT45_A_SPI
/// - BOARD_AT45_A_NPCS
/// - BOARD_AT45_A_NPCS_PIN
///
/// !AT26 Serial Flash
/// - BOARD_AT26_A_SPI_BASE
/// - BOARD_AT26_A_SPI_ID
/// - BOARD_AT26_A_SPI_PINS
/// - BOARD_AT26_A_SPI
/// - BOARD_AT26_A_NPCS
/// - BOARD_AT26_A_NPCS_PIN
///
/// !LCD
/// - PINS_LCD
/// - BOARD_LCD_WIDTH
/// - BOARD_LCD_HEIGHT
/// - BOARD_LCD_BPP
/// - BOARD_LCD_IFWIDTH
/// - BOARD_LCD_FRAMESIZE_PIXELS
/// - BOARD_LCD_FRAMESIZE
/// - BOARD_LCD_FRAMERATE
/// - BOARD_LCD_PIXELCLOCK
/// - BOARD_LCD_DISPLAYTYPE
/// - BOARD_LCD_POLARITY_INVVD
/// - BOARD_LCD_POLARITY_INVFRAME
/// - BOARD_LCD_POLARITY_INVLINE
/// - BOARD_LCD_POLARITY_INVCLK
/// - BOARD_LCD_POLARITY_INVDVAL
/// - BOARD_LCD_CLOCKMODE
/// - BOARD_LCD_TIMING_VFP
/// - BOARD_LCD_TIMING_VBP
/// - BOARD_LCD_TIMING_VPW
/// - BOARD_LCD_TIMING_VHDLY
/// - BOARD_LCD_TIMING_HFP
/// - BOARD_LCD_TIMING_HBP
/// - BOARD_LCD_TIMING_HPW

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_PINS         PINS_SPI
/// Dataflahs SPI number.
#define BOARD_AT45_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_A_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_PINS         PINS_SPI
/// Serialflash SPI number.
#define BOARD_AT26_A_SPI              0
/// Chip select connected to the serialflash.
#define BOARD_AT26_A_NPCS             0
/// Chip select pin connected to the serialflash.
#define BOARD_AT26_A_NPCS_PIN         PIN_SPI0_NPCS0
#define BOARD_AT26_A_NPCS0_PIN        PIN_SPI0_NPCS0
#define BOARD_AT26_A_NPCS1_PIN        PIN_SPI0_NPCS1

/// Base address of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_BASE           AT91C_BASE_MCI0
/// Peripheral identifier of the MCI connected to the SD card.
#define BOARD_SD_MCI_ID             AT91C_ID_MCI0
/// MCI pins that shall be configured to access the SD card.
#define BOARD_SD_PINS               PINS_MCI
/// MCI slot to which the SD card is connected to.
#define BOARD_SD_SLOT               MCI_SD_SLOTB

/// List of MCI pins definitions.
#define PINS_MCI  {0x0000003F, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// TWI0 pins definition.
#define PINS_TWI  {(1<<4) | (1<<5), AT91C_BASE_PIOB, AT91C_ID_PIOABCD, PIO_PERIPH_B, PIO_DEFAULT}

/*
/// LCD pin list.
#define PINS_LCD    \
    {0x000000E8, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}, \
    {0x03FFBE02, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT}
/// Display width in pixels.
#define BOARD_LCD_WIDTH             240
/// Display height in pixels.
#define BOARD_LCD_HEIGHT            320
/// Display resolution in bits per pixel (bpp).
#define BOARD_LCD_BPP               AT91C_LCDC_PIXELSIZE_TWENTYFOURBITSPERPIXEL
/// Display interface width in bits.
#define BOARD_LCD_IFWIDTH           24
/// Frame size in pixels (height * width * bpp).
#define BOARD_LCD_FRAMESIZE_PIXELS  (BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 24)
/// Frame size in words (height * width * bpp / 32)
#define BOARD_LCD_FRAMESIZE         (BOARD_LCD_FRAMESIZE_PIXELS / 32)
/// Frame rate in Hz.
#define BOARD_LCD_FRAMERATE         60
/// Pixel clock rate in Hz (frameSize * frameRate / interfaceWidth).
#define BOARD_LCD_PIXELCLOCK        (BOARD_LCD_FRAMESIZE_PIXELS * BOARD_LCD_FRAMERATE / BOARD_LCD_IFWIDTH)
/// LCD display type.
#define BOARD_LCD_DISPLAYTYPE       AT91C_LCDC_DISTYPE_TFT
/// LCDC polarity.
#define BOARD_LCD_POLARITY_INVVD    AT91C_LCDC_INVVD_NORMALPOL
/// LCDVSYNC polarity.
#define BOARD_LCD_POLARITY_INVFRAME AT91C_LCDC_INVFRAME_NORMALPOL
/// LCDHSYNC polarity.
#define BOARD_LCD_POLARITY_INVLINE  AT91C_LCDC_INVLINE_NORMALPOL
/// LCDDOTCLK polarity.
#define BOARD_LCD_POLARITY_INVCLK   AT91C_LCDC_INVCLK_NORMALPOL
/// LCDDEN polarity.
#define BOARD_LCD_POLARITY_INVDVAL  AT91C_LCDC_INVDVAL_NORMALPOL
/// Pixel clock mode.
#define BOARD_LCD_CLOCKMODE         AT91C_LCDC_CLKMOD_ALWAYSACTIVE
/// Vertical front porch in number of lines.
#define BOARD_LCD_TIMING_VFP        2
/// Vertical back porch in number of lines.
#define BOARD_LCD_TIMING_VBP        4
/// Vertical pulse width in LCDDOTCLK cycles.
#define BOARD_LCD_TIMING_VPW        1
/// Number of cycles between VSYNC edge and HSYNC rising edge.
#define BOARD_LCD_TIMING_VHDLY      1
/// Horizontal front porch in LCDDOTCLK cycles.
#define BOARD_LCD_TIMING_HFP        11
/// Horizontal back porch in LCDDOTCLK cycles.
#define BOARD_LCD_TIMING_HBP        17
/// Horizontal pulse width in LCDDOTCLK cycles.
#define BOARD_LCD_TIMING_HPW        5
*/
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "AT91CAP9-DK - Memories"
/// This page lists definitions related to external on-board memories.
///
/// !SDRAM
/// - BOARD_SDRAM_SIZE
/// - PIN_SDRAM
///
/// !Nandflash
/// - PINS_NANDFLASH
/// - BOARD_NF_EBI_COMMAND_ADDR
/// - BOARD_NF_EBI_ADDRESS_ADDR
/// - BOARD_NF_EBI_DATA_ADDR
/// - BOARD_NF_CE_PIN
/// - BOARD_NF_RB_PIN

/// Board SDRAM size
#ifdef SDRAM_SIZE_32MB
#define BOARD_SDRAM_SIZE   0x02000000  // 32 MB 
#else
#define BOARD_SDRAM_SIZE   0x01000000  // 16 MB 
#endif

/// Board SDRAM size
#define BOARD_DDRAM_SIZE   0x02000000  // with AT91CAP-MEM18 extension

/// List of all SDRAM pins definitions.
#define PINS_SDRAM  {0xFFFF0000, AT91C_BASE_PIOD, AT91C_ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}

/// Nandflash controller peripheral pins definition.
#define PINS_NANDFLASH          BOARD_NF_CE_PIN

/// Nandflash output enable pin definition.
//#define PIN_NF_OE              // => dedicated pin
/// Nandflash write enable pin definition
//#define PIN_NF_WE              // => dedicated pin
/// Nandflash chip enable pin definition.
#define BOARD_NF_CE_PIN        {1 << 15, AT91C_BASE_PIOD, AT91C_ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
/// Nandflash ready/busy pin definition.
#define BOARD_NF_RB_PIN        { 0, 0, 0, 0, 0}// Not used on the DK board. Use Read Status Register Command
/// Address for transferring command bytes to the nandflash.
#define BOARD_NF_COMMAND_ADDR  0x40400000
/// Address for transferring address bytes to the nandflash.
#define BOARD_NF_ADDRESS_ADDR  0x40200000
/// Address for transferring data bytes to the nandflash.
#define BOARD_NF_DATA_ADDR     0x40000000
/// Address for transferring command bytes to the norflash.
#define BOARD_NORFLASH_ADDR  0x10000000

//------------------------------------------------------------------------------

// NOT DEFINED IN AT91CAP9
/*
/// USB attributes configuration descriptor (bus or self powered, remote wakeup)
#define BOARD_USB_BMATTRIBUTES                  USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP
/// Source type definition
#define AIC_SRCTYPE   AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL
*/

//------------------------------------------------------------------------------
//         Library naming workaround
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//    Constants: Operating frequencies
//        AT91C_AIC_SRCTYPE_INT - Interrupt triggering mode.
//------------------------------------------------------------------------------
#if defined(at91cap9)
#define AT91C_AIC_SRCTYPE_INT  AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL
#elif defined(at91cap9_umc)
#define AT91C_AIC_SRCTYPE_INT  AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE
#else
    #error Board does not support the specified chip.
#endif

// AT91CAP9.h
// EBI_DDRAM
#define AT91C_EBI_DDRAM  (AT91_CAST(char *)     0x70000000) // DDRAM on EBI Chip Select 6 base address
#define AT91C_EBI_DDRAM_SIZE     (0x10000000) // DDRAM on EBI Chip Select 6 size in byte (262144 Kbytes)

#endif //#ifndef BOARD_H



















