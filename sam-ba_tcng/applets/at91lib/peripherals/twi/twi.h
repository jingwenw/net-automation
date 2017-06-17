/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

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
/// Functions and definitions for using a TWI peripheral.
/// 
/// !Usage
/// 
/// -# Configure the TWI using TWI_Configure.
/// -# Send data to a slave through the TWI using TWI_Write.
/// -# Reads data from a slave using TWI_Read.
//------------------------------------------------------------------------------

#ifndef TWI_H
#define TWI_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
/// \internal Calculates the value for the CHDIV and CLDIV field given the
/// desired operating frequency and the system master clock. This macro assumes
/// the CKDIV field value equals one.
#define TWI_CWGR_DIV(Fmck, Ftwi)    (((Fmck / Ftwi) - 3) / 2)

/// Calculates the value of the TWI_CWGR register of a twi peripheral given the
/// system frequency and the desired operating frequency (both in Hz).
#define TWI_CWGR(Fmck, Ftwi) \
    ((1 << 16) | (TWI_CWGR_DIV(Fmck, Ftwi) << 8) | TWI_CWGR_DIV(Fmck, Ftwi))

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
extern void TWI_Configure(AT91S_TWI *twi,
                                 unsigned int id,
                                 unsigned int cwgr);
extern unsigned char TWI_Write(AT91S_TWI *twi,
                                      void *buffer,
                                      unsigned int bufferSize,
                                      unsigned char address,
                                      unsigned int internalAddress,
                                      unsigned int internalAddressSize);
extern unsigned char TWI_Read(AT91S_TWI *twi,
                                     void *buffer,
                                     unsigned int bufferSize,
                                     unsigned char address,
                                     unsigned int internalAddress,
                                     unsigned int internalAddressSize);

#endif //#ifndef TWI_H

