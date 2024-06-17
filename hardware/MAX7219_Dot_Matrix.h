/* FILE:    MAX7219_Dot_Matrix.h
   DATE:    12/06/24
   VERSION: 1.0
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version
12/06/24 version 1.0: Updated to work with V1.2.2 library

This library adds hardware support to the HCDisplay library for MAX7219 based displays.
Current supported boards:

MAX7219 Serial LED dot matrix module - Red (SKU: HCOPTO0014). Available for purchase form our store here:
https://hobbycomponents.com/opto-electronics/190-max7219-serial-dot-matrix-display-module


This library is provided free to support the open source community. PLEASE SUPPORT HOBBY COMPONENTS 
so that we can continue to provide free content like this by purchasing items from our store - 
HOBBYCOMPONENTS.COM 


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

#ifndef MAX7219_DOT_MATRIX_h
#define MAX7219_DOT_MATRIX_h
#include "Arduino.h"

#include "fonts/fonts.h"


/* MAX7219 register addresses */
#define MAX7219NOOP 0x00
#define MAX7219DIGIT1 0x01
#define MAX7219DIGIT2 0x02
#define MAX7219DIGIT3 0x03
#define MAX7219DIGIT4 0x04
#define MAX7219DIGIT5 0x05
#define MAX7219DIGIT6 0x06
#define MAX7219DIGIT7 0x07
#define MAX7219DIGIT8 0x08
#define MAX7219DECODE 0x09
#define MAX7219INTESITY 0x0A
#define MAX7219SCANLIMIT 0x0B
#define MAX7219SHUTDOWN 0x0C
#define MAX7219TEST 0x0F

/* MAX7219 register options */
#define TESTMODEOFF 0
#define TESTMODEON 1
#define MAX7219OFF 0
#define MAX7219ON 1




