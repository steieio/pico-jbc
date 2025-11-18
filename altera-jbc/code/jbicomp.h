/*****************************************************************************/
/*                                                                           */
/* Module:           jbicomp.h                                               */
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
/* Description:      Contains the function prototypes for compressing        */
/*                   and uncompressing Boolean array data.                   */
/*                                                                           */
/*****************************************************************************/

#ifndef INC_JBICOMP_H
#define INC_JBICOMP_H

#if PORT==DOS

void jbi_uncompress_page
(
	int variable_id,
	int page,
	int version
);

#else

unsigned long jbi_uncompress
(
	unsigned char *in, 
	unsigned long in_length, 
	unsigned char *out, 
	unsigned long out_length,
	int version
);

#endif /* PORT==DOS */

#endif /* INC_JBICOMP_H */
