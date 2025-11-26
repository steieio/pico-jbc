/*****************************************************************************/
/*                                                                           */
/* Module:           jbistub.c                                               */
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
/* Description:      Jam STAPL ByteCode Player main source file              */
/*                                                                           */
/*                   Supports Intel ByteBlaster hardware download cable      */
/*                   on Windows 95 and Windows NT operating systems.         */
/*                   (A device driver is required for Windows NT.)           */
/*                                                                           */
/*                   Also supports BitBlaster hardware download cable on     */
/*                   Windows 95, Windows NT, and UNIX platforms.             */
/*                                                                           */
/* Revisions:        1.1 fixed control port initialization for ByteBlaster   */
/*                   2.0 added support for STAPL bytecode format, added code */
/*                       to get printer port address from Windows registry   */
/*                   2.1 improved messages, fixed delay-calibration bug in   */
/*                       16-bit DOS port, added support for "alternative     */
/*                       cable X", added option to control whether to reset  */
/*                       the TAP after execution, moved porting macros into  */
/*                       jbiport.h                                           */
/*                   2.2 added support for static memory                     */
/*                       fixed /W4 warnings                                  */
/*                                                                           */
/*****************************************************************************/



#include "altera-jbc/code/jbiport.h"

#include "jbipico.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(USE_STATIC_MEMORY)
	#define N_STATIC_MEMORY_KBYTES ((unsigned int) USE_STATIC_MEMORY)
	#define N_STATIC_MEMORY_BYTES (N_STATIC_MEMORY_KBYTES * 1024)
	#define POINTER_ALIGNMENT sizeof(DWORD)
#else /* USE_STATIC_MEMORY */
	#include <malloc.h>
	#define POINTER_ALIGNMENT sizeof(BYTE)
#endif /* USE_STATIC_MEMORY */
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"

#include "altera-jbc/code/jbiexprt.h"

/************************************************************************
*
*	Global variables
*/

/* file buffer for Jam STAPL ByteCode input file */
unsigned char *file_buffer = NULL;
long file_pointer = 0L;
long file_length = 0L;

/* delay count for one millisecond delay */
long one_ms_delay = 0L;

#if defined(USE_STATIC_MEMORY)
	unsigned char static_memory_heap[N_STATIC_MEMORY_BYTES] = { 0 };
#endif /* USE_STATIC_MEMORY */

#if defined(USE_STATIC_MEMORY) || defined(MEM_TRACKER)
	unsigned int n_bytes_allocated = 0;
#endif /* USE_STATIC_MEMORY || MEM_TRACKER */

#if defined(MEM_TRACKER)
	unsigned int peak_memory_usage = 0;
	unsigned int peak_allocations = 0;
	unsigned int n_allocations = 0;
#if defined(USE_STATIC_MEMORY)
	unsigned int n_bytes_not_recovered = 0;
#endif /* USE_STATIC_MEMORY */
	const DWORD BEGIN_GUARD = 0x01234567;
	const DWORD END_GUARD = 0x76543210;
#endif /* MEM_TRACKER */

/*
*	This structure stores information about each available vector signal
*/
struct VECTOR_LIST_STRUCT
{
	char *signal_name;
	int  hardware_bit;
	int  vector_index;
};

struct VECTOR_LIST_STRUCT vector_list[] =
{
	/* add a record here for each vector signal */
	{ "", 0, -1 }
};

#define VECTOR_SIGNAL_COUNT ((int)(sizeof(vector_list)/sizeof(vector_list[0])))

BOOL jbi_verbose = FALSE;


void jbi_init(BOOL verbose) {
  jbi_verbose = verbose;
  gpio_init(PIN_LED);
  gpio_set_dir(PIN_LED, GPIO_OUT);
  gpio_init(PIN_TCK);
  gpio_set_dir(PIN_TCK, GPIO_OUT);
  gpio_init(PIN_TMS);
  gpio_set_dir(PIN_TMS, GPIO_OUT);
  gpio_init(PIN_TDI);
  gpio_set_dir(PIN_TDI, GPIO_OUT);
  gpio_init(PIN_TDO);
  gpio_set_dir(PIN_TDO, GPIO_IN);
  gpio_put(PIN_LED, 1);  // turn on the LED to indicate ready
}


