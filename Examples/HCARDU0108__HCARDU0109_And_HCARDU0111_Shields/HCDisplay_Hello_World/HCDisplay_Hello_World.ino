/* FILE:    HCDisplay_Hello_World
   DATE:    22/02/19
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

A simple hello world sketch to demonstrate the minimum amount of code needed to 
display some text to the display.

Open-Smart 2.4 Inch TFT shield (SKU: HCARDU0108)
Open-Smart 3.2 Inch TFT shield with HX8352B controller (SKU: HCARDU0109)
Open-Smart 3.2 Inch TFT shield with ILI9327 controller (SKU: HCARDU0111)

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


#include "HCDisplay.h"

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library


void setup() 
{
  HCDisplay.Init();   //Initialise the display

  HCDisplay.Pos((HCDisplay.ResX() / 2) - 48, HCDisplay.ResY() / 2);
  HCDisplay.Print("Hello World!");
}



void loop() 
{
  
}
