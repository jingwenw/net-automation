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
//         Headers
//------------------------------------------------------------------------------

#include "twi.h"

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/// \internal Calculates the value of the IADRSZ field of the MMR register of a
/// TWI peripheral given the size of the slave internal address in bytes.
#define TWI_MMR_IADRSZ(size)    (((size) & 0x3) << 8)

/// \internal Calculates the value of the DADR field of the MMR register of a
/// TWI peripheral given the slave address.
#define TWI_MMR_DADR(address)   (((address) & 0x7F) << 16)

/// \internal Calculates the value of the IADR field of the IADR register of a
/// TWI peripheral, given the slave internal address.
#define TWI_IADR_IADR(address)  ((address) & 0x00FFFFFF)

/// \internal Returns the start command to perform depending on the number of 
/// bytes to transmit.
#define TWI_START(bytes) \
    ((bytes == 1) ? AT91C_TWI_START | AT91C_TWI_STOP : AT91C_TWI_START)

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Configures a TWI peripheral. The Clock Waveform Generator Register value can
/// be computed using TWI_CWGR.
/// \param twi  Pointer to an AT91S_TWI instance.
/// \param id  Peripheral ID of the TWI.
/// \param cwgr  Clock Waveform Generator Register value.
//------------------------------------------------------------------------------
void TWI_Configure(AT91S_TWI *twi,
                          unsigned int id,
                          unsigned int cwgr)
{
    // Enable the peripheral clock and PIOs
    AT91C_BASE_PMC->PMC_PCER = 1 << id;
    CHIP_EnableTWI(twi);

    // Disable & reset the TWI
    twi->TWI_CR = AT91C_TWI_MSDIS | AT91C_TWI_SWRST;

    // Configure clock
    twi->TWI_CWGR = cwgr;

    // Set the TWI in master mode
    twi->TWI_CR = AT91C_TWI_MSEN;
}

//------------------------------------------------------------------------------
/// Sends the content of a data buffer through a TWI peripheral, to a specified
/// slave. The slave is identified by its address, and optionally its internal
/// address.
/// This function returns true if the buffer has been successfully sent;
/// otherwise it returns false.
/// \param twi  Pointer to an AT91S_TWI instance.
/// \param buffer  Data buffer to send.
/// \param bufferSize Size of the data buffer.
/// \param address  Slave address.
/// \param internalAddress  Slave internal address (optional).
/// \param internalAddressSize  Size in bytes of the internal address (optional).
//------------------------------------------------------------------------------
unsigned char TWI_Write(AT91S_TWI *twi,
                               void *buffer,
                               unsigned int bufferSize,
                               unsigned char address,
                               unsigned int internalAddress,
                               unsigned int internalAddressSize)
{
    // Set address, internal address and write direction
    twi->TWI_CR = AT91C_TWI_MSEN;
    twi->TWI_MMR = TWI_MMR_DADR(address) | TWI_MMR_IADRSZ(internalAddressSize);
    if (internalAddressSize > 0) {

        twi->TWI_IADR = TWI_IADR_IADR(internalAddress);
    }

    // Send the data buffer
    unsigned char transferStarted = 0;
    while (bufferSize > 0) {
    
        // Load a byte of data & start transfer if necessary
        twi->TWI_THR = *((char *) buffer++);
        if (!transferStarted) {

            twi->TWI_CR = TWI_START(bufferSize);
            transferStarted = 1;
        }
        
        // Wait until the transmitter is ready
        while ((twi->TWI_SR & AT91C_TWI_TXRDY) == 0);
        bufferSize--;
    }

    // Stop transfer
    twi->TWI_CR = AT91C_TWI_STOP;
    while ((twi->TWI_SR & AT91C_TWI_TXCOMP) == 0);

    return ((twi->TWI_SR & AT91C_TWI_NACK) == 0);
}

//------------------------------------------------------------------------------
/// Reads data from a TWI peripheral into the provided buffer. The slave to
/// access is identified by its address, and optionally its internalAddress. If
/// the latter is present, then its size in bytes must also be provided.
/// Returns true if a byte has been correctly received; otherwise returns false.
/// \param twi  Pointer to an AT91S_TWI instance.
/// \param buffer  Data buffer used for reception.
/// \param address  Slave address.
/// \param internalAddress  Optional slave internal address.
/// \param internalAddress  Internal address size in bytes.
//------------------------------------------------------------------------------
unsigned char TWI_Read(AT91S_TWI *twi,
                              void *buffer,
                              unsigned int bufferSize,
                              unsigned char address,
                              unsigned int internalAddress,
                              unsigned int internalAddressSize)
{
    // Set address, internal address and write direction
    twi->TWI_MMR = TWI_MMR_DADR(address)
                   | TWI_MMR_IADRSZ(internalAddressSize)
                   | AT91C_TWI_MREAD;
    if (internalAddressSize > 0) {

        twi->TWI_IADR = TWI_IADR_IADR(internalAddress);
    }

    // Start receiving data
    twi->TWI_CR = TWI_START(bufferSize);
    while (bufferSize > 0) {

        // Wait for a byte to be received
        while ((twi->TWI_SR & AT91C_TWI_RXRDY) == 0);

        // Read data
        *((char *) buffer++) = twi->TWI_RHR;
        bufferSize--;

        // If this is the last data, issue a STOP condition
        if (bufferSize == 1) {

            twi->TWI_CR = AT91C_TWI_STOP;
        }
    }

    // Stop transfer
    twi->TWI_CR = AT91C_TWI_STOP;
    while ((twi->TWI_SR & AT91C_TWI_TXCOMP) == 0);

    return ((twi->TWI_SR & AT91C_TWI_NACK) == 0);
}

