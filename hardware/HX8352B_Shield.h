/* FILE:    HX8352B_Shield.cpp
   DATE:    07/06/24
   VERSION: 0.3
   AUTHOR:  Andrew Davies
   
19/10/18 version 0.1: Original version
08/01/21 version 0.2: Change lower case 'c' character to display as a lower case character.
07/06/24 version 0.3: Updated to work with ARM based devices

This library adds hardware support to the HCDisplay library for HX8352B based screens using the controllers SPI interface.
Current supported boards:

Open-Smart 3.2 Inch TFT shield with touch screen (HCARDU0109)

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

#ifndef HX8352B_SHIELD_h
#define HX8352B_SHIELD_h
#include "Arduino.h"

#include "fonts/fonts.h"

#define RES_X 400	//Screen X axis resolution
#define RES_Y 240	//Screen Y axis resolution


/* HX8352 command registers used in this library */
#define GRAM_HA	0x20
#define GRAM_VA 0x21
#define RAMWR	0x22
#define HSA		0x50
#define HEA		0x51
#define VSA		0x52
#define VEA		0x53



/* Shield pin definitions for ATMega328 (Uno) */
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined(__AVR_ATmega8__)
#define DATA1_DDR DDRB
#define DATA2_DDR DDRD

#define DATA1_PORT PORTB
#define DATA2_PORT PORTD

#define DATA1_PIN PINB
#define DATA2_PIN PIND

#define DATA1_MASK 0b00101111
#define DATA2_MASK 0b11010000

#define RD_DDR DDRC
#define WR_DDR DDRC
#define RS_DDR DDRC
#define CS_DDR DDRC

#define RD_PORT PORTC
#define WR_PORT PORTC
#define RS_PORT PORTC
#define CS_PORT PORTC

#define RD_MASK 0b00000001
#define WR_MASK 0b00000010
#define RS_MASK 0b00000100
#define CS_MASK 0b00001000

#define _RD_LOW  RD_PORT &= ~RD_MASK
#define _RD_HIGH RD_PORT |= RD_MASK

#define _WR_LOW  WR_PORT &= ~WR_MASK
#define _WR_HIGH WR_PORT |= WR_MASK

#define _RS_COMMAND  RS_PORT &= ~RS_MASK
#define _RS_DATA RS_PORT |= RS_MASK

#define _CS_EN  CS_PORT &= ~CS_MASK
#define _CS_DIS CS_PORT |= CS_MASK

#define _WR_PULSE {_WR_LOW; _WR_HIGH;}

#define _DATA_DIR_INPUT   {DATA1_DDR &= ~DATA1_MASK; DATA2_DDR &= ~DATA2_MASK;}
#define _DATA_DIR_OUTPUT  {DATA1_DDR |=  DATA1_MASK; DATA2_DDR |=  DATA2_MASK;}

#define _READ_DATA(d) {_RD_LOW; \
						delayMicroseconds(2); \
						d = (DATA2_PIN & DATA2_MASK) | (DATA1_PIN & DATA1_MASK); \
						_RD_HIGH;}

#define _WRITE_DATA(d) {DATA2_PORT = (DATA2_PORT & ~DATA2_MASK) | (d & DATA2_MASK); \
						DATA1_PORT = (DATA1_PORT & ~DATA1_MASK) | (d & DATA1_MASK); \
						_WR_PULSE;}

#define TOUCH_XP_PORT PORTD
#define TOUCH_YP_PORT PORTC
#define TOUCH_XN_PORT PORTC
#define TOUCH_YN_PORT PORTD

#define TOUCH_XP_DDR DDRD
#define TOUCH_YP_DDR DDRC
#define TOUCH_XN_DDR DDRC
#define TOUCH_YN_DDR DDRD

#define TOUCH_XP_MASK 0b01000000
#define TOUCH_YP_MASK 0b00000010
#define TOUCH_XN_MASK 0b00000100
#define TOUCH_YN_MASK 0b10000000


#define TOUCH_XP_LOW 		TOUCH_XP_PORT &= ~TOUCH_XP_MASK
#define TOUCH_XP_HIGH 		TOUCH_XP_PORT |= TOUCH_XP_MASK

