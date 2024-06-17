/* FILE:    ST7920.cpp
   Date:	12/06/24
   VERSION: 1.0.0
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1.0: Original version
12/06/24 version 1.0.0: Updated to work with ARM based devices

This library adds hardware support to the HCDisplay library for ST7920 based screens using the controllers serial interface.
Current supported boards:


12864B Parallel/Serial Graphic LCD Module (SKU: HCMODU0032). Available for purchase form our store here:

https://hobbycomponents.com/displays/285-12864b-parallel-serial-graphic-lcd-module


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
#include "ST7920.h"
#include "Arduino.h"
#include "SPI.h"

/* Constructor to initialises the display library*/
Display::Display(void)
{
}

/* Initialises the display and configures the serial and control pins where:
	
	DC is the digital pin for the Data/Command control pin.
	CS is the digital pin for the chip select control pin.
	RST is the digital pin for the reset control pin.
*/
void Display::DInit(uint8_t CS)
{
	/* Configure the control pins */
	pinMode(CS, OUTPUT);

	_CSP =  portOutputRegister(digitalPinToPort(CS));
	_CSM =  digitalPinToBitMask(CS);
	
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode (SPI_MODE3);
	
	_SPIMode = true;
	
	_Init();
}


void Display::DInit(uint8_t CS, uint8_t E, uint8_t RW)
{
	/* Configure the control pins */
	pinMode(CS, OUTPUT);
	pinMode(E, OUTPUT);
	pinMode(RW, OUTPUT);

	_CSP =  portOutputRegister(digitalPinToPort(CS));
	_CLKP =  portOutputRegister(digitalPinToPort(E));
	_DOUTP =  portOutputRegister(digitalPinToPort(RW));
	
	_CSM =  digitalPinToBitMask(CS);
	_CLKM =  digitalPinToBitMask(E);
	_DOUTM =  digitalPinToBitMask(RW);
	
	_SPIMode = false;
	
	_Init();
}



void Display::_Init(void)
{

	
//	_SID_LOW;	
//	_SCLK_LOW;
	_CS_DIS;

	
	/* Set the font to the standard system font */
	DSetFont(SystemFont);
	
	/* Put the cursor in the home position */
	DSetXY(0, 0);

	/* Turn on the display */
	DScreen(ON);

	_Write_Instruction(CLEAR_REG); //Clear the screen of any text
	
	delay(2); // > 1.2ms to clear the screen of text

	/* Enable graphics mode */
	_GraphicMode(ON);

	/* Clear the buffer and update the display */
	DClear();

	/* Set drawing mode to normal */
	DDrawMode(DRAWMODE_NORMAL);
	
	/* Set screen orientation to normal */
	DFlip(SCREEN_NORMAL);
	
}


/* Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
	for(uint8_t y = 0; y < _res_y; y++)	
	{
		if(y < 32)
		{
			_Write_Instruction(0b10000000 | y); 
			_Write_Instruction(0b10000000); 
		}
		else //GDRAM is twice the size of the display area and is split between two 32 row blocks.
		{
			_Write_Instruction(0b10000000 | y - 32); 
			_Write_Instruction(0b10001000); 
		}
		
		for(uint8_t x = 0; x < (_res_x / 8); x++)
		{
			_Write_Data(_Display_Buffer[y][x]);
			_Write_Data(_Display_Buffer[y][++x]);
		}	
	}
}





/* Toggles the reset pin to perform a hardware reset of the display. NOT IMPLEMENTED FOR THIS DISPLAY */
void Display::DReset(void)
{
}



/* Places the screen into or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{
	if(mode)
		_Write_Instruction(0b00000001);
	else
		DScreen(ON);
}



/* Turns the screen on or off where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	if(mode == ON)
		_Write_Instruction(DIS_STAT_REG | DISP_ON_MASK);
	else
		_Write_Instruction(DIS_STAT_REG);
}




/* Sets the state of the backlight. NOT APPLICABLE FOR THIS SCREEN */
void Display::DBacklight(boolean mode)
{
}




/* Sets the screen orientation and write direction where:
	mode is the direction, orientation to set the screen to. Valid vales for mode are:
		SCREEN_NORMAL 		(Default)
		SCREEN_R270 		
		SCREEN_R90 			
		SCREEN_R180 		
*/
void Display::DFlip(uint8_t mode)
{
	_FlipMode = mode;
}




