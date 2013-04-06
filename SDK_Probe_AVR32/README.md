This example is made to initiate all necessary modules for AVR32AP7000 microcontroller.

After initializing of the necessary modules, will setup all three GPIO pins for onboard LED's and will setup the MMC/SD controller, will open a FAT instance if a MMC/SD card is present.

If MMC/SD card is detected is made to play a MPAG1/2 file with extension ".mpg", I recommand to put videos with a maximum resolution of 320x240 because the controller is not enought faster to decode files with a larrger resolution.

The all three leds will blink when the video is playng, the video will be mute because no audio decoder is present.