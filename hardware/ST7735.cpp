/* FILE:    ST7735.cpp
   DATE:    04/07/23
   VERSION: 1.0
   AUTHOR:  Andrew Davies
   
04/07/23 version 1.0: Original version

This library adds hardware support to the HCDisplay library for PCD8544 based screens.
Current supported boards:


1.8 Inch TFT Full Colour Display - ST7735 (SKU: HCMODU0160). Available for purchase form our 
store here:

https://hobbycomponents.com/displays/1158-18-inch-tft-full-colour-display-st7735


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
#include "hardware/ST7735.h"
#include "Arduino.h"
#include <SPI.h>


/* LCD commands*/
#define SLEEP_IN_REG			0x10
#define SLEEP_OUT_REG			0x11
#define DISPLAY_OFF_REG			0x28
#define DISPLAY_ON_REG			0x29
#define COL_ADD_SET_REG			0x2A
#define ROW_ADD_SET_REG			0x2B
#define MEM_WRITE_REG			0x2C
#define MEM_READ_REG			0x2E
#define MEMORY_DATA_ACC_CON_REG	0x36
#define INT_PIX_FORMAT_REG		0x3A




/* Constructor to initialise the display library */
Display::Display(void)
{
	
}



/* Initialises the display and configures the serial and control pins where:

	din is the digital pin for the screens data pin
	clk is the digital pin for the screens clock pin
	rst is the digital pin for the reset control pin.
	ce is the digital pin for the chip select control pin.
	dc is the digital pin for the Data/Command control pin.
*/
void Display::DInit(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t bl)
{
	// Configure pins
	_CSP =  portOutputRegister(digitalPinToPort(cs));
	_CSM =  digitalPinToBitMask(cs);
	
	_DCP =  portOutputRegister(digitalPinToPort(dc));
	_DCM =  digitalPinToBitMask(dc);
	
	_RSP =  portOutputRegister(digitalPinToPort(rst));
	_RSM =  digitalPinToBitMask(rst);
	
	
	/* Configure the control pins */
	pinMode(cs, OUTPUT);
	pinMode(dc, OUTPUT);
	pinMode(rst, OUTPUT);

	// Configure the backlight pin if required
	if(bl != 255)
	{
		_BLP =  portOutputRegister(digitalPinToPort(bl));
		_BLM =  digitalPinToBitMask(bl);
		pinMode(bl, OUTPUT);
		_blFlag = true;
		DBacklight(OFF);
	}
	
	_CS_DIS;
	_RS_DIS;
	
	
	/* Enable the hardware SPI interface */
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	
	
	// Setup the screen
	DReset();
	DSleep(OFF);
	_CS_EN;
	_Send_Command(INT_PIX_FORMAT_REG, 0b101);		// 16 bit pixel format
	_CS_DIS;
	DFlip(SCREEN_NORMAL);
	DClear();
	DScreen(ON);
	
	
	// Set the font to the standard system font 
	DSetFont(SystemFont);
}




/* NOT IMPLEMENTED FOP THIS DISPLAY
	Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
}




/* Toggles the reset pin to perform a hardware reset of the display. */
void Display::DReset(void)
{
	_RS_EN;
	delayMicroseconds(10);  // At least 5us
	_RS_DIS;
	delay(120);
}




/*  Places the screen into or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{
	_CS_EN;
	if(mode)
		_Send_Command(SLEEP_IN_REG);	// Sleep in
	else
		_Send_Command(SLEEP_OUT_REG);	// Sleep out
	_CS_DIS;
	
	delay(120);
}




/* Turns the screen on or off where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	_CS_EN;
	if(mode)
		_Send_Command(DISPLAY_ON_REG);
	else
		_Send_Command(DISPLAY_OFF_REG);
	
	_CS_DIS;
}




/* NOT IMPLEMENTED FOR THIS DISPLAY
	Sets the screens contrast level where:
	level is the required contrast level. Valid values for level are
		0 	(min contrast)
		255 (max contrast)
*/
void Display::DContrast(uint8_t level)
{
}




/* Sets the state of the backlight where
	mode is the backlight state. Valid values for mode are:
		false = backlight off
		true = backlight on
*/
void Display::DBacklight(boolean mode)
{
	if(_blFlag)
	{
		if(mode)
			_BL_EN;
		else
			_BL_DIS;
	}
}




