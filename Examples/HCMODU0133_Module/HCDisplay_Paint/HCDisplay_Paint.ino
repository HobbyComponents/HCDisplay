/* FILE:    HCDisplay_Paint
   DATE:    20/12/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This example sketch makes use of the screens resistive touch sensor and the HCDisplay 
libraries touch sensor functions to allow you to draw on the screen using different
colours and pen sizes. This sketch supports the following display:

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


#include "HCDisplay.h"

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library


/*******************************************************************************************************/
/**************************************** USER SETTINGS ************************************************/
/*******************************************************************************************************/

/* Arduino pins used to connect to the display and displays touch sensor */
#define DC    9         //Displays Data/Command pin
#define CS    10        //Displays Chip Select pin
#define RST   8         //Displays Reset Pin

#define TOUCH_DIN   3   //Touch sensors Data in pin
#define TOUCH_DOUT  4   //Touch sensors Data out pin
#define TOUCH_CLK   5   //Touch sensors Clock pin
#define TOUCH_CS    6   //Touch sensors Chip Select pin



/* Min and max values for the touch sensor. Change these values if your touch sensor is not calibrated correctly.
   You can use the calibration sketch found within this library to obtain new values for your screen. */

#define X_RAW_MIN 22
#define X_RAW_MAX 485
#define Y_RAW_MIN 48
#define Y_RAW_MAX 493


/* If your screen has a noisy touch sensor you can add more averaging by increasing the value below */
#define AVERAGING 10


/*******************************************************************************************************/



/* Defines used for selecting the appropriate colour in the palette array */
#define R 0
#define G 1
#define B 2
#define RGB(x) Palette[x][R], Palette[x][G], Palette[x][B]

byte MenuWidth = 60;                  //With of the menu which will be displayed down the left hand side of the screen
byte ColWidth = MenuWidth / 2;
int RowHeight;

byte PenSize = 1;                     //Holds the current pen size

/* Array which holds the colour pallet */
byte Palette[][3] = {{0x00,0x00,0xFF},      //Blue
                     {0x00,0xFF,0x00},      //Green
                     {0x00,0xFF,0xFF},      //Cyan
                     {0xFF,0x00,0x00},      //Red
                     {0xFF,0x00,0xFF},      //Purple
                     {0xFF,0xFF,0x00},      //Yellow
                     {0x00,0x00,0x00},      //White
                     {0xFF,0xFF,0xFF}};     //Black


void setup() 
{
  HCDisplay.Init(DC, CS, RST);   //Initialise the display

  HCDisplay.TouchInit(TOUCH_DIN, TOUCH_DOUT, TOUCH_CLK, TOUCH_CS, X_RAW_MIN, X_RAW_MAX, Y_RAW_MIN, Y_RAW_MAX, TOUCH_270_NORMAL); //Initialise the touch sensor

  RowHeight = HCDisplay.ResY() / 6; //Sets the hight of each menu row

  DrawMenu(); //Draws the colour/pen menu down the left side of the screen
}



void loop() 
{
  byte Ave = 0;
  long x = 0;
  long y = 0;

  /* If the screen is pressed take an averaged measurement of the touch sensor */
  while(HCDisplay.Pressed() && Ave < AVERAGING) 
  {
    HCDisplay.ReadTouch();    //Make a measurement of the touch sensor
    
    x +=  HCDisplay.GetCoord(TOUCH_AXIS_X);
    y +=  HCDisplay.GetCoord(TOUCH_AXIS_Y);
  
    Ave++;
  }

  /* If the screen was pressed long enough to take an averaged measurement then do something based 
     on where the screen was pressed. */
  if(Ave == AVERAGING)
  {
    x /= Ave;
    y /= Ave;
    
    if(x > MenuWidth) //Has the screen been pressed in the drawing area?
    {
      if(x > MenuWidth + PenSize) //If so then draw something but make sure the pen doesn't draw over the menu.
        HCDisplay.Circle(x, y, PenSize, FILLED);
    }else //If not then a menu option has been selected so check which one.
    {
      ProcessMenu(x, y);   
    }
  }
}



/*Changes the current pen colour or pen size depending on where in 
 the menu area the screen has been pressed. */
void ProcessMenu(int x, int y)
{
  if(y < RowHeight)
    if(x < ColWidth)
      HCDisplay.SetFG(RGB(0));
    else
      HCDisplay.SetFG(RGB(1));
  else if(y < RowHeight * 2)
    if(x < ColWidth)
      HCDisplay.SetFG(RGB(2));
    else
      HCDisplay.SetFG(RGB(3));
  else if(y < RowHeight * 3)
    if(x < ColWidth)
      HCDisplay.SetFG(RGB(4));
    else
      HCDisplay.SetFG(RGB(5));
  else if(y < RowHeight * 4)
    if(x < ColWidth)
      HCDisplay.SetFG(RGB(6));
    else
      HCDisplay.SetFG(RGB(7));
  else if(y < RowHeight * 5)
    if(x < ColWidth)
      PenSize = 1;
    else
      PenSize = 4;
    else if(y < RowHeight * 6)
      if(x < ColWidth)
        PenSize = 8;
      else
        PenSize = 16;
}


/* Draws the pen colour/size menu down the left side of the screen */
void DrawMenu()
{
  for(byte i = 0; i < 4; i++)
  {
    HCDisplay.SetFG(RGB(i*2));
    HCDisplay.Rect(0, RowHeight * i, ColWidth, RowHeight * (i+1), FILLED);

    HCDisplay.SetFG(RGB(i*2+1));
    HCDisplay.Rect(ColWidth, RowHeight * i, MenuWidth, RowHeight * (i+1), FILLED);
  }


  HCDisplay.SetFG(255,255,255);
  HCDisplay.Rect(0, RowHeight * 4, ColWidth, RowHeight * 5, OUTLINE);
  HCDisplay.Rect(ColWidth, RowHeight * 4, MenuWidth, RowHeight * 5, OUTLINE);
  HCDisplay.Rect(0, RowHeight * 5, ColWidth, RowHeight * 6, OUTLINE);
  HCDisplay.Rect(ColWidth, RowHeight * 5, MenuWidth, RowHeight * 6, OUTLINE);

  HCDisplay.Circle(ColWidth / 2, (RowHeight * 4) + (RowHeight / 2), 1 , FILLED);
  HCDisplay.Circle(ColWidth + (ColWidth / 2), (RowHeight * 4) + (RowHeight / 2), 4, FILLED);
  HCDisplay.Circle(ColWidth / 2, (RowHeight * 5) + (RowHeight / 2), 8 , FILLED);
  HCDisplay.Circle(ColWidth + (ColWidth / 2), (RowHeight * 5) + (RowHeight / 2), 16, FILLED);
}
