/* FILE:    MAX7219_Dot_Matrix.cpp
   DATE:    12/06/24
   VERSION: 1.0
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version
12/06/24 version 1.0: Updated to work with V1.2.2 library

This library adds hardware support to the HCDisplay library for MAX7219 based displays.
Current supported boards:

MAX7219 Serial LED dot matrix module - Red (SKU: HCOPTO0014). Available for purchase form our store here:
https://hobbycomponents.com/opto-electronics/190-max7219-serial-dot-matrix-display-module


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
#include "MAX7219_Dot_Matrix.h"
#include "Arduino.h"
#include <SPI.h>


/* Constructor to initiliase the display library*/
Display::Display(void)
{
}


/* Initialises the display and configures the SPI and control pins where:
	
	CS is the digital pin for the chip select control pin.
	
	DriversX is the number of MAX7219 drivers used in the X axis.
	
	DriversY is the number of MAX7219 drivers used in the y axis.
	
	Buffer is a one dimension array of type byte used to hold the display data. 
	The array should be dimensioned as follows:
		byte Buffer[DriversX * DriversY * 8];
*/
void Display::DInit(uint8_t CS, uint8_t DriversX, uint8_t DriversY, uint8_t *Buffer)
{
	/* Set DIO pins for interfacing to the driver as outputs */
	_CS = CS;
	pinMode(CS, OUTPUT); 
	digitalWrite(CS, HIGH);
	
	_Buffer = Buffer;
	_res_x = DriversX * 8;
	_res_y = DriversY * 8;
	_NumberOfDrivers = DriversX * DriversY;
	
	/* Enable the hardware SPI interface */
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);

	//Initiliase the registers for each driver.
	for (uint16_t DriverIndex = 0; DriverIndex < _NumberOfDrivers; DriverIndex++)
	{
		_DWrite(MAX7219DECODE, 0,DriverIndex);
		_DWrite(MAX7219INTESITY, 0x0F, DriverIndex);
		_DWrite(MAX7219SCANLIMIT, 7, DriverIndex);
		_DWrite(MAX7219TEST, TESTMODEOFF, DriverIndex);
		_DWrite(MAX7219SHUTDOWN, MAX7219ON, DriverIndex); 
	}
	
	DClear();
	
	DUpdateDisplay();
	
	
	/* Set the font to the standard system font */
	DSetFont(SystemFont);
	
	/* Put the cursor in the home position */
	DSetXY(0, 0);  
	
	/* Set drawing mode to normal */
	DDrawMode(DRAWMODE_NORMAL);
}



/* Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
	for(uint16_t Driver = 0; Driver < _NumberOfDrivers; Driver++)
		for(uint8_t Col = 0; Col < 8; Col++)
			_DWrite(MAX7219DIGIT8 - Col, _Buffer[Col + (Driver * 8)], _NumberOfDrivers - 1 - Driver);
}



/* NOT APPLICABLE FOR THIS DISPLAY */
void Display::DReset(void)
{
}



/* Places the screen in to or out of sleep mode where:
	mode is the required state. Valid values are
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{
	for(uint16_t Driver = 0; Driver < _NumberOfDrivers; Driver++)
		_DWrite(MAX7219SHUTDOWN, !mode, Driver);		
}



/* Turn the screen on or off  where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	DSleep(!mode);
}



/* Sets the brightness of the LEDs where:

	Mode is the required intensity.Valid values for mode are:
		0 (min) to 0x0F (max)
*/
void Display::DBacklight(uint8_t mode)
{
	if(mode > 0x0F)
		mode = 0x0F;
	
	for(uint16_t Driver = 0; Driver < _NumberOfDrivers; Driver++)
		_DWrite(MAX7219INTESITY, mode, Driver);

}



/* Sets the screen orientation and write direction NOT APPLICABLE FOR THIS DISPLAY */
void Display::DFlip(uint8_t mode)
{
}



/* Sets the foreground colour to be used by the graphic and print functions NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
}



/* Sets the foreground colour to be used by the graphic and print functions where:
	
	colour is a boolean value representing the colour. Valid values for colour are
			0 = the screens background colour;
			1 = the screens foreground colour;			
*/
void Display::DSetFG(boolean colour)
{
	_FGColour = colour;
}



/* Sets the background colour to be used by the graphic and print functions NOT APPLICABLE FOR THIS DISPLAY */
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
}



/* Sets the background colour to be used by the graphic and print functions where:
	
	colour is a boolean value representing the colour. Valid values for colour are
			0 = the screens background colour;
			1 = the screens foreground colour;
			
*/
void Display::DSetBG(boolean colour)
{
	_BGColour = colour;
}



/* Clears the entire contents of the screen */
void Display::DClear(void)
{
	for(uint16_t i = 0; i < (_res_x * (_res_y  / 8)); i++)	
	{
		if(_BGColour)
			_Buffer[i] = 0xFF;
		else
			_Buffer[i] = 0;
	}
}



