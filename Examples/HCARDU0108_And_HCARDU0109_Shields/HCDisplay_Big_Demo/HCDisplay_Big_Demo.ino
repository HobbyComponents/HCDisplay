/* FILE:    HCDisplay_Big_Demo
   DATE:    20/12/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This example sketch demonstrates various features of the HCDisplay library including text 
with different fonts, graphic functions & screen rotation. This sketch has been written 
to work with the following module:

Open-Smart 2.4 Inch TFT shield (SKU: HCARDU0108)
Open-Smart 3.2 Inch TFT shield (SKU: HCARDU0109)

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

unsigned int ResX, ResY;

/*Example bitmap */
const PROGMEM uint8_t Tiny_Resistor_Bitmap[] =
{
  0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xE7, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x78, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xE7, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 
};



void setup() 
{
  HCDisplay.Init();   //Initialise the display

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
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomHello();                

  /* Print random hellos with the screen rotated 90 degrees */ 
  HCDisplay.Flip(SCREEN_R90);
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomHello();

  /* Print random hellos with the screen rotated 180 degrees */ 
  HCDisplay.Flip(SCREEN_R180);
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomHello();

  /* Print random hellos with the screen rotated 270 degrees */ 
  HCDisplay.Flip(SCREEN_R270);
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomHello();

  /* Print random circles */ 
  HCDisplay.Flip(SCREEN_NORMAL);
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();
  HCDisplay.SetBG(255,255,255);
  HCDisplay.Clear();
  for(int i = 0; i < 100; i++)
    RandomCircles();

  /* Print random rectangles */ 
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomRectangles();

  /* Print random bitmaps */ 
  HCDisplay.Clear();
  for(int i = 0; i < 200; i++)
    RandomBitmap();

  /* Print random lines */ 
  HCDisplay.Clear();
  HCDisplay.SetBG(0,0,0);
  for(int i = 0; i < 500; i++)
    RandomLines();

  /* Display a series of patterns */
  HCDisplay.Clear();
  Patterns();
}


void Intro()
{
  HCDisplay.SetFG(255,255,255);
  for(byte Scale = 1; Scale < 8; Scale++)
  {
    PrintCentre("Hello!", Scale, ResY / 2 - (Scale * 8 / 2));
    delay(500); 
  }

  HCDisplay.Clear();

  PrintCentre("Hobby Components", 2, (ResY / 2) - 68);
  PrintCentre("HCDisplay", 4,  (ResY / 2) - 36);
  PrintCentre("Library Demo", 2,  (ResY / 2) + 12);

  HCDisplay.SetFont(LCDLarge_24ptFont);
  
  for(int8_t i = 10; i >= 0; i--)
  {
    HCDisplay.Erase(ResX / 2, 160, ResX / 2 + 40, 220);
    HCDisplay.Pos(ResX / 2, 160);
    HCDisplay.Print(i, 0);
    delay(1000);
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
  byte R = random(0,255), G = random(0,255), B = random(0,255);
  HCDisplay.Pos(random(-100, ResX), random(-100, ResY));
  HCDisplay.ScaleXY(random(1,8), random(1,8));
  HCDisplay.SetFG(R,G,B);
  HCDisplay.SetBG(255-R,255-G,255-B);
  HCDisplay.Print("Hello!"); 
}



void RandomCircles()
{
  byte R = random(0,255), G = random(0,255), B = random(0,255);
  byte Radius = random(1, 100);
  byte Width = random (1, 50);

  HCDisplay.SetFG(R,G,B);
  HCDisplay.Circle(random(0, ResX), random(0, ResY), Radius, OUTLINE, Width); 
}



void RandomRectangles()
{
  byte R = random(0,255), G = random(0,255), B = random(0,255);
  byte Width = random (1, 10);
  boolean Mode;

  if(random(0,10) >= 5)
    Mode = FILLED;
  else
    Mode = OUTLINE;

  HCDisplay.SetFG(R,G,B);
  HCDisplay.Rect(random(-100, ResX), random(-100, ResY), random(-100, ResX), random(-100, ResY), Mode, Width); 
}



void RandomBitmap()
{
  byte R = random(0,255), G = random(0,255), B = random(0,255);
  HCDisplay.Pos(random(-100, ResX), random(-100, ResY));
  HCDisplay.ScaleXY(random(1,8), random(1,8));
  HCDisplay.SetFG(R,G,B);
  HCDisplay.SetBG(255-R,255-G,255-B);
  HCDisplay.BWBitmap(127, 3, Tiny_Resistor_Bitmap, false);
}



void RandomLines()
{
  byte R = random(0,255), G = random(0,255), B = random(0,255);

  HCDisplay.SetFG(R,G,B);
  HCDisplay.Line(random(0, ResX), random(0, ResY), random(0, ResX), random(0, ResY)); 
}



void Patterns()
{
  HCDisplay.SetFG(0,255,0);
  HCDisplay.Rect(0, 0, ResX - 1, ResY - 1, OUTLINE, 1); 
  
  for(int x = 0; x < ResX; x += 10)
    HCDisplay.Line(x, 0, x, ResY - 1);

  for(int y = 0; y < ResY; y += 10)
    HCDisplay.Line(0, y, ResX - 1, y);

  delay(2000);


  HCDisplay.Clear();

  HCDisplay.SetFG(255,0,0);
  for(int x = 0; x < ResX; x += 10)
    HCDisplay.Line(0, 0, x, ResY - 1);

  HCDisplay.SetFG(255,255,0);
  for(int x = 0; x < ResX; x += 10)
    HCDisplay.Line(0, ResY - 1, x, 0);

  HCDisplay.SetFG(0,255,0);
  for(int x = 0; x < ResX; x += 10)
    HCDisplay.Line(ResX - 1, ResY - 1, x, 0);

  HCDisplay.SetFG(0,0,255);
  for(int x = 0; x < ResX; x += 10)
    HCDisplay.Line(ResX - 1, 0, x, ResY - 1);

  delay(2000);

  HCDisplay.Clear();
  for(int y = 0; y < ResY + 20; y += 20)
    for(int x = 0; x < ResX + 20; x += 20)
    {
      HCDisplay.SetFG(random(100,255),random(100,255),random(100,255));
      HCDisplay.Circle(x, y, 10, FILLED); 
      HCDisplay.SetFG(random(100,255),random(100,255),random(100,255));
      HCDisplay.Circle(x, y, 10, OUTLINE, 5); 
    }
  
  delay(2000);
}
