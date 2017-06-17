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
//         Headers
//------------------------------------------------------------------------------

#include "mci.h"
#include <utility/assert.h>

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------

/// Bit mask for status register errors.
#define STATUS_ERRORS (AT91C_MCI_UNRE  \
                       | AT91C_MCI_OVRE \
                       | AT91C_MCI_DTOE \
                       | AT91C_MCI_DCRCE \
                       | AT91C_MCI_RTOE \
                       | AT91C_MCI_RENDE \
                       | AT91C_MCI_RCRCE \
                       | AT91C_MCI_RDIRE \
                       | AT91C_MCI_RINDE)

/// MCI data timeout configuration with 1048576 MCK cycles between 2 data transfers.
#define DTOR_1MEGA_CYCLES	        (AT91C_MCI_DTOCYC | AT91C_MCI_DTOMUL)

#define SDCARD_APP_OP_COND_CMD      (41 | AT91C_MCI_SPCMD_NONE	| AT91C_MCI_RSPTYP_48	| AT91C_MCI_TRCMD_NO )
#define MMC_SEND_OP_COND_CMD        (1	| AT91C_MCI_TRCMD_NO 	| AT91C_MCI_SPCMD_NONE	| AT91C_MCI_RSPTYP_48 | AT91C_MCI_OPDCMD)


//------------------------------------------------------------------------------
//         Local macros
//------------------------------------------------------------------------------

/// Used to write in PMC registers.
#define WRITE_PMC(pPmc, regName, value)     pPmc->regName = (value)

/// Used to write in MCI registers.
#define WRITE_MCI(pMci, regName, value)     pMci->regName = (value)

/// Used to read from MCI registers.
#define READ_MCI(pMci, regName)             (pMci->regName)

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Initializes a MCI driver instance and the underlying peripheral.
/// \param pMci  Pointer to a MCI driver instance.
/// \param pMciHw  Pointer to a MCI peripheral.
/// \param mciId  MCI peripheral identifier.
/// \param mode  Slot and type of connected card.
//------------------------------------------------------------------------------
void MCI_Init(
    Mci *pMci,
    AT91S_MCI *pMciHw,
    unsigned char mciId,
    unsigned int mode)
{
	unsigned short clkDiv;

    SANITY_CHECK(pMci);
    SANITY_CHECK(pMciHw);
    SANITY_CHECK((mode == MCI_MMC_SLOTA) || (mode == MCI_MMC_SLOTB)
                 || (mode == MCI_SD_SLOTA) || (mode == MCI_SD_SLOTB));

	// Initialize the MCI driver structure
	pMci->pMciHw = pMciHw;
	pMci->mciId  = mciId;
	pMci->semaphore = 1;
	pMci->pCommand = 0;

	// Enable the MCI clock
	WRITE_PMC(AT91C_BASE_PMC, PMC_PCER, (1 << mciId));
	
     // Reset the MCI
    WRITE_MCI(pMciHw, MCI_CR, AT91C_MCI_SWRST);

    // Disable the MCI
    WRITE_MCI(pMciHw, MCI_CR, AT91C_MCI_MCIDIS | AT91C_MCI_PWSDIS);

    // Disable all the interrupts
    WRITE_MCI(pMciHw, MCI_IDR, 0xFFFFFFFF);

    // Set the Data Timeout Register
    WRITE_MCI(pMciHw, MCI_DTOR, DTOR_1MEGA_CYCLES);

    // Set the Mode Register: 400KHz for MCK = 48MHz (CLKDIV = 58)
    clkDiv = (BOARD_MCK / (400000 * 2)) - 1;
    WRITE_MCI(pMciHw, MCI_MR, (clkDiv | (AT91C_MCI_PWSDIV & (0x7 << 8))));

    // Set the SDCard Register
    WRITE_MCI(pMciHw, MCI_SDCR, mode);

    // Enable the MCI and the Power Saving
    WRITE_MCI(pMciHw, MCI_CR, AT91C_MCI_MCIEN | AT91C_MCI_PWSEN);
    
 	// Disable the MCI peripheral clock.
	WRITE_PMC(AT91C_BASE_PMC, PMC_PCDR, (1 << mciId));
}