#define TOUCH_YP_LOW  		TOUCH_YP_PORT &= ~TOUCH_YP_MASK
#define TOUCH_YP_HIGH  		TOUCH_YP_PORT |= TOUCH_YP_MASK

#define TOUCH_XN_LOW  		TOUCH_XN_PORT &= ~TOUCH_XN_MASK
#define TOUCH_XN_HIGH  		TOUCH_XN_PORT |= TOUCH_XN_MASK

#define TOUCH_YN_LOW  		TOUCH_YN_PORT &= ~TOUCH_YN_MASK
#define TOUCH_YN_HIGH  		TOUCH_YN_PORT |= TOUCH_YN_MASK

#define TOUCH_Y_ANALOGUE	A1
#define TOUCH_X_ANALOGUE	A2


/* Shield pin definitions for ATMega2560 (Mega) */
#elif defined(__AVR_ATmega2560__)

#define D0_DDR DDRH
#define D1_DDR DDRH
#define D2_DDR DDRB
#define D3_DDR DDRB
#define D4_DDR DDRG
#define D5_DDR DDRB
#define D6_DDR DDRH
#define D7_DDR DDRH

#define D0_PORT PORTH
#define D1_PORT PORTH
#define D2_PORT PORTB
#define D3_PORT PORTB
#define D4_PORT PORTG
#define D5_PORT PORTB
#define D6_PORT PORTH
#define D7_PORT PORTH

#define D0_PIN PINH
#define D1_PIN PINH
#define D2_PIN PINB
#define D3_PIN PINB
#define D4_PIN PING
#define D5_PIN PINB
#define D6_PIN PINH
#define D7_PIN PINH

#define D0_MASK	0b00100000
#define D1_MASK	0b01000000
#define D2_MASK	0b00010000
#define D3_MASK	0b00100000
#define D4_MASK	0b00100000
#define D5_MASK	0b10000000
#define D6_MASK 0b00001000
#define D7_MASK	0b00010000

#define RD_DDR DDRF
#define WR_DDR DDRF
#define RS_DDR DDRF
#define CS_DDR DDRF

#define RD_PORT PORTF
#define WR_PORT PORTF
#define RS_PORT PORTF
#define CS_PORT PORTF

#define RD_MASK 0b00000001
#define WR_MASK 0b00000010
#define RS_MASK 0b00000100
#define CS_MASK 0b00001000

#define _RD_LOW  RD_PORT &= ~RD_MASK
#define _RD_HIGH RD_PORT |= RD_MASK

#define _WR_LOW  WR_PORT &= ~WR_MASK
#define _WR_HIGH WR_PORT |= WR_MASK

#define _RS_COMMAND  RS_PORT &= ~RS_MASK
#define _RS_DATA RS_PORT |= RS_MASK

#define _CS_EN  CS_PORT &= ~CS_MASK
#define _CS_DIS CS_PORT |= CS_MASK

#define _WR_PULSE {_WR_LOW; _WR_HIGH;}

#define _DATA_DIR_INPUT   {D0_DDR &= ~D0_MASK; \
						   D1_DDR &= ~D1_MASK; \
						   D2_DDR &= ~D2_MASK; \
						   D3_DDR &= ~D3_MASK; \
						   D4_DDR &= ~D4_MASK; \
						   D5_DDR &= ~D5_MASK; \
						   D6_DDR &= ~D6_MASK; \
						   D7_DDR &= ~D7_MASK;}
						   
#define _DATA_DIR_OUTPUT  {D0_DDR |= D0_MASK; \
						   D1_DDR |= D1_MASK; \
						   D2_DDR |= D2_MASK; \
						   D3_DDR |= D3_MASK; \
						   D4_DDR |= D4_MASK; \
						   D5_DDR |= D5_MASK; \
						   D6_DDR |= D6_MASK; \
						   D7_DDR |= D7_MASK;}


