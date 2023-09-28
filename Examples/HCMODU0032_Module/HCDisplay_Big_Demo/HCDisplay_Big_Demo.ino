/* FILE:    HCDisplay_Big_Demo
   DATE:    20/12/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This example sketch demonstrates various features of the HCDisplay library including text 
with different fonts, graphic functions & screen rotation. This sketch has been written 
to work with the following module:

12864 128x64 Pixel Graphic Display module ST7920 Controller (HCMODU0032)

The library support both hardware as software serial interfaces for this display.
To use the hardware (SPI) interface connect the display to your UNO/NANO as follows:

MODULE..........................UNO
K (backlight cathode).........GND
A (backlight anode)..........+5V
PSD (SPI Mode).................GND (SEE NOTE)
E (SCK)..........................D13
R/W (MOSI)....................D11
RS (CS)..........................D10*
VDD.............................+5V
VSS..............................GND

And initialise the library with the following function: HCDisplay.Init(10); 

For software serial you can use any digital pin for E (SLK), R/W (MOSI), & RS (CS).
Initialise the library for software serial module with the following function:
HCDisplay.Init(RS PIN, E PIN, R/W PIN);

*Some versions of this LCD are shipped with the PSB (parallel/serial select) pin 
 shorted to VDD by a 0 ohm resistor on the back of the display.

NOTE: TO USE THIS SKETCH YOU MUST FIRST SELECT THE VERSION OF DISPLAY YOU HAVE BY 
UNCOMMENTING THE APPROPRIATE LINE IN THE OPTIONS.H FILE WHICH CAN BE FOUND IN THE 
HCDISPLAY LIBRARY FOLDER. For windows users avoid using the Windows Notepad editor 
as it doesn't format things properly.  

More information about this library can be found in the software section of our support 
forum here:

http://forum.hobbycomponents.com/software


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

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library


/*******************************************************************************************************/
/**************************************** USER SETTINGS ************************************************/
/*******************************************************************************************************/

/* Arduino pins used to connect to the display and displays touch sensor */
#define RS      10        //Displays Chip Select pin
#define E       13        //Displays clock pin
#define RW      11        //Displays data pin

/*******************************************************************************************************/



unsigned int ResX, ResY;

/*Example bitmap */
const PROGMEM uint8_t Tiny_Resistor_Bitmap[] =
{
  0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xE7, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x78, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xE7, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 
};



void setup() 
{
  HCDisplay.Init(RS);         //Initialise the display (hardware serial mode - faster but must use hardware SPI pins)
  //HCDisplay.Init(RS, E, RW); //Initialise the display (software serial mode - slower but can use any digital pins)

  /* Get the screens X & Y resolution */
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();
}


void loop() 
{
  
  /* Display an intro */
  HCDisplay.Clear();
  Intro();   

  /* Print random hellos with the screen in its normal orientation */ 
  HCDisplay.Flip(SCREEN_NORMAL);
  RandomHello();                

  /* Print random hellos with the screen rotated 90 degrees */ 
  HCDisplay.Flip(SCREEN_R90);
  RandomHello();

  /* Print random hellos with the screen rotated 180 degrees */ 
  HCDisplay.Flip(SCREEN_R180);
  RandomHello();

  /* Print random hellos with the screen rotated 270 degrees */ 
  HCDisplay.Flip(SCREEN_R270);
  RandomHello();

  /* Print random circles */ 
  HCDisplay.Flip(SCREEN_NORMAL);
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();
 
  for(byte i = 0; i < 5; i++)
    RandomCircles();

  /* Print random rectangles */ 
  for(byte i = 0; i < 10; i++)
    RandomRectangles();

  /* Print random bitmaps */ 
  for(byte i = 0; i < 5; i++)
    RandomBitmap();

  /* Print random lines */ 
  for(int i = 0; i < 10; i++)
    RandomLines();

  /* Display a series of patterns */
  HCDisplay.Clear();
  Patterns();
}


