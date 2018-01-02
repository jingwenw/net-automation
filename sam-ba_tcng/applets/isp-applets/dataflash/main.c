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

#include "../common/applet.h"
#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <utility/trace.h>
#include <utility/math.h>
#include <spi-flash/at45.h>
#include <spi-flash/at45d.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Software version.
#define VERSION "1.1"

/// SPI clock frequency in Hz.
#define SPCK    10000000

#if defined (at91sam7s) || defined (at91sam7x) || defined (at91sam7l)|| defined (at91sam7se) || defined (at91sam7a3)
/// Build a tiny applet for devices with small SRAM size
#define TINY_APPPLET

/// Stack size in SRAM
#if defined (at91sam7se) 
/// For at91sam7se, the applet run in sdram, stack also in sdram. 
#define STACK_SIZE 0
#else
#define STACK_SIZE 0x100
#endif

#else
/// Read write buffer size in DF page number 
#define BUFFER_NB_PAGE 500
#endif

// Max size of data we can tranfsert in one shot
#define PDC_MAX_COUNT 0xFFFF

//------------------------------------------------------------------------------
//         Local structures
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Structure for storing parameters for each command that can be performed by
/// the applet.
//------------------------------------------------------------------------------
struct _Mailbox {

    /// Command send to the monitor to be executed.
    unsigned int command;
    /// Returned status, updated at the end of the monitor execution.
    unsigned int status;

    /// Input Arguments in the argument area
    union {

        /// Input arguments for the Init command.
        struct {

            /// Communication link used.
            unsigned int comType;
            /// Trace level.
            unsigned int traceLevel;
            /// AT45 dataflash index.
            unsigned int at45Idx;

        } inputInit;

        /// Output arguments for the Init command.
        struct {

            /// Memory size.
            unsigned int memorySize;
            /// Buffer address.
            unsigned int bufferAddress;
            /// Buffer size.
            unsigned int bufferSize;

        } outputInit;

        /// Input arguments for the Write command.
        struct {

            /// Buffer address.
            unsigned int bufferAddr;
            /// Buffer size.
            unsigned int bufferSize;
            /// Memory offset.
            unsigned int memoryOffset;

        } inputWrite;

        /// Output arguments for the Write command.
        struct {

            /// Bytes written.
            unsigned int bytesWritten; 

        } outputWrite;

        /// Input arguments for the Read command.
        struct {

            /// Buffer address.
            unsigned int bufferAddr;
            /// Buffer size.
            unsigned int bufferSize;
            /// Memory offset.
            unsigned int memoryOffset;

        } inputRead;

        /// Output arguments for the Read command.
        struct {

            /// Bytes read.
            unsigned int bytesRead;

        } outputRead;

        /// Input arguments for the Full Erase command.
        // NONE

        /// Output arguments for the Full Erase command.
        // NONE
        
		/// Input arguments for the Buffer Erase command.
         struct {

            /// Memory offset to be erase.
            unsigned int memoryOffset;

        } inputBufferErase;

        /// Output arguments for the Buffer Erase command.
        // NONE
    } argument;	
};

//------------------------------------------------------------------------------
/// Holds parameters to configure access to one AT45 dataflash.
//------------------------------------------------------------------------------
struct _At45Select {

    /// Address of the SPI peripheral connected to the dataflash.
    AT91PS_SPI pSpiHw;
    /// SPI peripheral index (e.g. SPI0 -> 0, SPI1 -> 1).
    unsigned char spiIndex;
    /// SPI peripheral identifier.
    unsigned char spiId;
    /// List of pins used by the SPI (MISO, MOSI & SPCK, no NPCS).
    const Pin *pPinsSpi;
    /// Number of pins in list.
    unsigned char numPinsSpi;
    /// Chip select value.
    unsigned char cs;
    /// Chip select pin definition.
    const Pin *pPinCs;
};

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// End of program space (code + data).
extern unsigned int end;

/// Trace level can be set at applet initialization
#if defined(DYNAMIC_TRACE_LEVEL)
    unsigned int trace_LEVEL = 0;
#endif

    
//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// SPI driver instance.
static Spid spid;

/// AT45 driver instance.
static At45 at45;

