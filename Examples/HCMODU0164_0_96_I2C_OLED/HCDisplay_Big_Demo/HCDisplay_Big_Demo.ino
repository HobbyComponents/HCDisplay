/* File:    HCDisplay_Big_Demo.ino
 * Version: 1.0
 * 
 * This sketch will demonstrates  various features of the HCDisplay library 
 * for the 0.96" SSD1306 I2C OLED display (HCMODU0164)
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting your display type.
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */


#include "HCDisplay.h"

HCDisplay HCDisplay;    // Creates an instance of the HCDisplay library

#define I2C_ADD 0x3C    // Default I2C address of the display


unsigned int ResX, ResY;

/*Example bitmap */
const PROGMEM uint8_t Tiny_Resistor_Bitmap[] =
{
  0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xE7, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x78, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xF0, 0x00, 0x0F, 0xC0, 0x00, 0x3F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xE0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x0F, 0x80, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x1F, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xC3, 0x00, 0x01, 0xC3, 0x80, 0x01, 0xC3, 0x80, 0x00, 0xC3, 0x00, 0x00, 0xE7, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 
};



void setup() 
{
  // Initialise the display 
  HCDisplay.Init(I2C_ADD);

  // Get the screens X & Y resolution
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();
}


void loop() 
{
  // Display an intro
  HCDisplay.Clear();
  Intro();   

  // Scroll some text inside a box
  ScrollInABox();
  delay(1000);

  // Print random hellos with the screen in its normal orientation
  HCDisplay.Flip(SCREEN_NORMAL);
  RandomHello();                

  // Print random hellos with the screen rotated 90 degrees 
  HCDisplay.Flip(SCREEN_R90);
  RandomHello();

  // Print random hellos with the screen rotated 180 degrees
  HCDisplay.Flip(SCREEN_R180);
  RandomHello();

  // Print random hellos with the screen rotated 270 degrees
  HCDisplay.Flip(SCREEN_R270);
  RandomHello();

  // Print random circles
  HCDisplay.Flip(SCREEN_NORMAL);
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();
  for(byte i = 0; i < 25; i++)
    RandomCircles();

  // Print random rectangles
  for(byte i = 0; i < 50; i++)
    RandomRectangles();

  // Print random bitmaps
  for(byte i = 0; i < 5; i++)
    RandomBitmap();

  // Print random lines
  for(int i = 0; i < 10; i++)
    RandomLines();

  // Display a series of patterns
  HCDisplay.Clear();
  Patterns();

  delay(2000);
}


void Intro()
{
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

  HCDisplay.Clear();
  
  for(byte Scale = 1; Scale < 8; Scale++)
  {
    PrintCentre("Hello!", Scale, ResY / 2 - (Scale * 8 / 2));
    delay(200); 
  }

  delay(2000);
}



void PrintCentre(char *string, byte Scale, byte y)
{
  HCDisplay.ScaleXY(Scale, Scale);
  HCDisplay.Pos((ResX / 2) - ((strlen(string) * (8* Scale)) / 2), y); 
  HCDisplay.Print(string);
}



void ScrollInABox()
{
  HCDisplay.SetFont(MedProp_12ptFont);
  HCDisplay.ScaleXY(1, 2);

  HCDisplay.AutoRefresh(OFF);
  
  for(int x = 90; x > -298; x -= 2 )
  {
    HCDisplay.Rect(40, 10, 87, 53, FILLED);

    HCDisplay.Pos(x, 16); 
    HCDisplay.DrawMode(DRAWMODE_AND_INV);
    HCDisplay.Print("HCDisplay - 0.9\" I2C OLED (HCMODU0164) ", false);
    HCDisplay.Refresh();
    HCDisplay.Clear();
    HCDisplay.DrawMode(DRAWMODE_NORMAL);
  }

  HCDisplay.SetFont(SystemFont);
  HCDisplay.AutoRefresh(ON);
}



void RandomHello()
{
  ResX = HCDisplay.ResX();
  ResY = HCDisplay.ResY();

  HCDisplay.Clear();
  
  for(int i = 0; i < 200; i++)
  {
    //Scaling the random number helps make it a bit more random
    byte FG = random(0,255); 
    FG >>= 7;

    HCDisplay.Pos(random(-10, ResX), random(-10, ResY));
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
  for(int y = 0; y < ResY + 16; y += 16)
    for(int x = 0; x < ResX + 16; x += 16)
    {
      HCDisplay.SetFG(1);
      HCDisplay.Circle(x, y, 8, OUTLINE, 2); 
    }
  
  delay(2000);
}
