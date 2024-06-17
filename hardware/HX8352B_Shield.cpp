/* FILE:    HX8352B_Shield.cpp
   DATE:    07/06/24
   VERSION: 0.3
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version
08/01/21 version 0.2: Change lower case 'c' character to display as a lower case character.
07/06/24 version 0.3: Updated to work with ARM based devices

This library adds hardware support to the HCDisplay library for HX8352B based screens using the controllers SPI interface.
Current supported boards:

Open-Smart 3.2 Inch TFT shield with touch screen (HCARDU0109)

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
#include "HX8352B_SHIELD.h"
#include "Arduino.h"
#include <SPI.h>


/* Constructor to initialise the display library*/
Display::Display(void)
{
}


/* Initialises the display and configures the SPI and control pins where:
*/
void Display::DInit(void)
{
	
	/* Configure the data and control pins */
	_DATA_DIR_OUTPUT;
  
	RD_DDR |= RD_MASK;
	WR_DDR |= WR_MASK;
	RS_DDR |= RS_MASK;
	CS_DDR |= CS_MASK;
	
	_RD_HIGH;
	_WR_HIGH;
	_RS_DATA;
	_CS_DIS;
	
	//Wait 50ms after screen reset
	delay(50);
	
	//Write initial setting to the screen
	_Init_Screen();
	
	// Set screen orientation to normal position 
	DFlip(SCREEN_NORMAL);
	
	//Clear the screen
	DClear();
	
	// Set the font to the standard system font 
	DSetFont(SystemFont);
	
	// Put the cursor in the home position 
	DSetXY(0, 0);
}


// Writes the initial settings to the screen. 
void Display::_Init_Screen(void)
{
	_CS_EN;
    
	_WriteRegister(0xe5,0x10);   
	_WriteRegister(0xe7,0x10);
	_WriteRegister(0xe8,0x48);
	_WriteRegister(0xec,0x09);
	_WriteRegister(0xed,0x6c);
	_WriteRegister(0x23,0x6F);
	_WriteRegister(0x24,0x57);
	_WriteRegister(0x25,0x71);
	_WriteRegister(0xE2,0x18);
	_WriteRegister(0x1B,0x15);
	_WriteRegister(0x01,0x00);
	_WriteRegister(0x1C,0x03);
	_WriteRegister(0x19,0x01);
	delay(5);
	_WriteRegister(0x1F,0x8C);
	_WriteRegister(0x1F,0x84);
	delay(10);
	_WriteRegister(0x1F,0x94);
	delay(10);
	_WriteRegister(0x1F,0xD4);
	delay(5);
	_WriteRegister(0x40,0x00);
	_WriteRegister(0x41,0x2B);
	_WriteRegister(0x42,0x29);
	_WriteRegister(0x43,0x3E);
	_WriteRegister(0x44,0x3D);
	_WriteRegister(0x45,0x3F);
	_WriteRegister(0x46,0x24);
	_WriteRegister(0x47,0x74);
	_WriteRegister(0x48,0x08);
	_WriteRegister(0x49,0x06);
	_WriteRegister(0x4A,0x07);
	_WriteRegister(0x4B,0x0D);
	_WriteRegister(0x4C,0x17);
	_WriteRegister(0x50,0x00);
	_WriteRegister(0x51,0x02);
	_WriteRegister(0x52,0x01);
	_WriteRegister(0x53,0x16);
	_WriteRegister(0x54,0x14);
	_WriteRegister(0x55,0x3F);
	_WriteRegister(0x56,0x0B);
	_WriteRegister(0x57,0x5B);
	_WriteRegister(0x58,0x08);
	_WriteRegister(0x59,0x12);
	_WriteRegister(0x5A,0x18);
	_WriteRegister(0x5B,0x19);
	_WriteRegister(0x5C,0x17);
	_WriteRegister(0x5D,0xFF);
	_WriteRegister(0x16,0x08);
	_WriteRegister(0x28,0x20);
	delay(40);
	_WriteRegister(0x28,0x38);
	delay(40);
	_WriteRegister(0x28,0x3C);
	_WriteRegister(0x17,0x05);
	
	_CS_DIS;
}