#define _READ_DATA(d) {_RD_LOW; \
						delayMicroseconds(2); \
						d = ((D7_PIN & D7_MASK) << 3) | \
							((D6_PIN & D6_MASK) << 3) | \
							((D5_PIN & D5_MASK) >> 2) | \
							((D4_PIN & D4_MASK) >> 1) | \
							((D3_PIN & D3_MASK) >> 2) | \
							((D2_PIN & D2_MASK) >> 2) | \
							((D1_PIN & D1_MASK) >> 5) | \
							((D0_PIN & D0_MASK) >> 5); \
						_RD_HIGH;}	
						

#define _WRITE_DATA(d) {D7_PORT = (D7_PORT & ~D7_MASK) | ((d & 0b10000000) >> 3); \
						D6_PORT = (D6_PORT & ~D6_MASK) | ((d & 0b01000000) >> 3); \
						D5_PORT = (D5_PORT & ~D5_MASK) | ((d & 0b00100000) << 2); \
						D4_PORT = (D4_PORT & ~D4_MASK) | ((d & 0b00010000) << 1); \
						D3_PORT = (D3_PORT & ~D3_MASK) | ((d & 0b00001000) << 2); \
						D2_PORT = (D2_PORT & ~D2_MASK) | ((d & 0b00000100) << 2); \
						D1_PORT = (D1_PORT & ~D1_MASK) | ((d & 0b00000010) << 5); \
						D0_PORT = (D0_PORT & ~D0_MASK) | ((d & 0b00000001) << 5); \
						_WR_PULSE;}
				
		
						
#define TOUCH_XP_PORT PORTH
#define TOUCH_YP_PORT PORTF
#define TOUCH_XN_PORT PORTF
#define TOUCH_YN_PORT PORTH

#define TOUCH_XP_DDR DDRH
#define TOUCH_YP_DDR DDRF
#define TOUCH_XN_DDR DDRF
#define TOUCH_YN_DDR DDRH

#define TOUCH_XP_MASK 0b00001000
#define TOUCH_YP_MASK 0b00000010
#define TOUCH_XN_MASK 0b00000100
#define TOUCH_YN_MASK 0b00010000

#define TOUCH_XP_LOW 		TOUCH_XP_PORT &= ~TOUCH_XP_MASK
#define TOUCH_XP_HIGH 		TOUCH_XP_PORT |= TOUCH_XP_MASK

#define TOUCH_YP_LOW  		TOUCH_YP_PORT &= ~TOUCH_YP_MASK
#define TOUCH_YP_HIGH  		TOUCH_YP_PORT |= TOUCH_YP_MASK

#define TOUCH_XN_LOW  		TOUCH_XN_PORT &= ~TOUCH_XN_MASK
#define TOUCH_XN_HIGH  		TOUCH_XN_PORT |= TOUCH_XN_MASK

#define TOUCH_YN_LOW  		TOUCH_YN_PORT &= ~TOUCH_YN_MASK
#define TOUCH_YN_HIGH  		TOUCH_YN_PORT |= TOUCH_YN_MASK

#define TOUCH_Y_ANALOGUE	A1
#define TOUCH_X_ANALOGUE	A2



/* Shield pin definitions for ATMega32U4 (Leonardo) */
#elif defined(__AVR_ATmega32U4__)

#define D0_DDR DDRB
#define D1_DDR DDRB
#define D2_DDR DDRB
#define D3_DDR DDRB
#define D4_DDR DDRD
#define D5_DDR DDRC
#define D6_DDR DDRD
#define D7_DDR DDRE

#define D0_PORT PORTB
#define D1_PORT PORTB
#define D2_PORT PORTB
#define D3_PORT PORTB
#define D4_PORT PORTD
#define D5_PORT PORTC
#define D6_PORT PORTD
#define D7_PORT PORTE

#define D0_PIN PINB
#define D1_PIN PINB
#define D2_PIN PINB
#define D3_PIN PINB
#define D4_PIN PIND
#define D5_PIN PINC
#define D6_PIN PIND
#define D7_PIN PINE

#define D0_MASK	0b00010000
#define D1_MASK	0b00100000
#define D2_MASK	0b01000000
#define D3_MASK	0b10000000
#define D4_MASK	0b00010000
#define D5_MASK	0b10000000
#define D6_MASK 0b10000000
#define D7_MASK	0b01000000

