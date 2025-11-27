/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 * Copyringt (c) 2025 Greg Steiert
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/bootrom.h"
#include "hardware/clocks.h"
#include "tusb.h"

#include "jbipico.h"

//#define WAIT_FOR_CONSOLE
#define RUN_DEFAULT_ACTION

#define COMMAND_BUFFER_LENGTH  127

jbc_uf2_hdr_t *juf2 = (jbc_uf2_hdr_t *)JUF2_ADDRESS;
const char *delimiters = ", \t";

void process_command(char *buf) {
  char *command = strtok(buf, delimiters);
  switch (command[0]) {
    case '?':
    case 'H':
    case 'h':
      printf("JBC Player Help\n");
      break;
    case 'D':
    case 'd':
      printf("Display details\n");
      jbi_play(juf2, NULL);
      break;
    case 'I':
    case 'i':
      printf("Check IDCODE\n");
      jbi_play(juf2, "CHECK_IDCODE");
      break;
    case 'B':
    case 'b':
      printf("Blank Check\n");
      jbi_play(juf2, "BLANKCHECK");
      break;
    case 'C':
    case 'c':
      printf("Configure\n");
      jbi_play(juf2, "CONFIGURE");
      break;
    case 'P':
    case 'p':
      printf("Information\n");
      jbi_play(juf2, "PROGRAM");
      break;
    case 'U':
    case 'u':
      printf("Read Usercode\n");
      jbi_play(juf2, "READ_USERCODE");
      break;
    case 'V':
    case 'v':
      printf("Verify\n");
      jbi_play(juf2, "VERIFY");
      break;
    case 'E':
    case 'e':
      printf("Erase\n");
      jbi_play(juf2, "ERASE");
      break;
    case 'A':
    case 'a':
      printf("Default Action\n");
      jbi_play(juf2, juf2->action);
      break;
    case 'R':
    case 'r':
      printf("Reseting\n");
	  reset_usb_boot(1<<PIN_LED, 0);
      break;
    default:
      printf("Unknown command:  %s\n", command);
  }
#ifdef CYCLOMOD
  printf("\nCycloMod");
#endif
  printf("\nJBC Player Commands:\n");
  printf("  A/a Action:       perform default action\n");
  printf("  B/b Blank Check:  check if device is blank\n");
  printf("  C/c Configure:    configure device\n");
  printf("  D/d Details:      display JBC details\n");
  printf("  E/e Erase:        erase device\n");
  printf("  H/h Help:         print command help\n");
  printf("  I/i IDCODE:       get device IDCODE\n");
  printf("  P/p Program:      program device\n");
  printf("  R/r Reset:        reset to bootloader\n");
  printf("  U/u Usercode:     read USERCODE\n");
  printf("  V/v Verify:       verify device\n");
}

int main() {
    bi_decl(bi_program_description("This is a port of the Altera JBC Player."));

    stdio_init_all();
    jbi_init(TRUE);

#ifdef CYCLOMOD
    clock_gpio_init(PIN_CLKOUT, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_XOSC_CLKSRC, 1);
#endif

#ifdef WAIT_FOR_CONSOLE
  // wait for something on STDIN before scanning to make sure the terminal is open
    while (!tud_cdc_connected()) {
      sleep_ms(50);
    } 
#endif

    printf("\nJAM Byte Code Player\n");
    printf("JUF2 header address:  0x%08X\n", (uint32_t)juf2);
	  printf(" Tag: 0x%08X\n Ver: 0x%08X\n Len: 0x%08X\n Adr: 0x%08X\n", juf2->tag, juf2->version, juf2->length, juf2->location);

#ifdef RUN_DEFAULT_ACTION
    jbi_play(juf2, juf2->action);
#else
    jbi_play(juf2, NULL);
#endif

    printf("\nPins:  TCK %d, TMS, %d, TDI %d, TDO %d, LED %d\n", PIN_TCK, PIN_TMS, PIN_TDI, PIN_TDO, PIN_LED);

    char cmd[COMMAND_BUFFER_LENGTH +1];
    cmd[0] = 0;
    process_command(cmd);

    while (1) {
        gets(cmd);
        printf("cmd:  %s\n", cmd);
        process_command(cmd);
        sleep_ms(1);
    }

}
