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

// Default pins
#define CS_PIN      10
#define DC_PIN      9
#define RST_PIN     8

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library

unsigned int MaxX, MaxY;
boolean FGColour = 1;



void setup() 
{
  // Initialise the display 
  HCDisplay.Init(CS_PIN, DC_PIN, RST_PIN);   //Initialise the display

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

  HCDisplay.Pos(14,30);
  HCDisplay.Print("1.3\" I2C OLED");

  /* Change the font */
  HCDisplay.SetFont(MedProp_12ptFont);
}


void loop() 
{
  /* Make some text flash by swapping the texts foreground and background colours */
  HCDisplay.SetFG(FGColour);
  HCDisplay.SetBG(!FGColour);
  HCDisplay.Pos(10,42);
  HCDisplay.Print("HCMODU0158");

  FGColour = !FGColour;

  delay(200);
}