/* NOT IMPLEMENTED FOR THIS DISPLAY */
void Display::DUpdateDisplay(void)
{
}



/* NOT IMPLEMENTED FOR THIS DISPLAY */
void Display::DReset(void)
{

}



/* Places the screen in to or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
		
		Code taken from Ilitek application notes.
*/
void Display::DSleep(boolean mode)
{
	_CS_EN;
	
	if(mode)
	{
		DScreen(OFF);
		_PowerMode(OFF);
		
		_WriteRegister(0x19,0x00); //OSC_EN = 0;
	}else
	{
		_WriteRegister(0x19,0x01); //OSC_EN = 1;
		delay(10);
		_WriteRegister(0x1F,0x8C); //STB = 0
		_PowerMode(ON);
		DScreen(ON);
	}

	_CS_DIS;
} 
	



/* Turn the screen on or off where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	_CS_EN;
	
	if(mode)
	{
		_WriteRegister(0x28,0x3C); //DTE = 1, D1 = 1, D0 = 1;
		delay(40);
	}
	else
	{
		_WriteRegister(0x28,0x28); // DTE = 0
		delay(40);
		_WriteRegister(0x28,0x20); // D1 = 0
	}
	
	_CS_DIS;
}



/* Internal function that powers the display up or down. Used as part of the sleep function */
void Display::_PowerMode(boolean mode)
{
	_CS_EN;
	
	if(mode)
	{
		_WriteRegister(0x1B,0x15); //Set VRH
		_WriteRegister(0x23,0x6F); //Set VMF
		_WriteRegister(0x25,0x71); //Set VML
		_WriteRegister(0x24,0x57); //Set VMH
		
		_WriteRegister(0x19,0x01); //OSC_EN = 1;
		delay(6);
		_WriteRegister(0x1F,0x8C); //STB = 0
		_WriteRegister(0x1C,0x03); //AP2:0 = 011;
		_WriteRegister(0x1F,0x8C); //STB = 0
		delay(5);
		_WriteRegister(0x1F,0x84); //DK = 0
		delay(5);
		_WriteRegister(0x1F,0x94); //PON = 1
		delay(5);
		_WriteRegister(0x1F,0xD4); //VCOMG = 1
		delay(6);
		DScreen(ON); 	
	}else
	{
		DScreen(OFF); 
		_WriteRegister(0x1F,0x94); //VCOMG = 0
		delay(5);
		_WriteRegister(0x1F,0x8C); //PON = 0, DK = 1
		_WriteRegister(0x1C,0x00); //AP2:0 = 0;
		_WriteRegister(0x1F,0x8D); //PON = 0, DK = 1, STB = 1
		delay(5);	
	}
	
	_CS_DIS;
}



/* Sets the state of the backlight. NOT APPLICABLE FOR THIS SCREEN */
void Display::DBacklight(boolean mode)
{
}




/* Sets the screen orientation and write direction where:
	mode is the direction, orientation to set the screen to. Valid vales are:
		SCREEN_NORMAL 		(Default)
		SCREEN_R90 			Screen is rotated 90o		
		SCREEN_R180 		Screen is rotated 180o
		SCREEN_R270 		Screen is rotated 270o		
*/
void Display::DFlip(uint8_t mode)
{
	switch(mode)
	{
		case(SCREEN_NORMAL):
			_CS_EN;
			_FlipMode = mode;
			_res_x = RES_X;
			_res_y = RES_Y;
			_WriteRegister(0x16, 0x68); 
			_CS_DIS;
			break;
			
		case(SCREEN_R90):
			_CS_EN;
			_FlipMode = mode;
			_res_x = RES_Y;
			_res_y = RES_X;
			_WriteRegister(0x16, 0xC8);
			_CS_DIS;
			break;
			
		case(SCREEN_R180):
			_CS_EN;
			_FlipMode = mode;
			_res_x = RES_X;
			_res_y = RES_Y;
			_WriteRegister(0x16, 0xa8);
			_CS_DIS;
			break;
			
		case(SCREEN_R270):
			_CS_EN;
			_FlipMode = mode;
			_res_x = RES_Y;
			_res_y = RES_X;
			_WriteRegister(0x16, 0x08);
			_CS_DIS;
			break;		
	}
}



