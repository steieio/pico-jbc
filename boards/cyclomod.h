/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

// This header may be included by other board headers as "boards/cyclomod.h"

#ifndef _BOARDS_CYCLOMOD_H
#define _BOARDS_CYCLOMOD_H

pico_board_cmake_set(PICO_PLATFORM, rp2350)

// For board detection
#define CYCLOMOD

// --- RP2350 VARIANT ---
#define PICO_RP2350A 1

// --- JTAG ---
#define JTAG_TCK_PIN 6
#define JTAG_TMS_PIN 5
#define JTAG_TDI_PIN 7
#define JTAG_TDO_PIN 4

// --- Clocks ---
#define CLKIN0_PIN  20
#define CLKOUT0_PIN 21
#define CLKIN1_PIN  22
#define CLKOUT1_PIN 23
#define CLKOUT2_PIN 24
#define CLKOUT3_PIN 25

// --- HSTX GPIO ---
#define HSTX_GPIO12_PIN 12
#define HSTX_GPIO13_PIN 13
#define HSTX_GPIO14_PIN 14
#define HSTX_GPIO15_PIN 15
#define HSTX_GPIO16_PIN 16
#define HSTX_GPIO17_PIN 17
#define HSTX_GPIO18_PIN 18
#define HSTX_GPIO19_PIN 19

// --- UART ---
#define PICO_DEFAULT_UART 1
#define PICO_DEFAULT_UART_TX_PIN 26
#define PICO_DEFAULT_UART_RX_PIN 27

// --- LED ---
#define PICO_DEFAULT_LED_PIN 28

// --- VBAT ---
#define BATT_VIN_PIN 29

// --- I2C ---
#define PICO_DEFAULT_I2C 0
#define PICO_DEFAULT_I2C_SDA_PIN 8
#define PICO_DEFAULT_I2C_SCL_PIN 9

// --- FLASH ---

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

// Save 1 MB for FPGA JBC image at top 
pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (15 * 1024 * 1024))
#define PICO_FLASH_SIZE_BYTES (15 * 1024 * 1024)

pico_board_cmake_set_default(PICO_RP2350_A2_SUPPORTED, 1)
#ifndef PICO_RP2350_A2_SUPPORTED
#define PICO_RP2350_A2_SUPPORTED 1
#endif

#endif
#