/* Sets the foreground colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).

	RGB value that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
	if(((uint16_t)r + (uint16_t)g + (uint16_t)b) >= 384)
		_FGColour = 1;
	else
		_FGColour = 0;
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




/* Sets the background colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).

	RGB value that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
	if(((uint16_t)r + (uint16_t)g + (uint16_t)b) >= 384)
		_BGColour = 1;
	else
		_BGColour = 0;
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
	for(uint8_t y = 0; y < (_res_y); y++)	
		for(uint8_t x = 0; x < _res_x / 8; x++)
			_Display_Buffer[y][x] = 0x00;
		
	DUpdateDisplay();
}




/* Erases a specified area of the screen by setting it to the background colour where:
	x1 is the x coordinate of the top left corner of the area to be erased.
	y1 is the y coordinate of the top left corner of the area to be erased.
	x2 is the x coordinate of the bottom right corner of the area to be erased.
	y2 is the y coordinate of the bottom right corner of the area to be erased.
*/
void Display::DErase(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	uint16_t FGTemp = _FGColour;
	
	_FGColour = _BGColour;
	DRect(x1, y1, x2, y2);
	_FGColour = FGTemp;
}




/* Sets a single pixel to the current foreground colour where:
	x is the x axis coordinate of the pixel.
	y is the y axis coordinate of the pixel. 
*/
void Display::DPlot(int16_t x, int16_t y)
{
	switch(_FlipMode)
	{	
		//Modify the X & Y coordinates depending on the orientation of the screen
		case(SCREEN_NORMAL):
			break;
			
		case(SCREEN_R90):
			int16_t temp;
			temp = y;
			y = x;
			x = (_res_x - 1) - temp;
			break;
			
		case(SCREEN_R180):
			y = (_res_y - 1) - y;
			x = (_res_x - 1) - x;
			break;
			
		case(SCREEN_R270):
			temp = y;
			y = (_res_y - 1) - x;
			x = temp;
			break;
			
		default:
			break;
	}
	
	//Write the pixel to the display buffer depending on the draw mode.
	if(x >= 0 && x < _res_x && y >= 0 && y < _res_y)
	{
		switch(_DrawMode)
		{
			case(DRAWMODE_NORMAL):
				if(_FGColour)
					_Display_Buffer[y][x / 8] |= 0b10000000 >> (x & 0b111);  
				else
					_Display_Buffer[y][x / 8] &= ~(0b10000000 >> (x & 0b111));  
				break;
				
			case(DRAWMODE_XOR):
				if(_FGColour)
					_Display_Buffer[y][x / 8] ^= 0b10000000 >> (x & 0b111);  
				break;
				
			case(DRAWMODE_AND_INV):
				if(_FGColour)
					_Display_Buffer[y][x / 8] &= ~(0b10000000 >> (x & 0b111));  
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
	for(int16_t x = x1; x <= x2; x++)
		for(int16_t y = y1; y <= y2; y++)
			DPlot(x, y);
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
void Display::DSetXY(uint16_t X, uint16_t Y)
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

void Display::DBWBitmap(uint16_t Cols,  uint8_t ByteRows, const uint8_t BitmapData[], boolean Background)
{
	int16_t x = _X1Pos;
	
	for(uint16_t i = 0; i < (Cols * ByteRows); i += ByteRows)
	{
		for(uint16_t Scale = 0; Scale < _ScaleX; Scale++)
		{
			if(Background)
				_DWriteCol(x, _Y1Pos, ByteRows, &BitmapData[i]/*, false*/);
			else
				_DPlotCol(x, _Y1Pos, ByteRows, &BitmapData[i]/*, false*/);
			x++;
		}
	}
}





/* Sets the current font to use for the print function where:
   NewFont is the name of the font. By default there is only one font - SystemFont
   You can enable more fonts by editing the options.h file found in the HCDisplay folder.
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
	//const uint8_t *FontByteAddress; 
	
	/* Set a pointer to the starting address for the characters bitmap */
	//FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]); 
	
	/* Use the draw bitmap function to write the character bitmap to the screen */
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background); 
	
	/* If the character is being printed with a background fill in the gaps between each character */
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1); 
	
	/* Move the cursor to the end of the printed character to it is in the correct position should we with to print another character */
	DSetXY(_X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos); 
}