// Character bitmaps for the default system font
const PROGMEM  uint8_t SystemFontBitmaps[] = 
{
	// @0 ' ' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x00, //         
	0x00, //         
	0x00, //         
	0x00, //         
	0x00, //         

	// @8 '!' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x4F, //  #  ####
	0x4F, //  #  ####
	0x00, //         
	0x00, //         
	0x00, //         

	// @16 '"' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x03, //       ##
	0x03, //       ##
	0x00, //         
	0x03, //       ##
	0x03, //       ##
	0x00, //         

	// @24 '#' (8 pixels wide)
	0x00, //         
	0x14, //    # #  
	0x7F, //  #######
	0x7F, //  #######
	0x14, //    # #  
	0x7F, //  #######
	0x7F, //  #######
	0x14, //    # #  

	// @32 '$' (8 pixels wide)
	0x00, //         
	0x24, //   #  #  
	0x2E, //   # ### 
	0x6B, //  ## # ##
	0x6B, //  ## # ##
	0x3A, //   ### # 
	0x12, //    #  # 
	0x00, //         

	// @40 '%' (8 pixels wide)
	0x00, //         
	0x4C, //  #  ##  
	0x6A, //  ## # # 
	0x36, //   ## ## 
	0x18, //    ##   
	0x6C, //  ## ##  
	0x56, //  # # ## 
	0x32, //   ##  # 

	// @48 '&' (8 pixels wide)
	0x30, //   ##    
	0x7E, //  ###### 
	0x4F, //  #  ####
	0x59, //  # ##  #
	0x77, //  ### ###
	0x7A, //  #### # 
	0x68, //  ## #   
	0x40, //  #      

	// @56 ''' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x04, //      #  
	0x07, //      ###
	0x03, //       ##
	0x00, //         
	0x00, //         

	// @64 '(' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x1C, //    ###  
	0x3E, //   ##### 
	0x63, //  ##   ##
	0x41, //  #     #
	0x00, //         
	0x00, //         

	// @72 ')' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x41, //  #     #
	0x63, //  ##   ##
	0x3E, //   ##### 
	0x1C, //    ###  
	0x00, //         
	0x00, //         

	// @80 '*' (8 pixels wide)
	0x08, //     #   
	0x2A, //   # # # 
	0x3E, //   ##### 
	0x1C, //    ###  
	0x1C, //    ###  
	0x3E, //   ##### 
	0x2A, //   # # # 
	0x08, //     #   

	// @88 '+' (8 pixels wide)
	0x00, //         
	0x08, //     #   
	0x08, //     #   
	0x3E, //   ##### 
	0x3E, //   ##### 
	0x08, //     #   
	0x08, //     #   
	0x00, //         

	// @96 ',' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x80, // #       
	0xE0, // ###     
	0x60, //  ##     
	0x00, //         
	0x00, //         
	0x00, //         

	// @104 '-' (8 pixels wide)
	0x00, //         
	0x08, //     #   
	0x08, //     #   
	0x08, //     #   
	0x08, //     #   
	0x08, //     #   
	0x08, //     #   
	0x00, //         

	// @112 '.' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x60, //  ##     
	0x60, //  ##     
	0x00, //         
	0x00, //         
	0x00, //         

	// @120 '/' (8 pixels wide)
	0x40, //  #      
	0x60, //  ##     
	0x30, //   ##    
	0x18, //    ##   
	0x0C, //     ##  
	0x06, //      ## 
	0x03, //       ##
	0x01, //        #

	// @128 '0' (8 pixels wide)
	0x00, //         
	0x3E, //   ##### 
	0x7F, //  #######
	0x59, //  # ##  #
	0x4D, //  #  ## #
	0x7F, //  #######
	0x3E, //   ##### 
	0x00, //         

	// @136 '1' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x04, //      #  
	0x06, //      ## 
	0x7F, //  #######
	0x7F, //  #######
	0x00, //         
	0x00, //         

	// @144 '2' (8 pixels wide)
	0x00, //         
	0x42, //  #    # 
	0x63, //  ##   ##
	0x71, //  ###   #
	0x59, //  # ##  #
	0x4F, //  #  ####
	0x46, //  #   ## 
	0x00, //         

	// @152 '3' (8 pixels wide)
	0x00, //         
	0x22, //   #   # 
	0x63, //  ##   ##
	0x49, //  #  #  #
	0x49, //  #  #  #
	0x7F, //  #######
	0x36, //   ## ## 
	0x00, //         

	// @160 '4' (8 pixels wide)
	0x00, //         
	0x18, //    ##   
	0x1C, //    ###  
	0x16, //    # ## 
	0x13, //    #  ##
	0x7F, //  #######
	0x7F, //  #######
	0x10, //    #    

	// @168 '5' (8 pixels wide)
	0x00, //         
	0x27, //   #  ###
	0x67, //  ##  ###
	0x45, //  #   # #
	0x45, //  #   # #
	0x7D, //  ##### #
	0x39, //   ###  #
	0x00, //         

	// @176 '6' (8 pixels wide)
	0x00, //         
	0x3C, //   ####  
	0x7E, //  ###### 
	0x4B, //  #  # ##
	0x49, //  #  #  #
	0x79, //  ####  #
	0x30, //   ##    
	0x00, //         

	// @184 '7' (8 pixels wide)
	0x00, //         
	0x01, //        #
	0x01, //        #
	0x71, //  ###   #
	0x79, //  ####  #
	0x0F, //     ####
	0x07, //      ###
	0x00, //         

	// @192 '8' (8 pixels wide)
	0x00, //         
	0x36, //   ## ## 
	0x7F, //  #######
	0x49, //  #  #  #
	0x49, //  #  #  #
	0x7F, //  #######
	0x36, //   ## ## 
	0x00, //         

	// @200 '9' (8 pixels wide)
	0x00, //         
	0x06, //      ## 
	0x4F, //  #  ####
	0x49, //  #  #  #
	0x69, //  ## #  #
	0x3F, //   ######
	0x1E, //    #### 
	0x00, //         

	// @208 ':' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x66, //  ##  ## 
	0x66, //  ##  ## 
	0x00, //         
	0x00, //         
	0x00, //         

	// @216 ';' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x80, // #       
	0xE6, // ###  ## 
	0x66, //  ##  ## 
	0x00, //         
	0x00, //         

	// @224 '<' (8 pixels wide)
	0x00, //         
	0x08, //     #   
	0x08, //     #   
	0x14, //    # #  
	0x14, //    # #  
	0x22, //   #   # 
	0x22, //   #   # 
	0x00, //         

	// @232 '=' (8 pixels wide)
	0x00, //         
	0x14, //    # #  
	0x14, //    # #  
	0x14, //    # #  
	0x14, //    # #  
	0x14, //    # #  
	0x14, //    # #  
	0x00, //         

	// @240 '>' (8 pixels wide)
	0x00, //         
	0x22, //   #   # 
	0x22, //   #   # 
	0x14, //    # #  
	0x14, //    # #  
	0x08, //     #   
	0x08, //     #   
	0x00, //         

	// @248 '?' (8 pixels wide)
	0x00, //         
	0x02, //       # 
	0x03, //       ##
	0x51, //  # #   #
	0x59, //  # ##  #
	0x0F, //     ####
	0x06, //      ## 
	0x00, //         

	// @256 '@' (8 pixels wide)
	0x00, //         
	0x3E, //   ##### 
	0x7F, //  #######
	0x41, //  #     #
	0x5D, //  # ### #
	0x55, //  # # # #
	0x1F, //    #####
	0x1E, //    #### 

	// @264 'A' (8 pixels wide)
	0x00, //         
	0x7E, //  ###### 
	0x7F, //  #######
	0x09, //     #  #
	0x09, //     #  #
	0x7F, //  #######
	0x7E, //  ###### 
	0x00, //         

	// @272 'B' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x49, //  #  #  #
	0x49, //  #  #  #
	0x7F, //  #######
	0x36, //   ## ## 
	0x00, //         

	// @280 'C' (8 pixels wide)
	0x00, //         
	0x1C, //    ###  
	0x3E, //   ##### 
	0x63, //  ##   ##
	0x41, //  #     #
	0x41, //  #     #
	0x41, //  #     #
	0x00, //         

	// @288 'D' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x41, //  #     #
	0x63, //  ##   ##
	0x3E, //   ##### 
	0x1C, //    ###  
	0x00, //         

	// @296 'E' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x49, //  #  #  #
	0x49, //  #  #  #
	0x41, //  #     #
	0x41, //  #     #
	0x00, //         

	// @304 'F' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x09, //     #  #
	0x09, //     #  #
	0x01, //        #
	0x01, //        #
	0x00, //         

	// @312 'G' (8 pixels wide)
	0x00, //         
	0x3E, //   ##### 
	0x7F, //  #######
	0x41, //  #     #
	0x49, //  #  #  #
	0x7B, //  #### ##
	0x7A, //  #### # 
	0x00, //         

	// @320 'H' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x08, //     #   
	0x08, //     #   
	0x7F, //  #######
	0x7F, //  #######
	0x00, //         

	// @328 'I' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x41, //  #     #
	0x7F, //  #######
	0x7F, //  #######
	0x41, //  #     #
	0x00, //         
	0x00, //         

	// @336 'J' (8 pixels wide)
	0x00, //         
	0x20, //   #     
	0x60, //  ##     
	0x40, //  #      
	0x40, //  #      
	0x7F, //  #######
	0x3F, //   ######
	0x00, //         

	// @344 'K' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x08, //     #   
	0x1C, //    ###  
	0x36, //   ## ## 
	0x63, //  ##   ##
	0x41, //  #     #

	// @352 'L' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x40, //  #      
	0x40, //  #      
	0x40, //  #      
	0x40, //  #      
	0x00, //         

	// @360 'M' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x06, //      ## 
	0x0C, //     ##  
	0x06, //      ## 
	0x7F, //  #######
	0x7F, //  #######

	// @368 'N' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x06, //      ## 
	0x0C, //     ##  
	0x18, //    ##   
	0x7F, //  #######
	0x7F, //  #######

	// @376 'O' (8 pixels wide)
	0x00, //         
	0x3E, //   ##### 
	0x7F, //  #######
	0x41, //  #     #
	0x41, //  #     #
	0x7F, //  #######
	0x3E, //   ##### 
	0x00, //         

	// @384 'P' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x09, //     #  #
	0x09, //     #  #
	0x0F, //     ####
	0x06, //      ## 
	0x00, //         

	// @392 'Q' (8 pixels wide)
	0x00, //         
	0x3E, //   ##### 
	0x7F, //  #######
	0x41, //  #     #
	0x61, //  ##    #
	0x7F, //  #######
	0x7E, //  ###### 
	0x40, //  #      

	// @400 'R' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x09, //     #  #
	0x19, //    ##  #
	0x7F, //  #######
	0x66, //  ##  ## 
	0x00, //         

	// @408 'S' (8 pixels wide)
	0x00, //         
	0x26, //   #  ## 
	0x6F, //  ## ####
	0x4D, //  #  ## #
	0x59, //  # ##  #
	0x7B, //  #### ##
	0x32, //   ##  # 
	0x00, //         

	// @416 'T' (8 pixels wide)
	0x00, //         
	0x01, //        #
	0x01, //        #
	0x7F, //  #######
	0x7F, //  #######
	0x01, //        #
	0x01, //        #
	0x00, //         

	// @424 'U' (8 pixels wide)
	0x00, //         
	0x3F, //   ######
	0x7F, //  #######
	0x40, //  #      
	0x40, //  #      
	0x7F, //  #######
	0x3F, //   ######
	0x00, //         

	// @432 'V' (8 pixels wide)
	0x00, //         
	0x0F, //     ####
	0x3F, //   ######
	0x70, //  ###    
	0x70, //  ###    
	0x3F, //   ######
	0x0F, //     ####
	0x00, //         

	// @440 'W' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x30, //   ##    
	0x18, //    ##   
	0x30, //   ##    
	0x7F, //  #######
	0x7F, //  #######

	// @448 'X' (8 pixels wide)
	0x41, //  #     #
	0x63, //  ##   ##
	0x36, //   ## ## 
	0x1C, //    ###  
	0x1C, //    ###  
	0x36, //   ## ## 
	0x63, //  ##   ##
	0x41, //  #     #

	// @456 'Y' (8 pixels wide)
	0x01, //        #
	0x03, //       ##
	0x06, //      ## 
	0x7C, //  #####  
	0x7C, //  #####  
	0x06, //      ## 
	0x03, //       ##
	0x01, //        #

	// @464 'Z' (8 pixels wide)
	0x00, //         
	0x61, //  ##    #
	0x71, //  ###   #
	0x59, //  # ##  #
	0x4D, //  #  ## #
	0x47, //  #   ###
	0x43, //  #    ##
	0x41, //  #     #

	// @472 '[' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x41, //  #     #
	0x41, //  #     #
	0x00, //         
	0x00, //         

	// @480 '\' (8 pixels wide)
	0x01, //        #
	0x03, //       ##
	0x06, //      ## 
	0x0C, //     ##  
	0x18, //    ##   
	0x30, //   ##    
	0x60, //  ##     
	0x40, //  #      

	// @488 ']' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x41, //  #     #
	0x41, //  #     #
	0x7F, //  #######
	0x7F, //  #######
	0x00, //         
	0x00, //         

	// @496 '^' (8 pixels wide)
	0x00, //         
	0x08, //     #   
	0x0C, //     ##  
	0x06, //      ## 
	0x03, //       ##
	0x06, //      ## 
	0x0C, //     ##  
	0x08, //     #   

	// @504 '_' (8 pixels wide)
	0x00, //         
	0x80, // #       
	0x80, // #       
	0x80, // #       
	0x80, // #       
	0x80, // #       
	0x80, // #       
	0x80, // #       

	// @512 '`' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x03, //       ##
	0x07, //      ###
	0x04, //      #  
	0x00, //         
	0x00, //         

	// @520 'a' (8 pixels wide)
	0x00, //         
	0x20, //   #     
	0x74, //  ### #  
	0x54, //  # # #  
	0x54, //  # # #  
	0x7C, //  #####  
	0x78, //  ####   
	0x00, //         

	// @528 'b' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x44, //  #   #  
	0x44, //  #   #  
	0x7C, //  #####  
	0x38, //   ###   
	0x00, //         

	// @536 'c' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x38, //   ###   
	0x7C, //  #####  
	0x44, //  #   #  
	0x44, //  #   #  
	0x44, //  #   #  
	0x00, //         

	// @544 'd' (8 pixels wide)
	0x00, //         
	0x38, //   ###   
	0x7C, //  #####  
	0x44, //  #   #  
	0x44, //  #   #  
	0x7F, //  #######
	0x7F, //  #######
	0x00, //         

	// @552 'e' (8 pixels wide)
	0x00, //         
	0x38, //   ###   
	0x7C, //  #####  
	0x54, //  # # #  
	0x54, //  # # #  
	0x5C, //  # ###  
	0x18, //    ##   
	0x00, //         

	// @560 'f' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x04, //      #  
	0x7E, //  ###### 
	0x7F, //  #######
	0x05, //      # #
	0x05, //      # #
	0x00, //         

	// @568 'g' (8 pixels wide)
	0x00, //         
	0x18, //    ##   
	0xBC, // # ####  
	0xA4, // # #  #  
	0xA4, // # #  #  
	0xFC, // ######  
	0x7C, //  #####  
	0x00, //         

	// @576 'h' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x04, //      #  
	0x04, //      #  
	0x7C, //  #####  
	0x78, //  ####   
	0x00, //         

	// @584 'i' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x3D, //   #### #
	0x7D, //  ##### #
	0x40, //  #      
	0x00, //         
	0x00, //         

	// @592 'j' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x80, // #       
	0x80, // #       
	0x80, // #       
	0xFD, // ###### #
	0x7D, //  ##### #
	0x00, //         

	// @600 'k' (8 pixels wide)
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x10, //    #    
	0x38, //   ###   
	0x6C, //  ## ##  
	0x44, //  #   #  
	0x00, //         

	// @608 'l' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x3F, //   ######
	0x7F, //  #######
	0x40, //  #      
	0x00, //         
	0x00, //         

	// @616 'm' (8 pixels wide)
	0x00, //         
	0x7C, //  #####  
	0x7C, //  #####  
	0x0C, //     ##  
	0x18, //    ##   
	0x0C, //     ##  
	0x7C, //  #####  
	0x78, //  ####   

	// @624 'n' (8 pixels wide)
	0x00, //         
	0x7C, //  #####  
	0x7C, //  #####  
	0x04, //      #  
	0x04, //      #  
	0x7C, //  #####  
	0x78, //  ####   
	0x00, //         

	// @632 'o' (8 pixels wide)
	0x00, //         
	0x38, //   ###   
	0x7C, //  #####  
	0x44, //  #   #  
	0x44, //  #   #  
	0x7C, //  #####  
	0x38, //   ###   
	0x00, //         

	// @640 'p' (8 pixels wide)
	0x00, //         
	0xFC, // ######  
	0xFC, // ######  
	0x24, //   #  #  
	0x24, //   #  #  
	0x3C, //   ####  
	0x18, //    ##   
	0x00, //         

	// @648 'q' (8 pixels wide)
	0x00, //         
	0x18, //    ##   
	0x3C, //   ####  
	0x24, //   #  #  
	0x24, //   #  #  
	0xFC, // ######  
	0xFC, // ######  
	0x00, //         

	// @656 'r' (8 pixels wide)
	0x00, //         
	0x7C, //  #####  
	0x7C, //  #####  
	0x04, //      #  
	0x04, //      #  
	0x0C, //     ##  
	0x08, //     #   
	0x00, //         

	// @664 's' (8 pixels wide)
	0x00, //         
	0x48, //  #  #   
	0x5C, //  # ###  
	0x54, //  # # #  
	0x54, //  # # #  
	0x74, //  ### #  
	0x20, //   #     
	0x00, //         

	// @672 't' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x04, //      #  
	0x3F, //   ######
	0x7F, //  #######
	0x44, //  #   #  
	0x44, //  #   #  
	0x00, //         

	// @680 'u' (8 pixels wide)
	0x00, //         
	0x3C, //   ####  
	0x7C, //  #####  
	0x40, //  #      
	0x40, //  #      
	0x7C, //  #####  
	0x7C, //  #####  
	0x00, //         

	// @688 'v' (8 pixels wide)
	0x00, //         
	0x1C, //    ###  
	0x3C, //   ####  
	0x60, //  ##     
	0x60, //  ##     
	0x3C, //   ####  
	0x1C, //    ###  
	0x00, //         

	// @696 'w' (8 pixels wide)
	0x00, //         
	0x3C, //   ####  
	0x7C, //  #####  
	0x60, //  ##     
	0x30, //   ##    
	0x60, //  ##     
	0x7C, //  #####  
	0x3C, //   ####  

	// @704 'x' (8 pixels wide)
	0x00, //         
	0x44, //  #   #  
	0x6C, //  ## ##  
	0x38, //   ###   
	0x10, //    #    
	0x38, //   ###   
	0x6C, //  ## ##  
	0x44, //  #   #  

	// @712 'y' (8 pixels wide)
	0x00, //         
	0x1C, //    ###  
	0xBC, // # ####  
	0xE0, // ###     
	0x60, //  ##     
	0x3C, //   ####  
	0x1C, //    ###  
	0x00, //         

	// @720 'z' (8 pixels wide)
	0x00, //         
	0x44, //  #   #  
	0x64, //  ##  #  
	0x74, //  ### #  
	0x5C, //  # ###  
	0x4C, //  #  ##  
	0x44, //  #   #  
	0x00, //         

	// @728 '{' (8 pixels wide)
	0x00, //         
	0x08, //     #   
	0x08, //     #   
	0x3E, //   ##### 
	0x77, //  ### ###
	0x41, //  #     #
	0x41, //  #     #
	0x00, //         

	// @736 '|' (8 pixels wide)
	0x00, //         
	0x00, //         
	0x00, //         
	0x7F, //  #######
	0x7F, //  #######
	0x00, //         
	0x00, //         
	0x00, //         

	// @744 '}' (8 pixels wide)
	0x00, //         
	0x41, //  #     #
	0x41, //  #     #
	0x77, //  ### ###
	0x3E, //   ##### 
	0x08, //     #   
	0x08, //     #   
	0x00, //         

	// @752 '~' (8 pixels wide)
	0x02, //       # 
	0x01, //        #
	0x01, //        #
	0x01, //        #
	0x02, //       # 
	0x02, //       # 
	0x02, //       # 
	0x01, //        #
};

