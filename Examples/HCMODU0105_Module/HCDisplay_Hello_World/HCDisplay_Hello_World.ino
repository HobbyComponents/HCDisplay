/* File:    HCDisplay_Hello_World.ino
 * Version: 1.0
 * 
 * A simple sketch to print some text to the display
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting your display type.
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */


#include "HCDisplay.h"

// Default I2C address
#define RST_PIN      3
#define CE_PIN       4
#define DC_PIN       5
#define DIN_PIN      6
#define CLK_PIN      7
#define BL_PIN       8

// Creates an instance of the HCDisplay library
HCDisplay HCDisplay;    


void setup() 
{
  // Initialise the display
  HCDisplay.Init(DIN_PIN, CLK_PIN, RST_PIN, CE_PIN, DC_PIN, BL_PIN);
  HCDisplay.Backlight(ON);
  
  // Print some text
  HCDisplay.Pos(22,4);
  HCDisplay.Print("HOBBY");
  
  HCDisplay.Pos(2,14);
  HCDisplay.Print("COMPONENTS");
  
  HCDisplay.SetFont(Tiny_6ptFont);
  HCDisplay.Pos(8,30);
  HCDisplay.Print("PCD8544 DISPLAY");
  
  HCDisplay.Pos(20,40);
  HCDisplay.Print("HCMODU0105");
}

void loop() 
{

}