/*  NOT APPLICABLE FOR THIS DiSPLAY
	Reads the RGB colour value for a single pixel at the specified coordinate where:
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
	//Modify the X & Y coordinates depending on the orientation of the screen
	switch(_FlipMode)
	{	
		case(SCREEN_NORMAL):
			break;
			
		case(SCREEN_R90):
			int16_t temp;
			temp = y;
			y = x;
			x = (_res_x - 1) - temp;
			break;
			
		case(SCREEN_R180):
			y = (_res_y - 1) - y;
			x = (_res_x - 1) - x;
			break;
			
		case(SCREEN_R270):
			temp = y;
			y = (_res_y - 1) - x;
			x = temp;
			break;
			
		default:
			break;
	}
	

	if(_Display_Buffer[y][x / 8] & (0b10000000 >> (x & 0b111)))
		return true;
	else
		return false;
}




/* Returns the resolution of the displays X axis as a 16 bit unsigned number */
uint16_t Display::DResX(void)
{
	if(_FlipMode == SCREEN_R270 ||
	   _FlipMode == SCREEN_R90)
		return _res_y;
	else
		return _res_x;
}




/* Returns the resolution of the displays Y axis as a 16 bit unsigned number */
uint16_t Display::DResY(void)
{
	if(_FlipMode == SCREEN_R270 ||
	   _FlipMode == SCREEN_R90)
		return _res_x;
	else
		return _res_y;
}




/* Not implemented for this display
*/
void Display::DContrast(uint8_t level)
{
}



/* Internal function that switches between extended function (graphic mode) and basic function (text mode) where:
	
	Mode selects the required mode. Valid values for mode are:
		true = Graphic mode
		false = text mode 
*/
void Display::_GraphicMode(boolean Mode)
{
	if(Mode)
	{
		_Write_Instruction(FUNC_REG | RE_MASK); //Function set 8 bit interface, extended instruction
		_Write_Instruction(FUNC_REG | RE_MASK | GRAPHIC_ON_MASK); //Function set 8 bit interface, extended instruction, Graphic mode on
	}else
	{
		_Write_Instruction(FUNC_REG); //Basic function, graphic mode off
	}
}



/* Internal function that writes a one pixel column of bitmap data by plotting the individual pixels. This results in a bitmap with no background. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, const char *Data)
{

	ByteRows--;
	for(uint8_t dr = 0; dr <= ByteRows; dr++)
	{
		for(uint8_t i = 0; i < 8; i++)
		{
			for(uint8_t sy = 0; sy < _ScaleY; sy++)
			{
				if((pgm_read_byte_near(&Data[ByteRows - dr]) >> /*(7 - */ i/*)*/) & 1)
					DPlot(x, y);
			
				y++;
			}
		}
	}
}




/* Internal function that writes a one pixel column of bitmap data. This results in background being written. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, const char *Data)
{
	boolean _FGTemp = _FGColour, _BGTemp = _BGColour;
	ByteRows--;
	for(uint8_t dr = 0; dr <= ByteRows; dr++)
	{
		for(uint8_t i = 0; i < 8; i++)
		{
			for(uint8_t sy = 0; sy < _ScaleY; sy++)
			{
				if((pgm_read_byte_near(&Data[ByteRows - dr]) >> /*(7 - */ i/*)*/) & 1)
					_FGColour = _FGTemp;
				else
					_FGColour = _BGTemp;
			
				DPlot(x, y);
			
				y++;
			}
		}
	}
	
	_FGColour = _FGTemp;
	_BGColour = _BGTemp;
}



	
	
/* Internal function that writes an 8 bit value to one of the instruction registers */
void Display::_Write_Instruction(uint8_t Data)
{
	_CS_EN;
	_SendByte(WRITE_IR);
	_SendByte(Data & 0xF0);
	_SendByte(Data << 4);
	_CS_DIS;
	
	delayMicroseconds(80);
}




/* Internal function that writes an 8 bit value to graphics RAM (GDRAM) */
void Display::_Write_Data(uint8_t Data)
{
	_CS_EN;
	_SendByte(WRITE_DR);
	_SendByte(Data & 0xF0);
	_SendByte(Data << 4);
	_CS_DIS;
	
	delayMicroseconds(80);
}


/* Internal function that writes a serial 8 bit value to the display by bit-bashing the digital pin connected tot he displays RW (SID) pin */ 
void Display::_SendByte(uint8_t Data)
{
	if(_SPIMode)
		SPI.transfer(Data);
	else
	{
		for(uint8_t i = 0; i < 8; i++)
		{
			if(Data & (0b10000000 >> i))
				_DOUT_HIGH;
			else
				_DOUT_LOW;

			_CLK_LOW;
			_CLK_HIGH;
		}
	}
}







/* TOUCH SCREEN FUNCTIONS ARE NOT SUPPORTED FOR THIS DISPLAY */


/* Constructor to initialise the optional touch library */
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
void Touch::SetScreenRes(uint16_t Res_X, uint16_t Res_Y)
{
}