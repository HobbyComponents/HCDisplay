/* FILE:    HCMODU0136_HT1621.cpp
   DATE:    19/10/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version

This library adds hardware support to the HCDisplay library for MAX7219 based displays.
Current supported boards:

6 Digit 7 Segment LCD Module - Green Backlight (HCMODU0136). Available for purchase form our store here:
[LINK TBA]


This library is provided free to support the open source community. PLEASE SUPPORT HOBBY COMPONENTS 
so that we can continue to provide free content like this by purchasing items from our store - 
HOBBYCOMPONENTS.COM 


You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.
THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


#include "HCDisplay.h"
#include "HCMODU0136_HT1621.h"
#include "Arduino.h"


/* Constructor to initiliase the display library*/
Display::Display(void)
{
}


/* Initialises the display and configures the SPI and control pins where:
	
	Pin1 is the Arduino digital pin for the displays Chip Select (CS).
	
	Pin2 is the Arduino digital pin for the displays Write Select (WR).
	
	Pin3 is the Arduino digital pin for the displays Data pin (Data).
*/
void Display::DInit(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3)
{
	/* Set DIO pins for interfacing to the driver as outputs */
	_CS = _Pin1;
	_WR = _Pin2;
	_DATA = _Pin3;
	
	pinMode(_CS, OUTPUT);
	pinMode(_WR, OUTPUT);
	pinMode(_DATA, OUTPUT);

	_CSP =  portOutputRegister(digitalPinToPort(_CS));
	_WRP =  portOutputRegister(digitalPinToPort(_WR));
	_DAP =  portOutputRegister(digitalPinToPort(_DATA));
	
	_CSM =  digitalPinToBitMask(_CS);
	_WRM =  digitalPinToBitMask(_WR);
	_DAM =  digitalPinToBitMask(_DATA);
	
	_CS_DIS;
	_WR_HIGH;
	_DATA_HIGH;
	
	
	/* Initiliase the displays registers */
	_DWriteCommand(SYSEN);
	_DWriteCommand(ROSC);
	_DWriteCommand(COMMODE);
	_DWriteCommand(LCDON);
			
	DClear();
	
	DUpdateDisplay();
	
	/* Put the cursor in the home position */
	DSetXY(0);  
	
	/* Set drawing mode to normal */
	DDrawMode(DRAWMODE_NORMAL);
}



/* Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
	_DWriteBuffer();
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DReset(void)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSleep(boolean mode)
{
/*	for(uint16_t Driver = 0; Driver < _NumberOfDrivers; Driver++)
		_DWrite(MAX7219SHUTDOWN, !mode, Driver);*/		
}



/* Turn the screen on or off  where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	if(mode == ON)
		_DWriteCommand(LCDON);
	else
		_DWriteCommand(LCDOFF);
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DBacklight(uint8_t mode)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DFlip(uint8_t mode)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetFG(boolean colour)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetBG(boolean colour)
{
}



/* Clears the entire contents of the screen */
void Display::DClear(void)
{
	for(uint8_t i = 0; i < 6; i++)
		_Buffer[i] = 0;
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DErase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DPlot(uint16_t x, uint16_t y)
{
}




/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
}



/* Stores the drawing mode used by the text function where:

	Mode is the required drawing mode. Valid values for Mode are:
		DRAWMODE_NORMAL		Pixels are drawn normally
		
		DRAWMODE_OR			Characters are logically OR'd with the characters in the display buffer. This mode
							can be used to set the displays additional battery by writing '.' to the 1st, 2nd, & 3rd
							character position of the display without overwriting the existing character's already 
							stored within these positions.
*/
		
void Display::DDrawMode(uint8_t Mode)
{
	_DrawMode = Mode;
}



