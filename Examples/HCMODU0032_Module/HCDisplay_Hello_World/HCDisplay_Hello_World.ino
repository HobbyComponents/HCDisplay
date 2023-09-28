/* FILE:    HCDisplay_Hello_World
   DATE:    20/12/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This is a simple sketch to demonstrates how to print text at specified locations
on the display. The sketch also shows how to change font style and 
foreground/background colour. This sketch has been written to work with the 
following module:

https://hobbycomponents.com/displays/285-12864b-parallel-serial-graphic-lcd-module

The library support both hardware as software serial interfaces for this display.
To use the hardware (SPI) interface connect the display to your UNO/NANO as follows:

MODULE..........................UNO
K (backlight cathode)...........GND
A (backlight anode).............+5V
PSD (SPI Mode)..................GND (SEE NOTE)
E (SCK).........................D13
R/W (MOSI)......................D11
RS (CS).........................D10*
VDD.............................+5V
VSS.............................GND

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

unsigned int MaxX, MaxY;
boolean FGColour = 1;

void setup() 
{
  HCDisplay.Init(RS);         //Initialise the display (hardware serial mode - faster but must use hardware SPI pins)
  //HCDisplay.Init(RS, E, RW); //Initialise the display (software serial mode - slower but can use any digital pins)

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

  HCDisplay.Pos(16,30);
  HCDisplay.Print("128 x 64 LCD");

  /* Change the font */
  HCDisplay.SetFont(MedProp_12ptFont);
}


void loop() 
{
  /* Make some text flash be swapping the texts foreground and background colours */
  HCDisplay.SetFG(FGColour);
  HCDisplay.SetBG(!FGColour);
  HCDisplay.Pos(10,42);
  HCDisplay.Print("HCMODU0032");

  FGColour = !FGColour;

  delay(200);
}
