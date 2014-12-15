On this example can be tested some interfaces like:

1) USB device MSC.
	Allow to bridge the USB0 device to MMCSD0, MMCSD1 or USB1 MSC host.
		The performance is:
			For bridge USB0 to MMCSD0(uSD) is  R about 4MB/s, W about 130 - 400 KB/s. 
			For bridge USB0 to MMCSD1(eMMC) is R about 10MB/s, W about 700 KB/s.

2) USB host MSC.

3) USB host mouse.

4) MMCSD0 for uSD.

5) MMCSD1 for eMMC.
	Is fully compatible with BBB 2GB and BBB 4GB, on maximum speed and 8 bit bus.

6) TWI (I2C) 0, 1 and 2.

7) ADXL345 on TWI2 if is present on BeagleBone Expansion.

8) PIO OUT onboard Leds 0, 1, 2 and 3.

9) PIO IN five buttons if LCD 3 cape is present, if detected BeagleBone Expansion board from CHIPSEE 
	four buttons is disponible the five button is in conflict with eMMC D3, 
	to use BeagleBone Expansion and eMMC is necessary to remove the C5 capacitor 
	from BeagleBone Expansion board, if not the eMMC wil be initiated in 8BIT data bus 
	but will not work , will read data with D3 = 0.

6) LCD and HDMI interface:

	BeagleBone Expansion 320x240 3.5 inch display with touchscreen 
		(autodetected by software) if LCD 3 cape is present.
	
	BeagleBone Expansion 800x480 7inch display with capacitive touchscreen 
		(autodetected by software throught ft5x06 circuitry).
	
	If last two displays has not been detected the software 
		initiate the LCD controler for one of two presetted HDMI settings:
			1) 720p 50hz.
			2) 1080 24hz.
			3) 720*480 60hz.
	
7) Test some GUY libraryes integrated in a sizeable window like:

	Button.
	
	CheckBox.
	
	ListBox.
	
	ProgressBar.
	
	ScrollBar.
	
	TextBox.
	
	PictureBox. 
	
	Virtual QWERTY keyboard.
	
	The screen is divided in three sections by three independent windows.
		1) A section ( "status bar" ) section with width = X screen size and height = 20.
		2) General screen section for application windows begin from X = 0, Y = 20 
			and end on X = screen width and Y = screen height - 100.
		3) Virtual QWERTY keyboard section an begin on X = 0, Y = screen height - 100 
			and end on X = screen width and Y = screen height.
