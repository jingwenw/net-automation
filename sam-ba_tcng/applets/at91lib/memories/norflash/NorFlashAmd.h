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
#ifndef NORFLASHAMD_H
#define NORFLASHAMD_H

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

void AMD_Reset(struct NorFlashInfo *pNorFlashInfo, unsigned int address);

unsigned int AMD_ReadManufactoryId(struct NorFlashInfo *pNorFlashInfo);

unsigned int AMD_ReadDeviceID(struct NorFlashInfo *pNorFlashInfo);

unsigned char AMD_EraseSector(
    struct NorFlashInfo *pNorFlashInfo, 
    unsigned int sectorAddr);
    
unsigned char AMD_EraseChip(struct NorFlashInfo *pNorFlashInfo);

unsigned char AMD_Write_Data(
    struct NorFlashInfo *pNorFlashInfo,
    unsigned int address,
    unsigned char *buffer,
    unsigned int size);

const struct NorFlashOperations amdOperations = {
   AMD_Reset, 
   AMD_Write_Data,
   AMD_ReadManufactoryId, 
   AMD_ReadDeviceID,
   AMD_EraseChip,
   AMD_EraseSector
};

#endif //#ifndef NORFLASHAMD_H