/* Erases the entire screen by drawing a rectangle using the current background colour */
void Display::DErase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	boolean Temp = _FGColour;
	_FGColour = _BGColour;
	DRect(x1, y1, x2, y2);
	_FGColour = Temp;
}



/* Sets a single pixel to the current foreground colour where:
	x is the x axis coordinate of the pixel.
	y is the y axis coordinate of the pixel. 
*/
void Display::DPlot(uint16_t x, uint16_t y)
{
	if(x >= 0 && x < _res_x && y >= 0 && y < _res_y)
	{
		switch(_DrawMode)
		{
			case(DRAWMODE_NORMAL):
				if(_FGColour)
					_Buffer[x + ((y/8) *_res_x)] |=  1 << (7 - (y % 8));
				else
					_Buffer[x + ((y/8) *_res_x)] &=  ~(1 << (7 - (y % 8)));
				break;
			
			case(DRAWMODE_XOR):
				if(_FGColour)
					_Buffer[x + ((y/8) *_res_x)] ^=  1 << (7 - (y % 8));
				else
					_Buffer[x + ((y/8) *_res_x)] &=  ~(1 << (7 - (y % 8)));
				break;
				
			case(DRAWMODE_AND_INV):
				_Buffer[x + ((y/8) *_res_x)] &=  ~(1 << (7 - (y % 8)));
				break;
		}
			
	}
}




/* Draws a solid rectangle to the using the current foreground colour where:
	x1 is the x coordinate of the top left corner of the rectangle.
	y1 is the y coordinate of the top left corner of the rectangle.
	x2 is the x coordinate of the bottom right corner of the rectangle.
	y2 is the y coordinate of the bottom right corner of the rectangle.
*/
void Display::DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	if(x1 < (int16_t)_res_x && x2 >= 0 && y1 < (int16_t)_res_y && y2 >= 0) 
	{
		for(int16_t x = x1; x <= x2; x++)
			for(int16_t y = y1; y <= y2; y++)
				DPlot(x,y);
	}
}



/* Stores the drawing mode used by the text and graphical functions where:

	Mode is the required drawing mode. Valid values for Mode are:
		DRAWMODE_NORMAL		Pixels are drawn normally
		
		DRAWMODE_XOR		Pixels are logically XOR'd with the pixels currently draw on the screen.
		
		DRAWMODE_AND_INV 	Pixels are logically AND'd and inverted with the pixels currently draw on the screen.
*/
		
		
void Display::DDrawMode(uint8_t Mode)
{
	_DrawMode = Mode;
}



/* Set an X/Y coordinate for the bitmap and print functions where:
	x is the x axis coordinate.
	y is the y axis coordinate.
*/
void Display::DSetXY(int16_t X, int16_t Y)
{
	_X1Pos = X;
	_Y1Pos = Y;
}



/* Sets the amount of scaling for the bitmap and print functions where:
	ScaleX is the magnitude of scaling in the x axis.
	ScaleY is the magnitude of scaling in the y axis.
	
	A value of 1 for ScaleX or ScaleY will result in no scaling.
	A value of 2 for ScaleX or ScaleY will result the bitmap or font being scaled by a factor of 2 in the appropriate axis.
	A value of n for ScaleX or ScaleY will result the bitmap or font being scaled by a factor of n in the appropriate axis.
*/	
void Display::DScaleXY(uint8_t ScaleX, uint8_t ScaleY)
{
	if(ScaleX)
		_ScaleX = ScaleX;
	if(ScaleY)
		_ScaleY = ScaleY;
}



/* Write BW bitmap data to the LCD starting using the current foreground colour at the cursor location where:
   Cols is the number byte columns to write to.
   ByteRows is the number of rows to write to in 8 pixel chunks 
   BitmapData is an array containing the bitmap data to be written 
   Background selects if the character will be printer with a solid background. Valid values for Background are
			true 	(character is printed with background with a colour set by the DSetBG() function
			false 	(Character is printed without a background)*/
void Display::DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t *BitmapData, boolean Background)
{
		int16_t x = _X1Pos;
		for(uint16_t i = 0; i < (Cols * ByteRows); i += ByteRows)
		{
			uint8_t Data = pgm_read_byte_near(&BitmapData[i]);
			for(uint8_t Scale = 0; Scale < _ScaleX; Scale++)
			{
				if(Background)
					_DWriteCol(x, _Y1Pos, ByteRows, Data /*&BitmapData[i]*/);
				else
					_DPlotCol(x, _Y1Pos, ByteRows, Data /*&BitmapData[i]*/);
				x++;
			}
		}
}




/* Sets the current font to use for the print function where:
   NewFont is the name of the font. By default there is only one font - SystemFont
   You can enable more fonts by editing the options.h file found in the HCLibrary folder.
*/
void Display::DSetFont(const FONT_INFO *NewFont)
{
	Font = NewFont;
}




