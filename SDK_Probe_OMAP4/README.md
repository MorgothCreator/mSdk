The mSDK for OMAP4 is at beginning because the TI has no support for standalone applications.

I succesfuly ported only the PIO's, UART's and TWI's drivers from Sitara AM335x starterware, to run a compiled programs I recomand to use a linux uBoot to setup the clock's and other necessary modules after the lauch the user program.

This demo application is made to initialize:
- All six GPIO controllers.
- Debug Uart 2.
- Led1 and LED 2 pins like outputs.
- TWI 0, 1, 2 and 3.
- Try to detect and initialize ADXL345 triple axis accelerometer on TWI3 if PandaBoard Expansion V3 is present.
- Try to detect and initialize the capacitive touchscreen on TWI1 if PandaBoard Expansion V3 is present.
- Initialize a little section of LCD controller (this section is not complete in SDK because the interrupt and dma drivers of OMAP4 is not provided by TI).