/* Set an X/Y coordinate for the bitmap and print functions where:
	x is the x axis coordinate.
	y is the y axis coordinate.
*/
void Display::DSetXY(int16_t X, int16_t Y = 0)
{
	_XPos = X;
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DScaleXY(uint8_t ScaleX, uint8_t ScaleY)
{
}



/* Not implemented for this display
*/
void Display::DContrast(uint8_t level)
{
}


/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t *BitmapData, boolean Background)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetFont(const FONT_INFO *NewFont)
{
}




/* Write a single character to the screen at the current cursor coordinate where:
	character is the ASCII character to print
	
	Background is applicable for this display
*/
void Display::DWriteChar(char Character, boolean Background = true)
{

	
	if(Character >= ' ' && Character <= 'z' && _XPos >=0 && _XPos < 6)	//Check that character is in font range and is in the screens display area.
	{
		if(_DrawMode == DRAWMODE_OR) //DRAWMODE_OR used for accessing the displays battery icons.
			_Buffer[_XPos] |= pgm_read_byte_near(&SystemFontBitmaps[Character - ' ']);
		else	//Else just print the character to the display buffer as normal.
		{
			_Buffer[_XPos] = pgm_read_byte_near(&SystemFontBitmaps[Character - ' ']);
	
			if(_DPLast)	//If the last character was a DP then enable the DP for the next character.
				_Buffer[_XPos] |= 0x80;
	
			if(Character == '.')	//If the current character is a DP set the DP flag and shift the character position as the DP doesn't use up any extra space.
			{
				_DPLast = true;
				_XPos--;
			}else
			{
				_DPLast = false;
			}
		}
	}

	_XPos++;
}




/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
boolean Display::DReadPixel(uint16_t x, uint16_t y)
{
}



/* NOT APPLICABLE FOR THIS DISPLAY */
uint16_t Display::DResX(void)
{
	return 6;
}



/* NOT APPLICABLE FOR THIS DISPLAY */
uint16_t Display::DResY(void)
{
	return 1;
}



void Display::_DWriteCommand(uint8_t Add)
{
	_CS_EN;
	
	//Write command ID (0b100)
	_DATA_HIGH;
	_WR_PULSE;
	_DATA_LOW;
	_WR_PULSE;
	_WR_PULSE;
	
	//Send 9th bit of Command code (always 0)
	_WR_PULSE;
	
	//Send the command
	for(uint8_t s = 0; s < 8; s++)
	{
		if(Add & 0x80)
			_DATA_HIGH;
		else
			_DATA_LOW;
		_WR_PULSE;
		
		Add <<= 1;
	}
	
	_CS_DIS;
}



void Display::_DWriteBuffer(void/*uint8_t Add, uint8_t Data*/)
{
	_CS_EN;
	
	//Write data write ID (0b101)
	_DATA_HIGH;
	_WR_PULSE;
	_DATA_LOW;
	_WR_PULSE;
	_DATA_HIGH;
	_WR_PULSE;
	
	
	//Send the address 0b000000
	_DATA_LOW;
	for(uint8_t s = 0; s < 6; s++)
	{
		_WR_PULSE;
	}
	
	
	//Write the display buffer to the screen
	for(uint8_t i = 0; i < 6; i++)
	{
		for(uint8_t s = 0; s < 8; s++)
		{
			if(_Buffer[5 - i] & (0x80 >> s))
				_DATA_HIGH;
			else
				_DATA_LOW;
			_WR_PULSE;
		}
	}
	
	_CS_DIS;
}




/* TOUCH SCREEN FUNCTIONS ARE NOT APPLICABLE FOR THIS DISPLAY */

/* Constructor to initiliase the optional touch library for the TSC2046 touch controller*/
Touch::Touch(void)
{
}



