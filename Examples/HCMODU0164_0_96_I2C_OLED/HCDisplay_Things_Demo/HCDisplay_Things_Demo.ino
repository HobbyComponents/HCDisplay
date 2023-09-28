/* File:    HCDisplay_Hello_World.ino
 * Version: 1.0
 * 
 * This is a simple sketch to demonstrate how to print text at specified locations
 * on the display. The sketch also shows how to change font style and 
 * foreground/background colour.
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting your display type.
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */

 
#include "HCDisplay.h"

// Default I2C address
#define I2C_ADD 0x3C

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library

unsigned int MaxX, MaxY;
boolean FGColour = 1;



void setup() 
{
  HCDisplay.Init(I2C_ADD);         

  /* Get the screens X & Y resolution */
  MaxX = HCDisplay.ResX() - 1;
  MaxY = HCDisplay.ResY() - 1;

  /* Draw a boarder */
  HCDisplay.Rect(0 , 0, MaxX, MaxY, OUTLINE, 1);
  HCDisplay.Rect(0 + 3 , 0 + 3, MaxX - 3, MaxY - 3, OUTLINE, 1);

  /* Print some text */
  HCDisplay.Pos(44,8);
  HCDisplay.Print("Hobby");
  HCDisplay.Pos(24,18);
  HCDisplay.Print("Components");

  HCDisplay.Pos(12,30);
  HCDisplay.Print("0.9\" I2C OLED");

  /* Change the font */
  HCDisplay.SetFont(MedProp_12ptFont);
}


void loop() 
{
  /* Make some text flash by swapping the texts foreground and background colours */
  HCDisplay.SetFG(FGColour);
  HCDisplay.SetBG(!FGColour);
  HCDisplay.Pos(9,42);
  HCDisplay.Print("HCMODU0164");

  FGColour = !FGColour;

  delay(200);
}