//------------------------------------------------------------------------------
/// Configure the  MCI CLKDIV in the MCI_MR register. The max. for MCI clock is 
/// MCK/2 and corresponds to CLKDIV = 0
/// \param pMci  Pointer to the low level MCI driver.
/// \param mciSpeed  MCI clock speed in Hz.
//------------------------------------------------------------------------------
void MCI_SetSpeed(Mci *pMci, unsigned int mciSpeed)
{
	AT91S_MCI *pMciHw = pMci->pMciHw;
	unsigned int mciMr;
	unsigned short clkdiv;
	
	SANITY_CHECK(pMci);
    SANITY_CHECK(pMci->pMciHw);
	
    // Set the Mode Register: 400KHz for MCK = 48MHz (CLKDIV = 58)
	mciMr = READ_MCI(pMciHw, MCI_MR) & (~AT91C_MCI_CLKDIV);

	// Multimedia Card Interface clock (MCCK or MCI_CK) is Master Clock (MCK)
    // divided by (2*(CLKDIV+1))
	if (mciSpeed > 0) {

		clkdiv = (BOARD_MCK / (mciSpeed * 2));
		if (clkdiv > 0) {
        
			clkdiv -= 1;
        }
	}
	else {
    
		clkdiv = 0;
    }

    WRITE_MCI(pMciHw, MCI_MR, mciMr | clkdiv);
}

//------------------------------------------------------------------------------
/// Starts a MCI  transfer. This is a non blocking function. It will return
/// as soon as the transfer is started.
/// Return 0 if successful; otherwise returns MCI_ERROR_LOCK if the driver is
/// already in use.
/// \param pMci  Pointer to an MCI driver instance.
/// \param pCommand  Pointer to the command to execute.
//------------------------------------------------------------------------------
unsigned char MCI_SendCommand(Mci *pMci, MciCmd *pCommand)
{
    AT91PS_MCI pMciHw = pMci->pMciHw;
    unsigned int mciIer, mciMr;

    SANITY_CHECK(pMci);
    SANITY_CHECK(pMciHw);
    SANITY_CHECK(pCommand);
     		
    // Try to acquire the MCI semaphore
    if (pMci->semaphore == 0) {
    
        return MCI_ERROR_LOCK;
    }
    pMci->semaphore--;

    // Command is now being executed
    pMci->pCommand = pCommand;
    pCommand->status = MCI_STATUS_PENDING;

    // Enable the MCI clock
    WRITE_PMC(AT91C_BASE_PMC, PMC_PCER, (1 << pMci->mciId));
    	
    // Disable transmitter and receiver
    WRITE_MCI(pMciHw, MCI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);

    // Initialize PDC for read/write operations with data
    mciMr = READ_MCI(pMciHw, MCI_MR) & (~(AT91C_MCI_BLKLEN | AT91C_MCI_PDCMODE));
    if (pCommand->dataSize > 0) {

        mciMr |= AT91C_MCI_PDCMODE | (pCommand->dataSize << 16);
        if (pCommand->isRead) {
    
            WRITE_MCI(pMciHw, MCI_RPR, (int) pCommand->pData);
            if ((pCommand->dataSize & 0x3) != 0) {
            
                    WRITE_MCI(pMciHw, MCI_RCR, pCommand->dataSize / 4 + 1);
            }
            else {
            
                WRITE_MCI(pMciHw, MCI_RCR, pCommand->dataSize / 4);
            }
            WRITE_MCI(pMciHw, MCI_PTCR, AT91C_PDC_RXTEN);
            mciIer = AT91C_MCI_RXBUFF | STATUS_ERRORS;
        }
        else {
    
            WRITE_MCI(pMciHw, MCI_TPR, (int) pCommand->pData);
            if ((pCommand->dataSize & 0x3) != 0) {
            
                WRITE_MCI(pMciHw, MCI_TCR, pCommand->dataSize / 4 + 1);
            }
            else {
            
                WRITE_MCI(pMciHw, MCI_TCR, pCommand->dataSize / 4);
            }
            mciIer = AT91C_MCI_TXBUFE | STATUS_ERRORS;
        }
    }
    // Otherwise stop at the end of the command
    else {
        mciIer = AT91C_MCI_CMDRDY | STATUS_ERRORS;
    }

    // Send the command
    WRITE_MCI(pMciHw, MCI_MR, mciMr);
    WRITE_MCI(pMciHw, MCI_ARGR, pCommand->arg);
    WRITE_MCI(pMciHw, MCI_CMDR, pCommand->cmd);

    // In case of transmit, the PDC shall be enabled after sending the command
    if ((pCommand->dataSize > 0) && !(pCommand->isRead)) {
    
        WRITE_MCI(pMciHw, MCI_PTCR, AT91C_PDC_TXTEN);
    }
    WRITE_MCI(pMciHw, MCI_IER, mciIer);

    return 0;
}

