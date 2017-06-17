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
#include "../common/applet.h"
#include <board.h>
#include <board_memories.h>
#include <drivers/flash/flashd.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <string.h>

//------------------------------------------------------------------------------
//         External definitions
//------------------------------------------------------------------------------
extern void LowLevelInit(void); 


//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------
/// Software version.
#define VERSION "1.1"

//#if defined (at91sam9xe)
//#define STACK_SIZE (AT91C_IFLASH_PAGE_SIZE + 0x100 + 0x1000)
//#else
//#define STACK_SIZE (AT91C_IFLASH_PAGE_SIZE + 0x100)
//#endif

#if defined (at91sam9xe)
#define STACK_SIZE (0x100 + 0x1000)
#else
#define STACK_SIZE (0x100)
#endif


#if defined(AT91C_PMC_CSS_PLLA_CLK) && !defined(AT91C_PMC_CSS_PLL_CLK)
    #define AT91C_PMC_CSS_PLL_CLK     AT91C_PMC_CSS_PLLA_CLK
#endif

#if defined (at91sam7se32)
/// Build applet for devices with SDRAM size
#define SDRAM_APPPLET
/// Read write buffer size in DF page number 
#define BUFFER_NB_PAGE 500
#endif
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
            struct {
                /// Lock region size in byte.
                unsigned short lockRegionSize;
                /// Number of Lock Bits.
                unsigned short numbersLockBits;
            } memoryInfo;
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

         /// Input arguments for the Lock page command.
        struct {

            /// Sector number to be lock.
            unsigned int sector;

        } inputLock;

        /// Output arguments for the Lock  page command.
        // NONE

         /// Input arguments for the Unlock  page command.
        struct {

            /// Sector number to be unlock.
            unsigned int sector;

        } inputUnlock;

        /// Output arguments for the Unlock  page command.
        // NONE

         /// Input arguments for the set/clear GPNVM bits command.
        struct {

            /// Activates or Deactivates
            unsigned int action;
            /// NVM bit to set/clear
            unsigned int bitsOfNVM;

        } inputGPNVM;

        /// Output arguments for the set/clear GPNVM bits command.
        // NONE

        /// Input arguments for the set secuity bit command.
        // NONE

        /// Output arguments for the set secuity bit command.
        // NONE

    } argument;
};

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// Start of program space.
extern unsigned int _sfixed;
/// End of program space (code + data).
extern unsigned int end;

/// Trace level can be set at applet initialization
#if defined(DYNAMIC_TRACE_LEVEL)
    unsigned int trace_LEVEL = 0;
#endif

