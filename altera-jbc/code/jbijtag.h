/*****************************************************************************/
/*                                                                           */
/* Module:           jbijtag.h                                               */
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
/* Description: Definitions of JTAG constants, types, and functions          */
/*                                                                           */
/*****************************************************************************/

#ifndef INC_JBIJTAG_H
#define INC_JBIJTAG_H

/****************************************************************************/
/*																			*/
/*	Function Prototypes														*/
/*																			*/
/****************************************************************************/
typedef enum
{
	JBI_ILLEGAL_JTAG_STATE = -1,
	RESET = 0,
	IDLE = 1,
	DRSELECT = 2,
	DRCAPTURE = 3,
	DRSHIFT = 4,
	DREXIT1 = 5,
	DRPAUSE = 6,
	DREXIT2 = 7,
	DRUPDATE = 8,
	IRSELECT = 9,
	IRCAPTURE = 10,
	IRSHIFT = 11,
	IREXIT1 = 12,
	IRPAUSE = 13,
	IREXIT2 = 14,
	IRUPDATE = 15

} JBIE_JTAG_STATE;


JBI_RETURN_TYPE jbi_init_jtag
(
	void
);

JBI_RETURN_TYPE jbi_set_drstop_state
(
    JBIE_JTAG_STATE state
);

JBI_RETURN_TYPE jbi_set_irstop_state
(
    JBIE_JTAG_STATE state
);

JBI_RETURN_TYPE jbi_set_dr_preamble
(
	unsigned int count,
	unsigned int start_index,
	unsigned char *preamble_data
);

JBI_RETURN_TYPE jbi_set_ir_preamble
(
	unsigned int count,
	unsigned int start_index,
	unsigned char *preamble_data
);

JBI_RETURN_TYPE jbi_set_dr_postamble
(
	unsigned int count,
	unsigned int start_index,
	unsigned char *postamble_data
);

JBI_RETURN_TYPE jbi_set_ir_postamble
(
	unsigned int count,
	unsigned int start_index,
	unsigned char *postamble_data
);

JBI_RETURN_TYPE jbi_goto_jtag_state
(
    JBIE_JTAG_STATE state
);

JBI_RETURN_TYPE jbi_do_wait_cycles
(
	long cycles,
	JBIE_JTAG_STATE wait_state
);

JBI_RETURN_TYPE jbi_do_wait_microseconds
(
	long microseconds,
	JBIE_JTAG_STATE wait_state
);

JBI_RETURN_TYPE jbi_do_irscan
(
	unsigned int count,
	unsigned char *tdi_data,
	unsigned int start_index
);

JBI_RETURN_TYPE jbi_swap_ir
(
	unsigned int count,
	unsigned char *in_data,
	unsigned int in_index,
	unsigned char *out_data,
	unsigned int out_index
);

JBI_RETURN_TYPE jbi_do_drscan
(
	unsigned int count,
	unsigned char *tdi_data,
	unsigned long start_index
);

JBI_RETURN_TYPE jbi_swap_dr
(
	unsigned int count,
	unsigned char *in_data,
	unsigned long in_index,
	unsigned char *out_data,
	unsigned int out_index
);

void jbi_free_jtag_padding_buffers
(
	int reset_jtag
);

JBI_RETURN_TYPE jbi_do_drscan_multi_page
(
	unsigned int variable_id,
	unsigned long long_count,
	unsigned long long_index,
	int version
);

#endif /* INC_JBIJTAG_H */
