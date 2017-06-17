
/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  SDC  -
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

#include "NandFlashModel.h"
#include "NandCommon.h"
#include <utility/trace.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------

/// Custom DEBUG trace level for the current file.
#define DEBUG           trace_DEBUG
#define IMPORTANT       trace_DEBUG

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Looks for a NandFlashModel corresponding to the given ID inside a list of
/// model. If found, the model variable is filled with the correct values.
/// This function returns 0 if a matching model has been found; otherwise it
/// returns NandCommon_ERROR_UNKNOWNMODEL.
/// \param modelList  List of NandFlashModel instances.
/// \param size  Number of models in list.
/// \param chipId  Identifier returned by the Nand(id1|(id2<<8)|(id3<<16)|(id4<<24)).
/// \param model  NandFlashModel instance to update with the model parameters.
//------------------------------------------------------------------------------
extern unsigned char NandFlashModel_Find(
    const struct NandFlashModel *modelList,
    unsigned int size,
    unsigned int chipId,
    struct NandFlashModel *model)
{
    unsigned char found = 0, id2, id4;
    unsigned int i;
    id2 = (unsigned char)(chipId>>8);
    id4 = (unsigned char)(chipId>>24);

    trace_LOG(trace_INFO, "Nandflash ID is 0x%08X\n\r", chipId);

    for(i=0; i<size; i++) {
        if(modelList[i].deviceId == id2) {
            found = 1;

            if(model) {

                memcpy(model, &modelList[i], sizeof(struct NandFlashModel));

                if(model->blockSizeInBytes == 0 || model->pageSizeInBytes == 0) {
                    trace_LOG(trace_DEBUG, "Fetch from ID4(0x%.2x):\r\n", id4);
                    /// Fetch from the extended ID4
                    /// ID4 D5  D4 BlockSize || D1  D0  PageSize
                    ///     0   0   64K      || 0   0   1K
                    ///     0   1   128K     || 0   1   2K
                    ///     1   0   256K     || 1   0   4K
                    ///     1   1   512K     || 1   1   8k
                    switch(id4 & 0x03) {
                        case 0x00: model->pageSizeInBytes = 1024; break;
                        case 0x01: model->pageSizeInBytes = 2048; break;
                        case 0x02: model->pageSizeInBytes = 4096; break;
                        case 0x03: model->pageSizeInBytes = 8192; break;
                    }
                    switch(id4 & 0x30) {
                        case 0x00: model->blockSizeInBytes = 64*1024;  break;
                        case 0x10: model->blockSizeInBytes = 128*1024; break;
                        case 0x20: model->blockSizeInBytes = 256*1024; break;
                        case 0x30: model->blockSizeInBytes = 512*1024; break;
                    }
                }
                
            }
            trace_LOG(IMPORTANT, "NAND Model found:\r\n");
            trace_LOG(IMPORTANT, ".deviceId = 0x%02X\r\n", model->deviceId);
            trace_LOG(IMPORTANT, ".deviceSizeInMegaBytes = %d\r\n", model->deviceSizeInMegaBytes);
            trace_LOG(IMPORTANT, ".blockSizeInBytes = %d\r\n", model->blockSizeInBytes);
            trace_LOG(IMPORTANT, ".pageSizeInBytes = %d\r\n", model->pageSizeInBytes);
            trace_LOG(IMPORTANT, ".options = 0x%02X\r\n", model->options);
            break;
        }
    }

    // Check if chip has been detected
    if (found) {

        return 0;
    }
    else {

        return NandCommon_ERROR_UNKNOWNMODEL;
    }
}

