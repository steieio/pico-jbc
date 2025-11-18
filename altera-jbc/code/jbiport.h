/*****************************************************************************/
/*                                                                           */
/* Module:           jbiport.h                                               */
/*                                                                           */
/*                   Copyright 2025 Altera Corporation                       */
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
/* Description:      Defines porting macros                                  */
/*                                                                           */
/*****************************************************************************/

#ifndef INC_JBIPORT_H
#define INC_JBIPORT_H

/*
*  PORT defines the target platform: DOS, WINDOWS, UNIX, or EMBEDDED
*
*  PORT = DOS      means a 16-bit DOS console-mode application
*
*  PORT = WINDOWS  means a 32-bit WIN32 console-mode application for
*                  Windows 95, 98, 2000, ME or NT.  On NT this will use the
*                  DeviceIoControl() API to access the Parallel Port.
*
*  PORT = UNIX     means any UNIX system.  BitBlaster access is support via
*                  the standard ANSI system calls open(), read(), write().
*                  The ByteBlaster is not supported.
*
*  PORT = EMBEDDED means all DOS, WINDOWS, and UNIX code is excluded. 
*                  Remaining code supports 16 and 32-bit compilers. 
*                  Additional porting steps may be necessary. See readme 
*                  file for more details.
*/

#define DOS      2
#define WINDOWS  3
#define UNIX     4
#define EMBEDDED 5

#ifndef PORT
/* change this line to build a different port */
#define PORT EMBEDDED
#endif

#endif /* INC_JBIPORT_H */
