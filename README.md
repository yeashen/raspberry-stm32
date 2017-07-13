# raspberry-stm32

## Introduce
Developing stm32 project, coding on raspberry Pi, and download binary or hex file to stm32 by stm32flash tools. raspberry Pi is responsisble for handling high-level task, for example Deep Learning, Image Recongnition and so on, and stm32 handle and communicate with peripherals, if raspberry Pi wants to get some peripherals's informations and control sensors, it can transfer data by uart0 of download, so it is conveniently for development.

## Hardware
* raspberry Pi3 and 5V USB power wire.
* stm32 extension board(made by myself)

raspberry Pi | stm32
------------ | -----
TX  | RX
RX  | TX
GPIO23 | PA1.1(reset)
GPIO24 | PA1.2(boot0)
GND | GND

>*Note: if you have no stm32 extension board, you can manually connect uart tx, rx, reset, boot, gnd between raspberry and stm32*

## Software
* raspberry system(make sure wifi and ssh connect)
* stm32flash
* build tools - arm-none-eabi-gcc
* vim

## Install
* stm32flash install
1. [download source files](https://sourceforge.net/projects/stm32flash/files/?source=navbar)
2. extract and enter the directory, make and make install, installed at /usr/local/bin default, and you have to add /usr/local/bin to your system PATH
3. input stm32flash to check if the stm32flash installation is successful.

`$ stm32flash

stm32flash 0.5

http://stm32flash.sourceforge.net/

ERROR: Device not specified

Usage: stm32flash [-bvngfhc] [-[rw] filename] [tty_device | i2c_device]

...`

* raspberry Pi's uart configure

the uart0 is used to communicate with Blueteeth modlule, so before use uart0 download stm32 program binary you must configure again at the first time.

1. run raspi-config, disable terminal uart debug, edit /boot/cmdline.txt to make sure "consle=" is not serial0.
2. modify /boot/config.txt, add follow:
`dtoverlay=pi3-miniuart-bt

enable_uart=1`
3. reboot, /dev/serial0 is the writed and read file, it is not same as the old raspberry Pi, not /dev/ttyAMA0

