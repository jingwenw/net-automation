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
/// Definition and functions for using AT91SAM9263-related features, such
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
/// Definition of AT91SAM9263-EK characteristics, AT91SAM9263-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM9263-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM9263-EK - PIO definitions".
/// -# Several USB definitions are included here (see "SAM9263-EK - USB device").
/// -# For external components definitions, see "SAM79263-EK - External components".
/// -# For memory-related definitions, see "SAM79263-EK - Memories".
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam9263)
    #include "at91sam9263/AT91SAM9263.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - Board Description"
/// This page lists several definition related to the board description.
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME              "AT91SAM9263-EK"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MAINOSC
/// - BOARD_MCK

/// Frequency in Hz of the board main oscillator.
#define BOARD_MAINOSC           16367660
/// Master clock frequency in Hz when using the board default LowLevelInit
#define BOARD_MCK               ((16367660 * 110 / 9) / 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - USB device"
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
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)  ((i >= 4) ? 256 : 64)

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)          (((i == 0) || (i == 3)) ? 1 : 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - PIO definitions"
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
/// - PIN_SPI0_NPCS3
/// 
/// !SSC
/// - PINS_SSC_TX
/// 
/// !AC97
/// - PINS_AC97
/// 
/// !USB
/// - PIN_USB_VBUS
/// 
/// !MCI
/// - PINS_MCI
/// 
/// !AC97
/// - PINS_AC97

/// List of all DBGU pin definitions.
#define PINS_DBGU  {(1<<30) | (1<<31), AT91C_BASE_PIOC, AT91C_ID_PIOCDE, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0    {1 << 7, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1    {1 << 8, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #2 pin definition.
#define PIN_LED_2    {1 << 29, AT91C_BASE_PIOC, AT91C_ID_PIOCDE, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of all LEDs definitions
#define PINS_LEDS  PIN_LED_0, PIN_LED_1, PIN_LED_2
/// Power LED index.
#define LED_POWER    0
/// DS1 LED index.
#define LED_DS1      1
/// DS2 LED index.
#define LED_DS2      2

/// Push button #1 pin definition.
#define PIN_PUSHBUTTON_1  {1 << 4, AT91C_BASE_PIOC, AT91C_ID_PIOCDE, PIO_INPUT, PIO_PULLUP}
/// Push button #2 pin definition.
#define PIN_PUSHBUTTON_2  {1 << 5, AT91C_BASE_PIOC, AT91C_ID_PIOCDE, PIO_INPUT, PIO_PULLUP}
/// List of all pushbutton pin definitions.
#define PINS_PUSHBUTTONS  PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2
/// Push button #1 index.
#define PUSHBUTTON_BP1   0
/// Push button #2 index.
#define PUSHBUTTON_BP2   1

/// USART0 TXD pin definition.
#define PIN_USART0_RXD  {1 << 26, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
#define PIN_USART0_TXD  {1 << 27, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI  {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK  {1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
/// SPI0 peripheral pins definition (includes SPCK, MISO & MOSI).
#define PINS_SPI0      PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0 {AT91C_PA5_SPI0_NPCS0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
/// SPI0 chip select 3 pin definition.
#define PIN_SPI0_NPCS3 {1 << 11, AT91C_BASE_PIOB, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_PULLUP}
	
/// SSC transmitter pins definition.
#define PINS_SSC_TX  {0x00038000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// USB VBus monitoring pin definition.
#define PIN_USB_VBUS    {1 << 25, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}

/// List of MCI peripheral pins definitions.
#define PINS_MCI     {0x0000103B, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}	

/// AC97 pins definition.
#define PINS_AC97   {0x0000000F, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91SAM9263-EK.
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
/// 
/// !SD Card
/// - BOARD_SD_MCI_BASE
/// - BOARD_SD_MCI_ID
/// - BOARD_SD_PINS
/// - BOARD_SD_PIN_CD
/// - BOARD_SD_PIN_WP
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


/// LCD pins list.
#define PINS_LCD    \
    {0x0DCDCFCE, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}, \
    {0x00021000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT}, \
    {1 << 9, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}
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

/// Base address of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_BASE   AT91C_BASE_MCI0
/// Identifier of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_ID     AT91C_ID_MCI0
/// List of pins to configure to access the SD card
#define BOARD_SD_PINS       PINS_MCI, {AT91C_PIO_PE20, AT91C_BASE_PIOE, AT91C_ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT}
/// SD card detection pin definition.
#define BOARD_SD_PIN_CD     {AT91C_PIO_PE16, AT91C_BASE_PIOE, AT91C_ID_PIOE, PIO_INPUT, PIO_DEFAULT}
/// SD card write protection pin definition.
#define BOARD_SD_PIN_WP     {AT91C_PIO_PE17, AT91C_BASE_PIOE, AT91C_ID_PIOE, PIO_INPUT, PIO_DEFAULT}
/// MCI slot connected to the SD card.
#define BOARD_SD_SLOT       MCI_SD_SLOTA
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9263-EK - Memories"
/// This page lists definitions related to external on-board memories.
/// 
/// !SDRAM
/// - AT91C_EBI_SDRAM
/// - BOARD_SDRAM_SIZE
/// - PIN_SDRAM
/// 
/// !Nandflash
/// - PINS_NANDFLASH
/// - BOARD_NF_COMMAND_ADDR
/// - BOARD_NF_ADDRESS_ADDR
/// - BOARD_NF_DATA_ADDR
/// - BOARD_NF_CE_PIN
/// - BOARD_NF_RB_PIN

/// SDRAM base address.
#define AT91C_EBI_SDRAM    0x20000000
/// Board SDRAM size
#define BOARD_SDRAM_SIZE   0x04000000

/// List of all SDRAM pins definitions.
#define PINS_SDRAM  {0xFFFF0000, AT91C_BASE_PIOD, AT91C_ID_PIOCDE, PIO_PERIPH_A, PIO_DEFAULT}


/// Nandflash controller peripheral pins definition.
#define PINS_NANDFLASH         BOARD_NF_CE_PIN, BOARD_NF_RB_PIN

/// Nandflash chip enable pin definition.
#define BOARD_NF_CE_PIN        {1 << 15, AT91C_BASE_PIOD, AT91C_ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
/// Nandflash ready/busy pin definition.
#define BOARD_NF_RB_PIN        {1 << 22, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
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

