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

#ifndef SSCD_H
#define SSCD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

/// An unspecified error has occured.
#define SSCD_ERROR          1

/// SPI driver is currently in use.
#define SSCD_ERROR_LOCK     2

/// compatible with spi invokes.
#define SPID_SendCommand(pSscd,pCommand)  SSCD_SendCommand(pSscd,pCommand) 

#define SPID_IsBusy(pSscd) SSCD_IsBusy(pSscd)

#define SPID_ConfigureCS(pSpid, cs, CSR) SSCD_ConfigureCS(pSpid, cs, CSR)


/// Configure SSC Receive Clock Mode Register
/// CKS = TK pin, 
/// CKO = None RK pin: Input-only
/// CKI = CLOCK_PHASE_FALLING_EDGE or CLOCK_PHASE_RISING_EDGE
/// CKI = CLOCK_PHASE_FALLING_EDGE or CLOCK_PHASE_RISING_EDGE 
/// Clock is not gated when TF is high
/// START on transmit START condition
/// Stop after a completion of a data TR
/// Start delay = 0 clock cycle (data has to be transmetted immediatly
/// Period = 0 no PERIOD signal is generated

#define SSCD_RCMR(Clock_Phase)  (\
                                 AT91C_SSC_CKS_TK |\
                                 AT91C_SSC_CKO_NONE |\
                                 ((~Clock_Phase & 1) << 5)|\
                                 AT91C_SSC_CKG_LOW  |\
                                 AT91C_SSC_START_TX |\
                                 (0 << 12) |\
                                 (1 << 16) |\
                                 (0 << 24))
                                 
/// Configure SSC Receive frame mode Register            
/// Data_length + 1                     
/// Loop mode disabled
/// MSB first
/// 1 words per frame (any value because this field is not user in our application)
/// FSLEN = 1 bits (any value because this field is not user in our application)
/// RF pin is not used
/// Fsedge Not used
#define SSCD_RFMR(Data_Length)  (\
								 (Data_Length << 0) |\
								 (0 << 5) |\
								 AT91C_SSC_MSBF |\
                                 (0 << 8) |\
                                 (0x0 << 16) |\
                                 AT91C_SSC_FSOS_NONE |\
                                 AT91C_SSC_FSEDGE)
                                 
/// Configure SSC Transmit Clock Mode Register       
/// CKS = diveded clock
/// CKO = Transmit Clock during data TX
/// CKI = CLOCK_PHASE_FALLING_EDGE or CLOCK_PHASE_RISING_EDGE
/// CKG enabled only if TF is low
/// As soon as the transmitter is enabled, and immediately after the end of transfer of the previous data.
/// Start delay = 0 clock cycle (data has to be transmetted immediatly
/// Period = 0 no PERIOD signal is generated
#define SSCD_TCMR(Clock_Phase)  (\
                                  AT91C_SSC_CKS_DIV |\
                                  AT91C_SSC_CKO_DATA_TX |\
	                              ((~Clock_Phase & 1) << 5) |\
                                  AT91C_SSC_CKG_LOW |\
                                  AT91C_SSC_START_CONTINOUS |\
                                  (0 << 16) |\
                                  (0 << 24))
                                  
                                  
/// Configure SSC Transmit frame mode Register      
/// Data_length + 1
/// TD is driven high while out of transmission may be driven low (the state of TD while out of transmission is not importante)
/// MSB first
/// 1 words per frame (any value because this field is not used in our application)
/// FSLEN = 1 bits (any value because this field is not used in our application)
/// TF pin is low during data TX (CS)
/// Frame synchro data not enabled
/// Fsedge Not used
#define SSCD_TFMR(Data_Length)   (\
                                   (Data_Length << 0) |\
                                   AT91C_SSC_DATDEF |\
                                   AT91C_SSC_MSBF |\
                                   (0 << 8) |\
                                   (0x0 << 16) |\
                                   AT91C_SSC_FSOS_LOW |\
                                   (0  << 23) |\
                                   AT91C_SSC_FSEDGE)
                                   
//------------------------------------------------------------------------------
//         Macros
//------------------------------------------------------------------------------
/// Calculates the value of the CMR field of given MCK and SPCK.
#define SSCD_CMR_DIV(mck, spck)    (((mck) / ((spck ) * 2 )))
/// Calculates the value of the SCBR field of the Chip Select Register given
/// MCK and SPCK.
#define SPID_CSR_SCBR(mck, spck)    ((((mck) / (spck)) << 8) & AT91C_SPI_SCBR)

/// Calculates the value of the DLYBS field of the Chip Select Register given
/// the delay in ns and MCK.
#define SPID_CSR_DLYBS(mck, delay) \
    (((((delay) * ((mck) / 1000000)) / 1000) << 16) & AT91C_SPI_DLYBS)

/// Calculates the value of the DLYBCT field of the Chip Select Register given
/// the delay in ns and MCK.
#define SPID_CSR_DLYBCT(mck, delay) \
    (((((delay) / 32 * ((mck) / 1000000)) / 1000) << 24) & AT91C_SPI_DLYBCT)
//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

/// SPI transfer complete callback.
typedef void (*SpidCallback )(unsigned char, void *);

//------------------------------------------------------------------------------
/// Ssc Transfer Request prepared by the application upper layer. This structure
/// is sent to the SSC_SendCommand function to start the transfer. At the end of 
/// the transfer, the callback is invoked by the interrupt handler.
//------------------------------------------------------------------------------
typedef struct _SscdCmd {

    /// Pointer to the command data.
	unsigned char *pCmd;
    /// Command size in bytes.
	unsigned char cmdSize;
    /// Pointer to the data to be sent.
	unsigned char *pData;
    /// Data size in bytes.
	unsigned short dataSize;
    /// SPI chip select.
	unsigned char spiCs;
    /// Callback function invoked at the end of transfer.
	SpidCallback callback;
    /// Callback arguments.
	void *pArgument;

} SscdCmd;

/// compatible with spi invokes.
typedef SscdCmd SpidCmd;
//------------------------------------------------------------------------------
/// Constant structure associated with SSC port. This structure prevents
/// client applications to have access in the same time.
//------------------------------------------------------------------------------
typedef struct {

    /// Pointer to SSC Hardware registers
    AT91S_SSC *pSscHw;
    /// SSC Id as defined in the product datasheet
    char sscId;
    /// Current SscCommand being processed
	SscdCmd *pCurrentCommand;
    /// Mutual exclusion semaphore.
	volatile char semaphore;

} Sscd;

/// compatible with spi invokes.
typedef Sscd Spid;

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

extern unsigned char SSCD_Configure(Sscd *pSscd, AT91S_SSC *pSscHw, unsigned char sscId);

extern void SSCD_ConfigureCS(Sscd *pSscd, unsigned char cs, unsigned int csr);
	
extern void SSCD_ConfigureCmr(Sscd *pSscd, unsigned int cmr);

extern void SSCD_ConfigureTiming(
    Sscd *pSscd, 
    unsigned int rcmr,
    unsigned int rfmr,
    unsigned int tcmr,
    unsigned int tfmr);

extern unsigned char SSCD_SendCommand(
	Sscd *pSscd,
	SscdCmd *pCommand);
	
extern void SSCD_Handler(Sscd *pSscd);

extern unsigned char SSCD_IsBusy(const Sscd *pSscd);

#endif // #ifndef SSCD_H