/* Initialises the touch screen controller and configures the control pins where:
	
	DIN is the digital pin used to connect to the controllers data in pin.
	DOUT is the digital pin used to connect to the controllers data out pin.
	CLK is the digital pin used to connect to the controllers clock pin.
	CS is the digital pin used to connect to the controllers chip select pin.
	
	Cal_X_Min is the raw minimum value from the touch screens x axis.
	Cal_X_Max is the raw maximum value from the touch screens x axis.
	Cal_Y_Min is the raw minimum value from the touch screens y axis.
	Cal_Y_Max is the raw maximum value from the touch screens y axis.
	
	Mapping is used to tell the library how the x & y axis of the touch senor has been mapped to the controller. Valid values for Mapping are:
		TOUCH_0_NORMAL 		Normal position
		TOUCH_90_NORMAL 	Sensor is rotated 90o
		TOUCH_180_NORMAL 	Sensor is rotated 180o
		TOUCH_270_NORMAL 	Sensor is rotated 270o
		TOUCH_0_FLIP_X 		X axis is in reverse direction
		TOUCH_90_FLIP_X 	X axis is in reverse direction - sensor is rotated 90o
		TOUCH_180_FLIP_X 	X axis is in reverse direction - sensor is rotated 180o
		TOUCH_270_FLIP_X 	X axis is in reverse direction - sensor is rotated 270o

		TOUCH_0_FLIP_Y 		Y axis is in reverse direction (same as TOUCH_180_FLIP_X)
		TOUCH_90_FLIP_Y 	Y axis is in reverse direction - sensor is rotated 90o (same as TOUCH_270_FLIP_X)
		TOUCH_180_FLIP_Y 	Y axis is in reverse direction - sensor is rotated 180o (same as TOUCH_0_FLIP_X)
		TOUCH_270_FLIP_Y 	Y axis is in reverse direction - sensor is rotated 270o (same as TOUCH_90_FLIP_X)
		TOUCH_0_FLIP_XY 	X & Y axis is in reverse direction (same as TOUCH_180_NORMAL)
		TOUCH_90_FLIP_XY 	X & Y axis is in reverse direction - sensor is rotated 90o (same as TOUCH_270_NORMAL)
		TOUCH_180_FLIP_XY 	X & Y axis is in reverse direction - sensor is rotated 180o (same as TOUCH_0_NORMAL)
		TOUCH_270_FLIP_XY 	X & Y axis is in reverse direction - sensor is rotated 270o (same as TOUCH_90_NORMAL)	
		
	Note that first 8 options for Mapping cover all possible orientations of the sensor. If you are not sure how your sensor is mapped
	it is recommended to try each of the first 8 options in succession until the correct mapping is found. The sensor will be mapped correctly 
	when the raw X axis coordinate reported from the sensor increases from left to right and the raw Y axis coordinate increases from top to bottom.
*/
void Touch::TInit(uint8_t DIN, uint8_t DOUT, uint8_t CLK, uint8_t CS, uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping)
{
}



/* Perform an X & Y axis read of the touch sensor. The results can then be obtained using the TReadAxis() function */
void Touch::TReadTouch(void)
{
}




/* Checks to see if the screen is currently pressed by checking if the current raw XY coordinates are within the calibrated area.

	Returns a boolean value where:
	
	true = screen is currently pressed.
	false = screen is not currently pressed.
*/
boolean Touch::TPressed(void)
{
	return false;
}




/* Returns the last X or Y axis reading from the touch sensor as a value mapped to the resolution of the screen where:

	Axis is a boolean value specifying which axis reading to return. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
*/
uint16_t Touch::TGetCoord(boolean Axis)
{
	return 0;
}



/* Triggers a sensor measurement of one of the axis and stores the result where:

	Axis is a boolean value specifying which axis to measure. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
		
	Returns an unsigned int value containing a raw sensor reading for the specified axis.
*/
uint16_t Touch::TReadRaw(boolean Axis)
{
	return 0;
}



/* Internal library function that writes to the touch controllers configuration register */
//void Touch::_OutputCommand(uint8_t Command)
//{
//}



/* Internal library function used by the HCDisplay class to pass the screens X & Y resolution to the touch class.
*/
//void Touch::SetScreenRes(uint16_t Res_X, uint16_t Res_Y)
//{
//}