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

#include "bmp.h"
#include <utility/trace.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Internal constants
//------------------------------------------------------------------------------

/// BMP magic number ('BM').
#define BMP_TYPE        0x4D42

//------------------------------------------------------------------------------
//         Internal types
//------------------------------------------------------------------------------

struct BMPPaletteEntry {

    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char filler;
};

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Returns 1 if the header of a BMP file is valid; otherwise returns 0.
/// \param file  Buffer holding the file to examinate.
//------------------------------------------------------------------------------
unsigned char BMP_IsValid(void *file)
{
    return ((struct BMPHeader *) file)->type == BMP_TYPE;
}

//------------------------------------------------------------------------------
/// Returns the size of a BMP image given at least its header (the file does
/// not have to be complete).
/// \param file  Pointer to the buffer which holds the BMP file.
//------------------------------------------------------------------------------
unsigned int BMP_GetFileSize(void *file)
{
    return ((struct BMPHeader *) file)->fileSize;
}

//------------------------------------------------------------------------------
/// Loads a BMP image located at the given address, decodes it and stores the
/// resulting image inside the provided buffer. Image must have the specified
/// width & height.
/// Returns 0 if the image has been loaded; otherwise returns an error code.
/// \param file  Buffer which holds the BMP file.
/// \param buffer  Buffer in which to store the decoded image.
/// \param width  Buffer width in pixels.
/// \param height  Buffer height in pixels.
/// \param bpp  Number of bits per pixels that the buffer stores.
//------------------------------------------------------------------------------
unsigned char BMP_Decode(
    void *file,
    unsigned char *buffer,
    unsigned int width,
    unsigned int height,
    unsigned char bpp)
{
	struct BMPHeader *header;
    unsigned int i, j;
    unsigned char r, g, b;
    unsigned char *image;

	// Read header information
	header = (struct BMPHeader *) file;

    // Verify that the file is valid
    if (!BMP_IsValid(file)) {

        trace_LOG(trace_ERROR,
                  "BMP_Decode: File type is not 'BM' (0x%04X).\n\r",
                  header->type);
        return 1;
    }

    // Check that parameters match
    if ((header->compression != 0)
        || (header->width != width)
        || (header->height != height)) {

        trace_LOG(trace_ERROR, "BMP_Decode: File format not supported\n\r");
        trace_LOG(trace_ERROR, " -> .compression = %d\n\r", header->compression);
        trace_LOG(trace_ERROR, " -> .width = %d\n\r", header->width);
        trace_LOG(trace_ERROR, " -> .height = %d\n\r", header->height);
        trace_LOG(trace_ERROR, " -> .bits = %d\n\r", header->bits);
        return 2;
    }

    // Get image data
    image = (unsigned char *) ((unsigned int) file + header->offset);

    // Check that the bpp resolution is supported
    // Only a 24-bit output & 24- or 8-bit input are supported
    if (bpp != 24) {

        trace_LOG(trace_ERROR, "BMP_Decode: Output resolution not supported\n\r");
        return 3;
    }
    else if (header->bits == 24) {

        // Copy raw data from BMP to buffer (reversing row order)
        for (i=0; i < height; i++) {
        
            memcpy(buffer + (i * width) * 3,
                   image + ((height - i - 1) * width) * 3,
                   width * 3);
        }

        // Swap red and blue
        for (i=0; i < height; i++) {
            for (j=0; j < width; j++) {

                r = buffer[(i * width + j) * 3 + 2];
                g = buffer[(i * width + j) * 3 + 1];
                b = buffer[(i * width + j) * 3];

                buffer[(i * width + j) * 3] = r;
                buffer[(i * width + j) * 3 + 1] = g;
                buffer[(i * width + j) * 3 + 2] = b;
            }
        }
    }
    else if (header->bits == 8) {

        // Retrieve palette
        struct BMPPaletteEntry palette[256];
        memcpy(palette,
               (unsigned char *) ((unsigned int) file + sizeof(struct BMPHeader)),
               header->offset - sizeof(struct BMPHeader));

        // Decode image (reversing row order)
        for (i=0; i < height; i++) {
            for (j=0; j < width; j++) {

                r = palette[image[(height - i - 1) * width + j]].r;
                g = palette[image[(height - i - 1) * width + j]].g;
                b = palette[image[(height - i - 1) * width + j]].b;

                buffer[(i * width + j) * 3] = r;
                buffer[(i * width + j) * 3 + 1] = g;
                buffer[(i * width + j) * 3 + 2] = b;
            }
        }
    }
    else {

        trace_LOG(trace_ERROR, "BMP_Decode: Input resolution not supported\n\r");
        return 4;
    }

    return 0;
}
