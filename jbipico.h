/*****************************************************************************/
/*                                                                           */
/* Module:           jbipico.h                                               */
/*                                                                           */
/*                   Copyright 2025 Altera Corporation                       */
/*                   Copyright 2025 Greg Steiert                             */
/*                                                                           */
/* SPDX-License-Identifier: MIT-0                                            */
/*                                                                           */
/* Permission is hereby granted, free of charge, to any person obtaining a   */
/* copy of this software and associated documentation files (the             */
/* "Software"),to deal in the Software without restriction, including        */
/* without limitation the rights to use, copy, modify, merge, publish,       */
/* distribute, sublicense, and/or sell copies of the Software, and to permit */
/* persons to whom the Software is furnished to do so.                       */
/*                                                                           */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                */
/* MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY      */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT */
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                */
/*                                                                           */
/* Description:      Jam STAPL ByteCode Player Pico Port Header File         */
/*                                                                           */
/* Revisions:                                                                */
/*                                                                           */
/*****************************************************************************/

#ifndef INC_JBIPICO_H
#define INC_JBIPICO_H

#include <unistd.h>

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define TRUE 1
#define FALSE 0


#ifdef CYCLOMOD

#define JUF2_ADDRESS   0x10F00000

#define PIN_TCK     6
#define PIN_TMS     5
#define PIN_TDI     7
#define PIN_TDO     4
#define PIN_LED     28
#define PIN_CLKOUT  21

#else

#define JUF2_ADDRESS   0x10020000

#define PIN_TCK   18
#define PIN_TMS   19
#define PIN_TDI   16
#define PIN_TDO   17
#define PIN_LED   25

#endif

#define JBD_UF2_HDR_TAG   0x3246554A  

typedef struct _jbc_uf2_hdr {
    uint32_t tag;
    uint32_t version;
    uint32_t length;
    uint32_t location;
    uint32_t speed;
    uint32_t options;
    char action[16];
    char filename[32];
    char description[128];
} jbc_uf2_hdr_t;

void jbi_init(BOOL verbose);

int jbi_play(jbc_uf2_hdr_t *juf2, char *action);

#endif /* INC_JBIPICO_H */