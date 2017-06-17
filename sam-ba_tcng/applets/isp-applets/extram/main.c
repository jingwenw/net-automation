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
#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <utility/assert.h>
#include <utility/trace.h>

#include <string.h>

//------------------------------------------------------------------------------
//         External definitions
//------------------------------------------------------------------------------
extern void LowLevelInit(void); 


//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Current project version.
#define VERSION "1.1"


//------------------------------------------------------------------------------
//         Local structure
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
            /// External memory voltage selection.
            unsigned int VddMemSel;
            /// External RAM type.
            unsigned int ramType;

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
    } argument;
};


//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// Marks the end of program space.
extern unsigned int end;

/// Trace level can be set at applet initialization
#if defined(DYNAMIC_TRACE_LEVEL)
    unsigned int trace_LEVEL = 0;
#endif

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
/// Go/No-Go test of the first 10K-Bytes of external RAM access.
/// \return 0 if test is failed else 1.
//------------------------------------------------------------------------------
static unsigned char ExtRAM_TestOk(void)
{
    unsigned int i;
    unsigned int *ptr = (unsigned int *) AT91C_EBI_SDRAM;

    for (i = 0; i < 10 * 1024; ++i) {

        if (i & 1) {
            ptr[i] = 0x55AA55AA | (1 << i);
        }
        else {
            ptr[i] = 0xAA55AA55 | (1 << i);
        }
    }

    for (i = 0; i < 10 * 1024; ++i) {
        if (i & 1) {
            if (ptr[i] != (0x55AA55AA | (1 << i))) {
                return 0;
            }
        }
        else {
            if (ptr[i] != (0xAA55AA55 | (1 << i))) {
                return 0;
            }
        }
    }

    return 1;
}

//------------------------------------------------------------------------------
/// Applet code for initializing the external RAM.
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    struct _Mailbox *pMailbox = (struct _Mailbox *) argv;

    // Configure the DBGU
    const Pin pinsDbgu[] = {PINS_DBGU};
    PIO_Configure(pinsDbgu, PIO_LISTSIZE(pinsDbgu));
    DBGU_Configure(DBGU_STANDARD, 115200, BOARD_MCK);

    // ----------------------------------------------------------
    // INIT:
    // ----------------------------------------------------------
    if (pMailbox->command == APPLET_CMD_INIT) {

        // Initialize PMC
        if (pMailbox->argument.inputInit.comType != JTAG_COM_TYPE) {
            LowLevelInit();
        }

#if defined(DYNAMIC_TRACE_LEVEL)
        trace_LEVEL = pMailbox->argument.inputInit.traceLevel;
#endif

        trace_LOG(trace_INFO, "-- EXTRAM ISP Applet %s %s %s --\n\r", VERSION, __DATE__, __TIME__);
        trace_LOG(trace_INFO, "-I- INIT command:\n\r");

        trace_LOG(trace_INFO, "-I- \tcommunication type : %d\n\r", pMailbox->argument.inputInit.comType);   

#if defined(at91cap9) 
        trace_LOG(trace_INFO, "-I- \tInit EBI Vdd : %s\n\r", (pMailbox->argument.inputInit.VddMemSel)?"3.3V":"1.8V");   
        BOARD_ConfigureVddMemSel( pMailbox->argument.inputInit.VddMemSel );
#endif //defined(at91cap9)

        if(pMailbox->argument.inputInit.ramType == 0) {
            // Configure SDRAMC       
            trace_LOG(trace_INFO, "-I- \tInit SDRAM...\n\r");               
            BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
        }
        else {      

            // Configure DDR-SDRAMC
#if defined(at91cap9)    
            trace_LOG(trace_INFO, "-I- \tInit DDR-SDRAM...\n\r");              
            BOARD_ConfigureDdram(0, BOARD_DDRAM_BUSWIDTH);
#endif //defined(at91cap9)        
        }

        // Test external RAM access
        if (ExtRAM_TestOk()) {

            pMailbox->status = APPLET_SUCCESS;
        }
        else {
            pMailbox->status = APPLET_FAIL;
        }

        pMailbox->argument.outputInit.bufferAddress = ((unsigned int) &end);
        pMailbox->argument.outputInit.bufferSize = 0;
        pMailbox->argument.outputInit.memorySize = BOARD_SDRAM_SIZE;

        trace_LOG(trace_INFO, "-I- \tInit successful.\n\r");
    }

    // Acknowledge the end of command
    trace_LOG(trace_INFO, "-I- \tEnd of applet (command : %x --- status : %x)\n\r", pMailbox->command, pMailbox->status);

    // Notify the host application of the end of the command processing
    pMailbox->command = ~(pMailbox->command);

    return 0;
}

