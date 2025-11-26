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

