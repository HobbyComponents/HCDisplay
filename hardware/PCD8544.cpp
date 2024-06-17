/* FILE:    PCD8544.cpp
   DATE:    12/06/24
   VERSION: 0.2
   AUTHOR:  Andrew Davies
   
27/06/22 version 0.1: Original version
12/06/24 version 0.2: Updated to work with ARM based devices

This library adds hardware support to the HCDisplay library for PCD8544 based screens.
Current supported boards:


Serial LCD (Nokia 5110) 84 x 48 - Blue Backlight (SKU: HCMODU0105). Available for purchase form our 
store here:

https://hobbycomponents.com/displays/1159-84-x-48-lcd-display-nokia-5110


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
#include "hardware/PCD8544.h"
#include "Arduino.h"


/* Display modes */
//#define BLANK 				0x08
//#define ALL_ON 				0x09
#define NORMAL 				0x0C
//#define INVERSE 			0x0D

/* LCD command sets*/
#define BASIC_COMMAND 		0x20
#define EXTENDED_COMMAND 	0x21

// Display registers
#define TEMP_COEF_REG		0x04
#define BIAS_SYS_REG		0x10
#define Y_ADD_REG 			0x40
#define X_ADD_REG 			0x80
#define VOP_REG				0x80



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
void Display::DInit(uint8_t din, uint8_t clk, uint8_t rst, uint8_t ce, uint8_t dc, uint8_t bl)
{
	// Store the pin settings
	_din = din;
	_clk = clk;
	_rst = rst;
	_ce = ce;
	_dc = dc;
	_bl = bl;
	

	// Set all pins to outputs
	pinMode(_ce, OUTPUT);
	pinMode(_rst, OUTPUT);
	pinMode(_dc, OUTPUT);
	pinMode(_din, OUTPUT);
	pinMode(_clk, OUTPUT);
	
	if(bl != 255)
	{
		pinMode(_bl, OUTPUT);
		DBacklight(OFF);
	}
  
	// Reset the 5110
	DReset();
	
	// Change to extended commands  
	_Send_Command(EXTENDED_COMMAND);
  
	// Set temperature coefficient to a typical value
	_Send_Command(TEMP_COEF_REG);

	// Set bias value to 1:24
	_Send_Command(BIAS_SYS_REG | 0b101);
	
	// Change back to basic commands 
	_Send_Command(BASIC_COMMAND);
  
	// Set the display mode to normal
	_Send_Command(NORMAL);
	
	// Set the screens contrast
	DContrast(0x48);
	
	// Set screen orientation to normal 
	DFlip(SCREEN_NORMAL);
	
	// Set drawing mode to normal
	DDrawMode(DRAWMODE_NORMAL);
	
	/* Set the font to the standard system font */
	DSetFont(SystemFont);
	
	// Clear the buffer and update the display
	DClear();
	
	// Output the buffer to the screen
	DUpdateDisplay();
	
	// Put the cursor in the home position
	DSetXY(0, 0);
	
}




/* Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
	// Set X & Y RAM address registers to 0
	_Send_Command(X_ADD_REG);
	_Send_Command(Y_ADD_REG);
	
	for(uint8_t row = 0; row < BUFFERBYTEROWSIZE; row++)
	{
		for(uint8_t col = 0; col < BUFFERCOLSIZE; col++)
		{
			_Send_Data(DisplayBuffer[col][row]);
		}
	}
}




/* Toggles the reset pin to perform a hardware reset of the display. */
void Display::DReset(void)
{
	digitalWrite(_rst, LOW);
	digitalWrite(_rst, HIGH);
}




/*  Places the screen into or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{
	if(mode)
	{
		// For optimum low power the screens RAM must first be cleared
		_Send_Command(X_ADD_REG);
		_Send_Command(Y_ADD_REG);
	
		for(uint8_t row = 0; row < BUFFERBYTEROWSIZE; row++)
			for(uint8_t col = 0; col < BUFFERCOLSIZE; col++)
				_Send_Data(0x00);

		DScreen(true);
	}
	else
	{
		DScreen(false);
		DUpdateDisplay();
	}
}




/* Turns the screen on or off where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	if(mode)
		_Send_Command(0x20 | 0b100);
	else
		_Send_Command(0x20 | 0b000);
}




/* Sets the screens contrast level where:
	level is the required contrast level. Valid values for level are
		0 	(min contrast)
		255 (max contrast)
*/
void Display::DContrast(uint8_t level)
{
	level = level >> 1;
	
	// Change to extended commands 
	_Send_Command(EXTENDED_COMMAND);
	// Set the contrast
	_Send_Command(VOP_REG | level); 
	// Change back to basic commands
	_Send_Command(BASIC_COMMAND);
}




