#include "HCDisplay.h"
#include "hardware/ILI9340C.h"
#include "Arduino.h"
#include <SPI.h>


/* Constructor to initiliase the I2C library*/
Display::Display(void)
{
}

/* Initialises the display and configures the SPI and control pins where:
	
	DC is the digital pin for the Data/Command control pin.
	CS is the digital pin for the chip select control pin.
	RST is the digital pin for the reset control pin.
*/
void Display::DInit(uint8_t DC, uint8_t CS, uint8_t RST)
{
	/* Configure the control pins */
	pinMode(DC, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(RST, OUTPUT);

	_DCP =  portOutputRegister(digitalPinToPort(DC));
	_CSP =  portOutputRegister(digitalPinToPort(CS));
	_RSTP =  portOutputRegister(digitalPinToPort(RST));
	
	_DCM =  digitalPinToBitMask(DC);
	_CSM =  digitalPinToBitMask(CS);
	_RSTM =  digitalPinToBitMask(RST);
	
	
	/* Enable the hardware SPI interface */
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);

	/* Reset the screen */
	DReset();

	/* Bring it our of its sleep state */
	DSleep(false);

	/*Set_pixel_format to 16 bit RGB (RRRRRGGGGGGBBBBB) format */
	_CS_EN;
	_WriteCommand(PIXSET); 	
	_WriteData(0x55); 		

	/* Set screen orientation to normal position */
	DFlip(SCREEN_NORMAL);
	
	/* Turn the screen on */
	DScreen(ON);
	
	/* Set the font to the standard system font */
	DSetFont(SystemFont);
	
	/* Put the cursor in the home position */
	DSetXY(0, 0);
}


/* Toggles the reset pin to perform a hardware reset of the display */
void Display::DReset(void)
{
	_RST_LOW;
	delay(1);
	_RST_HIGH;
	delay(5);
}


/* Places the screen in to or out of sleep mode where:
	mode is the required state. Valid values area
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void Display::DSleep(boolean mode)
{
	_CS_EN;
	
	if(mode)
		_WriteCommand(SPLIN);
	else
		_WriteCommand(SLPOUT); //Exit sleep mode
	
	_CS_DIS;
	delay(120);
}


/* Turn the screen on or off  where:
	mode is the required state. Valid values area
		ON (screen on)
		OFF (screen off)
*/
void Display::DScreen(boolean mode)
{
	_CS_EN;
	
	if(mode)
		_WriteCommand(DISPON);	//Screen on
	else
		_WriteCommand(DISPOFF); //Screen off
	
	_CS_DIS;
}


/* Sets the state of the backlight. NOT APPLICABLE FOR THIS SCREEN */
void Display::DBacklight(boolean mode)
{
}


/* Sets the screen orientation and write direction where:
	mode is the direction, orientation to set the screen to. Valid vales are:
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
	_WriteCommand(MADCTL); 
	_WriteData(((mode & 0x07) << 5) | 0b00001000);
	
	if(mode == SCREEN_FLIP_H_R270 ||
	   mode == SCREEN_R270 ||
	   mode == SCREEN_R90 ||
	   mode == SCREEN_FLIP_H_R90)
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



/* Sets the foreground colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
*/
void Display::DSetFG(byte r, byte g, byte b)
{
	_FGColour = ((r<<11) | ((g<<5) & 0x07E0) | (b & 0x1F));
}



/* Sets the background colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
	g is the intensity for the green components. Valid values are 0 to 255 (note for this screen this is scaled down to 6 bits).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for this screen this is scaled down to 5 bits).
*/
void Display::DSetBG(byte r, byte g, byte b)
{
	_BGColour = ((r<<11) | ((g<<5) & 0x07E0) | (b & 0x1F));
}



