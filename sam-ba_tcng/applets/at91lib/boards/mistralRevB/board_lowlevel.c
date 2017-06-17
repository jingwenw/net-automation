/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation
 *
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
//         Headers
//------------------------------------------------------------------------------

#include "board.h"
#include "board_memories.h"

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/*
    Constants: Clock and PLL settings

        BOARD_OSCOUNT - Startup time of main oscillator (in number of slow clock
                        ticks). 
        BOARD_USBDIV - USB PLL divisor value to obtain a 48MHz clock.
        BOARD_CKGR_PLL - PLL frequency range.
        BOARD_PLLCOUNT - PLL startup time (in number of slow clock ticks).
        BOARD_MUL - PLL MUL value.
        BOARD_DIV - PLL DIV value.
        BOARD_PRESCALER - Master clock prescaler value.
*/
#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (64 << 8))
#define BOARD_USBDIV            AT91C_CKGR_USBDIV_1
#define BOARD_CKGR_PLLA         (AT91C_CKGR_SRCA | AT91C_CKGR_OUTA_2)
#define BOARD_PLLACOUNT         (63 << 8)
#define BOARD_MULA              (AT91C_CKGR_MULA & (72 << 16))
#define BOARD_DIVA              (AT91C_CKGR_DIVA & 7)
#define BOARD_PRESCALER         AT91C_PMC_MDIV_2

//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------
/*!
    Default spurious interrupt handler
 */
void defaultSpuriousHandler( void )
{
    while (1);
}

/*!
    Default handler for fast interrupt requests.
 */
void defaultFiqHandler( void )
{
    while (1);
}

/*!
    Default handler for standard interrupt requests.
 */
void defaultIrqHandler( void )
{
    while (1);
}

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
/*!
    Performs the low-level initialization of the chip.
 */
void LowLevelInit( void )
{

    /* Watchdog initialization
     *************************/
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;

    /* Remap
     *******/
    BOARD_RemapRam();
}