void Intro()
{
  for(byte Scale = 1; Scale < 4; Scale++)
  {
    PrintCentre("Hello!", Scale, ResY / 2 - (Scale * 8 / 2));
    delay(500); 
  }

  HCDisplay.Clear();

  PrintCentre("Hobby Components", 1, (ResY / 2) - 30);
  HCDisplay.SetFG(0); HCDisplay.SetBG(1);
  PrintCentre("HCDisplay", 1,  (ResY / 2) - 20);
  HCDisplay.SetFG(1); HCDisplay.SetBG(0);
  PrintCentre("Library Demo", 1,  (ResY / 2) - 10);

  HCDisplay.SetFont(LCDLarge_24ptFont);
  
  for(int8_t i = 10; i >= 0; i--)
  {
    HCDisplay.Pos(60, 30);
    HCDisplay.Print(i, 0);
    delay(1000);
    HCDisplay.SetFG(0);
    HCDisplay.Pos(60, 30);
    HCDisplay.Print(i, 0);
    HCDisplay.SetFG(1);
  }

  HCDisplay.SetFont(SystemFont);
}



void PrintCentre(char *string, byte Scale, byte y)
{
  HCDisplay.ScaleXY(Scale, Scale);
  HCDisplay.Pos((ResX / 2) - ((strlen(string) * (8* Scale)) / 2), y); 
  HCDisplay.Print(string);
}



void RandomHello()
{
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();

  HCDisplay.Clear();
  
  for(int i = 0; i < 100; i++)
  {
    //Scaling the random number helps make it a bit more random
    byte FG = random(0,255); 
    FG >>= 7;

    HCDisplay.Pos(random(-50, ResX), random(-50, ResY));
    HCDisplay.ScaleXY(random(1,4), random(1,4));
    HCDisplay.SetFG(FG);
    HCDisplay.SetBG(1 - FG);
    HCDisplay.Print("Hello!"); 
  }

  HCDisplay.SetFG(1);
  HCDisplay.SetBG(0);
  HCDisplay.ScaleXY(1, 1);
}



void RandomCircles()
{
  HCDisplay.Clear();
  HCDisplay.DrawMode(DRAWMODE_XOR);
  
  for(int i = 0; i < 10; i++)
  {
    byte Radius = random(1, 40);
    byte Width = random (1, 20);

    HCDisplay.Circle(random(0, ResX), random(0, ResY), Radius, OUTLINE, Width); 
  }
  
  HCDisplay.DrawMode(DRAWMODE_NORMAL);
}



void RandomRectangles()
{
  HCDisplay.Clear();
  HCDisplay.DrawMode(DRAWMODE_XOR);
  
  for(int i = 0; i < 10; i++)
  { 
    byte Width = random (1, 5);
    boolean Mode;

    if(random(0,10) >= 5)
      Mode = FILLED;
    else
      Mode = OUTLINE;

    HCDisplay.Rect(random(0, ResX), random(0, ResY), random(0, ResX), random(0, ResY), Mode, Width); 
  }

  HCDisplay.DrawMode(DRAWMODE_NORMAL);
}



void RandomBitmap()
{
  HCDisplay.Clear();
  HCDisplay.DrawMode(DRAWMODE_XOR);
  
  for(int i = 0; i < 20; i++)
  { 
    HCDisplay.Pos(random(-100, ResX), random(-100, ResY));
    HCDisplay.BWBitmap(127, 3, Tiny_Resistor_Bitmap, false);
  }

  HCDisplay.DrawMode(DRAWMODE_NORMAL);
}



void RandomLines()
{
  HCDisplay.Clear();
  
  for(byte i = 0; i < 20; i++)
    HCDisplay.Line(random(0, ResX), random(0, ResY), random(0, ResX), random(0, ResY)); 
}



void Patterns()
{
  HCDisplay.Rect(0, 0, ResX - 1, ResY - 1, OUTLINE, 1); 
  
  for(int x = 0; x < ResX; x += 8)
    HCDisplay.Line(x, 0, x, ResY - 1);

  for(int y = 0; y < ResY; y += 8)
    HCDisplay.Line(0, y, ResX - 1, y);

  delay(2000);


  HCDisplay.Clear();

  for(int x = 0; x < ResX; x += 8)
    HCDisplay.Line(0, 0, x, ResY - 1);

  for(int x = 0; x < ResX; x += 8)
    HCDisplay.Line(0, ResY - 1, x, 0);

  for(int x = 0; x < ResX; x += 8)
    HCDisplay.Line(ResX - 1, ResY - 1, x, 0);

  for(int x = 0; x < ResX; x += 8)
    HCDisplay.Line(ResX - 1, 0, x, ResY - 1);

  delay(2000);

  HCDisplay.Clear();
  for(int y = 0; y < ResY + 8; y += 8)
    for(int x = 0; x < ResX + 8; x += 8)
    {
      HCDisplay.SetFG(1);
      HCDisplay.Circle(x, y, 4, OUTLINE, 2); 
    }
  
  delay(2000);
}