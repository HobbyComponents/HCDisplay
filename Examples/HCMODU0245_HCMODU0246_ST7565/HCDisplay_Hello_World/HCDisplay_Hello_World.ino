/* File:    HCDisplay_Hello_World.ino
 * Version: 1.0
 * 
 * This is a simple sketch to demonstrate how to print text at specified locations
 * on the display. The sketch also shows how to change font style and 
 * foreground/background colour.
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting the line:
 * 
 * #define ST7565
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */

 
#include "HCDisplay.h"

// Default pins
#define CE_PIN      A0
#define DC_PIN      A1
#define CLK_PIN     A2
#define DIN_PIN     A3
#define RST_PIN     2  // Optional reset pin

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library

unsigned int MaxX, MaxY;
boolean FGColour = 1;



void setup() 
{
  // Initialise the display - Note HCMODU0246 must be reset after powering up!
  HCDisplay.Init(DIN_PIN, CLK_PIN, CE_PIN, DC_PIN, RST_PIN);  // Software SPI option - slower but more compatible
  //HCDisplay.Init(CE_PIN, DC_PIN, RST_PIN);                  // Hardware SPI option - faster but must use the hardware SPI pins for clock and data

  // Uncomment if screen contrast needs adjusting (contrast = 0 to 255)
  // HCDisplay.Contrast(16);

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

  HCDisplay.Pos(32,30);
  HCDisplay.Print("2.8\" LCD");

  /* Change the font */
  HCDisplay.SetFont(MedProp_12ptFont);
}


void loop() 
{
  /* Make some text flash by swapping the texts foreground and background colours */
  HCDisplay.SetFG(FGColour);
  HCDisplay.SetBG(!FGColour);
  HCDisplay.Pos(8,42);
  HCDisplay.Print("HCMODU0245");

  FGColour = !FGColour;

  delay(200);
}