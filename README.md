AVR Fun
=======

This is just me having some fun with my arduino and C++.

Dependencies
============
* cmake
* avr-gcc (you probably need version 4.9)

Setting Up
==========

Within the CMakeList.txt "TTYUSB" is used for setting up the USB port to upload 
the program. If you want to upload make sure that that variable is set 
correctly.

After setting that variable you should be able to run these commands:

```
  git submodule update --init
  mkdir build
  cd build
  cmake ..
  make
  make upload
```

The submodule is needed for C++ headers.

Outputs
=======
There are serveral outputs of the make command.

* LEDBlink (The elf file built by gcc)
* kernel.hex (What gets uploaded)
* kernel.asm (The disassembled elf file)
