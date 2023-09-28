/* FILE:    SH1106_128X64_SPI.cpp
   DATE:    05/07/23
   VERSION: 1.0
   AUTHOR:  Andrew Davies
   
05/07/23 version 1.0: Original version

This library adds hardware support to the HCDisplay library for SH1106 based screens.
Current supported boards:


1.3" SPI 128x64 OLED SH1106 - White (SKU: HCMODU0058) & Blue (HCMODU0059). 
Available for purchase form our store here:

https://hobbycomponents.com/displays/621-3-sh1106-uoled-display-module-white
https://hobbycomponents.com/displays/622-3-sh1106-uoled-display-module-white


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
#include "hardware/SH1106_128X64_SPI.h"
#include "Arduino.h"
#include <SPI.h>

/* Display command register addresses */
#define COLLOWADD 0x00
#define COLHIGHADD 0x10
#define MEMORYADDRESSMODE 0x20
#define SETCOLADDRESS 0x21
#define SETPAGEADDRESS 0x22
#define SETCONTRAST 0x81
#define CHARGEPUMP 0x8D
#define SETMUXRATIO 0xA8
#define DISPLAYONADD 0xAE
#define PAGEADD 0xB0
#define STARTLINEADD 0x40
#define SCANDIRECTIONADD 0xC0
#define SEGMENTMAPADD 0xA0

/* Command register parameters */
#define SCANDIRNORMAL 0x00
#define SCANDIRREVERSE 0x08
#define SEGMENTMAPNORMAL 0
#define SEGMENTMAPREVERSE 1
#define ENABLECHARGEPUMP 0x14
#define HORIZONTALADDRESSMODE 0x00

/* I2C control byte command / data modes */
#define I2C_COMMAND 0b00000000
#define I2C_DATA 0b01000000

/* Display settings */
#define SH1106_RES_X 128 
#define SH1106_RES_Y 64
#define SH1106_GRAM_COL_START 0x02
#define SH1106_GRAM_COL_END 0x7F
#define SH1106_GRAM_PAGE_START 0
#define SH1106_GRAM_PAGE_END 7
#define SH1106_RAM_PAGES  (SH1106_GRAM_PAGE_END - SH1106_GRAM_PAGE_START + 1)

/* Constructor to initialise the display library */
Display::Display(void)
{
	
}




/* Initialises the display and configures the serial and control pins where:
	
	DC is the digital pin for the Data/Command control pin.
	CS is the digital pin for the chip select control pin.
	RST is the digital pin for the reset control pin.
*/
void Display::DInit(uint8_t cs, uint8_t dc, uint8_t rst)
{
	// Set the slave select (CS), Data/Command mode (DC) and reset (RST) pins to outputs 
	_cs = cs;
	_dc = dc;
	_rst = rst;
	
	pinMode(_cs, OUTPUT);
	pinMode(_dc, OUTPUT);
	
	if(_rst != 0xFF)
	{
		pinMode(_rst, OUTPUT);
		DReset();
	}
	
	/* Enable the hardware SPI interface */
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	
	
	_Send_Command(SETMUXRATIO); //Set MUX ratio
	_Send_Command(0x3F);
	
	_Send_Command(CHARGEPUMP); //Enable charge pump
	_Send_Command(ENABLECHARGEPUMP);
   
	// Set memory addressing mode to horizontal addressing
	_Send_Command(MEMORYADDRESSMODE);
	_Send_Command(HORIZONTALADDRESSMODE);
	
	
	_FlipMode = SCREEN_NORMAL;
	_DrawMode = DRAWMODE_NORMAL;
	
	// Initialise library settings
	_Init();
}




/* Initialises various library settings */
void Display::_Init(void)
{
	/* Set the font to the standard system font */
	DSetFont(SystemFont);
	
	/* Put the cursor in the home position */
	DSetXY(0, 0);

	/* Turn on the display */
	DScreen(ON);


	/* Clear the buffer and update the display */
	DClear();

	/* Set drawing mode to normal */
	DDrawMode(DRAWMODE_NORMAL);
	
	/* Set screen orientation to normal */
	DFlip(SCREEN_NORMAL);	
	
	/* Set contrast level to maximum */
	DContrast(255);
}




