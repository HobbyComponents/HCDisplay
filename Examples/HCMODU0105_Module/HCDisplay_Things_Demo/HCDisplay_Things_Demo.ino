/* File:    HCDisplay_Things_Demo.ino
 * Version: 1.0
 * 
 * The sketch uses the line, rectangle and bitmap graphic functions together 
 * with the INVERT DrawMode to create a simple animation.
 * 
 * IMPORTANT: To use this sketch you must first select your display type
 * by opening up the Options.txt file (found the HCDisplay library folder)
 * in a text editor and uncommenting your display type.
 * 
 * Please see Lincence.txt file in the library folder for terms of use
 */

#include "HCDisplay.h"

// Default pins
#define RST_PIN      3
#define CE_PIN       4
#define DC_PIN       5
#define DIN_PIN      6
#define CLK_PIN      7
#define BL_PIN       8

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library

/* Example bitmap */
const PROGMEM byte Tiny_Logo_Resistor[] =
{
  0x0, 0xC0, 0x1, 0xE0, 0x3, 0x30, 0x3, 0x30, 0x3, 0xF0, 0x1, 0xE0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x3, 0xC0, 0xF, 0x0, 0x3C, 0x0, 0x38, 0x0, 0xE, 0x0, 0x3, 0x80, 0x0, 0xE0, 0x0, 0x38, 0x0, 0xE, 0x0, 0x7, 0x0, 0x1E, 0x0, 0x78, 0x1, 0xE0, 0x7, 0x80, 0x1E, 0x0, 0x38, 0x0, 0x1C, 0x0, 0x7, 0x0, 0x1, 0xC0, 0x0, 0x70, 0x0, 0x1C, 0x0, 0x7, 0x0, 0xF, 0x0, 0x3C, 0x0, 0xF0, 0x3, 0x80, 0xE, 0x0, 0x38, 0x0, 0x38, 0x0, 0xE, 0x0, 0x3, 0x80, 0x0, 0xF0, 0x0, 0x3C, 0x0, 0xF, 0x0, 0x7, 0x0, 0x1C, 0x0, 0x70, 0x1, 0xC0, 0x7, 0x0, 0x1C, 0x0, 0x38, 0x0, 0x1E, 0x0, 0x7, 0x80, 0x1, 0xE0, 0x0, 0x78, 0x0, 0x1E, 0x0, 0x7, 0x0, 0xE, 0x0, 0x38, 0x0, 0xE0, 0x3, 0x80, 0xE, 0x0, 0x38, 0x0, 0x3C, 0x0, 0xF, 0x0, 0x3, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x0, 0xC0, 0x1, 0xE0, 0x3, 0xF0, 0x3, 0x30, 0x3, 0x30, 0x1, 0xE0, 0x0, 0xC0
};


/* Array indexes for the X and Y coordinates */
#define X1 0
#define Y1 1
#define X2 2
#define Y2 3

/* Arrays to store coordinates and direction for the line, rectangle and bitmap */
byte Box[4] = {1,1,20,20};
byte BoxDir[4] = {1,1,1,1};
byte Line[4] = {75,33,24,10};
byte LineDir[4] = {-1,-1,-1,-1};
int Bitmap[2] = {30,30};
int BitmapDir[2] = {-1,-1};



void setup() 
{
  // Initialise the display
  HCDisplay.Init(DIN_PIN, CLK_PIN, RST_PIN, CE_PIN, DC_PIN, BL_PIN);

  // Set the screens contrast a little darker than default - make 
  // moving objects look a bit clearer
  HCDisplay.Contrast(0x50);

  // Turn backlight on
  HCDisplay.Backlight(ON);
}


void loop() 
{
  /* Display some text using a small 8x8 fixed width font */
  HCDisplay.SetFont(SystemFont);
  HCDisplay.Pos(24,0);
  HCDisplay.Print("HOBBY");
  HCDisplay.Pos(2,10);
  HCDisplay.Print("COMPONENTS");

  /* Stretch the last line of text to make it bigger */
  HCDisplay.ScaleXY(1,2);
  HCDisplay.Pos(6,24);
  HCDisplay.Print("HCDISPLAY");
  HCDisplay.ScaleXY(1,1);
 
  /* Change the draw mode from NORMAL to INVERT */
  HCDisplay.DrawMode(DRAWMODE_XOR);

  /* Turn off auto refresh, the sketch will control when to update the display */
  HCDisplay.AutoRefresh(OFF);
  
  while(1)
  {
    /* Move the positions of the 3 objects */
    MoveRect();
    MoveLine();
    MoveBitmap();
    
    /* Draw the objects to the display buffer */
    HCDisplay.Rect(Box[X1],Box[Y1],Box[X2],Box[Y2], FILLED);
    HCDisplay.Line(Line[X1],Line[Y1],Line[X2],Line[Y2]);
    
    HCDisplay.Pos(Bitmap[X1],Bitmap[Y1]);
    HCDisplay.BWBitmap(84, 2, Tiny_Logo_Resistor, false);
    
    /* Write the display buffer to the screen */
    HCDisplay.Refresh();
    
    /* Draw the objects again. As we are in INVERT mode this will remove them */
    HCDisplay.Rect(Box[X1],Box[Y1],Box[X2],Box[Y2], FILLED); 
    HCDisplay.Line(Line[X1],Line[Y1],Line[X2],Line[Y2]);
    
    HCDisplay.Pos(Bitmap[X1],Bitmap[Y1]);
    HCDisplay.BWBitmap(84, 2, Tiny_Logo_Resistor, false); 
  }
}


/* Update the X and Y coordinates for the box */
void MoveRect(void)
{
  if(Box[X1] == 0 || Box[X1] == HCDisplay.ResX())
    BoxDir[X1] *= -1;
  Box[X1] += BoxDir[X1];
  
  if(Box[Y1] == 0 || Box[Y1] == HCDisplay.ResY())
    BoxDir[Y1] *= -1;
  Box[Y1] += BoxDir[Y1];
  
  if(Box[X2] == 0 || Box[X2] == HCDisplay.ResX())
    BoxDir[X2] *= -1;
  Box[X2] += BoxDir[X2];
  
  if(Box[Y2] == 0 || Box[Y2] == HCDisplay.ResY())
    BoxDir[Y2] *= -1;
  Box[Y2] += BoxDir[Y2];   
}

/* Update the X and Y coordinates for the Line */
void MoveLine(void)
{
  if(Line[X1] == 0 || Line[X1] == HCDisplay.ResX())
    LineDir[X1] *= -1;
  Line[X1] += LineDir[X1];
  
  if(Line[Y1] == 0 || Line[Y1] == HCDisplay.ResY())
    LineDir[Y1] *= -1;
  Line[Y1] += LineDir[Y1];
  
  if(Line[X2] == 0 || Line[X2] == HCDisplay.ResX())
    LineDir[X2] *= -1;
  Line[X2] += LineDir[X2];
  
  if(Line[Y2] == 0 || Line[Y2] == HCDisplay.ResY())
    LineDir[Y2] *= -1;
  Line[Y2] += LineDir[Y2];   
}

/* Update the X and Y coordinates for the bitmap */
void MoveBitmap(void)
{
  if(Bitmap[X1] == -50 || Bitmap[X1] == 43)
    BitmapDir[X1] *= -1;
  Bitmap[X1] += BitmapDir[X1];
  
  if(Bitmap[Y1] == -8 || Bitmap[Y1] == 40)
    BitmapDir[Y1] *= -1;
  Bitmap[Y1] += BitmapDir[Y1];
}