/* Sets the foreground colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
*/
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
	_FGColour = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);	
}



/* NOT IMPLEMENTED FOR THIS DISPLAY */
void Display::DSetFG(boolean colour)
{
}



/* Sets the background colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
*/
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
	_BGColour = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);	
}



/* NOT IMPLEMENTED FOR THIS DISPLAY */
void Display::DSetBG(boolean colour)
{
	_BGColour = colour;
}



/* Clears the entire contents of the screen */
void Display::DClear(void)
{
	DErase(0, 0, _res_x - 1, _res_y - 1);
}



/* Erases the entire screen by drawing a rectangle using the current background colour */
void Display::DErase(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	uint16_t Temp = _FGColour;
	_FGColour = _BGColour;
	DRect(x1, y1, x2, y2);
	_FGColour = Temp;
}



/* Sets a single pixel to the current foreground colour where:
	x is the x axis coordinate of the pixel.
	y is the y axis coordinate of the pixel. 
*/
void Display::DPlot(int16_t x, int16_t y)
{
	if(x >= 0 && x < _res_x && y >= 0 && y < _res_y)
	{
		_CS_EN;
		_SetWriteArea(x, y, x, y);

		_WriteCommand(RAMWR); //Write to RAM */
		_WriteData(_FGColour >> 8); 
		_WriteData(_FGColour); 
		_CS_DIS;
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
	//if(x2 >= 0 && x1 < _res_x && y2 >= 0 && y2 < _res_y)
	if(x1 < _res_x && x2 >= 0 && y1 < _res_y && y2 >= 0) 
	{
		_CS_EN;
	
		if(x1 < 0)
			x1 = 0;
		if(y1 < 0)
			y1 = 0;
	
		_SetWriteArea(x1, y1, x2, y2);

		_WriteCommand(RAMWR); //Write to RAM */
	
		for(uint16_t y = y1; y <= y2; y++)
			for(uint16_t x = x1; x <= x2; x++)
			{
				_WriteData(_FGColour >> 8); 
				_WriteData(_FGColour); 
			}
		_CS_DIS;
	}
}



/* NOT IMPLEMENTED FOR THIS DISPLAY */	
void Display::DDrawMode(uint8_t Mode)
{
}




/* Set an X/Y coordinate for the bitmap and print functions where:
	x is the x axis coordinate.
	y is the y axis coordinate.
*/
void Display::DSetXY(uint16_t X, uint16_t Y)
{
	_X1Pos = X;
	_Y1Pos = Y;
	_X2Pos = X;
	_Y2Pos = Y;
	_CS_EN;
	_SetWriteArea(_X1Pos, _Y1Pos, _X2Pos, _Y2Pos);
	_CS_DIS;
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
void Display::DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t BitmapData[], boolean Background)
{
	_CS_EN;
	
		int16_t x = _X1Pos;
		for(uint16_t i = 0; i < (Cols * ByteRows); i += ByteRows)
		{
			for(uint8_t Scale = 0; Scale < _ScaleX; Scale++)
			{
				if(Background)
					_DWriteCol(x, _Y1Pos, ByteRows, &BitmapData[i]);
				else
					_DPlotCol(x, _Y1Pos, ByteRows, &BitmapData[i]);
				x++;
			}
		}
	
	_CS_DIS;	
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
	//const uint8_t *FontByteAddress;

	/* Set a pointer to the starting address for the characters bitmap */
	//FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]);

	/* Use the draw bitmap function to write the character bitmap to the screen */
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background);

	
	/* If the character is being printed with a background fill in the gaps between each character */
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1);

	/* Move the cursor to the end of the printed character so it is in the correct position should we wish to print another character */
	DSetXY(_X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos);
}