/* Sets the screen orientation and write direction where:
	mode is the direction, orientation to set the screen to. Valid vales for mode are:
		SCREEN_NORMAL 		(Default)
		SCREEN_FLIP_H_R270 	
		SCREEN_FLIP_V_R180 	
		SCREEN_R270 		
		SCREEN_FLIP_V 		
		SCREEN_R90 			
		SCREEN_R180 		
		SCREEN_FLIP_H_R90 		
*/
void Display::DFlip(uint8_t mode)
{
	_CS_EN;
	
	uint8_t data = 0;
	
	// Modes requiring the row and column to be exchanged
	if(mode == SCREEN_NORMAL || mode == SCREEN_R180 || mode == SCREEN_FLIP_V || mode == SCREEN_FLIP_V_R180)
		data |= (0b1 << 5);
	
	// Modes requiring the x axis to be mirrored
	if(mode == SCREEN_R180 || mode == SCREEN_R270 || mode == SCREEN_FLIP_H_R90 || mode == SCREEN_FLIP_V_R180)
		data |= (0b1 << 6);
	
	// Modes requiring the y axis to be mirrored
	if(mode == SCREEN_NORMAL || mode == SCREEN_R270 || mode == SCREEN_FLIP_V_R180 || mode == SCREEN_FLIP_H_R270)
		data |= (0b1 << 7);
	
	_Send_Command(MEMORY_DATA_ACC_CON_REG, data);
	
	// Flip the x & y resolutions for these modes
	if(mode == SCREEN_FLIP_H_R270 || mode == SCREEN_R270 || mode == SCREEN_R90 || mode == SCREEN_FLIP_H_R90)
	{
		_res_x = RES_Y, 
		_res_y = RES_X;
	}else
	{
		_res_x = RES_X, 
		_res_y = RES_Y;
	}
	
	_CS_DIS;
}




/* Sets the foreground colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
*/
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
	r >>= 3;
	g >>= 2;
	b >>= 3;
	_FGColour = (r << 11) | (g << 5) | b;
}




/*  NOT IMPLEMENTED FOR THIS DISPLAY
	Sets the foreground colour to be used by the graphic and print functions where:
	
	colour is a boolean value representing the colour. Valid values for colour are
			0 = the screens background colour;
			1 = the screens foreground colour;		

	RGB values that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetFG(boolean colour)
{
}




/* Sets the background colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).

	RGB values that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
	r >>= 3;
	g >>= 2;
	b >>= 3;
	_BGColour = (r << 11) | (g << 5) | b;
}




/* NOT IMPLEMENTED FOR THIS DISPLAY
	Sets the background colour to be used by the graphic and print functions where:
	
	colour is a boolean value representing the colour. Valid values for colour are
			0 = the screens background colour;
			1 = the screens foreground colour;
*/
void Display::DSetBG(boolean colour)
{
}




/* Clears the entire contents of the screen */
void Display::DClear(void)
{
	DErase(0, 0, _res_x - 1, _res_y - 1);
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
		_Send_Command(MEM_WRITE_REG, _FGColour >> 8, _FGColour);
		_CS_DIS;
	}
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




/* Draws a solid rectangle to the using the current foreground colour where:
	x1 is the x coordinate of the top left corner of the rectangle.
	y1 is the y coordinate of the top left corner of the rectangle.
	x2 is the x coordinate of the bottom right corner of the rectangle.
	y2 is the y coordinate of the bottom right corner of the rectangle.
*/
void Display::DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	if(x1 < 0) x1 = 0;
	if(x2 >= _res_x) x2 = _res_x - 1;
	if(y1 < 0) y1 = 0;
	if(y2 >= _res_y) y2 = _res_y - 1;

	_CS_EN;
	_SetWriteArea(x1, y1, x2, y2);
	
	_DC_CMD;
	SPI.transfer(MEM_WRITE_REG);
	_DC_DAT;
	
	for(uint8_t x = x1; x <= x2; x++)
	{
		for(uint8_t y = y1; y <= y2; y++)
		{
			SPI.transfer(_FGColour >> 8);
			SPI.transfer(_FGColour);
		}
	}
	_CS_DIS;
}




/* NOT IMPLEMENTED FOR THIS DISPLAY

	Stores the drawing mode used by the text and graphical functions where:

	Mode is the required drawing mode. Valid values for Mode are:
		DRAWMODE_NORMAL		Pixels are drawn normally
		
		DRAWMODE_XOR		Pixels are logically XOR'd with the pixels currently drawn on the screen.
		
		DRAWMODE_AND_INV 	Pixels are logically AND'd and inverted with the pixels currently drawn on the screen.
*/
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
}




/* Sets the amount of scaling for the bitmap and print functions where:
	ScaleX is the magnitude of scaling in the x axis.
	ScaleY is the magnitude of scaling in the y axis.
	
	A value of 1 for ScaleX or ScaleY will result in no scaling.
	A value of 2 for ScaleX or ScaleY will result in the bitmap or font being scaled by a factor of 2 in the appropriate axis.
	A value of n for ScaleX or ScaleY will result in the bitmap or font being scaled by a factor of n in the appropriate axis.
*/	
void Display::DScaleXY(uint8_t ScaleX, uint8_t ScaleY)
{
	if(ScaleX)
		_ScaleX = ScaleX;
	if(ScaleY)
		_ScaleY = ScaleY;	
}