/************************************************************************
*
*	Customized interface functions for Jam STAPL ByteCode Player I/O:
*
*	jbi_jtag_io()
*	jbi_message()
*	jbi_delay()
*/

int jbi_jtag_io(int tms, int tdi, int read_tdo) {
  int tdo = 0;
  gpio_put(PIN_TMS, tms);
  gpio_put(PIN_TDI, tdi);
  if (read_tdo) {
    tdo = gpio_get(PIN_TDO);
  }
  gpio_put(PIN_LED, 1);
  gpio_put(PIN_TCK, 1);
  sleep_us(1);
  gpio_put(PIN_TCK, 0);
  gpio_put(PIN_LED, 0);
  return (tdo);
}

void jbi_message(char *message_text) {
	if (jbi_verbose) {
	    printf("%s\n", message_text);
	}
}

void jbi_export_integer(char *key, long value) {
	if (jbi_verbose) {
		printf("Export: key = \"%s\", value = %ld\n", key, value);
  }
}

#define HEX_LINE_CHARS 72
#define HEX_LINE_BITS (HEX_LINE_CHARS * 4)

char conv_to_hex(unsigned long value)
{
	char c;

	if (value > 9)
	{
		c = (char) (value + ('A' - 10));
	}
	else
	{
		c = (char) (value + '0');
	}

	return (c);
}

void jbi_export_boolean_array(char *key, unsigned char *data, long count)
{
	char string[HEX_LINE_CHARS + 1];
	long i, offset;
	unsigned long size, line, lines, linebits, value, j, k;

	if (jbi_verbose)
	{
		if (count > HEX_LINE_BITS)
		{
			printf("Export: key = \"%s\", %ld bits, value = HEX\n", key, count);
			lines = (count + (HEX_LINE_BITS - 1)) / HEX_LINE_BITS;

			for (line = 0; line < lines; ++line)
			{
				if (line < (lines - 1))
				{
					linebits = HEX_LINE_BITS;
					size = HEX_LINE_CHARS;
					offset = count - ((line + 1) * HEX_LINE_BITS);
				}
				else
				{
					linebits = count - ((lines - 1) * HEX_LINE_BITS);
					size = (linebits + 3) / 4;
					offset = 0L;
				}

				string[size] = '\0';
				j = size - 1;
				value = 0;

				for (k = 0; k < linebits; ++k)
				{
					i = k + offset;
					if (data[i >> 3] & (1 << (i & 7))) value |= (1 << (i & 3));
					if ((i & 3) == 3)
					{
						string[j] = conv_to_hex(value);
						value = 0;
						--j;
					}
				}
				if ((k & 3) > 0) string[j] = conv_to_hex(value);

				printf("%s\n", string);
			}

		}
		else
		{
			size = (count + 3) / 4;
			string[size] = '\0';
			j = size - 1;
			value = 0;

			for (i = 0; i < count; ++i)
			{
				if (data[i >> 3] & (1 << (i & 7))) value |= (1 << (i & 3));
				if ((i & 3) == 3)
				{
					string[j] = conv_to_hex(value);
					value = 0;
					--j;
				}
			}
			if ((i & 3) > 0) string[j] = conv_to_hex(value);

			printf("Export: key = \"%s\", %ld bits, value = HEX %s\n",
				key, count, string);
		}
	}
}

void jbi_delay(long microseconds) {
  sleep_us(microseconds);
}

void *jbi_malloc(unsigned int size) {
  void *malloc_result = malloc(size);
  if (malloc_result == NULL) { printf("\n!! Malloc Failed !!\n"); }
  return malloc_result;
}

void jbi_free(void *ptr) {
  free(ptr);
}

