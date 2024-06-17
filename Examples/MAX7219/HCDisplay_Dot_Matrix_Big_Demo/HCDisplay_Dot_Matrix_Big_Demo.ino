/* FILE:    HCDisplay_Big_Demo
   DATE:    12/06/24
   VERSION: 1.0
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version
12/06/24 version 1.0: Fixed compile error with print function

This example sketch demonstrates some of the HCDisplay features when used with
MAX7219 dot matrix display modules. This includes printing text, numbers, and 
graphic and bitmap functions. This sketch supports the following displays:

MAX7219 Serial LED dot matrix module - Red (SKU: HCOPTO0014).
8x8 LED Square Dot Matrix Module (Red) (SKU: HCOPTO0014).

Connect your module(s) to you Arduino as follows:

MODULE............UNO/NANO
GND...............GND
VCC...............5V
DIN...............11 (MOSI)
CLK...............13 (SCK)
CS................10 (SS)

NOTE: TO USE THIS SKETCH YOU MUST FIRST SELECT THE VERSION OF DISPLAY YOU HAVE BY 
UNCOMMENTING THE APPROPRIATE LINE IN THE OPTIONS.H FILE WHICH CAN BE FOUND IN THE 
HCDISPLAY LIBRARY FOLDER. For windows users avoid using the Windows Notepad editor 
as it doesn't format things properly. 

If you have more than one module connected you must tell the sketch what 
configuration you have them connected by changing the values for DRIVERS_X &
DRIVERS_Y in the sketch below.

More information about this library can be found in the software section of our 
support forum here:

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


/*******************************************************************************************************/
/**************************************** USER SETTINGS ************************************************/
/*******************************************************************************************************/

#define CS 10         	//Digital pin used for the modules chip select (CS)

#define DRIVERS_X  1   	//Number of modules daisy-chained in the X direction
#define DRIVERS_Y  1   	//Number of modules daisy-chained in the Y direction

/*******************************************************************************************************/


#define RES_X (DRIVERS_X * 8)  //Resolution in pixels in the x direction
#define RES_Y (DRIVERS_Y * 8)  //Resolution in pixels in the y direction

#define LINE 0
#define RECT 1
#define X1 0
#define Y1 1
#define X2 2
#define Y2 3

//Example bitmaps
const PROGMEM byte PM1[] = {0x2C,0x6E,0xEF,0xEF,0xFF,0xFB,0x7E,0x3C,0x00};
const PROGMEM byte PM2[] = {0x00,0x42,0xE7,0xFF,0xFF,0xFB,0x7E,0x3C,0x00};

uint8_t Buffer[RES_X * DRIVERS_Y]; //Display buffer used by the HCDisplay library

HCDisplay HCDisplay;    //Creates an instance of the HCDisplay library



void setup() 
{
  HCDisplay.Init(CS, DRIVERS_X, DRIVERS_Y, Buffer);  //Initialise the display
}


void loop() 
{
  CountDown();
  ScrollText("Hobby Components HCDisplay Library");
  Bounce();
  Circles();
  Bitmap();
  Life();
}


/* Countdown to start the demo. Demonstrates printing integers. */
void CountDown(void)
{
  boolean FGColour = 1;

  HCDisplay.Clear();

  // Countdown form 9 to 0
  for(int i = 9; i >= 0; i--)
  {
    HCDisplay.Pos((RES_X - 8) / 2,( RES_Y - 8) / 2);
    HCDisplay.Print(i);
    delay(1000);
  }

  // Flash an astrix
  for(int i = 0; i < 19; i ++)
  {
    HCDisplay.SetBG(!FGColour);
    HCDisplay.SetFG(FGColour);
    HCDisplay.Pos((RES_X - 8) / 2,( RES_Y - 8) / 2);
    HCDisplay.Print("*");

    FGColour = !FGColour;
    delay(100);
  }

  delay(1000);
}



/* Scroll some text. Demonstrates the Print() & Pos() functions. */
void ScrollText(char *Text)
{
  int Length = strlen(Text) * 8;  // Calculate length of string in pixels.

  // Scroll the text across the display
  for(int x = 20; x > - Length; x--)
  {
    HCDisplay.Clear();
    HCDisplay.Pos(x, (sin((float)x/2) * 4) + ((RES_Y - 8) /2));
    HCDisplay.Print(Text);
    delay(50);
  }
}