/* Write BW bitmap data to the LCD starting using the current foreground colour at the cursor location where:
   
   Cols is the number of byte columns to write to.
   
   ByteRows is the number of rows to write to in 8 pixel chunks 
   
   BitmapData is an array containing the bitmap data to be written 
   
   Background selects if the character will be printed with a solid background. Valid values for Background are
			true 	(character is printed with background with a colour set by the DSetBG() function
			false 	(Character is printed without a background)*/

void Display::DBWBitmap(uint16_t Cols,  uint8_t ByteRows, const uint8_t BitmapData[], boolean Background)
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
   You can enable more fonts by editing the options.h file found in the HCDisplay folder.
*/
void Display::DSetFont(FONT_INFO *NewFont)
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
	#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
	FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]);
	#else
	FontByteAddress = &Font->p_character_bitmaps[BitmapIndex];
	#endif
	
	/* Use the draw bitmap function to write the character bitmap to the screen */
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background);
	
	/* If the character is being printed with a background fill in the gaps between each character */
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1);
	
	/* Move the cursor to the end of the printed character so it is in the correct position should we wish to print another character */
	DSetXY(_X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos);
}




/* Internal function that writes a one pixel column of bitmap data by plotting the individual pixels. This results in a bitmap with no background. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t Data[]/*, boolean Update*/)
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




/* Internal function that writes a one pixel column of bitmap data. This results in background being written. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t Data[])
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
					else
						DErase(x, y, x, y2);	
					y = y2 + 1;
				}				
			}
		}
	}
};




/*  NOT IMPLEMENTED FOR THIS DISPLAY
	Reads the RGB colour value for a single pixel at the specified coordinate where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	Data is a 3 byte array used to store the RGB values 
*/
void Display::DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
/*	_CS_EN;
	_SetWriteArea(x, y, x, y);

	_Send_Command(MEMORY_DATA_ACC_CON_REG); //Read from RAM

	SPI.transfer(0); //Dummy write
	
	Data[0] = SPI.transfer(0) >> 3;	// Read the red component
	Data[1] = SPI.transfer(0) >> 2;	// Read the green component
	Data[2] = SPI.transfer(0) >> 3;	// Read the blue component
	_CS_DIS;*/
}




/* NOT IMPLEMENTED FOR THIS DISPLAY
	Checks if the pixel at the specified coordinate is set to the foreground colour where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	
	Returns a boolean true or false:
		true = pixel is set to current foreground colour.
		false = pixel not is set to current foreground colour.
		
	Use this function for black and white screens.
*/
boolean Display::DReadPixel(uint16_t x, uint16_t y)
{
}




/* Returns the resolution of the displays X axis as a 16 bit unsigned number */
uint16_t Display::DResX(void)
{
	return _res_x;
}




/* Returns the resolution of the displays Y axis as a 16 bit unsigned number 
*/
uint16_t Display::DResY(void)
{
	return _res_y;
}




/* Writes a command byte to the display where:
	Command is the byte address of the command
*/
void Display::_Send_Command(uint8_t Command)
{	
	_DC_CMD;
	SPI.transfer(Command);
}




/* Writes a command byte + data to the display where:
	Command is the byte address of the command
	Data is the byte of data to send to the command register 
*/
void Display::_Send_Command(uint8_t Command, uint8_t Data)
{	
	_DC_CMD;
	SPI.transfer(Command);

	_DC_DAT;
	SPI.transfer(Data);

}




/* Writes a command byte + 2 bytes of data to the display where:
	Command is the byte address of the command
	Data1 is the 1st byte of data to send to the command register 
	Data2 is the 2nd byte of data to send to the command register 
*/
void Display::_Send_Command(uint8_t Command, uint8_t Data1, uint8_t Data2)
{
	_DC_CMD;
	SPI.transfer(Command);
	_DC_DAT;
	SPI.transfer(Data1);
	SPI.transfer(Data2);
}



/* Writes a single byte of data to the displays ram
*/
void Display::_Send_Data(uint8_t Data)
{	
	_DC_DAT;
	SPI.transfer(Data);
}




/* Internal function that sets the area of the display to write to */
void Display::_SetWriteArea(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	_DC_CMD;
	SPI.transfer(COL_ADD_SET_REG);	
	_DC_DAT;
	SPI.transfer(0x00);	
	SPI.transfer(X1);	
	SPI.transfer(0x00);	
	SPI.transfer(X2);	
	_DC_CMD;
	SPI.transfer(ROW_ADD_SET_REG);	
	_DC_DAT;
	SPI.transfer(0x00);	
	SPI.transfer(Y1);	
	SPI.transfer(0x00);	
	SPI.transfer(Y2);
}




/* TOUCH SCREEN FUNCTIONS ARE NOT SUPPORTED FOR THIS DISPLAY */


/* Constructor to initialise the optional touch library */
Touch::Touch(void)
{
}
