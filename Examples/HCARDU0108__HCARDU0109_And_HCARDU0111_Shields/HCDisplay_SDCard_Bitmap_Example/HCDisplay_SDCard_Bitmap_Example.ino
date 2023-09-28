/* FILE:    HCDisplay_SDCard_Bitmap_Example
   DATE:    20/12/18
   VERSION: 0.1
   AUTHOR:  Andrew Davies
   
20/12/18 version 0.1: Original version

This example sketch demonstrates how to read a bitmap image from an SD card and
plot it to the display. This sketch supports the following shields:

Open-Smart 2.4 Inch TFT shield (SKU: HCARDU0108)
Open-Smart 3.2 Inch TFT shield with HX8352B controller (SKU: HCARDU0109)
Open-Smart 3.2 Inch TFT shield with ILI9327 controller (SKU: HCARDU0111)

NOTE: TO USE THIS SKETCH YOU MUST FIRST SELECT THE VERSION OF DISPLAY YOU HAVE BY 
UNCOMMENTING THE APPROPRIATE LINE IN THE OPTIONS.H FILE WHICH CAN BE FOUND IN THE 
HCDISPLAY LIBRARY FOLDER. For windows users avoid using the Windows Notepad editor 
as it doesn't format things properly.  

An example image (TestImg.bmp) can be found in the HCDisplay library folder in
the examples/image sub folder. You should copy this to the root folder of your
SD card.

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
#include "SPI.h"
#include "SD.h"

#define SDCS 5        //Digital pin used for the SD card readers chip select pin

boolean SDError = false;

File SDFileData;

HCDisplay HCDisplay;  //Creates an instance of the HCDisplay library

uint8_t SPCR_Temp;    //Used to store the current state of the SPI control register


void setup() 
{
  HCDisplay.Init(); //Initialise the display
 
  Serial.begin(9600);

  if(!SD.begin(SDCS))
  {
    SDError = true;
    Serial.println("ERROR: Can't start SD card library");
    while(1);
  }

  /* The shield shares two of its data bus pins with the SD card interface
     So each time we need to send data to the display we need to save
     the current state of the SD card (SPI) interface so that it can disable it 
     whilst we talk to the display. */
  SPCR_Temp = SPCR;

   
   /* Load a bitmap from the SD card and output it to the display starting 
      at X Y coordinates 0,0 */
   Bitmap2TFT("TestImg.bmp", 0, 0);
}


void loop() 
{
  //Nothing to do here !
}



/* Reads a bitmap file from the SD card and outputs it to the 
 * display at the specified coordinates */
boolean Bitmap2TFT(char FileName[], int XStart, int YStart)
{
  unsigned int BMID;
  long BMFileSize, BMWidth, BMHight;
  byte R,G,B;
  int x, y;

  SDError = false;
  
  /* Check if the text file exists */
  if(!SD.exists(FileName))
  {
    SDError = true;
    Serial.println("ERROR: File not found!");
  }

  /* If the file exists open it */
  if(!SDError)
    SDFileData = SD.open(FileName); 

  /* Check if file is a bitmap */ 
  if(!SDError)
  {  
    BMID = (SDFileData.read() << 8) | SDFileData.read();
    if (BMID != 0x424D)
    {
      SDError = true;
      Serial.println("ERROR: File is not a bitmap!");
    }
  }
 

  /* Read the bitmaps RGB data and plot it to the display */
  if(!SDError)
  {
    /* Get the size of the image so we know how much data to load */
    BMFileSize = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);
    SDFileData.seek(0x0012);
    BMWidth = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);
    SDFileData.seek(0x0016);
    BMHight = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);   


    /* Read the bitmap image data and write it straight to the display */
    for (y = YStart; y < BMHight + YStart; y++)
    {
      BMFileSize -= (BMWidth * 3); 
      SDFileData.seek(BMFileSize); 

      for(x = XStart; x < BMWidth + XStart; x++)
      {  
        B = SDFileData.read();
        G = SDFileData.read();
        R = SDFileData.read();

        if(x >= 0 && x < HCDisplay.ResX() && y >= 0 && y < HCDisplay.ResY()) //If pixel is within screen area then plot it
        {
          SPCR = 0;        //Disable the SPI interface.
          HCDisplay.SetFG(R,G,B);
          HCDisplay.Plot(x, y);
          SPCR = SPCR_Temp;   //Restore the state of the SPI interface.
        }
      }  
    }
    
    SDFileData.close(); 
  }

  return !SDError;
}