char *error_text[] =
{
/* JBIC_SUCCESS            0 */ "success",
/* JBIC_OUT_OF_MEMORY      1 */ "out of memory",
/* JBIC_IO_ERROR           2 */ "file access error",
/* JBIC_SYNTAX_ERROR       3 */ "syntax error",
/* JBIC_UNEXPECTED_END     4 */ "unexpected end of file",
/* JBIC_UNDEFINED_SYMBOL   5 */ "undefined symbol",
/* JBIC_REDEFINED_SYMBOL   6 */ "redefined symbol",
/* JBIC_INTEGER_OVERFLOW   7 */ "integer overflow",
/* JBIC_DIVIDE_BY_ZERO     8 */ "divide by zero",
/* JBIC_CRC_ERROR          9 */ "CRC mismatch",
/* JBIC_INTERNAL_ERROR    10 */ "internal error",
/* JBIC_BOUNDS_ERROR      11 */ "bounds error",
/* JBIC_TYPE_MISMATCH     12 */ "type mismatch",
/* JBIC_ASSIGN_TO_CONST   13 */ "assignment to constant",
/* JBIC_NEXT_UNEXPECTED   14 */ "NEXT unexpected",
/* JBIC_POP_UNEXPECTED    15 */ "POP unexpected",
/* JBIC_RETURN_UNEXPECTED 16 */ "RETURN unexpected",
/* JBIC_ILLEGAL_SYMBOL    17 */ "illegal symbol name",
/* JBIC_VECTOR_MAP_FAILED 18 */ "vector signal name not found",
/* JBIC_USER_ABORT        19 */ "execution cancelled",
/* JBIC_STACK_OVERFLOW    20 */ "stack overflow",
/* JBIC_ILLEGAL_OPCODE    21 */ "illegal instruction code",
/* JBIC_PHASE_ERROR       22 */ "phase error",
/* JBIC_SCOPE_ERROR       23 */ "scope error",
/* JBIC_ACTION_NOT_FOUND  24 */ "action not found",
};

#define MAX_ERROR_CODE (int)((sizeof(error_text)/sizeof(error_text[0]))+1)


/************************************************************************/

