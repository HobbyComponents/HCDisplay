/* File:    HCDisplay_Hello_World.ino
 * Version: 1.0
 * 
 * This is a simple sketch to demonstrate how to print text at specified locations
 * on the display. The sketch also shows how to change font style.
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting your display type.
 * 
 * For the HCMODU0118 & 119 this will be SSD1306_128x32_I2C
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */

 
#include "HCDisplay.h"

// Default I2C address
#define I2C_ADD 0x3C

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library


void setup() 
{
  HCDisplay.Init(I2C_ADD);         

  // Print some text
  HCDisplay.Pos(44,0);
  HCDisplay.Print("Hobby");
  HCDisplay.Pos(24,10);
  HCDisplay.Print("Components");

  // Change the font
  HCDisplay.SetFont(Tiny_6ptFont);
  HCDisplay.Pos(12,24);
  HCDisplay.Print("HCMODU0118 & HCMODU0119");
}


void loop() 
{

}
