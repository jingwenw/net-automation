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
//         Headers
//------------------------------------------------------------------------------

#include "../common/applet.h"
#include <nandflash/SkipBlockNandFlash.h>
#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <utility/trace.h>
#include <utility/math.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Software version.
#define VERSION "1.1"


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

        // Input arguments for the Full Erase command.
        struct {

            /// Type of Erase to perform
            unsigned int eraseType;

        } inputFullErase;
        
        // Output arguments for the Full Erase command.
        // NONE

    } argument;	
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

/// Nandflash memory size.
static unsigned int memSize;
/// Number of blocks in nandflash.
static unsigned int numBlocks;
/// Size of one block in the nandflash, in bytes.
static unsigned int blockSize;
/// Size of one page in the nandflash, in bytes.
static unsigned int pageSize;
/// Number of page per block
static unsigned int numPagesPerBlock;
// Nandflash bus width
static unsigned char nfBusWidth = 8;


#ifdef PINS_NANDFLASH

/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/// Nandflash device structure.
static struct SkipBlockNandFlash skipBlockNf;
/// Address for transferring command bytes to the nandflash.
static unsigned int cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/// Address for transferring address bytes to the nandflash.
static unsigned int addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/// Address for transferring data bytes to the nandflash.
static unsigned int dataBytesAddr = BOARD_NF_DATA_ADDR;
/// Nandflash chip enable pin.
static const Pin nfCePin = BOARD_NF_CE_PIN;
/// Nandflash ready/busy pin.
static const Pin nfRbPin = BOARD_NF_RB_PIN;


#else

/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {{0, 0, 0, 0, 0}};
/// Nandflash device structure.
static struct SkipBlockNandFlash skipBlockNf;
/// Address for transferring command bytes to the nandflash.
static unsigned int cmdBytesAddr = 0;
/// Address for transferring address bytes to the nandflash.
static unsigned int addrBytesAddr = 0;
/// Address for transferring data bytes to the nandflash.
static unsigned int dataBytesAddr = 0;
/// Nandflash chip enable pin.
static const Pin nfCePin = {0, 0, 0, 0, 0};
/// Nandflash ready/busy pin.
static const Pin nfRbPin = {0, 0, 0, 0, 0};