#define RD_DDR DDRF
#define WR_DDR DDRF
#define RS_DDR DDRF
#define CS_DDR DDRF

#define RD_PORT PORTF
#define WR_PORT PORTF
#define RS_PORT PORTF
#define CS_PORT PORTF

#define RD_MASK 0b10000000
#define WR_MASK 0b01000000
#define RS_MASK 0b00100000
#define CS_MASK 0b00010000

#define _RD_LOW  RD_PORT &= ~RD_MASK
#define _RD_HIGH RD_PORT |= RD_MASK

#define _WR_LOW  WR_PORT &= ~WR_MASK
#define _WR_HIGH WR_PORT |= WR_MASK

#define _RS_COMMAND  RS_PORT &= ~RS_MASK
#define _RS_DATA RS_PORT |= RS_MASK

#define _CS_EN  CS_PORT &= ~CS_MASK
#define _CS_DIS CS_PORT |= CS_MASK

#define _WR_PULSE {_WR_LOW; _WR_HIGH;}

#define _DATA_DIR_INPUT   {D0_DDR &= ~D0_MASK; \
						   D1_DDR &= ~D1_MASK; \
						   D2_DDR &= ~D2_MASK; \
						   D3_DDR &= ~D3_MASK; \
						   D4_DDR &= ~D4_MASK; \
						   D5_DDR &= ~D5_MASK; \
						   D6_DDR &= ~D6_MASK; \
						   D7_DDR &= ~D7_MASK;}
						   
#define _DATA_DIR_OUTPUT  {D0_DDR |= D0_MASK; \
						   D1_DDR |= D1_MASK; \
						   D2_DDR |= D2_MASK; \
						   D3_DDR |= D3_MASK; \
						   D4_DDR |= D4_MASK; \
						   D5_DDR |= D5_MASK; \
						   D6_DDR |= D6_MASK; \
						   D7_DDR |= D7_MASK;}


#define _READ_DATA(d) {_RD_LOW; \
						delayMicroseconds(2); \
						d = ((D7_PIN & D7_MASK) << 1) | \
							((D6_PIN & D6_MASK) >> 1) | \
							((D5_PIN & D5_MASK) >> 2) | \
							((D4_PIN & D4_MASK)) 	  | \
							((D3_PIN & (D3_MASK | D2_MASK | D1_MASK | D0_MASK)) >> 4); \
							_RD_HIGH;}	
						/*	((D3_PIN & D3_MASK) >> 4) | \
							((D2_PIN & D2_MASK) >> 4) | \
							((D1_PIN & D1_MASK) >> 4) | \
							((D0_PIN & D0_MASK) >> 4); \
						_RD_HIGH;}	*/
						
						  
#define _WRITE_DATA(d) {D7_PORT = (D7_PORT & ~D7_MASK) | ((d & 0b10000000) >> 1); \
						D6_PORT = (D6_PORT & ~D6_MASK) | ((d & 0b01000000) << 1); \
						D5_PORT = (D5_PORT & ~D5_MASK) | ((d & 0b00100000) << 2); \
						D4_PORT = (D4_PORT & ~D4_MASK) | ((d & 0b00010000)); 	  \
						D3_PORT = (D3_PORT & ~(D3_MASK | D2_MASK | D1_MASK | D0_MASK)) | ((d & 0b00001111) << 4); \
						_WR_PULSE;}
						/*D3_PORT = (D3_PORT & ~D3_MASK) | ((d & 0b00001000) << 4); \
						D2_PORT = (D2_PORT & ~D2_MASK) | ((d & 0b00000100) << 4); \
						D1_PORT = (D1_PORT & ~D1_MASK) | ((d & 0b00000010) << 4); \
						D0_PORT = (D0_PORT & ~D0_MASK) | ((d & 0b00000001) << 4); \*/
						
						
#define TOUCH_XP_PORT PORTD
#define TOUCH_YP_PORT PORTF
#define TOUCH_XN_PORT PORTF
#define TOUCH_YN_PORT PORTE

#define TOUCH_XP_DDR DDRD
#define TOUCH_YP_DDR DDRF
#define TOUCH_XN_DDR DDRF
#define TOUCH_YN_DDR DDRE

