
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
 * this list of conditions and the disclaiimer below.
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

#include "NandFlashModelList.h"
#include "NandSpareScheme.h"

//------------------------------------------------------------------------------
//         Exported variables
//------------------------------------------------------------------------------

/// List of NandFlash models which can be recognized by the software.
const struct NandFlashModel nandFlashModelList[NandFlashModelList_SIZE] = {

// |  ID  | Mo  | Block | Page |        Options
    {0x6e,    1,   4096,  256,  NandFlashModel_DATABUS8,  &nandSpareScheme256},
	{0x64,    2,   4096,  256,  NandFlashModel_DATABUS8,  &nandSpareScheme256},
	{0x6b,    4,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0xe8,    1,   4096,  256,  NandFlashModel_DATABUS8,  &nandSpareScheme256},
	{0xec,    1,   4096,  256,  NandFlashModel_DATABUS8,  &nandSpareScheme256},
	{0xea,    2,   4096,  256,  NandFlashModel_DATABUS8,  &nandSpareScheme256},
	{0xd5,    4,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0xe3,    4,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0xe5,    4,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0xd6,    8,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
                                                          
	{0x39,    8,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0xe6,    8,   8192,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x49,    8,   8192,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x59,    8,   8192,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
                                                          
	{0x33,   16,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x73,   16,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x43,   16,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x53,   16,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
                                                          
	{0x35,   32,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x75,   32,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x45,   32,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x55,   32,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
                                                          
	{0x36,   64,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x76,   64,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x46,   64,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x56,   64,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
                          
	{0x78,  128,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x39,  128,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x79,  128,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
	{0x72,  128,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x49,  128,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x74,  128,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
	{0x59,  128,  16384,  512,  NandFlashModel_DATABUS16, &nandSpareScheme512},
                                
	{0x71,  256,  16384,  512,  NandFlashModel_DATABUS8,  &nandSpareScheme512},
           
// Large blocks devices. Parameters must be fetched from the extended I
#define OPTIONS     NandFlashModel_COPYBACK

	{0xA2,   64, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048},
	{0xF2,   64, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048},
	{0xB2,   64, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xC2,   64, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
                       
	{0xA1,  128, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xF1,  128, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xB1,  128, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xC1,  128, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	                   
	{0xAA,  256, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xDA,  256, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xBA,  256, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xCA,  256, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	                   
	{0xAC,  512, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xDC,  512, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xBC,  512, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xCC,  512, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
                                                 
	{0xA3, 1024, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xD3, 1024, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xB3, 1024, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xC3, 1024, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
                                                 
	{0xA5, 2048, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xD5, 2048, 0, 0, NandFlashModel_DATABUS8  | OPTIONS, &nandSpareScheme2048}, 
	{0xB5, 2048, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
	{0xC5, 2048, 0, 0, NandFlashModel_DATABUS16 | OPTIONS, &nandSpareScheme2048},
};