// Character descriptors for the system font
// { [Char width in bits], [Offset into SystemFontBitmaps in bytes] }
const PROGMEM  FONT_CHAR_INFO SystemFontDescriptors[] = 
{
	{8, 0}, 		//   
	{8, 8}, 		// ! 
	{8, 16}, 		// " 
	{8, 24}, 		// # 
	{8, 32}, 		// $ 
	{8, 40}, 		// % 
	{8, 48}, 		// & 
	{8, 56}, 		// ' 
	{8, 64}, 		// ( 
	{8, 72}, 		// ) 
	{8, 80}, 		// * 
	{8, 88}, 		// + 
	{8, 96}, 		// , 
	{8, 104}, 		// - 
	{8, 112}, 		// . 
	{8, 120}, 		// / 
	{8, 128}, 		// 0 
	{8, 136}, 		// 1 
	{8, 144}, 		// 2 
	{8, 152}, 		// 3 
	{8, 160}, 		// 4 
	{8, 168}, 		// 5 
	{8, 176}, 		// 6 
	{8, 184}, 		// 7 
	{8, 192}, 		// 8 
	{8, 200}, 		// 9 
	{8, 208}, 		// : 
	{8, 216}, 		// ; 
	{8, 224}, 		// < 
	{8, 232}, 		// = 
	{8, 240}, 		// > 
	{8, 248}, 		// ? 
	{8, 256}, 		// @ 
	{8, 264}, 		// A 
	{8, 272}, 		// B 
	{8, 280}, 		// C 
	{8, 288}, 		// D 
	{8, 296}, 		// E 
	{8, 304}, 		// F 
	{8, 312}, 		// G 
	{8, 320}, 		// H 
	{8, 328}, 		// I 
	{8, 336}, 		// J 
	{8, 344}, 		// K 
	{8, 352}, 		// L 
	{8, 360}, 		// M 
	{8, 368}, 		// N 
	{8, 376}, 		// O 
	{8, 384}, 		// P 
	{8, 392}, 		// Q 
	{8, 400}, 		// R 
	{8, 408}, 		// S 
	{8, 416}, 		// T 
	{8, 424}, 		// U 
	{8, 432}, 		// V 
	{8, 440}, 		// W 
	{8, 448}, 		// X 
	{8, 456}, 		// Y 
	{8, 464}, 		// Z 
	{8, 472}, 		// [ 
	{8, 480}, 		// backslash 
	{8, 488}, 		// ] 
	{8, 496}, 		// ^ 
	{8, 504}, 		// _ 
	{8, 512}, 		// ` 
	{8, 520}, 		// a 
	{8, 528}, 		// b 
	{8, 536}, 		// c 
	{8, 544}, 		// d 
	{8, 552}, 		// e 
	{8, 560}, 		// f 
	{8, 568}, 		// g 
	{8, 576}, 		// h 
	{8, 584}, 		// i 
	{8, 592}, 		// j 
	{8, 600}, 		// k 
	{8, 608}, 		// l 
	{8, 616}, 		// m 
	{8, 624}, 		// n 
	{8, 632}, 		// o 
	{8, 640}, 		// p 
	{8, 648}, 		// q 
	{8, 656}, 		// r 
	{8, 664}, 		// s 
	{8, 672}, 		// t 
	{8, 680}, 		// u 
	{8, 688}, 		// v 
	{8, 696}, 		// w 
	{8, 704}, 		// x 
	{8, 712}, 		// y 
	{8, 720}, 		// z 
	{8, 728}, 		// { 
	{8, 736}, 		// | 
	{8, 744}, 		// } 
	{8, 752}, 		// ~ 
};