/// Number of pages in dataflash.
static unsigned int numPages;
/// Size of one page in the dataflash, in bytes.
static unsigned int pageSize;
/// Size of the buffer used for read/write operations in bytes.
static unsigned int bufferSize;

#ifdef BOARD_AT45_A_SPI_BASE

/// SPI pin list instance for dataflash A.
static const Pin pinsSpiA[] = {BOARD_AT45_A_SPI_PINS};
/// SPI chip select pin instance for dataflash A.
static const Pin pinCsA[] = {BOARD_AT45_A_NPCS_PIN};

#endif //#ifdef BOARD_AT45_A_SPI_BASE

#ifdef BOARD_AT45_B_SPI_BASE

/// SPI pin list instance for dataflash B.
static const Pin pinsSpiB[] = {BOARD_AT45_B_SPI_PINS};
/// SPI chip select pin instance for dataflash B.
static const Pin pinCsB[] = {BOARD_AT45_B_NPCS_PIN};

#endif //#ifdef BOARD_AT45_B_SPI_BASE

/// List of dataflash that are defined in the current board.
static const struct _At45Select at45Select[2] = {
#ifdef BOARD_AT45_A_SPI_BASE
    {
        BOARD_AT45_A_SPI_BASE,
        BOARD_AT45_A_SPI,
        BOARD_AT45_A_SPI_ID,
        pinsSpiA,
        PIO_LISTSIZE(pinsSpiA),
        BOARD_AT45_A_NPCS,
        pinCsA
    },
#else
    {0, 0, 0, 0, 0, 0, 0},
#endif
#ifdef BOARD_AT45_B_SPI_BASE
    {
        BOARD_AT45_B_SPI_BASE,
        BOARD_AT45_B_SPI,
        BOARD_AT45_B_SPI_ID,
        pinsSpiB,
        PIO_LISTSIZE(pinsSpiB),
        BOARD_AT45_B_NPCS,
        pinCsB
    }
#else
    {0, 0, 0, 0, 0, 0, 0}
#endif
};