/* Write a single character to the screen at the current cursor coordinate where:
	character is the ASCII character to print
	
	Background selects if the character will be printer with a solid background. Valid values for Background are
		true 	(character is printed with background with a colour set by the DSetBG() function
		false 	(Character is printed without a background)
*/
void Display::DWriteChar(char Character, boolean Background)
{
	uint8_t DescriptorIndex = Character - Font->start_char;												// Find the position of the character in the fonts descriptor array.
	uint16_t BitmapIndex = pgm_read_word_near(&Font->p_character_descriptor[DescriptorIndex].offset); 	// Get the fonts bitmap array index for the character bitmap.
	uint8_t BitmapWidth = pgm_read_word_near(&Font->p_character_descriptor[DescriptorIndex].width);		// Get the width of the bitmap.
	uint8_t Bitmapbyteheight = Font->height;															// Get the height of the bitmap in bytes.
	const uint8_t *FontByteAddress; 
	
	/* Set a pointer to the starting address for the characters bitmap */
	FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]); 
	
	/* Use the draw bitmap function to write the character bitmap to the screen */
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background); 
	
	/* If the character is being printed with a background fill in the gaps between each character */
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1); 
	
	/* Move the cursor to the end of the printed character to it is in the correct position should we with to print another character */
	DSetXY(_X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos); 
}




/* Reads the RGB colour value for a single pixel at the specified coordinate where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	Data is a 3 byte array used to store the RGB values 
*/
void Display::DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
}



/* Checks if the pixel at the specified coordinate is set to the foreground colour where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	
	Returns a boolean true or false:
		true = pixel is set to current foreground colour.
		false = pixel not is set to current foreground colour.
		
	Use this function for black and white screens.
*/
boolean Display::DReadPixel(uint16_t x, uint16_t y)
{
	return _Buffer[x + ((y/8) *_res_x)] &  (1 << (7 - (y % 8)));
}



/* Returns the resolution of the displays X axis as a 16 bit unsigned number */
uint16_t Display::DResX(void)
{
	return _res_x;
}



/* Returns the resolution of the displays Y axis as a 16 bit unsigned number */
uint16_t Display::DResY(void)
{
	return _res_y;
}



/* Not implemented for this display
*/
void Display::DContrast(uint8_t level)
{
}



/* Write to one of the drivers registers. No-ops are sent to all other 
   drivers in the chain.
   Driver is the driver number in the chain */
void Display::_DWrite(byte Address, byte Data, byte Driver)
{
	digitalWrite(_CS, LOW);

	for (uint8_t DriverIndex = 0; DriverIndex < _NumberOfDrivers; DriverIndex++)
	{
		if(DriverIndex == Driver)
		{
			SPI.transfer(Address); 
			SPI.transfer(Data);
		}else
		{
			SPI.transfer(MAX7219NOOP); 
			SPI.transfer(0); 
		} 	
	}
	
	digitalWrite(_CS, HIGH);
	digitalWrite(_CS, LOW);
}




/* Internal function that writes a one pixel column of bitmap data by using the screens burst write function. This is slightly faster but also results in background being written. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, /*const*/ uint8_t Data /*Data[]*/)
{	
	if(x >= 0 && x < _res_x)
	{
		boolean Temp = _FGColour;
		
		for(uint8_t i = ByteRows; i > 0; i--)
		{
			for(uint8_t bit = 0; bit < 8; bit++)
			{				
				if(y < (int16_t)_res_y)
				{		
					int16_t y2 = y + _ScaleY - 1;
					if(y2 >= (int16_t)_res_y)
						y2 = _res_y - 1;		
				
					if((/*pgm_read_byte_near(&Data[i - 1])*/ Data >> bit) & 0x01)
						_FGColour = Temp;
					else
						_FGColour = _BGColour;
					
					
					DRect(x, y, x, y2);
					
					y = y2 + 1;
				}				
			}
		}
		
		_FGColour = Temp;
	}
	
	
}


/* Internal function that writes a one pixel column of bitmap data by plotting the individual pixels. This results in a bitmap with no background. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, uint8_t Data/*const uint8_t Data[]*/)
{
	if(x >= 0 && x < _res_x)
	{
		for(uint8_t i = ByteRows; i > 0; i--)
		{
			for(uint8_t bit = 0; bit < 8; bit++)
			{				
				if(y < (int16_t)_res_y)
				{		
					int16_t y2 = y + _ScaleY - 1;
					if(y2 >= (int16_t)_res_y)
						y2 = _res_y - 1;		
				
					if((/*pgm_read_byte_near(&Data[i - 1])*/ Data >> bit) & 0x01)
						DRect(x, y, x, y2);
					
					y = y2 + 1;
				}				
			}
		}
	}
}





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