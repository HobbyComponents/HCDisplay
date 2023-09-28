/* FILE:    HCDisplay_Hello_World
   DATE:    22/02/19
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
222/02/19 version 0.1: Original version

A simple hello world sketch to demonstrate the minimum amount of code needed to 
display some text to the display.

2.4 inch TFT LCD Module with Touch Panel ILI9341 240 x 320 (SKU: HCMODU0133)

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
REASON WHATSOEVER. */


/* Arduino pins used to connect to the display and displays touch sensor */
#define DC_PIN  9         //Displays/Data Command pin
#define CS_PIN  10        //Displays Chip Select pin
#define RST_PIN 8         //Displays Reset Pin


#include "HCDisplay.h"

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library


void setup() 
{
  HCDisplay.Init(DC_PIN, CS_PIN, RST_PIN);   //Initialise the display

  HCDisplay.Pos((HCDisplay.ResX() / 2) - 48, HCDisplay.ResY() / 2);
  HCDisplay.Print("Hello World!");
}



void loop() 
{
  
}