//------------------------------------------------------------------------------
/// Processes pending events on the given MCI driver.
/// \param pMci  Pointer to a MCI driver instance.
//------------------------------------------------------------------------------
void MCI_Handler(Mci *pMci)
{
	AT91S_MCI *pMciHw = pMci->pMciHw;
	MciCmd *pCommand = pMci->pCommand;
	unsigned int status;
	unsigned char i;

    SANITY_CHECK(pMci);
    SANITY_CHECK(pMciHw);
    SANITY_CHECK(pCommand);

	// Read the status register
	status = READ_MCI(pMciHw, MCI_SR) & READ_MCI(pMciHw, MCI_IMR);

    // Check if an error has occured
	if ((status & STATUS_ERRORS) != 0) {

        // Check error code
        if ((status & STATUS_ERRORS) == AT91C_MCI_RTOE) {
        
        	pCommand->status = MCI_STATUS_NORESPONSE;
        }
        // if the command is SEND_OP_COND the CRC error flag is always present
        // (cf : R3 response)
        else if (((status & STATUS_ERRORS) != AT91C_MCI_RCRCE)
                  || ((pCommand->cmd != SDCARD_APP_OP_COND_CMD)
                      && (pCommand->cmd != MMC_SEND_OP_COND_CMD))) {
        
        	pCommand->status = MCI_STATUS_ERROR;
        }
	}
	
	// Check if a transfer has been completed
	if (((status & AT91C_MCI_CMDRDY) != 0)
	    || ((status & AT91C_MCI_RXBUFF) != 0)
	    || ((status & AT91C_MCI_TXBUFE) != 0)) {

        // If no error occured, the transfer is successful
        if (pCommand->status == MCI_STATUS_PENDING) {
        
            pCommand->status = 0;
        }

        // Disable interrupts
        WRITE_MCI(pMciHw, MCI_IDR, READ_MCI(pMciHw, MCI_IMR));

        // Store the card response in the provided buffer
        if (pCommand->pResp) {
        
            for (i=0; i < pCommand->resSize; i++) {
            
                pCommand->pResp[i] = READ_MCI(pMciHw, MCI_RSPR[0]);
            }
        }

        // Release the semaphore
        pMci->semaphore++;
            
        // Invoke the callback associated with the current command (if any)
        if (pCommand->callback) {
        
            (pCommand->callback)(pCommand->status, pCommand);
        }		   
	}
} 

//------------------------------------------------------------------------------
/// Returns 1 if the given MCI transfer is complete; otherwise returns 0.
/// \param pCommand  Pointer to a MciCmd instance.
//------------------------------------------------------------------------------
unsigned char MCI_IsTxComplete(MciCmd *pCommand)
{
	return (pCommand->status != MCI_STATUS_PENDING);
}