int jbi_play(jbc_uf2_hdr_t *juh, char *action) {
    int jbc_length = juh->length;
    char *jbc_buffer = (char *) juh->location;
    JBI_RETURN_TYPE crc_result = JBIC_SUCCESS;
	JBI_RETURN_TYPE exec_result = JBIC_SUCCESS;
	unsigned short expected_crc = 0;
	unsigned short actual_crc = 0;
	char key[33] = {0};
	char value[257] = {0};
	long offset = 0L;
	long error_address = 0L;

	int exit_code = 0;
	int format_version = 0;

//	char *action = juh->action;
	int action_count = 0;
	char **init_list = NULL;

	long workspace_size = 0;
    char *workspace = NULL;
	char *exit_string = NULL;
	int reset_jtag = 1;
//	int execute_program = 1;
	int procedure_count = 0;
	int index = 0;
	char *action_name = NULL;
	char *description = NULL;
	JBI_PROCINFO *procedure_list = NULL;
	JBI_PROCINFO *procptr = NULL;

			/*
			*	Check CRC
			*/
			crc_result = jbi_check_crc(jbc_buffer, jbc_length,
				&expected_crc, &actual_crc);

			if (jbi_verbose || (crc_result == JBIC_CRC_ERROR))
			{
				switch (crc_result)
				{
				case JBIC_SUCCESS:
					printf("CRC matched: CRC value = %04X\n", actual_crc);
					break;

				case JBIC_CRC_ERROR:
					printf("CRC mismatch: expected %04X, actual %04X\n",
						expected_crc, actual_crc);
					break;

				case JBIC_UNEXPECTED_END:
					printf("Expected CRC not found, actual CRC value = %04X\n",
						actual_crc);
					break;

				case JBIC_IO_ERROR:
					printf("Error: File format is not recognized.\n");
					exit(1);
					break;

				default:
					printf("CRC function returned error code %d\n", crc_result);
					break;
				}
			}

			if (jbi_verbose || (action == NULL))
			{
				/*
				*	Display file format version
				*/
				jbi_get_file_info(jbc_buffer, jbc_length,
					&format_version, &action_count, &procedure_count);

				printf("File format is %s ByteCode format\n",
					(format_version == 2) ? "Jam STAPL" : "pre-standardized Jam 1.1");

				/*
				*	Dump out NOTE fields
				*/
				while (jbi_get_note(jbc_buffer, jbc_length,
					&offset, key, value, 256) == 0)
				{
					printf("NOTE \"%s\" = \"%s\"\n", key, value);
				}

				/*
				*	Dump the action table
				*/
				if ((format_version == 2) && (action_count > 0))
				{
					printf("\nActions available in this file:\n");

					for (index = 0; index < action_count; ++index)
					{
						jbi_get_action_info(jbc_buffer, jbc_length,
							index, &action_name, &description, &procedure_list);

						if (description == NULL)
						{
							printf("%s\n", action_name);
						}
						else
						{
							printf("%s \"%s\"\n", action_name, description);
						}
						procptr = procedure_list;
						while (procptr != NULL)
						{
							if (procptr->attributes != 0)
							{
								printf("    %s (%s)\n", procptr->name,
									(procptr->attributes == 1) ?
									"optional" : "recommended");
							}
							procedure_list = procptr->next;
							jbi_free(procptr);
							procptr = procedure_list;
						}
					}

					/* add a blank line before execution messages */
					if (action != NULL) printf("\n");
				}
			}

			if (action != NULL)
			{
				gpio_put(PIN_LED, 0);
				printf("Executing %s\n", juh->action);
				printf("Filename: %s\n", juh->filename);
				printf("Description: %s\n", juh->description);
				/*
				*	Execute the Jam STAPL ByteCode program
				*/
				exec_result = jbi_execute(jbc_buffer, jbc_length, workspace,
					workspace_size, action, init_list, reset_jtag,
					&error_address, &exit_code, &format_version);

				if (exec_result == JBIC_SUCCESS)
				{
					if (format_version == 2)
					{
						switch (exit_code)
						{
						case  0: exit_string = "Success"; break;
						case  1: exit_string = "Checking chain failure"; break;
						case  2: exit_string = "Reading IDCODE failure"; break;
						case  3: exit_string = "Reading USERCODE failure"; break;
						case  4: exit_string = "Reading UESCODE failure"; break;
						case  5: exit_string = "Entering ISP failure"; break;
						case  6: exit_string = "Unrecognized device"; break;
						case  7: exit_string = "Device revision is not supported"; break;
						case  8: exit_string = "Erase failure"; break;
						case  9: exit_string = "Device is not blank"; break;
						case 10: exit_string = "Device programming failure"; break;
						case 11: exit_string = "Device verify failure"; break;
						case 12: exit_string = "Read failure"; break;
						case 13: exit_string = "Calculating checksum failure"; break;
						case 14: exit_string = "Setting security bit failure"; break;
						case 15: exit_string = "Querying security bit failure"; break;
						case 16: exit_string = "Exiting ISP failure"; break;
						case 17: exit_string = "Performing system test failure"; break;
						default: exit_string = "Unknown exit code"; break;
						}
					}
					else
					{
						switch (exit_code)
						{
						case 0: exit_string = "Success"; break;
						case 1: exit_string = "Illegal initialization values"; break;
						case 2: exit_string = "Unrecognized device"; break;
						case 3: exit_string = "Device revision is not supported"; break;
						case 4: exit_string = "Device programming failure"; break;
						case 5: exit_string = "Device is not blank"; break;
						case 6: exit_string = "Device verify failure"; break;
						case 7: exit_string = "SRAM configuration failure"; break;
						default: exit_string = "Unknown exit code"; break;
						}
					}

					printf("Exit code = %d... %s\n", exit_code, exit_string);
				}
				else if ((format_version == 2) &&
					(exec_result == JBIC_ACTION_NOT_FOUND))
				{
					if ((action == NULL) || (*action == '\0'))
					{
						printf("Error: no action specified for Jam STAPL file.\nProgram terminated.\n");
					}
					else
					{
						printf("Error: action \"%s\" is not supported for this Jam STAPL file.\nProgram terminated.\n", action);
					}
				}
				else if (exec_result < MAX_ERROR_CODE)
				{
					printf("Error at address %ld: %s.\nProgram terminated.\n",
						error_address, error_text[exec_result]);
				}
				else
				{
					printf("Unknown error code %ld\n", exec_result);
				}

			}

    return exec_result;

}

/************************************************************************/

#if !defined (DEBUG)
#pragma optimize ("ceglt", off)
#endif
