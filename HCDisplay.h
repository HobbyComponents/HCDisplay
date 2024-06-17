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


#ifndef HCDISPLAY_h
#define HCDISPLAY_h

#include "Arduino.h"
#include "Options.h"

#include "Fonts/Tiny_6pt.h"
#include "Fonts/MedProp_12pt.h"
#include "Fonts/LargeProp_26pt.h"
#include "Fonts/LCDLarge_24pt.h"
#include "Fonts/LCDLarge_52pt.h"
#include "Fonts/NarrowProp24pt.h"


#if defined(__SAM3X8E__)
#include <avr/dtostrf.h>
#endif

#if defined(NOKIA5110)
	#include "hardware/Nokia_5110.h"
#elif defined(ST7920)
	#include "hardware/ST7920.h"
#elif defined(ILI9325_SHIELD)
	#include "hardware/ILI9325_SHIELD.h"
#elif defined(ILI9327_SHIELD)
	#include "hardware/ILI9327_SHIELD.h"
#elif defined(HX8352B_SHIELD)
	#include "hardware/HX8352B_Shield.h"
#elif defined(ILI9341_SPI)
	#include "hardware/ILI9341_SPI.h"
#elif defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
	#include "hardware/ILI9341_SPI_With_TSC2046_Touch_Sensor.h"
#elif defined(MAX7219_DOT_MATRIX)
	#include "hardware/MAX7219_Dot_Matrix.h"
#elif defined(HCMODU0136_HT1621)
	#include "hardware/HCMODU0136_HT1621.h"
#elif defined(SSD1306_128X64_I2C)
	#include "hardware/SSD1306_128X64_I2C.h"
#elif defined(PCD8544)
	#include "hardware/PCD8544.h"
#elif defined(ST7735)
	#include "hardware/ST7735.h"
#elif defined(SSD1306_128X64_SPI)
	#include "hardware/SSD1306_128X64_SPI.h"
#elif defined(SH1106_128X64_SPI)
	#include "hardware/SH1106_128X64_SPI.h"
#elif defined(SSD1306_128x32_I2C)
	#include "hardware/SSD1306_128x32_I2C.h"
#elif defined(ST7565)
	#include "hardware/ST7565.h"	
#elif defined(ST7565_SPI)
	#include "hardware/ST7565_SPI.h"	
#else	
	#error "No display defined! Please select a display type in the Options.h file"
#endif


#define SCREEN_NORMAL 		0
#define SCREEN_FLIP_H_R270 	1
#define SCREEN_FLIP_V_R180 	2
#define SCREEN_R270 		3
#define SCREEN_FLIP_V 		4
#define SCREEN_R90 			5
#define SCREEN_R180 		6
#define SCREEN_FLIP_H_R90 	7

#define OUTLINE 			0
#define FILLED 				1

#define DRAWMODE_NORMAL 	0x00
#define DRAWMODE_XOR		0x01
#define DRAWMODE_AND_INV	0x02
#define DRAWMODE_OR			0x03

#ifndef ON
	#define ON 1
#endif

#ifndef OFF
	#define OFF 0
#endif


#define TOUCH_AXIS_X 0
#define TOUCH_AXIS_Y 1
//#define TOUCH_AXIS_NORMAL 0
//#define TOUCH_AXIS_FLIP 1


#define	TOUCH_0_NORMAL 		0
#define	TOUCH_90_NORMAL 	1
#define	TOUCH_180_NORMAL 	2
#define	TOUCH_270_NORMAL 	3
#define	TOUCH_0_FLIP_X 		4
#define	TOUCH_90_FLIP_X 	5 
#define	TOUCH_180_FLIP_X 	6
#define	TOUCH_270_FLIP_X 	7

#define	TOUCH_0_FLIP_Y 		6
#define	TOUCH_90_FLIP_Y 	7
#define	TOUCH_180_FLIP_Y 	4
#define	TOUCH_270_FLIP_Y 	5
#define	TOUCH_0_FLIP_XY 	2
#define	TOUCH_90_FLIP_XY 	3
#define	TOUCH_180_FLIP_XY 	0
#define	TOUCH_270_FLIP_XY 	1