/* Reads the RGB colour value for a single pixel at the specified coordinate where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	Data is a 3 byte array used to store the RGB values 
*/
void Display::DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
	uint8_t r, g, b;
	uint16_t t;
	
	//Swap x & y axis depending on screen orientation
	switch(_FlipMode)
	{
		case(SCREEN_NORMAL):
		case(SCREEN_R180):
			t = y;
			y = x;
			x = t;
			break;
	}

	_CS_EN;
		
	_WriteRegister(0x80,x >> 8);	//GRAM X axis address H
	_WriteRegister(0x81,x);			//GRAM X axis address L
  	_WriteRegister(0x82,y >> 8);	//GRAM Y axis address H
  	_WriteRegister(0x83,y);			//GRAM Y axis address L
	
	
	//Select command register GDRAM
	_RS_COMMAND;
	_WRITE_DATA(RAMWR);
	
	//Select data
	_RS_DATA;
	
	//Change data pins to inputs
	_DATA_DIR_INPUT;
	
	//Dummy read
	_RD_LOW;
	_RD_HIGH;

	//Read 16 bit RGB data for selected pixel
	_READ_DATA(r);
	_READ_DATA(g);
	_READ_DATA(b);

	// Change data pins back to outputs
	_DATA_DIR_OUTPUT;

	_CS_DIS;
	
	//Serial.print(r, BIN); Serial.print(" : "); Serial.print(g, BIN); Serial.print(" : "); Serial.println(b, BIN); 
	
	//Save 16 bit RGB data
	Data[0] = r >> 3;
	Data[1] = g >> 2;
	Data[2] = b >> 3;
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
	uint8_t Data[3];
	DReadPixelRGB(x, y, Data);
 
	if(Data[0] == _FGColour >> 11 && 
	   Data[1] == ((_FGColour >> 5) & 0x3F) && 
	   Data[2] == (_FGColour & 0x1F))
		return true;
	   
	return false;
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




/* Internal function that sets the area of the display to write to */
void Display::_SetWriteArea(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	/* Make sure none of the area to write to is outside the display area. If so then crop it */
	if(X1 >= _res_x)
		X1 = _res_x - 1;
	if(X2 >= _res_x)
		X2 = _res_x - 1;
	if(Y1 >= _res_y)
		Y1 = _res_y - 1;
	if(Y2 >= _res_y)
		Y2 = _res_y - 1; 

	uint16_t x, y, temp1, temp2;
	
	// Flip the XY axis depending on the screen orientation
	switch(_FlipMode)
	{
		case(SCREEN_NORMAL):
		case(SCREEN_R180):
			x = Y1;
			y = X1; 
			break;
			
		case(SCREEN_R90):
		case(SCREEN_R270):
			x = X1;
			y = Y1; 		
			break;
	}

	
	_WriteRegister(0x02,X1 >> 8);	//X axis start address H
	_WriteRegister(0x03,X1);		//X axis start address L
	_WriteRegister(0x04,X2 >> 8);	//X axis end address H
   	_WriteRegister(0x05,X2);		//X axis end address L
   	_WriteRegister(0x06,Y1 >> 8);	//Y axis start address H
   	_WriteRegister(0x07,Y1);		//Y axis start address L
   	_WriteRegister(0x08,Y2 >> 8);	//Y axis end address H
   	_WriteRegister(0x09,Y2);		//Y axis end address L
	_WriteRegister(0x80,x >> 8);	//GRAM X axis address H
	_WriteRegister(0x81,x);			//GRAM X axis address L
  	_WriteRegister(0x82,y >> 8);	//GRAM Y axis address H
  	_WriteRegister(0x83,y);			//GRAM Y axis address L
	
}



/* Internal function that writes 16 bits of data to one of the displays registers */
void Display::_WriteRegister(uint8_t command, uint8_t data)
{
	_WriteCommand(command);
	_WriteData(data);
}