#define TOUCH_XP_MASK 0b10000000
#define TOUCH_YP_MASK 0b01000000
#define TOUCH_XN_MASK 0b00100000
#define TOUCH_YN_MASK 0b01000000

#define TOUCH_XP_LOW 		TOUCH_XP_PORT &= ~TOUCH_XP_MASK
#define TOUCH_XP_HIGH 		TOUCH_XP_PORT |= TOUCH_XP_MASK

#define TOUCH_YP_LOW  		TOUCH_YP_PORT &= ~TOUCH_YP_MASK
#define TOUCH_YP_HIGH  		TOUCH_YP_PORT |= TOUCH_YP_MASK

#define TOUCH_XN_LOW  		TOUCH_XN_PORT &= ~TOUCH_XN_MASK
#define TOUCH_XN_HIGH  		TOUCH_XN_PORT |= TOUCH_XN_MASK

#define TOUCH_YN_LOW  		TOUCH_YN_PORT &= ~TOUCH_YN_MASK
#define TOUCH_YN_HIGH  		TOUCH_YN_PORT |= TOUCH_YN_MASK

#define TOUCH_Y_ANALOGUE	A1
#define TOUCH_X_ANALOGUE	A2

	
#endif




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
  void DInit(void);
  void DUpdateDisplay(void);
  void DReset(void);
  void DSleep(boolean mode);
  void DScreen(boolean mode);
  void DBacklight(boolean mode);
  void DFlip(uint8_t mode);
  void DSetFG(uint8_t r, uint8_t g, uint8_t b);
  void DSetFG(boolean colour);
  void DSetBG(uint8_t r, uint8_t g, uint8_t b);
  void DSetBG(boolean colour);
  void DClear(void);
  void DErase(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void DPlot(int16_t x, int16_t y);
  void DRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void DDrawMode(uint8_t Mode);
  void DSetXY(uint16_t X, uint16_t Y);
  void DScaleXY(uint8_t ScaleX, uint8_t ScaleY);
  void DBWBitmap(uint16_t Cols, uint8_t ByteRows, const uint8_t BitmapData[], boolean Background = true);
  void DSetFont(const FONT_INFO *NewFont);
  void DWriteChar(char character, boolean Background = true);
  void DReadPixelRGB(uint16_t x, uint16_t y, uint8_t *Data);
  boolean DReadPixel(uint16_t x, uint16_t y);
  uint16_t DResX(void);
  uint16_t DResY(void);
  void DContrast(uint8_t mode);
  
  
  private:
  void _SetWriteArea(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
  void _WriteRegister(uint8_t command, uint8_t data);
  void _Init_Screen(void);
  void _WriteCommand(uint8_t command);
  void _WriteData(uint8_t data);
  void _DWriteCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t *Data);
  void _PowerMode(boolean mode);
  void _DPlotCol(int16_t x, int16_t y, uint8_t ByteRows, const uint8_t *Data);
 
  uint16_t _FGColour = 0xFFFF, _BGColour = 0x0000, _res_x = RES_X, _res_y = RES_Y;
  int16_t _X1Pos, _Y1Pos, _X2Pos, _Y2Pos;
  uint8_t _ScaleX = 1, _ScaleY = 1, _FlipMode = 0;

  
  const FONT_INFO *Font;
};


class Touch
{
	public:
	Touch(void);
	void TInit(uint16_t Cal_X_Min, uint16_t Cal_X_Max, uint16_t Cal_Y_Min, uint16_t Cal_Y_Max, uint8_t Mapping);
	uint16_t TReadRaw(boolean Axis);
	boolean TPressed(void);
	void TReadTouch(void);
	uint16_t TGetCoord(boolean Axis);
	void SetScreenRes(uint16_t Res_X, uint16_t Res_Y); 
	
	
	private:
	uint8_t _Touch_Mapping;
	uint16_t _Cal_X_Min, _Cal_X_Max, _Cal_Y_Min, _Cal_Y_Max, _TouchX, _TouchY;
	uint16_t _res_x = RES_X, _res_y = RES_Y;
};

#endif