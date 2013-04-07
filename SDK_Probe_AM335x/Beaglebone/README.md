This example will create instances for:

1) Uart 0 (default debug com port).

2) Led 1, Led 2, Led 3 and led 4.

3) TWI 0 to communicate with PMIC and onboard EEPROM memory.

4) TWI 1 to detect and communicare with capacitive touch screen if BeagleBone Expansion V2 is pressent.

5) TWI 2 to detect and communicate with ADXL345 acceleration sensor if BeagleBone Expansion V2 is pressent.

6) Will detect what expansion boards is attached to the motherboard, if is BeagleBone LCD3 Cape will enable BacLight to 80%, and will initiate calibration sequence.

7) MMC/SD module and attach LED1to wach activity of memory card.

8) USB0 MSC Host.

If LCD3 Cape is detected will init attached pins to the LCD3 Cape buttons and the two pins attached to the two leds from LCD3 Cape.

If is not detected LCD3 Cape but is detected Capacitive touchscreen controller is supposed to be attached the BeagleBone Expansion V2 with 800x480 lcd display and capacitive touch screen.

If is not detected one of LCD3 cape or Capacitive touchscreen controller the LCD controller wil not be initiated.

If LCD controller is initiated in idle loop will display a window like Windows2000 with one button, one check box, a progress bar, a list box, a scrollbar, and a text box.

The window can be in full screen mode or little window that can be moved on screen.
