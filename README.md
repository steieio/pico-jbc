# pico-jbc
Altera JBC implementation for Raspberry Pi Pico boards

This is currently working with Pico and Pico2
Merged .uf2 files are presently only working with Pico
You currently need to generate a stand alone .uf2 file for Pico 2

## Pico 

* Build the project to create pico_jbc.uf2
* Add jbc file to uf2 file
* drag-n-drop file onto RP2040 drive

```
python2 util/jbcuf2.py -u build/pico_jbc.uf2 -b 0x10020000 -j path-to-image.jbc -d "description" -a "PROGRAM" -o output.uf2
```

## Pico 2

* Build the project to create pico_jbc.uf2
* Generate separate jbc uf2 file
* drag-n-drop both files onto RP2350 drive

```
python2 util/jbc2u.py -f 0xe48bff57 -b 0x10020000 -j path-to-image.jbc -d "description" -a "PROGRAM" -o output.uf2
```

## CycloMod

* Build the project to create pico_jbc.uf2
* Generate separate jbc uf2 file
* drag-n-drop both files onto RP2350 drive

```
python2 util/jbc2u.py -f 0xe48bff57 -b 0x10F00000 -j path-to-image.jbc -d "description" -a "CONFIGURE" -o output.uf2
```

## JBC to UF2

### -f Family

jbcuf2.py will automatically use the family from the provided RP2040 .uf2 file.
Jbc2u.py requires manual setting:
 * RP2040:  0xe48bff56
 * RP2350:  0xe48bff57

### -b Base Address

This must match the `JUF2_ADDRESS` defined in jbipico.h
For using Pico and Pico2 boards as a programmer, use 0x1002000 to provide as much space as possible for FPGA JBC images.
For CycloMod, the FPGA image should fit in the top 1MB, so use 0x10F00000 to leave the rest for RP2350 and other data.

### -a Default Action

The default action allows you to specify an action to run immediately at startup.  Use "CONFIGURE" for SRAM based devices like Cyclone and "PROGRAM" for flash based devices like MAX10.

### -d Description

This is any text you want to include to identify the image

## Generating an uncompressed JBC File

JBC is the STAPL JAM Byte Code format.  Quartus can generate these files.  In order to fit within the memory constraints of a microcontroller, compression is not supported.  Instructions for generating an uncompressed JBC file can be [found here](https://www.intel.com/content/www/us/en/support/programmable/articles/000079036.html).  

To build get an uncompressed .jbc file:

 1.  Go to Device \> Device and Pin Options \> Programming Files and select the "JEDEC STAPL Format File (.jam)" checkbox, or add ```set_global_assignment -name GENERATE_JAM_FILE ON``` to your Quartus project settings
 2.  Run ```quartus_jbcc.exe -n <filename>.jam <filename>.jbc```