/* Erases the entire screen by drawing a rectangle using the current background colour */
void Display::DErase(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
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
void Display::DPlot(unsigned int x, unsigned int y)
{
	if(x < _res_x && y < _res_y)
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
void Display::DRect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	_CS_EN;
	_SetWriteArea(x1, y1, x2, y2);

	_WriteCommand(RAMWR); //Write to RAM */
	
	for(unsigned int i = x1; i <= x2; i++)
	for(unsigned int i = y1; i <= y2; i++)
	{
		_WriteData(_FGColour >> 8);
		_WriteData(_FGColour);
	}
	_CS_DIS;
}



/* Set an X/Y coordinate for the bitmap and print functions where:
	x is the x axis coordinate.
	y is the y axis coordinate.
*/
void Display::DSetXY(unsigned int X, unsigned int Y)
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
void Display::DBWBitmap(int Cols, byte ByteRows, const byte BitmapData[], boolean Background)
{
	_CS_EN;
		uint16_t x = _X1Pos;
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
   You can enable more fonts by editiing the options.h file found in the HCLibrary folder.
*/
void Display::DSetFont(const FONT_INFO *NewFont)
{
	Font = NewFont;
}



/* Write a single character or digit at the current cursor coordinate where:
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
	//const byte *FontByteAddress;
	
	/* Set a pointer to the startign address for the characters bitmap */
	//FontByteAddress = pgm_read_byte_near(&Font->p_character_bitmaps[BitmapIndex]);
	
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
	y is the x axis coordinate of the pixel 
	Data is a 3 byte array used to store the RGB values 
*/
void Display::DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
	_CS_EN;
	_SetWriteArea(x, y, x, y);

	_WriteCommand(RAMRD); //Read from RAM */

	SPI.transfer(0); //Dummy write
	
	Data[0] = SPI.transfer(0) >> 3;	// Read the red component
	Data[1] = SPI.transfer(0) >> 2;	// Read the green component
	Data[2] = SPI.transfer(0) >> 3;	// Read the blue component
	_CS_DIS;
}



/* Checks if the pixel at the specified coordinate is set to the foreground colour where:
	x is the x axis coordinate of the pixel
	y is the x axis coordinate of the pixel 
	
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





/* Internal function that sets the area of the display to write to */
void Display::_SetWriteArea(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2)
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
	

	_WriteCommand(0x2a); 
	_WriteData(Y1>>8);
	_WriteData(Y1);
	_WriteData(Y2>>8);
	_WriteData(Y2);
	_WriteCommand(0x2b); 
	_WriteData(X1>>8);
	_WriteData(X1);
	_WriteData(X2>>8);
	_WriteData(X2);
	_WriteCommand(RAMWR); //Write to RAM 			
}


/* Internal function writes an 8 bit command byte to the display */
void Display::_WriteCommand(byte command)
{
	_DC_COMMAND;
	SPI.transfer(command);
}


/* Internal function writes an 9 bit data parameter byte to the display */
void Display::_WriteData(byte data)
{
	_DC_DATA;
	SPI.transfer(data);	
}



/* Internal function that writes a one pixel column of bitmap data by using the screens burst write function. This is slightly faster but also results in background being written. Where:
	x is the x axis coordinate of the top of the bitmap.
	y is the y axis coordinate of the top of the bitmap.
	ByteRows is the number of pixels in bytes of the bitmap.
	Data[] is a constant array containing the bitmap data to print.
 */
void Display::_DWriteCol(uint16_t x, uint16_t y, uint8_t ByteRows, const char Data[])
{

	if(x < _res_x)
	{	
		_SetWriteArea(x, y, x, y + (ByteRows * 8 * _ScaleY) - 1);

		for(uint8_t i = ByteRows; i > 0; i--)
		{
			for(uint8_t bit = 0; bit < 8; bit++)
			{
			
				for(uint8_t Scale = 0; Scale <	_ScaleY; Scale++)
				{
					//if(y >= _res_y)
						//break;
					if(y < _res_y)
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
void Display::_DPlotCol(uint16_t x, uint16_t y, uint8_t ByteRows, const char Data[])
{
	if(x < _res_x)
	{
		for(uint8_t i = ByteRows; i > 0; i--)
		{
			for(byte bit = 0; bit < 8; bit++)
			{				
				if(y < _res_y)
				{		
					uint16_t y2 = y + _ScaleY - 1;
					if(y2 >= _res_y)
						y2 = _res_y - 1;		
				
					if((pgm_read_byte_near(&Data[i - 1]) >> bit) & 0x01)
						DRect(x, y, x, y2);
					
					y = y2 + 1;
				}				
			}
		}
	}
}