/* Writes the entire contents of the display buffer to the display */
void Display::DUpdateDisplay(void)
{
	uint8_t ColIndex;
	uint8_t RowIndex;   
	uint8_t Temp1, Temp2;
     
	for (RowIndex = 0; RowIndex < SH1106_RAM_PAGES; RowIndex++)
	{
		_Send_Command(PAGEADD | RowIndex);
		_Send_Command(COLLOWADD | SH1106_GRAM_COL_START); /* Lower column address */
		_Send_Command(COLHIGHADD | 0x0); /* Higher column address */
	
		/* Write to display RAM */ 
		ColIndex = 0;
		do
		{
			// I2C buffer can only hold 32 bytes so write data to graphics memory in 32 byte chunks
			for(byte i = 0; i < 31 && ColIndex < SH1106_RES_X; i++)
			{
				digitalWrite(_dc, HIGH); 
				digitalWrite(_cs, LOW);
				SPI.transfer(DisplayBuffer[ColIndex][RowIndex]);
				digitalWrite(_cs, HIGH);
					
				ColIndex++;
			}
		}while(ColIndex < SH1106_RES_X);
	}
}




/* NOT IMPLEMENTED FOR THIS DISPLAY 
   Toggles the reset pin to perform a hardware reset of the display. */
void Display::DReset(void)
{
	/* Reset the module */
	if(_rst != 0xFF)
	{
		digitalWrite(_rst, LOW);
		delay(1);
		digitalWrite(_rst, HIGH);
	}
 
	/* Wait 100mS for DC-DC to stabilise. This can probably be reduced */
	delay(100);
}




/*  NOT IMPLEMENTED FOR THIS DISPLAY 
    Places the screen into or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{

}




/* Turns the screen on or off where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	if(mode == ON)
		_Send_Command(DISPLAYONADD | 1);
	else
		_Send_Command(DISPLAYONADD);
}




/* Sets the screens contrast level where:
	level is the required contrast level. Valid values for level are
		0 	(min contrast)
		255 (max contrast)
*/
void Display::DContrast(uint8_t level)
{
	_Send_Command(SETCONTRAST);
	_Send_Command(level);
}




/* NOT APPLICABLE FOR THIS SCREEN
   Sets the state of the backlight */
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

	RGB values that when summed together are >= 384 (3 x 128) will be drawn in the foreground colour, values < 384 will be drawn in the background colour.
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
	uint8_t ColIndex;
	uint8_t RowIndex;

	for (RowIndex = 0; RowIndex < 8; RowIndex++)   
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
		uint8_t row = y / BUFFERROWSIZE;
	
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
				/*if(_FGColour)
					DisplayBuffer[x][row] ^=  (0x01 << (y % 8));
				else
					DisplayBuffer[x][row] |= (0x01 << (y % 8));
				break;*/
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
   
   Cols is the number byte columns to write to.
   
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
	FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]); 
	
	/* Use the draw bitmap function to write the character bitmap to the screen */
	DBWBitmap(BitmapWidth, Bitmapbyteheight, &Font->p_character_bitmaps[BitmapIndex], Background); 
	
	/* If the character is being printed with a background fill in the gaps between each character */
	if(Background)
		DErase(_X1Pos + (BitmapWidth * _ScaleX), _Y1Pos, _X1Pos + ((BitmapWidth + Font->char_spacing) * _ScaleX), _Y1Pos + ((Bitmapbyteheight * 8) * _ScaleY) - 1); 
	
	/* Move the cursor to the end of the printed character to it is in the correct position should we with to print another character */
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
void Display::_DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t Data[])
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
	uint8_t row = y / BUFFERROWSIZE;
	
	if(x < BUFFERCOLSIZE && row < BUFFERROWSIZE)
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
void Display::_Send_Command(byte Data)
{	
		digitalWrite(_dc, LOW); /* Write to command registers */
		digitalWrite(_cs, LOW);
		SPI.transfer(Data);
		digitalWrite(_cs, HIGH);
}







/* TOUCH SCREEN FUNCTIONS ARE NOT SUPPORTED FOR THIS DISPLAY */


/* Constructor to initialise the optional touch library */
Touch::Touch(void)
{
}