/* Sets the state of the backlight */
void Display::DBacklight(boolean mode)
{
	digitalWrite(_bl, !mode);
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




/* NOT IMPLEMENTED FOR THIS DISPLAY
   Sets the foreground colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).

	RGB values that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetFG(uint8_t r, uint8_t g, uint8_t b)
{
/*	if(((uint16_t)r + (uint16_t)g + (uint16_t)b) >= 384)
		_FGColour = 1;
	else
		_FGColour = 0;*/
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




/* NOT IMPLEMENTED FOR THIS DISPLAY
   Sets the background colour to be used by the graphic and print functions as an RGB value where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).

	RGB values that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
*/
void Display::DSetBG(uint8_t r, uint8_t g, uint8_t b)
{
/*	if(((uint16_t)r + (uint16_t)g + (uint16_t)b) >= 384)
		_BGColour = 1;
	else
		_BGColour = 0;*/
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
	uint8_t ColIndex;
	uint8_t RowIndex;

	for (RowIndex = 0; RowIndex < BUFFERBYTEROWSIZE; RowIndex++)   
		for (ColIndex = 0; ColIndex < BUFFERCOLSIZE; ColIndex++)
			DisplayBuffer[ColIndex][RowIndex] = 0x00;
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
			x = (RES_X - 1) - temp;
			break;
			
		case(SCREEN_R180):
			y = (RES_Y - 1) - y;
			x = (RES_X - 1) - x;
			break;
			
		case(SCREEN_R270):
			temp = y;
			y = (RES_Y - 1) - x;
			x = temp;
			break;
			
		default:
			break;
	}
	
	// Modify the appropriate bit in the display buffer depending on the draw mode.
	if(x >= 0 && x < RES_X && y >= 0 && y < RES_Y)
	{
		uint8_t row = y / 8;
	
		switch(_DrawMode)
		{	
			// Draw mode is normal so set the bit
			case(DRAWMODE_NORMAL):
				if(_FGColour)
					DisplayBuffer[x][row] |=  (0x01 << (y % 8));
				else
					DisplayBuffer[x][row] &=  ~(0x01 << (y % 8));
				break;
	
			// Draw mode is XOR so invert the bit if FG = 1
			case(DRAWMODE_XOR):
				if(_FGColour)
					DisplayBuffer[x][row] ^=  (0x01 << (y % 8));	
				break;
			
			// Draw mode is AND invert so invert the bit of FG = 1, or set it if FG = 0
			case(DRAWMODE_AND_INV):
				DisplayBuffer[x][row] &=  ~(0x01 << (y % 8));

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
		
		DRAWMODE_XOR		Pixels are logically XOR'd with the pixels currently drawn on the screen.
		
		DRAWMODE_AND_INV 	Pixels are logically AND'd and inverted with the pixels currently drawn on the screen.
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
	int16_t x = _X1Pos;
	
	for(uint16_t i = 0; i < (Cols * ByteRows); i += ByteRows)
	{
		for(uint16_t Scale = 0; Scale < _ScaleX; Scale++)
		{
			if(Background)
				_DWriteCol(x, _Y1Pos, ByteRows, &BitmapData[i]);
			else
				_DPlotCol(x, _Y1Pos, ByteRows, &BitmapData[i]);
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
	
	// Set a pointer to the starting address for the characters bitmap 
	//FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]); 
	
	// Use the draw bitmap function to write the character bitmap to the screen
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background); 
	
	// If the character is being printed with a background fill in the gaps between each character
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1); 
	
	// Move the cursor to the end of the printed character to it is in the correct position should we with to print another character
	DSetXY(_X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos); 
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
				if((pgm_read_byte_near(&Data[ByteRows - dr]) >> i) & 1)
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
				if((pgm_read_byte_near(&Data[ByteRows - dr]) >>  i) & 1)
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




/*  NOT APPLICABLE FOR THIS DISPLAY
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
	// Modify the x & y coordinates depending on the orientation of the screen
	switch(_FlipMode)
	{	
		// Modify the x & y coordinates depending on the orientation of the screen
		case(SCREEN_NORMAL):
			break;
			
		case(SCREEN_R90):
			int16_t temp;
			temp = y;
			y = x;
			x = (RES_X - 1) - temp;
			break;
			
		case(SCREEN_R180):
			y = (RES_Y - 1) - y;
			x = (RES_X - 1) - x;
			break;
			
		case(SCREEN_R270):
			temp = y;
			y = (RES_Y - 1) - x;
			x = temp;
			break;
			
		default:
			break;
	}
	
	// Get the current state of the pixel from the display buffer - Not the screen !
	uint8_t row = y / 8;
	
	if(x < BUFFERCOLSIZE && row < BUFFERBYTEROWSIZE)
		return DisplayBuffer[x][row] & (0x01 << (y % 8));

	return false;
}




/* Returns the resolution of the displays X axis as a 16 bit unsigned number */
uint16_t Display::DResX(void)
{
	if(_FlipMode == SCREEN_R270 ||
	   _FlipMode == SCREEN_R90)
		return RES_Y;
	else
		return RES_X;
}




/* Returns the resolution of the displays Y axis as a 16 bit unsigned number */
uint16_t Display::DResY(void)
{
	if(_FlipMode == SCREEN_R270 ||
	   _FlipMode == SCREEN_R90)
		return RES_X;
	else
		return RES_Y;
}




/* Sends a command byte to the display where:
   Data is the byte of data to send to the command register */
void Display::_Send_Command(uint8_t Command)
{	
	// Write command
	digitalWrite(_dc, LOW); 
	// Shift out the command to the 5110 
	digitalWrite(_ce, LOW);
	shiftOut(_din, _clk, 1, Command);
	digitalWrite(_ce, HIGH);
}




void Display::_Send_Data(uint8_t Data)
{	
	// Write data
	digitalWrite(_dc, HIGH); 
	// Shift out the data to the 5110
	digitalWrite(_ce, LOW);
	shiftOut(_din, _clk, 1, Data);
	digitalWrite(_ce, HIGH);
}




/* TOUCH SCREEN FUNCTIONS ARE NOT SUPPORTED FOR THIS DISPLAY */


/* Constructor to initialise the optional touch library */
Touch::Touch(void)
{
}
