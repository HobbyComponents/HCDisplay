/* FILE:    HCMODU0136_HT1621.h
   DATE:    19/10/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version

This library adds hardware support to the HCDisplay library for HT1621 based displays. Current supported boards:

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

#ifndef HCMODU0136_HT1621_h
#define HCMODU0136_HT1621_h
#include "Arduino.h"

#include "fonts/fonts.h"

#define _CS_EN 		*_CSP &= ~_CSM
#define _CS_DIS 	*_CSP |= _CSM

#define _WR_LOW 	*_WRP &= ~_WRM
#define _WR_HIGH 	*_WRP |= _WRM

#define _DATA_LOW 	*_DAP &= ~_DAM
#define _DATA_HIGH 	*_DAP |= _DAM

#define _WR_PULSE {_WR_LOW; _WR_HIGH;}


/* HT1621 register addresses */
#define  COMMODE	0x52  
#define  ROSC      	0x30  
#define  LCDON     	0x06  
#define  LCDOFF    	0x04  
#define  SYSEN      0x02  




// Character bitmaps for the default system font
const PROGMEM  uint8_t SystemFontBitmaps[] = {0x0, //  
										0xA0, // !
										0x21, // "
										0x65, // #
										0x1B, // $
										0x59, // %
										0x3F, // &
										0x01, // '
										0x1D, // (
										0x78, // )
										0x33, // *
										0x07, // +
										0x48, // ,
										0x02, // -
										0x80, // .
										0x26, // /
										0x7D, // 0
										0x60, // 1
										0x3E, // 2
										0x7A, // 3
										0x63, // 4
										0x5B, // 5
										0x5F, // 6
										0x70, // 7
										0x7F, // 8
										0x7B, // 9
										0x0A, // :
										0x4A, // ;
										0x13, // <
										0x12, // =
										0x32, // >
										0x36, // ?
										0x7E, // @
										0x77, // A
										0x4F, // B
										0x1D, // C
										0x6E, // D
										0x1F, // E
										0x17, // F
										0x5D, // G
										0x67, // H
										0x60, // I
										0x68, // J
										0x57, // K
										0x0D, // L
										0x54, // M
										0x75, // N
										0x7D, // O
										0x37, // P
										0x73, // Q
										0x35, // R
										0x5B, // S
										0x0F, // T
										0x6D, // U
										0x69, // V
										0x29, // W
										0x67, // X
										0x6B, // Y
										0x3A, // Z
										0x1D, // [
										0x43, // Backslash
										0x78, // ]
										0x31, // ^
										0x08, // _
										0x01, // `
										0x77, // a
										0x4F, // b
										0x0E, // c
										0x6E, // d
										0x1F, // e
										0x17, // f
										0x5D, // g
										0x67, // h
										0x60, // i
										0x68, // j
										0x57, // k
										0x0D, // l
										0x54, // m
										0x75, // n
										0x7D, // o
										0x37, // p
										0x73, // q
										0x35, // r
										0x5B, // s
										0x0F, // t
										0x6D, // u
										0x69, // v
										0x29, // w
										0x67, // x
										0x6B, // y
										0x3A}; // z
      



class Display
{
  public:
  Display(void);
  void DInit(uint8_t _Pin1, uint8_t _Pin2, uint8_t _Pin3);
  void DUpdateDisplay(void);
  void DReset(void);
  void DSleep(boolean mode);
  void DScreen(boolean mode);
  void DBacklight(uint8_t mode);
  void DFlip(uint8_t mode);
  void DSetFG(uint8_t r, uint8_t g, uint8_t b);
  void DSetFG(boolean colour);
  void DSetBG(uint8_t r, uint8_t g, uint8_t b);
  void DSetBG(boolean colour);
  void DClear(void);
  void DErase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void DPlot(uint16_t x, uint16_t y);
  void DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void DDrawMode(uint8_t Mode);
  void DSetXY(int16_t X, int16_t Y = 0);
  void DScaleXY(uint8_t ScaleX, uint8_t ScaleY);
  void DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t *BitmapData, boolean Background = true);
  void DSetFont(const FONT_INFO *NewFont);
  void DWriteChar(char character, boolean Background = true);
  void DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data);
  boolean DReadPixel(uint16_t x, uint16_t y);
  uint16_t DResX(void);
  uint16_t DResY(void);
  void DContrast(uint8_t level);
  
  
  private:
  void _DWriteCommand(uint8_t Add);
  void _DWriteBuffer(void);
 
  uint8_t _CS, _WR, _DATA;
  volatile uint8_t *_CSP, *_WRP, *_DAP;
  uint8_t _CSM, _WRM, _DAM;
  uint8_t _Buffer[6], _DPLast = false, _DrawMode;
  int16_t _XPos;
 
};



class Touch
{
	public:
	Touch(void);
	void TInit(uint8_t DIN, uint8_t DOUT, uint8_t CLK, uint8_t CS, uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping);
	uint16_t TReadRaw(boolean Axis);
	boolean TPressed(void);
	void TReadTouch(void);
	uint16_t TGetCoord(boolean Axis);
	void SetScreenRes(uint16_t Res_X, uint16_t Res_Y);
	
	
	private:
	
};

#endif