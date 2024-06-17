/* FILE:    HCDisplay.cpp
   DATE:    12/06/24
   VERSION: 1.2.2
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: 	Original version
05/04/19 version 0.2: 	Updated to support Open-Smart 3.2 Inch TFT shield with touch screen (HCARDU0111)
08/01/21 version 0.2.1: Minor update to font for HT1621 - see HCMODU0136_HT1621.h
22/06/23 version 1.0.0:	Added support for 0.96" SSD1306 I2C displays (HCMODU0164)
						Added support for 0.96" SSD1306 SPI displays (HCMODU0150 & HCMODU0152)
						Added support for 1.3" SH1106 SPI displays (HCMODU0158 & HCMODU0159)
						Added support for 160x128 ST7735 TFT displays (HCMODU0160)
						Added support for 0.9" I2C IIC 128x32 SSD1306 OLED Display (HCMODU0118 & HCMODU0119)
						Added support for serial LCD (Nokia 5110) 84x48 - Blue Backlight (SKU: HCMODU0105)
						Added Tiny_6pt font (thanks to Chris Sharp)
						Added new contrast function
						Changes library folder structure to make it more compatible with Arduino IDE 1.5+
06/03/24 version 1.2.0	Clear function now updates display if autoupdate is enabled.
						Added support for 128x64 ST7565 based LCD displays (HCMODU0245 & HCMODU0246)
17/05/24 version 1.2.1	Fixed issues causing compile errors when compiling for ARM based devices
						Add a hardware SPI option for ST7565 based displays (ST7565_SPI)
12/06/24 version 1.2.2	Updated HX8352B files to work with V1.2.1 fix.

This Arduino library provides print and graphics functions for various compatible displays. 
For a full list of supported displays and details of how to use this library please visit the
forum thread found in the software section of our support forum here:

Current supported boards:

6 Digit 7 Segment LCD Module - Green Backlight (SKU: HCMODU0136)
3.2" TFT shield with touch screen HX8352B (SKU: HCARDU0109)
3.2" TFT shield with touch screen ILI9327 (SKU: HCARDU0111)
2.4" TFT shield with touch screen (SKU: HCARDU0108)
MAX7219 Serial LED dot matrix module - Red (SKU: HCOPTO0014)
12864B Parallel/Serial Graphic LCD Module (SKU: HCMODU0032)
3.2" TFT LCD Module with Touch Panel ILI9341 240 x 320 (SKU: HCMODU0132)
2.4" TFT LCD Module with Touch Panel ILI9341 240 x 320 (SKU: HCMODU0133)
1.8" TFT Full Colour Display - ST7735 (SKU: HCMODU0160)
0.96" I2C / IIC OLED screens (SKU: HCMODU0164)
0.96" SPI 128x64 OLED SSD1306 - White (SKU: HCMODU0050) & Blue (HCMODU0052)
1.3" SPI 128x64 OLED SH1106 - White (SKU: HCMODU0058) & Blue (HCMODU0059)
0.9" I2C IIC 128x32 SSD1306 OLED Display (HCMODU0118 & HCMODU0119)
Serial LCD (Nokia 5110) 84x48 - Blue Backlight (SKU: HCMODU0105)
Low Power 2.4 Inch 128x64 (ST7565) Serial LCD Display  (HCMODU0245)
1.8 Inch 128x64 (ST7565) Serial LCD Display With Backlight (HCMODU0246)

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

#if defined(ST7920)
	#include "hardware/ST7920.h"
	#include "hardware/ST7920.cpp"
#elif defined(ILI9325_SHIELD)
	#include "hardware/ILI9325_SHIELD.h"
	#include "hardware/ILI9325_SHIELD.cpp"
#elif defined(ILI9327_SHIELD)
	#include "hardware/ILI9327_SHIELD.h"
	#include "hardware/ILI9327_SHIELD.cpp"
#elif defined(HX8352B_SHIELD)
	#include "hardware/HX8352B_Shield.h"
	#include "hardware/HX8352B_Shield.cpp"
#elif defined(ILI9341_SPI)
	#include "hardware/ILI9341_SPI.h"
	#include "hardware/ILI9341_SPI.cpp"
#elif defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
	#include "hardware/ILI9341_SPI_With_TSC2046_Touch_Sensor.cpp"
	#include "hardware/ILI9341_SPI_With_TSC2046_Touch_Sensor.h"
#elif defined(MAX7219_DOT_MATRIX)
	#include "hardware/MAX7219_Dot_Matrix.cpp"
	#include "hardware/MAX7219_Dot_Matrix.h"
#elif defined(HCMODU0136_HT1621)
	#include "hardware/HCMODU0136_HT1621.cpp"
	#include "hardware/HCMODU0136_HT1621.h"
#elif defined(SSD1306_128X64_I2C)
	#include "hardware/SSD1306_128X64_I2C.h"
	#include "hardware/SSD1306_128X64_I2C.cpp"
#elif defined(PCD8544)
	#include "hardware/PCD8544.h"
	#include "hardware/PCD8544.cpp"
#elif defined(ST7735)
	#include "hardware/ST7735.h"
	#include "hardware/ST7735.cpp"
#elif defined(SSD1306_128X64_SPI)
	#include "hardware/SSD1306_128X64_SPI.h"
	#include "hardware/SSD1306_128X64_SPI.cpp"
#elif defined(SH1106_128X64_SPI)
	#include "hardware/SH1106_128X64_SPI.h"
	#include "hardware/SH1106_128X64_SPI.cpp"
#elif defined(SSD1306_128x32_I2C)
	#include "hardware/SSD1306_128x32_I2C.h"
	#include "hardware/SSD1306_128x32_I2C.cpp"	
#elif defined(ST7565)
	#include "hardware/ST7565.h"
	#include "hardware/ST7565.cpp"
#elif defined(ST7565_SPI)
	#include "hardware/ST7565_SPI.h"
	#include "hardware/ST7565_SPI.cpp"	
#else
	#error "No display defined! Please select a display type in the Options.h file"
#endif



/* Constructor to initiliase the library*/
HCDisplay::HCDisplay(void)
{
	
}


