/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
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

#include "sscd.h"
#include <board.h>

//------------------------------------------------------------------------------
//         Macros
//------------------------------------------------------------------------------


/// Write PMC register
#define WRITE_PMC(pPmc, regName, value) pPmc->regName = (value)

/// Write SSC register
#define WRITE_SSC(pSsc, regName, value) pSsc->regName = (value)

/// Read SSC registers
#define READ_SSC(pSsc, regName) (pSsc->regName)

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Initializes the Sscd structure and the corresponding SSC hardware.
/// Always returns 0.
/// \param pSscd  Pointer to a Sscd instance.
/// \param pSscHw  Associated SSC peripheral.
/// \param sscId  SSC peripheral identifier.
//------------------------------------------------------------------------------
unsigned char SSCD_Configure(Sscd *pSscd, AT91S_SSC *pSscHw, unsigned char sscId)
{
	// Initialize the SSC structure
	pSscd->pSscHw = pSscHw;
	pSscd->sscId  = sscId;
	pSscd->semaphore = 1;
	pSscd->pCurrentCommand = 0;

	// Enable the SSC clock
	WRITE_PMC(AT91C_BASE_PMC, PMC_PCER, (1 << pSscd->sscId));
	// Execute a software reset of the SSC
	WRITE_SSC(pSscHw, SSC_CR, AT91C_SSC_SWRST);
	// Disable the PDC transfer	
	WRITE_SSC(pSscHw, SSC_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
	
	// Disable buffer complete interrupt
	WRITE_SSC(pSscHw, SSC_IDR, AT91C_SSC_RXBUFF);
	
	return 0;
}

//------------------------------------------------------------------------------
/// Configures the parameters for the device corresponding to the cs.
/// \param pSscd  Pointer to a Sscd instance.
/// \param cs  number corresponding to the SPI chip select.
/// \param csr  SPI_CSR value to setup.
//------------------------------------------------------------------------------
void SSCD_ConfigureCS(Sscd *pSscd, unsigned char cs, unsigned int csr)
{
	///Dummy function using by at45/at26.
}

//------------------------------------------------------------------------------
/// Configures the parameters for the device corresponding to the cs.
/// \param pSscd  Pointer to a Sscd instance.
/// \param cmr  SSC_CMR value to setup.
//------------------------------------------------------------------------------
void SSCD_ConfigureCmr(Sscd *pSscd, unsigned int cmr)
{
	AT91S_SSC *pSscHw = pSscd->pSscHw;
	WRITE_SSC(pSscHw, SSC_CMR, cmr);
}

//------------------------------------------------------------------------------
/// Configures the parameters for the device corresponding to the cs.
/// \param pSscd  Pointer to a Sscd instance.
/// \param rcmr  SSC_RCMR value to setup.
/// \param rfmr  SSC_RFMR value to setup.
/// \param tcmr  SSC_TCMR value to setup.
/// \param tfmr  SSC_TFMR value to setup.
//------------------------------------------------------------------------------
void SSCD_ConfigureTiming(
    Sscd *pSscd, 
    unsigned int rcmr,
    unsigned int rfmr,
    unsigned int tcmr,
    unsigned int tfmr)
{
	
	AT91S_SSC *pSscHw = pSscd->pSscHw;
	WRITE_SSC(pSscHw, SSC_RCMR, rcmr);
	WRITE_SSC(pSscHw, SSC_RFMR, rfmr);
	WRITE_SSC(pSscHw, SSC_TCMR, tcmr);
	WRITE_SSC(pSscHw, SSC_TFMR, tfmr);
	
	WRITE_SSC(pSscHw, SSC_CR, AT91C_SSC_TXEN);
	WRITE_SSC(pSscHw, SSC_CR, AT91C_SSC_RXEN);
}
	
//------------------------------------------------------------------------------
/// Starts a SSC master transfer. This is a non blocking function. It will
/// return as soon as the transfer is started.
/// Returns 0 if the transfer has been started successfully; otherwise returns
/// SSCD_ERROR_LOCK is the driver is in use, or SSCD_ERROR if the command is not
/// valid.
/// \param pSscd  Pointer to a Sscd instance.
/// \param pCommand Pointer to the SSC command to execute.
//------------------------------------------------------------------------------
unsigned char SSCD_SendCommand(Sscd *pSscd, SscdCmd *pCommand)
{
	AT91S_SSC *pSscHw = pSscd->pSscHw;

 	// Try to get the dataflash semaphore
 	if (pSscd->semaphore == 0) {
    
 		return SSCD_ERROR_LOCK;
    }
 	pSscd->semaphore--;
	
    // Enable transmitter and receiver
	WRITE_SSC(pSscHw, SSC_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
    	
	// Initialize the two SSC PDC buffer
	WRITE_SSC(pSscHw, SSC_RPR, (int) pCommand->pCmd);
	WRITE_SSC(pSscHw, SSC_RCR, pCommand->cmdSize);
	WRITE_SSC(pSscHw, SSC_TPR, (int) pCommand->pCmd);
	WRITE_SSC(pSscHw, SSC_TCR, pCommand->cmdSize);
	
	WRITE_SSC(pSscHw, SSC_RNPR, (int) pCommand->pData);
	WRITE_SSC(pSscHw, SSC_RNCR, pCommand->dataSize);
	WRITE_SSC(pSscHw, SSC_TNPR, (int) pCommand->pData);
	WRITE_SSC(pSscHw, SSC_TNCR, pCommand->dataSize);
	
	// Initialize the callback
	pSscd->pCurrentCommand = pCommand;
	
	// Enable transmitter and receiver
	WRITE_SSC(pSscHw, SSC_PTCR, AT91C_PDC_RXTEN | AT91C_PDC_TXTEN);

	// Enable buffer complete interrupt
	WRITE_SSC(pSscHw, SSC_IER, AT91C_SSC_RXBUFF);
	
	return 0;	
}


//------------------------------------------------------------------------------
/// The SSC_Handler must be called by the SSC Interrupt Service Routine with the
/// corresponding Ssc instance.
/// The SSC_Handler will unlock the Ssc semaphore and invoke the upper application 
/// callback.
/// \param pSscd  Pointer to a Sscd instance.
//------------------------------------------------------------------------------
void SSCD_Handler(Sscd *pSscd)
{
	
	SscdCmd *pSscdCmd = pSscd->pCurrentCommand;
	AT91S_SSC *pSscHw = pSscd->pSscHw;
	volatile unsigned int sscSr;
	
	// Read the status register
	sscSr = READ_SSC(pSscHw, SSC_SR);	
	if (sscSr & AT91C_SSC_RXBUFF) {

		// Disable transmitter and receiver
		WRITE_SSC(pSscHw, SSC_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);

		// Disable buffer complete interrupt
		WRITE_SSC(pSscHw, SSC_IDR, AT91C_SSC_RXBUFF);

		// Release the dataflash semaphore
		pSscd->semaphore++;
			
		// Invoke the callback associated with the current command
		if (pSscdCmd && pSscdCmd->callback) {
        
			pSscdCmd->callback(0, pSscdCmd->pArgument);
        }
			
		// Nothing must be done after. A new DF operation may have been started
		// in the callback function.
	}
}

//------------------------------------------------------------------------------
/// Returns 1 if the SSC driver is currently busy executing a command; otherwise
/// returns 0.
/// \param pSscd  Pointer to a SSC driver instance.
//------------------------------------------------------------------------------
unsigned char SSCD_IsBusy(const Sscd *pSscd)
{
    if (pSscd->semaphore == 0) {

        return 1;
    }
    else {

        return 0;
    }
}
