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

#ifndef NORFLASHCOMMON_H
#define NORFLASHCOMMON_H

//------------------------------------------------------------------------------
//          Defination
//------------------------------------------------------------------------------
/// \page "NorFlash Error Codes"
/// NorFlash API methods return either 0 (indicating that there was no error),
/// or one of the following error codes:
/// 
/// !Codes
/// - NorCommon_ERROR_NOMOREBLOCKS

/// Can not detect common flash infterface.
#define NorCommon_ERROR_UNKNOWNMODEL       1

/// A read operation cannot be carried out.
#define NorCommon_ERROR_CANNOTREAD         2

/// A write operation cannot be carried out.
#define NorCommon_ERROR_CANNOTWRITE        3

/// A erase operation cannot be carried out.
#define NorCommon_ERROR_CANNOTERASE        4

/// A locked operation cannot be carried out.
#define NorCommon_ERROR_PROTECT            5

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
extern unsigned char WriteCommand(
    unsigned char portWidth, 
    unsigned int commandAddress, 
    unsigned int command);
    
extern void ReadRawData(
    unsigned char portWidth,
    unsigned int  dataAddress,
    unsigned char *buffer);
        
extern void WriteRawData(
    unsigned char portWidth,
    unsigned int  dataAddress,
    unsigned char *buffer);
    
#endif //#ifndef NORFLASHCOMMON_H