class HCDisplay : private Display, Touch
{
	public: 
	HCDisplay(void);
	#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD)
	void Init(void);
	#elif defined(ST7920)
	void Init(uint8_t _Pin1);
	void Init(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3);
	#elif defined(ILI9341_SPI) || defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR) || defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HCMODU0136_HT1621)
	void Init(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3);
	#elif defined(MAX7219_DOT_MATRIX)
	void Init(uint8_t CS, uint8_t DriversX, uint8_t DriversY, uint8_t *Buffer);
	#elif defined (SSD1306_128X64_I2C) || defined (SSD1306_128x32_I2C)
	void Init(uint8_t add);
	#elif defined(PCD8544)
	void Init(uint8_t din, uint8_t clk, uint8_t rst, uint8_t ce, uint8_t dc, uint8_t bl = 255);
	#elif defined(ST7735)
	void Init(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t bl = 255);
	#elif defined(SSD1306_128X64_SPI) || defined(SH1106_128X64_SPI)
	void Init(uint8_t cs, uint8_t dc, uint8_t rst = 0xFF);
	#elif defined(ST7565)
	void Init(uint8_t din, uint8_t clk, uint8_t ce, uint8_t dc);
	void Init(uint8_t din, uint8_t clk, uint8_t ce, uint8_t dc, uint8_t rst);
	#elif defined(ST7565_SPI)
	void Init(uint8_t ce, uint8_t dc);
	void Init(uint8_t ce, uint8_t dc, uint8_t rst);
	#endif
	void Reset(void);
	void Sleep(boolean mode);
	void Screen(boolean mode);
	void Contrast(uint8_t level);
	void Backlight(uint8_t mode);
	void Flip(uint8_t mode = 0);
	void SetFG(uint8_t r, uint8_t g, uint8_t b); 
	void SetFG(boolean colour);
	void SetBG(uint8_t r, uint8_t g, uint8_t b);
	void SetBG(boolean colour);
	void AutoRefresh(boolean Mode);
	void Refresh(void);
	void Clear(void);
	void Erase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2/*, boolean Update = true*/);
	void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2/*, boolean update = true*/);
	void Plot(uint16_t x, uint16_t y);
	void DrawMode(uint8_t Mode);
	void Rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, boolean mode, uint16_t width = 1);
	void Circle(int32_t x1, int32_t y1, int16_t r, boolean mode, int16_t width = 1);
	void Pos(int16_t X, int16_t Y = 0);
	void ScaleXY(uint8_t ScaleX, uint8_t ScaleY);
	void BWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t BitmapData[], boolean Background = true);
	void SetFont(const FONT_INFO *NewFont);
	void WriteChar(char character);
	void Print(const char *TextString, boolean Background = true);
	void Print(char *TextString, boolean Background = true);
	//void Print(uint8_t *TextString, boolean Background = true);
	void Print(float Value, uint8_t DP = 0, boolean Background = true);
	void ReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data);
	boolean ReadPixel(uint16_t x, uint16_t y);
	uint16_t ResX(void);
	uint16_t ResY(void);
	
	#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD)
	void TouchInit(uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping);
	#elif defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
	void TouchInit(uint8_t DIN, uint8_t DOUT, uint8_t CLK, uint8_t CS, uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping = TOUCH_0_NORMAL);
	#endif
	#if defined(ILI9325_SHIELD) || defined(ILI9327_SHIELD) || defined(HX8352B_SHIELD) || defined(ILI9341_SPI_WITH_TSC2046_TOUCH_SENSOR)
	uint16_t ReadRaw(boolean Axis);
	void ReadTouch(void);
	uint16_t GetCoord(boolean Axis);
	boolean Pressed(void);
	#endif
	
	boolean _AutoRefresh = ON;
	
  
	private:
};

#endif