/* Internal function writes an 8 bit command byte to the display */
void Display::_WriteCommand(uint8_t command)
{
	_RS_COMMAND;
	_WRITE_DATA(command);
}



/* Internal function writes an 9 bit data parameter byte to the display */
void Display::_WriteData(uint8_t data)
{
	_RS_DATA;
	_WRITE_DATA(data);
}



/* Internal function that writes a one pixel column of bitmap data by using the screens burst write function. This is slightly faster but also results in background being written. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t Data[])
{

	if(x >= 0 && x < _res_x)
	{	
		int16_t dy = y;
		if(dy < 0)
			dy = 0;
		_SetWriteArea(x, dy, x, dy + (ByteRows * 8 * _ScaleY) - 1);
		_WriteCommand(RAMWR); //Write to RAM

		for(uint8_t i = ByteRows; i > 0; i--)
		{
			for(uint8_t bit = 0; bit < 8; bit++)
			{
				for(uint8_t Scale = 0; Scale <	_ScaleY; Scale++)
				{
					if(y >= 0 && y < _res_y)
					{
						if((pgm_read_byte_near(&Data[i - 1]) >> bit) & 0x01)
						{
							_WriteData(_FGColour >> 8); 
							_WriteData(_FGColour); 
						}else
						{
							_WriteData(_BGColour >> 8); 
							_WriteData(_BGColour); 
						}	
					}
					y++;				
				}
			}
		}
	}
}



/* Internal function that writes a one pixel column of bitmap data by plotting the individual pixels. This results in a bitmap with no background. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t Data[])
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

					if((pgm_read_byte_near(&Data[i - 1]) >> bit) & 0x01)
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

void Touch::TInit(uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping)
{
	//Save the calibration data (raw min/max x/y axis data) and sensor mapping. 
	_Cal_X_Min = Cal_X_Min;
	_Cal_X_Max = Cal_X_Max;
	_Cal_Y_Min = Cal_Y_Min;
	_Cal_Y_Max = Cal_Y_Max;
	
	_Touch_Mapping = Mapping;
}



/* Perform an X & Y axis read of the touch sensor. The results can then be obtained using the TReadAxis() function */
void Touch::TReadTouch(void)
{
	/* If touch sensor is rotated by 90 or 270 then flip the x & y axis, otherwise recored the sensor readings */
	if(_Touch_Mapping == TOUCH_0_NORMAL ||
	   _Touch_Mapping == TOUCH_180_NORMAL ||
	   _Touch_Mapping == TOUCH_0_FLIP_X ||
	   _Touch_Mapping == TOUCH_180_FLIP_X)
	{
		_TouchY = TReadRaw(TOUCH_AXIS_X);
		_TouchX = TReadRaw(TOUCH_AXIS_Y);
	}else
	{
		_TouchX = TReadRaw(TOUCH_AXIS_X);
		_TouchY = TReadRaw(TOUCH_AXIS_Y);
	}
	

	//Depending on the orientation of the sensor, map the raw XY coordinates to the resolution of the screen.
	switch(_Touch_Mapping)
	{
		case(TOUCH_0_NORMAL):
		case(TOUCH_270_FLIP_X):
			_TouchX = constrain(map(_TouchX, _Cal_X_Max, _Cal_X_Min, 0, _res_x - 1), 0 , _res_x - 1);
			_TouchY = constrain(map(_TouchY, _Cal_Y_Min, _Cal_Y_Max, 0, _res_y - 1), 0 , _res_y - 1);
			break;
			
		case(TOUCH_90_NORMAL):
		case(TOUCH_180_FLIP_X):
			_TouchX = constrain(map(_TouchX, _Cal_X_Min, _Cal_X_Max, 0, _res_x - 1), 0 , _res_x - 1);
			_TouchY = constrain(map(_TouchY, _Cal_Y_Min, _Cal_Y_Max, 0, _res_y - 1), 0 , _res_y - 1);
			break;
			
		case(TOUCH_180_NORMAL):
		case(TOUCH_90_FLIP_X):
			_TouchX = constrain(map(_TouchX, _Cal_X_Min, _Cal_X_Max, 0, _res_x - 1), 0 , _res_x - 1);
			_TouchY = constrain(map(_TouchY, _Cal_Y_Max, _Cal_Y_Min, 0, _res_y - 1), 0 , _res_y - 1);
			break;
			
		case(TOUCH_270_NORMAL):
		case(TOUCH_0_FLIP_X):
			_TouchX = constrain(map(_TouchX, _Cal_X_Max, _Cal_X_Min, 0, _res_x - 1), 0 , _res_x - 1);
			_TouchY = constrain(map(_TouchY, _Cal_Y_Max, _Cal_Y_Min, 0, _res_y - 1), 0 , _res_y - 1);
			break;	
	}
}