#if defined(ILI9325_SHIELD) ||defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD)
/* Initialises the display and configures the digital pins */
void HCDisplay::Init(void)
{
	DInit();
}

#elif defined(ST7920)
/* Initialises the display and configures the digital pins */
void HCDisplay::Init(uint8_t _Pin1)
{
	DInit(_Pin1);
}

void HCDisplay::Init(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3)
{
	DInit(_Pin1, _Pin2, _Pin3);
}

#elif defined(ILI9341_SPI) || defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR) || defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HCMODU0136_HT1621)
/* Initialises the display and configures the digital pins */
void HCDisplay::Init(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3)
{
	DInit(_Pin1, _Pin2, _Pin3);
}
#elif defined (MAX7219_DOT_MATRIX)
void HCDisplay::Init(uint8_t CS, uint8_t DriversX, uint8_t DriversY, uint8_t *Buffer)
{
	DInit(CS, DriversX, DriversY, Buffer);
}

#elif defined (SSD1306_128X64_I2C) || defined (SSD1306_128x32_I2C)
void HCDisplay::Init(uint8_t add)
{
	DInit(add);
}

#elif defined (PCD8544)
void HCDisplay::Init(uint8_t din, uint8_t clk, uint8_t rst, uint8_t ce, uint8_t dc, uint8_t bl)
{
	DInit(din, clk, rst, ce, dc, bl);
}

#elif defined (ST7735)
void HCDisplay::Init(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t bl)
{
	DInit(cs, dc, rst, bl);
}

#elif defined (SSD1306_128X64_SPI) || defined(SH1106_128X64_SPI)
void HCDisplay::Init(uint8_t cs, uint8_t dc, uint8_t rst)
{
	DInit(cs, dc, rst);
}

#elif defined (ST7565)
void HCDisplay::Init(uint8_t din, uint8_t clk, uint8_t ce, uint8_t dc, uint8_t rst)
{
	DInit(din, clk, ce, dc, rst);
}

void HCDisplay::Init(uint8_t din, uint8_t clk, uint8_t ce, uint8_t dc)
{
	DInit(din, clk, ce, dc);
}

#elif defined(ST7565_SPI)
void HCDisplay::Init(uint8_t ce, uint8_t dc, uint8_t rst)
{
	DInit(ce, dc, rst);
}