#endif


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
    unsigned int bufferSize, bufferAddr, memoryOffset;
    unsigned int bytesToWrite;
    unsigned int bytesRead = 0;
    // Temporary buffer used for non block aligned read / write 
    unsigned int tempBufferAddr;
    unsigned short block, page, offset, i;
    // Index in source buffer during buffer copy
    unsigned int offsetInSourceBuff;
    // Index in destination buffer during buffer copy
    unsigned int offsetInTargetBuff;
    // Errors returned by SkipNandFlash functions
    unsigned char error = 0;


    // Configure the DBGU
    trace_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // Configure SMC for Nandflash accesses (done each time applet is launched because of old ROM codes)
    BOARD_ConfigureNandFlash(nfBusWidth);
    PIO_Configure(pPinsNf, PIO_LISTSIZE(pPinsNf));

    // ----------------------------------------------------------
    // INIT: 
    // ----------------------------------------------------------
    if (pMailbox->command == APPLET_CMD_INIT) {
        
#if defined(DYNAMIC_TRACE_LEVEL)
        trace_LEVEL = pMailbox->argument.inputInit.traceLevel;
#endif

        trace_LOG(trace_INFO, "-- NandFlash ISP applet %s %s %s --\n\r", VERSION, __DATE__, __TIME__);
        trace_LOG(trace_INFO, "-I- INIT command\n\r");

        if (pPinsNf->pio == 0) {
            pMailbox->status = APPLET_NO_DEV;
            pMailbox->argument.outputInit.bufferSize = 0;
            pMailbox->argument.outputInit.memorySize = 0;
            pMailbox->argument.outputInit.bufferAddress = (unsigned int) &end;

            trace_LOG(trace_INFO, "-E- INIT command: No Nandflash defined for this board\n\r");
        }
        else {

            memset(&skipBlockNf, 0, sizeof(skipBlockNf));            

            if (SkipBlockNandFlash_Initialize(&skipBlockNf,
                                              0,
                                              cmdBytesAddr,
                                              addrBytesAddr,
                                              dataBytesAddr,
                                              nfCePin,
                                              nfRbPin)) {
    
                pMailbox->status = APPLET_DEV_UNKNOWN;
                pMailbox->argument.outputInit.bufferSize = 0;
                pMailbox->argument.outputInit.memorySize = 0;
                trace_LOG(trace_INFO, "-E- \tDevice Unknown\n\r");
            }
            else {

                // Check the data bus width of the NandFlash
                nfBusWidth = NandFlashModel_GetDataBusWidth((struct NandFlashModel *)&skipBlockNf);
                // Reconfigure bus width
                BOARD_ConfigureNandFlash(nfBusWidth);

                trace_LOG(trace_INFO, "-I- \tNandflash driver initialized\n\r");
        
                pMailbox->argument.outputInit.bufferAddress = (unsigned int) &end;
        
                // Get device parameters
                memSize = NandFlashModel_GetDeviceSizeInBytes(&skipBlockNf.ecc.raw.model);
                blockSize = NandFlashModel_GetBlockSizeInBytes(&skipBlockNf.ecc.raw.model);
                numBlocks = NandFlashModel_GetDeviceSizeInBlocks(&skipBlockNf.ecc.raw.model);
                pageSize = NandFlashModel_GetPageDataSize(&skipBlockNf.ecc.raw.model);
                numPagesPerBlock = NandFlashModel_GetBlockSizeInPages(&skipBlockNf.ecc.raw.model);
                
                pMailbox->status = APPLET_SUCCESS;
                pMailbox->argument.outputInit.bufferSize = blockSize; 
                pMailbox->argument.outputInit.memorySize = memSize;

                trace_LOG(trace_INFO, "-I- \t pageSize : 0x%x blockSize : 0x%x blockNb : 0x%x bus width : %d\n\r",
                          pageSize, blockSize, numBlocks, nfBusWidth);
                trace_LOG(trace_INFO, "-I- \t bufferAddr : 0x%x\n\r", (unsigned int) &end);
            }
        }
    }

    // ----------------------------------------------------------
    // WRITE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_WRITE) {

        memoryOffset = pMailbox->argument.inputWrite.memoryOffset;
        bufferAddr = pMailbox->argument.inputWrite.bufferAddr;
        tempBufferAddr = bufferAddr + blockSize;
        bytesToWrite = pMailbox->argument.inputWrite.bufferSize;

        trace_LOG(trace_INFO, 
                  "-I- WRITE arguments : offset 0x%x, buffer at 0x%x, of 0x%x Bytes\n\r",
                  memoryOffset, bufferAddr, bytesToWrite);

        pMailbox->argument.outputWrite.bytesWritten = 0;

        // Check word alignment
        if (memoryOffset % 4) {

            pMailbox->status = APPLET_ALIGN_ERROR;
            goto exit;
        }

        // Retrieve page and block addresses        
        if (NandFlashModel_TranslateAccess(&(skipBlockNf.ecc.raw.model),
                                           memoryOffset,
                                           bytesToWrite,
                                           &block,
                                           &page,
                                           &offset)) {
            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO, 
                "-I- WRITE at block 0x%x, page 0x%x, offset in page 0x%x\n\r", block, page, offset);

        if (page || offset || (bytesToWrite < blockSize)) {
            // We are not block aligned, retrieve block content to update it
            memset((unsigned int *)tempBufferAddr, 0xFF, blockSize);

            error = SkipBlockNandFlash_ReadBlock(&skipBlockNf, block, (unsigned int *)tempBufferAddr);
            if (error == NandCommon_ERROR_BADBLOCK) {

                pMailbox->status = APPLET_BAD_BLOCK;
                goto exit;
            }
            if (error) {

                pMailbox->status = APPLET_FAIL;
                goto exit;
            }

            // Fill retrieved block with data to be programmed
            offsetInTargetBuff = (page * pageSize) + offset;
            offsetInSourceBuff = 0;

            while ((offsetInTargetBuff < blockSize)
                    && (bytesToWrite > 0)) {

                *(unsigned int *)(tempBufferAddr + offsetInTargetBuff) = *(unsigned int *)(bufferAddr + offsetInSourceBuff);
                offsetInSourceBuff += 4;
                offsetInTargetBuff += 4;
                bytesToWrite -= 4;
            }
        }
        else {

            // Write a full and aligned block 
            tempBufferAddr = bufferAddr;
            bytesToWrite = 0;
        }

        // Erase target block
        error = SkipBlockNandFlash_EraseBlock(&skipBlockNf, block, NORMAL_ERASE);
        if (error == NandCommon_ERROR_BADBLOCK) {

            pMailbox->status = APPLET_BAD_BLOCK;
            goto exit;
        }
        if (error) {

            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        // Write target block
        error = SkipBlockNandFlash_WriteBlock(&skipBlockNf, block, (unsigned int *)tempBufferAddr);
        if (error == NandCommon_ERROR_BADBLOCK) {

            pMailbox->status = APPLET_BAD_BLOCK;
            goto exit;
        }
        if (error) {

            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        pMailbox->argument.outputWrite.bytesWritten = pMailbox->argument.inputWrite.bufferSize - bytesToWrite;
        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // READ: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_READ) {

        memoryOffset = pMailbox->argument.inputRead.memoryOffset;
        bufferAddr   = pMailbox->argument.inputRead.bufferAddr;
        tempBufferAddr = bufferAddr + blockSize;
        bufferSize   = pMailbox->argument.inputRead.bufferSize;

        trace_LOG(trace_INFO, 
                  "-I- READ at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
                  memoryOffset, bufferAddr, bufferSize);

        pMailbox->argument.outputRead.bytesRead = 0;

        // Check word alignment
        if (memoryOffset % 4) {

            pMailbox->status = APPLET_ALIGN_ERROR;
            goto exit;
        }

        // Retrieve page and block addresses        
        if (NandFlashModel_TranslateAccess(&(skipBlockNf.ecc.raw.model),
                                           memoryOffset,
                                           bufferSize,
                                           &block,
                                           &page,
                                           &offset)) {
            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO, "-I- READ at block 0x%x, page 0x%x, offset in page 0x%x\n\r", block, page, offset);

        if (page || offset) {
            memset((unsigned int *)tempBufferAddr, 0xFF, blockSize);            

            error = SkipBlockNandFlash_ReadBlock(&skipBlockNf, block, (unsigned int *)tempBufferAddr);
            if (error == NandCommon_ERROR_BADBLOCK) {
            
                pMailbox->status = APPLET_BAD_BLOCK;
                goto exit;
            }
            if (error) {
                    
                pMailbox->status = APPLET_FAIL;
                goto exit;
            }

            // Fill dest buffer with read data
            offsetInSourceBuff = (page * pageSize) + offset;
            offsetInTargetBuff = 0;

            while ((offsetInSourceBuff < blockSize)
                    && (offsetInTargetBuff < blockSize)
                    && (bytesRead < bufferSize)) {

                *(unsigned int *)(bufferAddr + offsetInTargetBuff) = *(unsigned int *)(tempBufferAddr + offsetInSourceBuff);
                offsetInSourceBuff += 4;
                offsetInTargetBuff += 4;
                bytesRead += 4;
            }

            pMailbox->argument.outputRead.bytesRead = bytesRead;
            pMailbox->status = APPLET_SUCCESS;
        }
        else {

            memset((unsigned int *)bufferAddr, 0xFF, blockSize);
                
            error = SkipBlockNandFlash_ReadBlock(&skipBlockNf, block, (unsigned int *)bufferAddr);
            if (error == NandCommon_ERROR_BADBLOCK) {

                pMailbox->status = APPLET_BAD_BLOCK;
                goto exit;
            }
            if (error) {

                pMailbox->status = APPLET_FAIL;
                goto exit;
            }

            pMailbox->argument.outputRead.bytesRead = bufferSize;
            pMailbox->status = APPLET_SUCCESS;
        }                    
    }

    // ----------------------------------------------------------
    // FULL ERASE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_FULL_ERASE) {

        trace_LOG(trace_INFO, "-I- FULL ERASE command\n\r");
        trace_LOG(trace_INFO, "-I- \tForce erase flag: 0x%x\n\r", pMailbox->argument.inputFullErase.eraseType);

        for (i = 0; i < numBlocks; i++) {

            // Erase the page
            if (SkipBlockNandFlash_EraseBlock(&skipBlockNf, i, pMailbox->argument.inputFullErase.eraseType)) {

                trace_LOG(trace_INFO, "-I- Found block #%d BAD, skip it\n\r", i);
            }
        }

        trace_LOG(trace_INFO, "-I- Full Erase achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }

exit :
    // Acknowledge the end of command
    trace_LOG(trace_INFO, "-I- \tEnd of applet (command : %x --- status : %x)\n\r", pMailbox->command, pMailbox->status);

    // Notify the host application of the end of the command processing
    pMailbox->command = ~(pMailbox->command);

    return 0;
}