/// Size of the buffer used for read/write operations in bytes.
static unsigned int bufferSize;


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

    unsigned int bytesToWrite, bufferAddr, memoryOffset;

    unsigned int l_start;
    unsigned int l_end;
    unsigned int *pActualStart = NULL;
    unsigned int *pActualEnd = NULL;

    unsigned char error;

    trace_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // ----------------------------------------------------------
    // INIT:
    // ----------------------------------------------------------
    if (pMailbox->command == APPLET_CMD_INIT) {

#if !defined (at91sam7l) && !defined (at91sam7s32) &&  !defined (at91sam7s16) && !defined (at91sam7s321) &&  !defined (at91sam7s161)
        if ((pMailbox->argument.inputInit.comType != JTAG_COM_TYPE) && (pMailbox->argument.inputInit.comType != DBGU_COM_TYPE)) {
            LowLevelInit();
        }
#endif

#if defined(DYNAMIC_TRACE_LEVEL)
        trace_LEVEL = pMailbox->argument.inputInit.traceLevel;
#endif

        trace_LOG(trace_INFO,  "-- Internal Flash ISP Applet %s %s %s --\n\r", VERSION, __DATE__, __TIME__);

        // Initialize flash driver
        FLASHD_Initialize(BOARD_MCK);

        // Set Wait State number
        BOARD_ConfigureFlash48MHz();

        // flash accesses must be 4 bytes aligned
        pMailbox->argument.outputInit.bufferAddress = ((unsigned int) &end);

#ifdef SDRAM_APPPLET
        bufferSize = BUFFER_NB_PAGE * AT91C_IFLASH_PAGE_SIZE;
#else
        bufferSize = AT91C_ISRAM_SIZE                           // sram size
                     - ( ((unsigned int) &end) - AT91C_ISRAM )  // program size (romcode, code+data)
                     //- (((unsigned int) &_sfixed) - AT91C_ISRAM )
                     - STACK_SIZE;                              // stack size at the end
#endif

        bufferSize -= bufferSize % (AT91C_IFLASH_PAGE_SIZE);// integer number of pages can be contained in each buffer
        pMailbox->argument.outputInit.bufferSize = bufferSize;

        pMailbox->argument.outputInit.memorySize = AT91C_IFLASH_SIZE;
        pMailbox->argument.outputInit.memoryInfo.lockRegionSize = AT91C_IFLASH_LOCK_REGION_SIZE;
        pMailbox->argument.outputInit.memoryInfo.numbersLockBits = AT91C_IFLASH_NB_OF_LOCK_BITS;

        trace_LOG(trace_INFO,"-I- \t bufferSize : %d  bufferAddr: 0x%x \n\r",
                      pMailbox->argument.outputInit.bufferSize,
                      (unsigned int) &end );

        trace_LOG(trace_INFO,"-I- \t memorySize : %d lockRegionSize : 0x%x numbersLockBits : 0x%x \n\r",
                      pMailbox->argument.outputInit.memorySize,
                      pMailbox->argument.outputInit.memoryInfo.lockRegionSize,
                      pMailbox->argument.outputInit.memoryInfo.numbersLockBits);

        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // WRITE:
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_WRITE) {

        memoryOffset  = pMailbox->argument.inputWrite.memoryOffset;
        bufferAddr    = pMailbox->argument.inputWrite.bufferAddr;
        bytesToWrite  = pMailbox->argument.inputWrite.bufferSize;

        trace_LOG(trace_INFO,
                 "-I- WRITE at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
                  memoryOffset, bufferAddr, bytesToWrite);

        // Check the giving sector have been locked before.
        if( FLASHD_IsLocked(AT91C_IFLASH + memoryOffset, AT91C_IFLASH + memoryOffset + bytesToWrite) != 0 ) {

            trace_LOG(trace_INFO,  "-E- Error page locked\n\r");
            pMailbox->argument.outputWrite.bytesWritten = 0;
            pMailbox->status = APPLET_PROTECT_FAIL;
            goto exit;
        }

        // Write data
        if( FLASHD_Write(  AT91C_IFLASH + memoryOffset, (const void *)bufferAddr, bytesToWrite) != 0) {

            trace_LOG(trace_INFO,  "-E- Error write operation\n\r");
            pMailbox->argument.outputWrite.bytesWritten = 0;
            pMailbox->status = APPLET_WRITE_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- Write achieved\n\r");
        pMailbox->argument.outputWrite.bytesWritten = bytesToWrite;
        pMailbox->status = APPLET_SUCCESS;
    }
    // ----------------------------------------------------------
    // READ:
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_READ) {

        memoryOffset = pMailbox->argument.inputRead.memoryOffset;
        bufferAddr   = pMailbox->argument.inputRead.bufferAddr;
        bufferSize   = pMailbox->argument.inputRead.bufferSize;

        trace_LOG(trace_INFO,
                  "-I- READ at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
                  memoryOffset,
                  bufferAddr,
                  bufferSize);

        // read data
        memcpy((void *)bufferAddr, (void *)(AT91C_IFLASH + memoryOffset), bufferSize);

        trace_LOG(trace_INFO,  "-I- Read achieved\n\r");
        pMailbox->argument.outputRead.bytesRead = bufferSize;
        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // FULL ERASE:
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_FULL_ERASE) {
        trace_LOG(trace_INFO,  "-I- FULL ERASE command \n\r");

        // Check if at least one page has been locked
        if( FLASHD_IsLocked(AT91C_IFLASH, AT91C_IFLASH + AT91C_IFLASH_SIZE) != 0 ) {

            trace_LOG(trace_INFO,  "-E- Error page locked \n\r");
            pMailbox->status = APPLET_PROTECT_FAIL;
            goto exit;
        }

        // Implement the erase all command
        if( FLASHD_Erase() != 0) {

            trace_LOG(trace_INFO,  "-E- Full erase failed! \n\r");
            pMailbox->status = APPLET_ERASE_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- Full erase achieved\n\r");

        pMailbox->status = APPLET_SUCCESS;
    }
    // ----------------------------------------------------------
    // LOCK SECTOR:
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_LOCK) {
        trace_LOG(trace_INFO,  "-I- LOCK command \n\r");

        l_start = (pMailbox->argument.inputLock.sector * AT91C_IFLASH_LOCK_REGION_SIZE) + AT91C_IFLASH;
        l_end = l_start + AT91C_IFLASH_LOCK_REGION_SIZE;

        if( FLASHD_Lock(l_start, l_end, pActualStart, pActualEnd) != 0) {

            trace_LOG(trace_INFO,  "-E- Lock failed! \n\r");
            ASSERT( *pActualStart == l_start, "-F- Lock failed! \n\r");
            ASSERT( *pActualEnd == (l_start + AT91C_IFLASH_LOCK_REGION_SIZE), "-F- Lock failed! \n\r");
            pMailbox->status = APPLET_PROTECT_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- Lock sector achieved\n\r");

        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // UNLOCK SECTOR:
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_UNLOCK) {
        trace_LOG(trace_INFO,  "-I- UNLOCK command \n\r");

        l_start = (pMailbox->argument.inputLock.sector * AT91C_IFLASH_LOCK_REGION_SIZE) + AT91C_IFLASH;
        l_end = l_start + AT91C_IFLASH_LOCK_REGION_SIZE;

        if( FLASHD_Unlock(l_start, l_end, pActualStart, pActualEnd) != 0) {

            trace_LOG(trace_INFO,  "-E- Unlock failed! \n\r");
            ASSERT( *pActualStart == l_start, "-F- Unlock failed! \n\r");
            ASSERT( *pActualEnd == (l_start + AT91C_IFLASH_LOCK_REGION_SIZE), "-F- Unock failed! \n\r");
            pMailbox->status = APPLET_UNPROTECT_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- Unlock sector achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // GPNVM :
    // ----------------------------------------------------------
#if !defined (at91sam7a3)    
    else if (pMailbox->command == APPLET_CMD_GPNVM) {
        if( pMailbox->argument.inputGPNVM.action == 0) {
            trace_LOG(trace_INFO,  "-I- DEACTIVATES GPNVM command \n\r");
            error = FLASHD_ClearGPNVM(pMailbox->argument.inputGPNVM.bitsOfNVM);
        }
        else {
            trace_LOG(trace_INFO,  "-I- ACTIVATES GPNVM command \n\r");
            error = FLASHD_SetGPNVM(pMailbox->argument.inputGPNVM.bitsOfNVM);
        }

        if(error != 0) {
            trace_LOG(trace_INFO,  "-E- GPNVM failed! \n\r");
            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- GPNVM achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }
#endif // #if !defined (at91sam7a3)    

    // ----------------------------------------------------------
    // SET SECURITY :
    // ----------------------------------------------------------
#if defined(BOARD_FLASH_EFC) && !defined (at91sam7a3)
    else if (pMailbox->command == APPLET_CMD_SECURITY) {
        trace_LOG(trace_INFO,  "-I- SET SECURITY BIT command \n\r");

        if( FLASHD_SetSecurityBit() != 0)
        {
            trace_LOG(trace_INFO,  "-E- SET SECURITY BIT failed! \n\r");
            pMailbox->status = APPLET_FAIL;
            goto exit;
        }

        trace_LOG(trace_INFO,  "-I- SET SECURITY BIT achieved\n\r");
        pMailbox->status = APPLET_SUCCESS;
    }
#endif

exit:
    // Acknowledge the end of command
    trace_LOG(trace_INFO,  "-I- \tEnd of Applet %x %x.\n\r", pMailbox->command, pMailbox->status);
    // Notify the host application of the end of the command processing
    pMailbox->command = ~(pMailbox->command);

    return 0;
}