/* Checks to see if the screen is currently pressed.

	Returns a boolean value where:
	
	true = screen is currently pressed.
	false = screen is not currently pressed.
*/
boolean Touch::TPressed(void)
{
	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);

	boolean state = true;
	TOUCH_XP_DDR |= TOUCH_XP_MASK;
	TOUCH_XN_DDR &= ~TOUCH_XN_MASK;
	TOUCH_YP_DDR &= ~TOUCH_YP_MASK;
	TOUCH_YN_DDR &= ~TOUCH_YN_MASK;
	
	TOUCH_XP_HIGH;
	TOUCH_XP_LOW;

		
	if(analogRead(TOUCH_Y_ANALOGUE) > (min(_Cal_X_Min, _Cal_X_Max) / 2))
		state = false;
			
	TOUCH_XN_DDR |= TOUCH_XN_MASK;
	TOUCH_YP_DDR |= TOUCH_YP_MASK;
	TOUCH_YN_DDR |= TOUCH_YN_MASK;

	return state;
}




/* Returns the last X or Y axis reading from the touch sensor as a value mapped to the resolution of the screen where:

	Axis is a boolean value specifying which axis reading to return. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
*/
uint16_t Touch::TGetCoord(boolean Axis)
{
	if(Axis == TOUCH_AXIS_X)
		return _TouchX;
	else
		return _TouchY;
}



/* Triggers a sensor measurement of one of the axis and stores the result where:

	Axis is a boolean value specifying which axis to measure. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
		
	Returns an unsigned int value containing a raw sensor reading for the specified axis.
*/
uint16_t Touch::TReadRaw(boolean Axis)
{
	uint16_t Result;
	if(Axis == TOUCH_AXIS_X)
	{
		TOUCH_XP_DDR |= TOUCH_XP_MASK;
		TOUCH_XN_DDR |= TOUCH_XN_MASK;
		TOUCH_YP_DDR &= ~TOUCH_YP_MASK;
		TOUCH_YN_DDR &= ~TOUCH_YN_MASK;
	
		TOUCH_XP_HIGH;
		TOUCH_XN_LOW;
		
		Result = analogRead(TOUCH_Y_ANALOGUE);
		
		TOUCH_YP_DDR |= TOUCH_YP_MASK;
		TOUCH_YN_DDR |= TOUCH_YN_MASK;	
	}else
	{
		TOUCH_YP_DDR |= TOUCH_YP_MASK;
		TOUCH_YN_DDR |= TOUCH_YN_MASK;
		TOUCH_XP_DDR &= ~TOUCH_XP_MASK;
		TOUCH_XN_DDR &= ~TOUCH_XN_MASK;
	
		TOUCH_YP_HIGH;
		TOUCH_YN_LOW;
		
		Result = analogRead(TOUCH_X_ANALOGUE);
		
		TOUCH_XP_DDR |= TOUCH_XP_MASK;
		TOUCH_XN_DDR |= TOUCH_XN_MASK;
	}
	
	return Result;
}



/* Internal library function used by the HCDisplay class to pass the screens X & Y resolution to the touch class.
*/
void Touch::SetScreenRes(uint16_t Res_X, uint16_t Res_Y)
{
	_res_x = Res_X;
	_res_y = Res_Y;
}