/// Current Dataflash index 
volatile unsigned char at45Index = 0;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Applet main entry. This function decodes received command and executes it.
/// \param argc  always 1
/// \param argv  Address of the argument area.
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    struct _Mailbox *pMailbox = (struct _Mailbox *) argv;
    const At45Desc *pDesc = 0;
    unsigned int bytesToWrite, bytesToRead, bufferAddr, memoryOffset, packetSize;
    // index on read/write buffer
    unsigned char *pBuffer;
    // Temporary buffer used for non page aligned read/write 
    unsigned int tempBufferAddr;
    // Offset in destination buffer during buffer copy
    unsigned int bufferOffset;
    
    // Configure the DBGU
    trace_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // Configure pins (must be done each time because of some old ROM codes that reset PIO usage)
    if (at45Select[at45Index].pSpiHw != 0) {
        PIO_Configure(at45Select[at45Index].pPinsSpi, at45Select[at45Index].numPinsSpi);
        PIO_Configure(at45Select[at45Index].pPinCs, 1);
    }

    // ----------------------------------------------------------
    // INIT: 
    // ----------------------------------------------------------
    if (pMailbox->command == APPLET_CMD_INIT) {

        at45Index = pMailbox->argument.inputInit.at45Idx;

#if defined(DYNAMIC_TRACE_LEVEL)
        trace_LEVEL = pMailbox->argument.inputInit.traceLevel;
#endif
        
        trace_LOG(trace_INFO, "-- DataFlash AT45 ISP applet %s %s %s --\n\r", VERSION, __DATE__, __TIME__);

        if (at45Select[at45Index].pSpiHw == 0) {

            pMailbox->status = APPLET_NO_DEV;
            pMailbox->argument.outputInit.bufferSize = 0;
            pMailbox->argument.outputInit.memorySize = 0;
            pMailbox->argument.outputInit.bufferAddress = (unsigned int) &end;

            trace_LOG(trace_INFO, "-E- INIT command: No Dataflash %d defined for this board\n\r", \
                      pMailbox->argument.inputInit.at45Idx);
          
            pMailbox->status = APPLET_NO_DEV;
            goto exit;
        }

        trace_LOG(trace_INFO, "-I- INIT command: Dataflash %d : SPI 0x%x SPI_NPCS 0x%x (0x%x)\n\r", \
                  pMailbox->argument.inputInit.at45Idx,
                  at45Select[at45Index].spiIndex,
                  at45Select[at45Index].cs,
                  (unsigned int) &(pMailbox->argument.inputInit.at45Idx));

        // Initialize the SPI and serial flash
        SPID_Configure(&spid, at45Select[at45Index].pSpiHw, at45Select[at45Index].spiId);        
        PIO_Configure(at45Select[at45Index].pPinsSpi, at45Select[at45Index].numPinsSpi);
        PIO_Configure(at45Select[at45Index].pPinCs, 1);

        trace_LOG(trace_INFO, "-I- \tSPI NCSR 0x%x\n\r", AT45_CSR(BOARD_MCK, SPCK));
        SPID_ConfigureCS(&spid, at45Select[at45Index].cs, AT45_CSR(BOARD_MCK, SPCK));
        AT45_Configure(&at45, &spid, at45Select[at45Index].cs);
        trace_LOG(trace_INFO, "-I- \tSPI and AT45 drivers initialized\n\r");
        
        pMailbox->argument.outputInit.bufferAddress = (unsigned int) &end;
        // Read the JEDEC ID of the device to identify it
        pDesc = AT45_FindDevice(&at45, AT45D_GetStatus(&at45));

        if (!pDesc) {

            pMailbox->status = APPLET_DEV_UNKNOWN;
            pMailbox->argument.outputInit.bufferSize = 0;
            pMailbox->argument.outputInit.memorySize = 0;
            trace_LOG(trace_INFO, "-E- \tDevice Unknown\n\r");
            goto exit;
        }

        // Get device parameters
        pMailbox->status = APPLET_SUCCESS;
        numPages = AT45_PageNumber(&at45);
        pageSize = AT45_PageSize(&at45);
        
#ifdef TINY_APPPLET        
       bufferSize = AT91C_ISRAM_SIZE                 // sram size
       	   - ( ((unsigned int) &end) - AT91C_ISRAM ) // program size (romcode, code+data)
           - STACK_SIZE								 // stack size (if same area of applet code)
           - pageSize;								 // tempbuffer size to to make not aligned write operations

#else        
        bufferSize = BUFFER_NB_PAGE * pageSize;
#endif        
	    bufferSize -= bufferSize % pageSize;  // integer number of pages can be contained in each buffer
        pMailbox->argument.outputInit.bufferSize = bufferSize;
        pMailbox->argument.outputInit.memorySize = numPages * pageSize;
        trace_LOG(trace_INFO, "-I- \t%s numPages : %x pageSize : %x bufferAddr : 0x%x\n\r",
                  at45.pDesc->name, numPages, pageSize, pMailbox->argument.outputInit.bufferAddress);
    }

    // ----------------------------------------------------------
    // WRITE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_WRITE) {

        memoryOffset = pMailbox->argument.inputWrite.memoryOffset;
        bufferAddr   = pMailbox->argument.inputWrite.bufferAddr;
        bytesToWrite = pMailbox->argument.inputWrite.bufferSize;
        trace_LOG(trace_INFO, 
                  "-I- WRITE at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
                  memoryOffset, bufferAddr, bytesToWrite);

        pBuffer = (unsigned char *) bufferAddr;
        tempBufferAddr = bufferAddr + bufferSize;
        
        if ((memoryOffset % pageSize) != 0) {
            
            // We are not page aligned, retrieve first page content to update it
            // Flush temp buffer
            memset((unsigned int *)tempBufferAddr, 0xFF, pageSize);

            bufferOffset = (memoryOffset % pageSize);
            
            if( (bytesToWrite + bufferOffset) < pageSize) {
                packetSize = bytesToWrite;
            }
            else {
                packetSize = pageSize - bufferOffset;
            }
            
            memoryOffset -= bufferOffset;

            // Read page to be updated
            AT45D_Read(&at45, (unsigned char *) tempBufferAddr, pageSize, memoryOffset);

            // Fill retrieved page with data to be programmed
            memcpy((unsigned char *)(tempBufferAddr + bufferOffset), pBuffer, packetSize);

            // Write the page contents
            AT45D_Write(&at45, (unsigned char *) tempBufferAddr, pageSize, memoryOffset);

            bytesToWrite -= packetSize;
            pBuffer += packetSize;
            memoryOffset += pageSize;
        }

        // If it remains more than one page to write
        while (bytesToWrite >= pageSize) {
            // Write the page contents 
            AT45D_Write(&at45, pBuffer, pageSize, memoryOffset);
            pBuffer += pageSize;
            memoryOffset += pageSize;
            bytesToWrite -= pageSize;
        }

        // Write remaining data
        if (bytesToWrite > 0) {

            // Read previous content of page
            AT45D_Read(&at45, (unsigned char *) tempBufferAddr, pageSize, memoryOffset);

            // Fill retrieved block with data to be programmed
            memcpy((unsigned char *)tempBufferAddr, pBuffer, bytesToWrite);

            // Write the page contents
            AT45D_Write(&at45, (unsigned char *) tempBufferAddr, pageSize, memoryOffset);

            // No more bytes to write
            bytesToWrite = 0;
        }

        trace_LOG(trace_INFO, 
                  "-I- WRITE return byte written : 0x%x Bytes\n\r",
                  pMailbox->argument.inputWrite.bufferSize - bytesToWrite);

        pMailbox->argument.outputWrite.bytesWritten = pMailbox->argument.inputWrite.bufferSize - bytesToWrite;
        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // READ: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_READ) {

        memoryOffset = pMailbox->argument.inputRead.memoryOffset;
        bufferAddr   = pMailbox->argument.inputRead.bufferAddr;
        bytesToRead  = pMailbox->argument.inputRead.bufferSize;

        trace_LOG(trace_INFO, 
                  "-I- READ at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
                  memoryOffset, bufferAddr, bytesToRead);

        pBuffer = (unsigned char *) bufferAddr;

        // Read packet after packets
        while (((unsigned int)pBuffer < (bufferAddr + bufferSize)) && (bytesToRead > 0)) {

            packetSize = min(PDC_MAX_COUNT, bytesToRead);
            AT45D_Read(&at45, pBuffer, packetSize, memoryOffset);

            pBuffer += packetSize;
            bytesToRead -= packetSize;
            memoryOffset += packetSize;
        }
        
        trace_LOG(trace_INFO, "-I- READ return byte read : 0x%x Bytes\n\r",
                  pMailbox->argument.inputRead.bufferSize - bytesToRead);

        pMailbox->argument.outputRead.bytesRead = pMailbox->argument.inputRead.bufferSize - bytesToRead;
        pMailbox->status = APPLET_SUCCESS;
    }

	// ----------------------------------------------------------
    // FULL ERASE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_FULL_ERASE) {

        trace_LOG(trace_INFO, "-I- FULL ERASE command\n\r");

        memoryOffset = 0;
        while (memoryOffset < (pageSize * numPages)) {

            // Erase the page
            AT45D_Erase(&at45, memoryOffset);
            memoryOffset += pageSize;
        }
        trace_LOG(trace_INFO, "-I- Full Erase achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }
    
    // ----------------------------------------------------------
    // BUFFER ERASE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_BUFFER_ERASE) {

        trace_LOG(trace_INFO, "-I- BUFFER ERASE command\n\r");
 		
 		memoryOffset = pMailbox->argument.inputBufferErase.memoryOffset;
        while (memoryOffset < (pMailbox->argument.inputBufferErase.memoryOffset + bufferSize)) {

            // Erase the page
            AT45D_Erase(&at45, memoryOffset);
            memoryOffset += pageSize;
        }
        trace_LOG(trace_INFO, "-I- Buffer Erase achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }
    
    // ----------------------------------------------------------
    // CONFIGURE IN BINARY MODE (power of two page size): 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_BINARY_PAGE) {

        trace_LOG(trace_INFO, "-I- BINARY PAGE SET command\n\r");
 		// Configure power-of-2 binary page size.
        AT45D_BinaryPage(&at45);
        trace_LOG(trace_INFO, "-I- Binary Page achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }

exit :
    // Acknowledge the end of command
    trace_LOG(trace_INFO, "-I- \tEnd of applet (command : %x --- status : %x)\n\r", pMailbox->command, pMailbox->status);

    // Notify the host application of the end of the command processing
    pMailbox->command = ~(pMailbox->command);

    return 0;
}