//------------------------------------------------------------------------------
/// Translates address/size access of a NandFlashModel to block, page and
/// offset values. The values are stored in the provided variables if their
/// pointer is not 0.
/// Returns 0 if the access is correct; otherwise returns
/// NandCommon_ERROR_OUTOFBOUNDS.
/// \param model  Pointer to a NandFlashModel instance.
/// \param address  Access address.
/// \param size  Access size in bytes.
/// \param block  Stores the first accessed block number.
/// \param page  Stores the first accessed page number inside the first block.
/// \param offset  Stores the byte offset inside the first accessed page.
//------------------------------------------------------------------------------
unsigned char NandFlashModel_TranslateAccess(
    const struct NandFlashModel *model,
    unsigned int address,
    unsigned int size,
    unsigned short *block,
    unsigned short *page,
    unsigned short *offset)
{
     // Check that access is not too big
    if ((address + size) > NandFlashModel_GetDeviceSizeInBytes(model)) {

        trace_LOG(DEBUG,
                  "NandFlashModel_TranslateAccess: out-of-bounds access.\n\r");
        return NandCommon_ERROR_OUTOFBOUNDS;
    }

    // Get Nand info
    unsigned int blockSize = NandFlashModel_GetBlockSizeInBytes(model);
    unsigned int pageSize = NandFlashModel_GetPageDataSize(model);

    // Translate address
    unsigned short tmpBlock = address / blockSize;
    address -= tmpBlock * blockSize;
    unsigned short tmpPage = address / pageSize;
    address -= tmpPage * pageSize;
    unsigned short tmpOffset = address;

    // Save results
    if (block) {

        *block = tmpBlock;
    }
    if (page) {

        *page = tmpPage;
    }
    if (offset) {

        *offset = tmpOffset;
    }

    return 0;
}

//------------------------------------------------------------------------------
/// Returns the spare area placement scheme used by a particular nandflash
/// model.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline const struct NandSpareScheme * NandFlashModel_GetScheme(
    const struct NandFlashModel *model)
{
    return model->scheme;
}

//------------------------------------------------------------------------------
/// Returns the device ID of a particular NandFlash model.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned char NandFlashModel_GetDeviceId(
   const struct NandFlashModel *model)
{
    return model->deviceId;
}

//------------------------------------------------------------------------------
/// Returns the number of blocks in the entire device.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned short NandFlashModel_GetDeviceSizeInBlocks(
   const struct NandFlashModel *model)
{
    return ((1024*1024) / model->blockSizeInBytes) * model->deviceSizeInMegaBytes;
}

//------------------------------------------------------------------------------
/// Returns the number of pages in the entire device.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned int NandFlashModel_GetDeviceSizeInPages(
   const struct NandFlashModel *model)
{
    return (unsigned int) NandFlashModel_GetDeviceSizeInBlocks(model) //* 8 // HACK
           * NandFlashModel_GetBlockSizeInPages(model);
}

//------------------------------------------------------------------------------
/// Returns the size of the whole device in bytes (this does not include the
/// size of the spare zones).
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned long long NandFlashModel_GetDeviceSizeInBytes(
   const struct NandFlashModel *model)
{
    return ((unsigned long long) model->deviceSizeInMegaBytes) << 20;
}

//------------------------------------------------------------------------------
/// Returns the number of pages in one single block of a device.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned short NandFlashModel_GetBlockSizeInPages(
   const struct NandFlashModel *model)
{
    return model->blockSizeInBytes / model->pageSizeInBytes;
}

//------------------------------------------------------------------------------
/// Returns the size in bytes of one single block of a device. This does not
/// take into account the spare zones size.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned int NandFlashModel_GetBlockSizeInBytes(
   const struct NandFlashModel *model)
{
    return model->blockSizeInBytes;
}

//------------------------------------------------------------------------------
/// Returns the size of the data area of a page in bytes.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned short NandFlashModel_GetPageDataSize(
   const struct NandFlashModel *model)
{
    return model->pageSizeInBytes;
}

//------------------------------------------------------------------------------
/// Returns the size of the spare area of a page in bytes.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned char NandFlashModel_GetPageSpareSize(
   const struct NandFlashModel *model)
{
    return (model->pageSizeInBytes>>5); /// Spare size is 16/512 of data size
}

//------------------------------------------------------------------------------
/// Returns the number of bits used by the data bus of a NandFlash device.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned char NandFlashModel_GetDataBusWidth(
   const struct NandFlashModel *model)
{
    return (model->options&NandFlashModel_DATABUS16)? 16: 8;
}

//------------------------------------------------------------------------------
/// Returns 1 if the given NandFlash model uses the "small blocks/pages"
/// command set; otherwise returns 0.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned char NandFlashModel_HasSmallBlocks(
   const struct NandFlashModel *model)
{
    return (model->pageSizeInBytes <= 512 )? 1: 0;
}

//------------------------------------------------------------------------------
/// Returns 1 if the device supports the copy-back operation. Otherwise returns
/// 0.
/// \param model  Pointer to a NandFlashModel instance.
//------------------------------------------------------------------------------
inline unsigned char NandFlashModel_SupportsCopyBack(
    const struct NandFlashModel *model)
{
    return ((model->options & NandFlashModel_COPYBACK) != 0);
}

