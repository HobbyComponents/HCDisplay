/* FILE:    HCDisplay_Screen_Calibration
   DATE:    05/01/19
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This example sketch can be used to obtain raw min & max x/y reading from the screens
resistive touch sensor. Theses measurements can then be passed to the HCDisplay.TouchInit()
function to calibrate the sensor. This sketch supports the following shields:

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

To use this sketch upload it to your Arduino and then open up the serial monitor window.
Next, touch the display in the extreme 4 corners of the screen. The sketch will record the 
minimum and maximum raw values obtained from the touch sensor and output them to the serial 
monitor as a set of defines. E.g.

#define X_RAW_MIN 157
#define X_RAW_MAX 850
#define Y_RAW_MIN 105
#define Y_RAW_MAX 866

These defines can then be pasted into your sketch and then passed to the HCDisplay.TouchInit()
function as follows:

HCDisplay.TouchInit(X_RAW_MIN, X_RAW_MAX, Y_RAW_MIN, Y_RAW_MAX, TOUCH_0_NORMAL);

Note that often with resistive type touch sensors your screen sensor may be less accurate in
the extreme edges of the screen and therefore it may not be possible to accurate calibrate
the sensor up to its edges.

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


/* Some default values to initialise the touch sensor with */
#define X_RAW_MIN 157
#define X_RAW_MAX 850
#define Y_RAW_MIN 105
#define Y_RAW_MAX 866


unsigned int XMin = 65535, YMin = 65535, XMax = 0, YMax = 0;
int x = 0, y = 0;


void setup() 
{
  Serial.begin(9600);
  
  HCDisplay.Init();   //Initialise the display

  HCDisplay.TouchInit(X_RAW_MIN, X_RAW_MAX, Y_RAW_MIN, Y_RAW_MAX, TOUCH_0_NORMAL); //Initialise the touch sensor
}


void loop() 
{

  byte Ave = 0;
  unsigned long AveX = 0, AveY = 0; 

  /* If the screen is pressed take an averaged measurement of the touch sensor */
  while(HCDisplay.Pressed() && Ave < 100)
  {
    AveX += HCDisplay.ReadRaw(TOUCH_AXIS_X);
    AveY += HCDisplay.ReadRaw(TOUCH_AXIS_Y);

    Ave++;
  }


  /* If the screen was pressed long enough to take an averaged measurement then do something. */  
  if(Ave == 100)
  {
    /* Calculate the average raw values */
    AveX /= 100;
    AveY /= 100;

    /* Store the min and max X and Y raw values */
    if(AveX < XMin)
      XMin = AveX;

    if(AveX > XMax)
      XMax = AveX;

    if(AveY < YMin)
      YMin = AveY;

    if(AveY > YMax)
      YMax = AveY;

    HCDisplay.TouchInit(XMin, XMax, YMin, YMax, TOUCH_0_NORMAL); //Initialise the touch sensor


   /* Draw the cross-hair based on newly calibrated coordinates */
    HCDisplay.SetFG(0,0,0);

    HCDisplay.Line(x - 5, y, x + 5, y);
    HCDisplay.Line(x, y - 5, x, y + 5);
    
    HCDisplay.ReadTouch();
    x = HCDisplay.GetCoord(TOUCH_AXIS_X);
    y = HCDisplay.GetCoord(TOUCH_AXIS_Y);
    
    HCDisplay.SetFG(255,255,255);
    HCDisplay.Line(x - 5, y, x + 5, y);
    HCDisplay.Line(x, y - 5, x, y + 5);

    /* Output new raw values to the serial monitor */
    Serial.print("#define X_RAW_MIN ");
    Serial.println(XMin);
    Serial.print("#define X_RAW_MAX ");
    Serial.println(XMax);
    Serial.print("#define Y_RAW_MIN ");
    Serial.println(YMin);
    Serial.print("#define Y_RAW_MAX ");
    Serial.println(YMax);
    Serial.println(); 
  }
}
