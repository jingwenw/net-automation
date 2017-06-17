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
#ifndef NORFLASHAPI_H
#define NORFLASHAPI_H
//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
#include "NorFlashCFI.h"
//------------------------------------------------------------------------------
//         Type
//------------------------------------------------------------------------------

/// Reset or exit CFI query mode function.
typedef void (*fReset) (struct NorFlashInfo *, unsigned int );
/// Write buffer to pNorFlash function.
typedef unsigned char (*fWriteData)(struct NorFlashInfo *, unsigned int , unsigned char *, unsigned int );
/// Read manufactory function.
typedef unsigned int (*fReadManufactoryID)(struct NorFlashInfo *);
/// Read device ID code function.
typedef unsigned int (*fReadDeviceID)(struct NorFlashInfo *);
/// Full erase chip function.
typedef unsigned char (*fEraseChip) (struct NorFlashInfo *);
/// Erase single sector function.
typedef unsigned char (*fEraseSector)(struct NorFlashInfo *, unsigned int );


struct NorFlashOperations {
   /// Reset or exit CFI query mode function.
    void (*_fReset)(struct NorFlashInfo *pNorFlashInfo, unsigned int address);
   /// Write buffer to norflash function.
    unsigned char (*_fWriteData)(struct NorFlashInfo *pNorFlashInfo,
       unsigned int address,
       unsigned char *buffer,
       unsigned int size);
   /// Read manufactory function.
    unsigned int  (*_fReadManufactoryID)(struct NorFlashInfo *pNorFlashInfo);
   /// Read device ID code function.
    unsigned int (*_fReadDeviceID)(struct NorFlashInfo *pNorFlashInfo);
   /// Full erase chip function.
    unsigned char (*_fEraseChip) (struct NorFlashInfo *pNorFlashInfo);
   /// Erase single sector function.
    unsigned char (*_fEraseSector)(struct NorFlashInfo *pNorFlashInfo, unsigned int address);
};

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

extern void NORFLASH_Reset(struct NorFlash *norFlash, unsigned int address);

extern unsigned int NORFLASH_ReadManufactoryID(struct  NorFlash *norFlash);

extern unsigned int NORFLASH_ReadDeviceID(struct  NorFlash *norFlash);

extern unsigned char NORFLASH_EraseSector(
    struct NorFlash *norFlash, 
    unsigned int sectorAddr);

extern unsigned char NORFLASH_EraseChip(
    struct NorFlash *norFlash);

extern unsigned char NORFLASH_WriteData(
    struct NorFlash *norFlash,
    unsigned int address,
    unsigned char *buffer,
    unsigned int size);

extern unsigned char NORFLASH_ReadData(
    struct NorFlash *norFlash,
    unsigned int address,
    unsigned char *buffer,
    unsigned int size);


#endif //#ifndef NORFLASHAPI_H




