On this example can be tested some interfaces like:

1) USB device MSC.

2) USB host MSC.

3) USB host mouse.

4) MMCSD0 for uSD.

5) MMCSD1 for eMMC.

6) TWI (I2C) 0, 1 and 2.

7) ADXL345 on TWI2 if is present on BeagleBone Expansion.

8) PIO OUT onboard Leds 0, 1, 2 and 3.

9) PIO IN five buttons if LCD 3 cape is present.

6) LCD and HDMI interface:

	BeagleBone Expansion 320x240 3.5 inch display with touchscreen 
		(autodetected by software) if LCD 3 cape is present.
	
	BeagleBone Expansion 800x480 7inch display with capacitive touchscreen 
		(autodetected by software throught ft5x06 circuitry).
	
	If last two displays has not been detected the software 
		initiate the LCD controler for one of two presetted HDMI settings.
	
7) Test some GUY libraryes integrated in a sizeable window like:

	Button.
	
	CheckBox.
	
	ListBox.
	
	ProgressBar.
	
	ScrollBar.
	
	TextBox.
	
	PictureBox. 