// Font information for the system font
const FONT_INFO SystemFont[] =
{
	1,		// Character height
	' ', 	// Start character
	'~', 	// End character
	0,		// Spacing between characters
	SystemFontDescriptors, //  Character descriptor array
	SystemFontBitmaps, //  Character bitmap array
};



class Display
{
  public:
  Display(void);
  void DInit(uint8_t CS, uint8_t DriversX, uint8_t DriversY, uint8_t *Buffer);
  void DUpdateDisplay(void);
  void DReset(void);
  void DSleep(boolean mode);
  void DScreen(boolean mode);
  void DBacklight(uint8_t mode);
  void DFlip(uint8_t mode);
  void DSetFG(uint8_t r, uint8_t g, uint8_t b);
  void DSetFG(boolean colour);
  void DSetBG(uint8_t r, uint8_t g, uint8_t b);
  void DSetBG(boolean colour);
  void DClear(void);
  void DErase(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void DPlot(uint16_t x, uint16_t y);
  void DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void DDrawMode(uint8_t Mode);
  void DSetXY(int16_t X, int16_t Y);
  void DScaleXY(uint8_t ScaleX, uint8_t ScaleY);
  void DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t *BitmapData, boolean Background = true);
  void DSetFont(const FONT_INFO *NewFont);
  void DWriteChar(char character, boolean Background = true);
  void DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data);
  boolean DReadPixel(uint16_t x, uint16_t y);
  uint16_t DResX(void);
  uint16_t DResY(void);
  void DContrast(uint8_t level);
  
  
  private:
  void _DWrite(byte Address, byte Data, byte Driver);
  void _DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, uint8_t Data /*const uint8_t *Data*/);
  void _DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, uint8_t Data /*const uint8_t *Data*/); 
 
 
  uint16_t _res_x, _res_y, _NumberOfDrivers;
  int16_t _X1Pos, _Y1Pos;
  uint8_t _DrawMode, _CS, _ScaleX = 1, _ScaleY = 1;
  uint8_t *_Buffer;
  boolean _FGColour = 1, _BGColour = 0;
  
  const FONT_INFO *Font;
};



class Touch
{
	public:
	Touch(void);
	void TInit(uint8_t DIN, uint8_t DOUT, uint8_t CLK, uint8_t CS, uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping);
	uint16_t TReadRaw(boolean Axis);
	boolean TPressed(void);
	void TReadTouch(void);
	uint16_t TGetCoord(boolean Axis);
	void SetScreenRes(uint16_t Res_X, uint16_t Res_Y);
	
	
	private:
	
};

#endif