void HCDisplay::Init(uint8_t ce, uint8_t dc)
{
	DInit(ce, dc);
}
#endif



/* Resets the screen */
void HCDisplay::Reset(void)
{
	DReset();
}


/* Places the screen in to or out of sleep mode where:
	mode is the required state. Valid values are
		true (screen will enter sleep state)
		false (screen will wake from sleep state)
*/
void HCDisplay::Sleep(boolean mode)
{
	DSleep(mode);
}


/* Turn the screen on or off  where:
	mode is the required state. Valid values are
		ON (screen on)
		OFF (screen off)
*/
void HCDisplay::Screen(boolean mode)
{
	DScreen(mode);
}


/* Sets the screens contrast level where:
	level is required contrast level. Valid values for level are
		0 	(min contrast)
		255 (max contrast)
*/
void HCDisplay::Contrast(uint8_t mode)
{
	DContrast(mode);
}



/* Sets the state of the backlight where:
	mode is required state. Valid values for mode are
		ON 	(backlight on)
		OFF (backlight off)
*/
void HCDisplay::Backlight(uint8_t mode)
{
	DBacklight(mode);
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
void HCDisplay::Flip(uint8_t mode)
{
	DFlip(mode);
	
#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD) || defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
	/* Pass the screen res to the touch sensor so that it can scale the input from the sensor to the screen res correctly */
	if(mode == SCREEN_NORMAL ||
	   mode == SCREEN_FLIP_V_R180 ||
	   mode == SCREEN_FLIP_V ||
	   mode == SCREEN_R180)
		SetScreenRes(RES_X, RES_Y);
	else
		SetScreenRes(RES_Y, RES_X);
#endif
}


/* Sets the foreground colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
	
	g is the intensity for the green components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
	
	b is the intensity for the blue components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
*/
void HCDisplay::SetFG(uint8_t r, uint8_t g, uint8_t b)
{
	DSetFG(r,g,b);
}

void HCDisplay::SetFG(boolean colour)
{
	DSetFG(colour);
}


/* Sets the background colour to be used by the graphic and print functions where:
	r is the intensity for the red components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
	g is the intensity for the green components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
	b is the intensity for the blue components. Valid values are 0 to 255 (note for screens supporting less colours this value will be automatically scaled down).
*/
void HCDisplay::SetBG(uint8_t r, uint8_t g, uint8_t b)
{
	DSetBG(r,g,b);
}

void HCDisplay::SetBG(boolean colour)
{
	DSetBG(colour);
}


/*Sets the auto screen refresh state. When turned on (default) the display will immediately update when writing text and graphics to the display. When turned off the display will not update until the HCDisplay.Refresh() function is executed. Valid values for Mode are:

	ON (auto refresh is on - default)
	
	OFF (auto refresh is off)
*/
void HCDisplay::AutoRefresh(boolean Mode)
{
	_AutoRefresh = Mode;
}


/* Forces an update of the display. This function should be used when the auto refresh modes is set to OFF. 
*/
void HCDisplay::Refresh(void)
{
	DUpdateDisplay();
}


//Clears the entire contents of the screen
void HCDisplay::Clear(void)
{
	DClear();
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Erases a specified area of the screen by setting it to the background colour where:
	x1 is the x coordinate of the top left corner of the area to be erased.
	y1 is the y coordinate of the top left corner of the area to be erased.
	x2 is the x coordinate of the bottom right corner of the area to be erased.
	y2 is the y coordinate of the bottom right corner of the area to be erased.
	*/
void HCDisplay::Erase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	DErase(x1, y1, x2, y2);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}


/* Draw a line using foreground colour where:
   x1 is the starting X axis coordinate of the line
   y1 is the starting Y axis coordinate of the line
   x2 is the ending X axis coordinate of the line
   y2 is the ending Y axis coordinate of the line */
void HCDisplay::Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	double step;
	int16_t x, y;

	/* If start coordinates are below and to the right of the end coordinate then flip them */
	if((x2 <= x1 && y2 <= y1) || (x2 >= x1 && y1 >= y2) || (x2 >= x1 && y1 >= y2)) 
	//if((x2 < x1 && y2 < y1) || (x2 > x1 && y1 > y2) || (x2 > x1 && y1 > y2)) 
	{
		x2^=x1;
		x1^=x2;
		x2^=x1;
		y2^=y1;
		y1^=y2;
		y2^=y1;
	}

	/* If X axis is wider than Y axis then step though X axis */
	if(((x2-x1) >= (y2-y1)) || ((x1 - x2) >= (y2-y1)))
	{
		step = (double)(y2-y1) / (double)(x2-x1);
	
		if(x2 >= x1 && y2 >= y1)
		{
			
			for(x = x1; x <= x2; x++)
			{
				y = ((x-x1) * step) + y1;
				DPlot(x, y);
				//Serial.print(x); Serial.print(" : "); Serial.println(y); 
			}
		}else
		{
			for(x = x1; x >= x2; x--)
			{
				y = y2 + ((x-x2) * step);

				DPlot(x, y);
			}	
		} 
	}else /* If Y axis is wider than X axis then step though Y axis */
	{
		step = (double)(x2-x1) / (y2-y1);
		
		if(y2 >= y1 && x2 >= x1)
		{
			for(y = y1; y <= y2; y++)
			{
				x = ((y-y1) * step) + x1;
				DPlot(x, y);
			}		
		}else
		{
			for(y = y2; y >= y1; y--)
			{
				x = x2 + ((y-y2) * step);
				DPlot(x, y);
			}
		}
	}
  
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Sets a single pixel to the current foreground colour where:
	x is the x axis coordinate of the pixel.
	y is the y axis coordinate of the pixel. 
*/
void HCDisplay::Plot(uint16_t x, uint16_t y)
{
	DPlot(x, y);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Sets the drawing mode used by the text and graphic functions where:
	Mode is the required drawing mode. Valid values for Mode are:
		DRAWMODE_NORMAL		Pixels are drawn normally
		DRAWMODE_XOR		Pixels are logically XOR'd with existing pixels on the display
		DRAWMODE_OR			Pixels are logically OR'd with existing pixels on the display
		DRAWMODE_AND_INV	Pixels are logically AND'd and inverted with existing pixels on the display
*/

void HCDisplay::DrawMode(uint8_t Mode)
{
	DDrawMode(Mode);
}


/* Draws a rectangle using the current foreground colour where:
	x1 is the x coordinate of the top left corner of the rectangle.
	y1 is the y coordinate of the top left corner of the rectangle.
	x2 is the x coordinate of the bottom right corner of the rectangle.
	y2 is the y coordinate of the bottom right corner of the rectangle.
	mode is the drawing mode. Valid values for mode are
		FILLED (draw as a solid rectangle)
		OUTLINE (draw as an outlined rectangle)
	
	width is width in pixels when drawing in outlined mode.	
*/
void HCDisplay::Rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, boolean mode, uint16_t width)
{
	boolean Temp = _AutoRefresh;
	
	_AutoRefresh = OFF;
	
	/* If first corner is below second corner then flip the coordinates */
	if(y1 > y2)
	{
		y2^=y1;
		y1^=y2;
		y2^=y1;  
	}
  
	if(x1 > x2)
	{
		x2^=x1;
		x1^=x2;
		x2^=x1;  
	}
	
	if(mode == FILLED)
	{
		DRect(x1, y1, x2, y2);
	}else
	{
		for(uint16_t w = 0; w < width; w++)
		{
			Line(x1, y1, x2, y1/*, false*/);
			Line(x2, y1, x2, y2/*, false*/);
			Line(x2, y2, x1, y2/*, false*/);
			Line(x1, y2, x1, y1/*, false*/);	
			
			if(x1 < x2)
				x1++;
			if(x2 > x1)
				x2--;
			if(y1 < y2)
				y1++;
			if(y2 > y1)
				y2--;
		}
	}
	
	_AutoRefresh = Temp;
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Draws a circle using the current foreground colour where:
	x1 is the x coordinate of the center of the circle.
	y1 is the y coordinate of the center of the circle.
	r is the radius in pixels.

	mode is the drawing mode. Valid values for mode are
		FILLED (draw as a solid rectangle)
		OUTLINE (draw as an outlined rectangle)
	
	width is width in pixels when drawing in outlined mode.	
*/
void HCDisplay::Circle(int32_t x1, int32_t y1, int16_t r, boolean mode, int16_t width)
{
	int32_t rr = (int32_t)r * (int32_t)r;
	
	int16_t x = 0;
	
	if(width > r >> 1)
		width = r >>1;
	
	for(x = x - r; x <= r; x++)
	{
		int16_t y = 0;

		for(y = y - r - width; y <= r + width; y++)
		{
			int32_t xx = (int32_t)x * (int32_t)x;
			int32_t yy = (int32_t)y * (int32_t)y;
			
			if(xx < 0)
				xx *= -1;
			
			if(yy < 0)
				yy *= -1;

			if(mode == FILLED)
			{
				if(xx + yy <= rr)
					DPlot(x1 + x, y1 + y);
			}else
			{
				int32_t w = (int32_t)r * (int32_t)width;
				if((xx + yy) >= rr - w && (xx + yy) <= rr + r/*+ w*/)
					DPlot(x1 + x, y1 + y);
			}
		}
	}
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Set an X/Y coordinate for the bitmap and print functions where:
	x is the x axis coordinate.
	y is the y axis coordinate.
*/
void HCDisplay::Pos(int16_t X, int16_t Y)
{
	DSetXY(X, Y);
}



/* Sets the amount of scaling for the bitmap and print functions where:
	ScaleX is the magnitude of scaling in the x axis.
	ScaleY is the magnitude of scaling in the y axis.
	
	A value of 1 for ScaleX or ScaleY will result in no scaling.
	A value of 2 for ScaleX or ScaleY will result the bitmap or font being scaled by a factor of 2 in the appropriate axis.
	A value of n for ScaleX or ScaleY will result the bitmap or font being scaled by a factor of n in the appropriate axis.
*/	
void HCDisplay::ScaleXY(uint8_t ScaleX, uint8_t ScaleY)
{
	DScaleXY(ScaleX, ScaleY);
}



/* Writes a 1D array containing BW bitmap data to the display using the current foreground colour at the current cursor location where:
   Cols is the number of columns in the x direction. I.e the x resolution of the bitmap.
   
   ByteRows is the number of rows in 8 pixel chunks. I.e. the y resolution of the bitmap divided by 8.
   
   BitmapData is an array containing the bitmap data (see below)
   
   Background selects if the character will be printed with a solid or transparent background. Valid values for Background are
		true 	(character is printed with background with a colour set by the SetBG() function.
		false 	(Character is printed without a background).

Bitmap data is stored in the array as 8 bit (byte) values representing one BW 8 pixel column. These 8 bit columns are then stored in the array in the following order:

const uint8_t BitmapData[Row_0_Col_0, Row_0_Col_1,.......Row_0_Col_n, 
						 Row_1_Col_0, Row_1_Col_1,.......Row_1_Col_n,
						 Row_n_Col_0, Row_n_Col_1,.......Row_n_Col_n,];
*/
void HCDisplay::BWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t BitmapData[], boolean Background)
{
	DBWBitmap(Cols, ByteRows, BitmapData, Background);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Sets the current font to use for the print function where:
   Font is the name of the font. Valid values for font are:
   SystemFont 				An 8pt fixed width font (default)
   MedProp_12ptFont			A 12pt proportional font
   LargeProp_26ptFont		A 26pt proportional font
   LCDLarge_24ptFont		A 24pt LCD style font
   LCDLarge_52ptFont		A 52pt LCD style font
   NarrowProp24ptFont		A 24pt narrow proportional font
*/
void HCDisplay::SetFont(const FONT_INFO *NewFont)
{
	DSetFont(NewFont);
}



/* Write a single character to the screen at the current cursor coordinate where:
	character is the ASCII character to print
	
	Background selects if the character will be printer with a solid background. Valid values for Background are
		true 	(character is printed with background with a colour set by the DSetBG() function
		false 	(Character is printed without a background)
*/
void HCDisplay::WriteChar(char character)
{
	DWriteChar(character);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}



/* Prints a string of text to the screen starting at the current cursor coordinate where:
	TextString is a string containing the text to be printed.
	
	Background is optional (default = true) and selects if the character will be printed with a solid or transparent background. Valid values for Background are
		true 	(character is printed with background with a colour set by the SetBG() function
		false 	(Character is printed without a background)

Note - To conserve memory only commonly used ASCII characters ranging from ' ' (ASCII code 0x20) to '~' (ASCII code 0x7E) are printable.
*/
void HCDisplay::Print(const char *TextString, boolean Background)
{
	uint8_t StringLength = strlen(TextString);
	
	
	for(uint8_t i = 0; i < StringLength; i++)
		DWriteChar(TextString[i], Background);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}


void HCDisplay::Print(char *TextString, boolean Background)
{
	uint8_t StringLength = strlen(TextString);
	
	
	for(uint8_t i = 0; i < StringLength; i++)
		DWriteChar(TextString[i], Background);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}

/*void HCDisplay::Print(uint8_t *TextString, boolean Background)
{
	uint8_t StringLength = strlen(TextString);
	
	
	for(uint8_t i = 0; i < StringLength; i++)
		DWriteChar(TextString[i], Background);
	
	if(_AutoRefresh)
		DUpdateDisplay();
}*/



/* Prints a floating point number to the screen starting at the current cursor coordinate where:
   Value is the number to print.

   DP is optional (default = 0) and specifies the number of decimal places to display the floating point number to. 
   
   
   Background is optional (default = true) and selects if the number will be printed with a solid or transparent background. Valid values for Background are
		true 	(character is printed with background with a colour set by the SetBG() function
		false 	(Character is printed without a background) */
void HCDisplay::Print(float Value, uint8_t DP, boolean Background)
{
	char sTemp[15];
	
	Print(dtostrf(Value, 0, DP, sTemp), Background);
}



/* Reads the RGB colour value for a single pixel at the specified coordinate where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	Data is a 3 byte array used to store the RGB values 
*/
void HCDisplay::ReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data)
{
	DReadPixelRGB(x, y, Data);
}



/* Checks if the pixel at the specified coordinate is set to the foreground colour where:
	x is the x axis coordinate of the pixel
	y is the y axis coordinate of the pixel 
	
	Returns a boolean true or false:
		true = pixel is set to current foreground colour.
		false = pixel not is set to current foreground colour.
		
	Use this function for black and white screens.
*/
boolean HCDisplay::ReadPixel(uint16_t x, uint16_t y)
{
	return DReadPixel(x, y);
}



/* Returns the resolution of the displays X axis as a 16 bit unsigned number */
uint16_t HCDisplay::ResX(void)
{
	return DResX();
}


/* Returns the resolution of the displays Y axis as a 16 bit unsigned number */
uint16_t HCDisplay::ResY(void)
{
	return DResY();
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
#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD)
void HCDisplay::TouchInit(uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping)
{
	TInit(Cal_X_Min, Cal_X_Max, Cal_Y_Min, Cal_Y_Max, Mapping);
}

#elif defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
void HCDisplay::TouchInit(uint8_t DIN, uint8_t DOUT, uint8_t CLK, uint8_t CS, uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping)
{
	TInit(DIN, DOUT, CLK, CS, Cal_X_Min, Cal_X_Max, Cal_Y_Min, Cal_Y_Max, Mapping);
}
#endif




#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD) || defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)

/* Triggers a sensor measurement of one of the axis and stores the result where:

	Axis is a boolean value specifying which axis to measure. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
		
	Returns an unsigned int value containing a raw sensor reading for the specified axis.
*/
uint16_t HCDisplay::ReadRaw(boolean Axis)
{
	return TReadRaw(Axis);
}



/* Perform an X & Y axis read of the touch sensor. The results can then be obtained using the GetCoord() function */
void HCDisplay::ReadTouch(void)
{
	TReadTouch();
}



/* Returns the last X or Y axis reading from the touch sensor as a value mapped to the resolution of the screen where:

	Axis is a boolean value specifying which axis reading to return. Valid values for Axis are:

		TOUCH_AXIS_X - Specifies the X axis
		TOUCH_AXIS_Y - Specifies the Y axis	
*/
uint16_t HCDisplay::GetCoord(boolean Axis)
{
	return TGetCoord(Axis);
}



/* Checks to see if the touch sensor is currently pressed.

	Returns a boolean value where:
	
	true = screen is currently pressed.
	false = screen is not currently pressed.
*/
boolean HCDisplay::Pressed(void)
{
	return TPressed();
}

#endif