/* Bounce a line and a rectangle around the display. Demonstrates the libraries graphic functions */
void Bounce(void)
{
  int16_t Coord[2][4] = {{1,3,4,6}, {RES_X - 6, RES_Y - 4, RES_X - 4, RES_Y - 2}};
  int8_t  Dir[2][4] = {{1,1,1,1}, {-1, -1, -1, -1}};

  for(int k = 0; k < 100; k++)
  {
    HCDisplay.Clear();

    //Draw line and rectangle at current coordinates
    HCDisplay.Line(Coord[LINE][X1],Coord[LINE][Y1],Coord[LINE][X2],Coord[LINE][Y2]);
    HCDisplay.Rect(Coord[RECT][X1],Coord[RECT][Y1],Coord[RECT][X2],Coord[RECT][Y2], FILLED);
    
    delay(100);

    //Check if the edges of the rectangle or line are at the edge of the display and if so change their direction
    for(byte i = 0; i < 4; i++)
    {
      if(Coord[LINE][i] == 0 || Coord[LINE][i] >= ((i%2 == 0) ? RES_X - 1 : RES_Y - 1))
        Dir[LINE][i] *= -1;

      if(Coord[RECT][i] == 0 || Coord[RECT][i] >= ((i%2 == 0) ? RES_X - 1: RES_Y - 1))
        Dir[RECT][i] *= -1;

      Coord[LINE][i] += Dir[LINE][i];
      Coord[RECT][i] += Dir[RECT][i];
    }
  }
}



/* Draw expanding circles at random points on the screen. Demonstrates the libraries circle graphic function */
void Circles(void)
{
  for(int k = 0; k < 10; k++)
  {
    int16_t x = random(0, RES_X - 1), y = random(0, RES_Y - 1);

    for(int16_t r = 0; r < RES_X || r < RES_Y; r++)
    {
      HCDisplay.Clear();
      HCDisplay.Circle(x, y, r, OUTLINE);
      delay(100);
    }
  }
}



/* Draws a bitmap animation. Demonstrates the libraries BWBitmap() function. */
void Bitmap(void)
{
  HCDisplay.Clear();

  // Run the animation 5 times
  for(uint8_t i = 0; i < 5; i++)
  {
    // Draw some food
    for(int16_t x = 0; x < RES_X; x += 4)
      HCDisplay.Rect(x, (RES_Y / 2) - 1, x+1, (RES_Y / 2), FILLED);

    /* Scroll the animated bitmap across the display */
    for(int16_t x = RES_X; x > -9; x--)
    {
      HCDisplay.Pos(x, (RES_Y / 2) - 4);
      (x % 2)? HCDisplay.BWBitmap(9, 1, PM1) : HCDisplay.BWBitmap(9, 1, PM2);
      delay(100);
    }
    delay(1000);
  }
}



/* Conway's game of life simulation */
void Life(void)
{
  boolean Life;
  boolean NextGen[RES_X][RES_Y];
  byte Count, Gen;
  
  HCDisplay.AutoRefresh(OFF); //Don't automatically update the display every time we write something.

  //Run the simulation 10 times 
  for(uint8_t k = 0; k < 10; k++)
  {
    HCDisplay.Clear();

    Gen = 0;

    //Create a starting pattern 
    for(uint16_t x = 0; x < RES_X; x++)
      for(uint16_t y = 0; y < RES_Y; y++)
        if(random(0, 10) < 5)
          HCDisplay.Plot(x, y);

    HCDisplay.Refresh(); //Update the display

  
    //Cycle though each generation until there are no longer any living cells
    do
    {
      //Clear the next generation grid
      for(int16_t x = 0; x < RES_X; x++)
        for(int16_t y = 0; y < RES_Y; y++)
          NextGen[x][y] = 0;

      //Step through each cell on the current grid to determine if it will survive the next generation 
      for(int16_t x = 0; x < RES_X; x++)
        for(int16_t y = 0; y < RES_Y; y++)
        {
          Count = 0;

          //Count how many cells surrounding the current cell are alive
          for(int16_t dx = x - 1; dx <= x + 1; dx++)
            for(int16_t dy = y - 1; dy <= y + 1; dy++)
              if(dx >= 0 && dx < RES_X && dy >= 0 && dy < RES_Y && (dx != x || dy != y))
                if(HCDisplay.ReadPixel(dx, dy))
                  Count++;


          // If their are 2 to 3 live cells surrounding the current live cell then it lives to the next generation
          if(Count >= 2 && Count <= 3 && HCDisplay.ReadPixel(x, y))
            NextGen[x][y] = true;

          //If a dead cell has 3 live cells surrounding it then it becomes alive in the next generation
          if(Count == 3 && HCDisplay.ReadPixel(x, y) == 0)
            NextGen[x][y] = true;
        }
    
      delay(50);
      HCDisplay.Clear();
      Life = false;

      //Copy the next generation to the display
      for(int16_t x = 0; x < RES_X; x++)
        for(int16_t y = 0; y < RES_Y; y++)
          if(NextGen[x][y])
          {
            HCDisplay.Plot(x,y);
            Life = true;
          } 

      //Update the display
      HCDisplay.Refresh();

      Gen++;
    }while(Life && Gen < 100); //Keep running until their is either no life or we have reached 100 generations
  }

  HCDisplay.AutoRefresh(ON);
}
