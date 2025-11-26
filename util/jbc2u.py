#!/usr/bin/env python3
import sys
import struct
import subprocess
import re
import os
import os.path
import argparse
import json
from time import sleep


UF2_MAGIC_START0 = 0x0A324655 # "UF2\n"
UF2_MAGIC_START1 = 0x9E5D5157 # Randomly selected
UF2_MAGIC_END    = 0x0AB16F30 # Ditto

PICO_RP2040      = 0xe48bff56 # Pico RP2040 Family Code
PICO_RP2350      = 0xe48bff57 # RP2350 Absolute Address Family Code

jbchdraddr = 0x10020000
jbcspeed = 1000000
familyid = 0x0
jbcaction = b""
jbcfilename = b""
jbcdescription = b""


def convert_to_uf2(jbc_content):
    global jbchdraddr, familyid, jbcaction, jbcdescription, jbcfilename, jbcspeed

    datapadding = b""
    while len(datapadding) < 512 - 256 - 32 - 4:
        datapadding += b"\x00\x00\x00\x00"

    jbc_blocks = ((len(jbc_content) + 255) // 256)
    numblocks = jbc_blocks + 1

    outp = []

#        familyid = PICO_RP2350
    blockno = 0

# Create JUF2 Header Block
    chunk = struct.pack(b"<IIIIII16s32s128s", 0x3246554A, 0, 
          len(jbc_content), (jbchdraddr + 256), jbcspeed, 0, jbcaction.encode('utf-8'), 
          jbcfilename.encode('utf-8'), jbcdescription.encode('utf-8')) 
    flags = 0x2000

    hdo = struct.pack(b"<IIIIIIII",
        UF2_MAGIC_START0, UF2_MAGIC_START1,
        flags, jbchdraddr, 256, blockno, numblocks, familyid)
    while len(chunk) < 256:
        chunk += b"\x00"
    block = hdo + chunk + datapadding + struct.pack(b"<I", UF2_MAGIC_END)
    assert len(block) == 512
    outp.append(block)

# Add JBC Blocks
    for jbcblock in range(0, jbc_blocks):
        blockno += 1
        ptr = 256 * jbcblock
        chunk = jbc_content[ptr:ptr + 256]
        hdo = struct.pack(b"<IIIIIIII",
            UF2_MAGIC_START0, UF2_MAGIC_START1,
            flags, (ptr + jbchdraddr + 256), 256, blockno, numblocks, familyid)
        while len(chunk) < 256:
            chunk += b"\x00"
        block = hdo + chunk + datapadding + struct.pack(b"<I", UF2_MAGIC_END)
        assert len(block) == 512
        outp.append(block)

    return b"".join(outp)

def write_file(name, buf):
    with open(name, "wb") as f:
        f.write(buf)
    print("Wrote %d bytes to %s" % (len(buf), name))


def error(msg):
    print(msg, file=sys.stderr)
    sys.exit(1)

def main():
    global jbchdraddr, familyid, jbcaction, jbcdescription, jbcfilename, jbcspeed
    parser = argparse.ArgumentParser(description='Convert to UF2 or flash directly.')
    parser.add_argument('-a', '--action', dest='action', type=str,
                        default="",
                        help='set default action to perform')
    parser.add_argument('-b', '--base', dest='base', type=str,
                        default="0x20000",
                        help='set address of JUF2 header (default: 0x20000)')
    parser.add_argument('-d', '--description', dest='desc', type=str,
                        default="",
                        help='optional description of the JUF2')
    parser.add_argument('-f', '--family', dest='family', type=str,
                        default="0x0",
                        help='specify familyID - number or name (default: 0x0)')
    parser.add_argument('-j', '--jbc', metavar="FILE", dest='jbc', type=str,
                        help='jbc input file')
    parser.add_argument('-o', '--output', metavar="FILE", dest='output', type=str,
                        help='write output to named file; defaults to "flash.uf2" or "flash.bin" where sensible')
    parser.add_argument('-s', '--speed', dest='speed', type=str,
                        help='set JTAG speed in Hz')
    parser.add_argument('-i', '--info', action='store_true',
                        help='display header information from UF2, do not convert')
    args = parser.parse_args()

    jbchdraddr = int(args.base, 0)

    if not args.jbc:
        error("Need input jbc file")
    with open(args.jbc, mode='rb') as fj:
        jbcbuf = fj.read()
    jf = args.jbc.rpartition("/")
    jbcfilename = jf[-1]

    if not args.output:
        error("Need output file")

    if args.speed:
        jbcspeed = int(args.speed, 0)

    if args.action:
        jbcaction = args.action

    if args.desc:
        jbcdescription = args.desc

    if args.family:
        familyid = int(args.family, 0)

    outbuf = convert_to_uf2(jbcbuf)
    write_file(args.output, outbuf)


if __name__ == "__main__":
    main()
