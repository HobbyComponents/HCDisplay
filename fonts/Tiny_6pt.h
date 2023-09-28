// A tiny 6pt proportional font thanks to forum user ChrisSharp for creating this font


#ifndef Tiny_6pt_h
#define Tiny_6pt_h
#include <avr/pgmspace.h>
#include "fonts/fonts.h"

// Character bitmaps for Large Prop 26pt font
const PROGMEM uint8_t Tiny_6pt_Bitmaps[] = 
{
	// @0 ' ' (2 pixels wide)
	//   
	//   
	//   
	//   
	//   
	//   
	//   
	//   
	0x00, 0x00, 

	// @2 '!' (1 pixels wide)
	// #
	// #
	// #
	// #
	//  
	// #
	//  
	//  
	0x2F, 

	// @3 '"' (3 pixels wide)
	// # #
	// # #
	//   
	//   
	//   
	//   
	//   
	//   
	0x03, 0x00, 0x03, 

	// @5 '#' (5 pixels wide)
	//   
	//  # #
	// #####
	//  # #
	// #####
	//  # #
	//    
	//    
	 0x14, 0x3E, 0x14, 0x3E, 0x14,

	// @8 '$' (4 pixels wide)
	//   # 
	//  ###
	// # #  
	// ### 
	//  # #
	// ### 
	//  #  
	//     
	0x2C, 0x7A, 0x2F, 0x12, 

	// @12 '%' (7 pixels wide)
	// ###  #
	// # # #  
	// ####  
	//   ####
	//  # # #
	// #  ###
	//        
	//        
	0x27, 0x15, 0x0F, 0x3C, 0x2A, 0x39, 0x00, 

	// @19 '&' (6 pixels wide)
	//  ###  
	// #  #  
	//  ## # 
	//  # # 
	// #   # 
	//  ### #
	//       
	//       
	0x12, 0x2D, 0x25, 0x2B, 0x14, 0x20, 

	// @25 ''' (1 pixels wide)
	// #
	// #
	//  
	//  
	//  
	//  
	//  
	//  
	0x03, 

	// @26 '(' (2 pixels wide)
	//  #
	// # 
	// # 
	// # 
	// # 
	// # 
	//  # 
	//  
	0x3E, 0x41, 

	// @28 ')' (2 pixels wide)
	// # 
	//  #
	//  #
	//  #
	//  #
	//  #
	// # 
	//  
	0x41, 0x3E, 

	// @30 '*' (3 pixels wide)
	// # # 
	//  #
	// # # 
	// 
	// 
	//     
	//    
	//    
	0x05, 0x02, 0x05, 

	// @33 '+' (3 pixels wide)
	//      
	//   
	//  #  
	// ###
	//  #  
	//   
	//      
	//      
	0x08, 0x1C, 0x08, 

	// @38 ',' (2 pixels wide)
	//  
	//  
	//  
	//  
	//  
	//  #
	// #
	//  
	0x40,0x20, 

	// @39 '-' (3 pixels wide)
	//   
	//   
	//   
	// ###
	//   
	//   
	//   
	//   
	0x08, 0x08, 0x08, 

	// @41 '.' (1 pixels wide)
	//  
	//  
	//  
	//  
	//  
	// #
	//  
	//  
	0x20, 

	// @42 '/' (3 pixels wide)
	//   
	//   #
	//   #
	//  # 
	//  # 
	// #  
	// #  
	//    
	0x60, 0x18, 0x06, 

	// @45 '0' (3 pixels wide)
	//  # 
	// # # 
	// # #
	// # #
	// # #
	//  # 
	//     
	//     
	0x1E, 0x21, 0x1E, 

	// @49 '1' (3 pixels wide)
	//  # 
	// ## 
	//  # 
	//  # 
	//  # 
	// ###
	//    
	//    
	0x22, 0x3F, 0x20, 

	// @52 '2' (3 pixels wide)
	//  ## 
	//    # 
	//   # 
	//  #  
	//  #   
	//  ###
	//     
	//     
	0x39, 0x25, 0x22, 

	// @56 '3' (3 pixels wide)
	// ##
	//   #
	// ##
	//   #
	//   #
	// ##
	//    
	//    
	0x25, 0x25, 0x1A, 

	// @59 '4' (4 pixels wide)
	//    #
	//   ##
	//  # #
	// #  #
	// ####
	//    #
	//     
	//     
	0x18, 0x14, 0x12, 0x3F, 

	// @63 '5' (3 pixels wide)
	// ###
	// #   
	// ##  
	//   # 
	//   #
	// ## 
	//     
	//     
	0x27, 0x25, 0x19, 

	// @67 '6' (4 pixels wide)
	//  ## 
	// #   
	// ### 
	// #  #
	// #  #
	//  ## 
	//     
	//     
	0x1E, 0x25, 0x25, 0x18, 

	// @71 '7' (3 pixels wide)
	// ###
	//   # 
	//  # 
	// #  
	// #  
	// #   
	//     
	//     
	0x39, 0x05, 0x03, 

	// @75 '8' (4 pixels wide)
	//  ## 
	// #  #
	//  ## 
	// #  # 
	// #  #
	//  ## 
	//      
	//      
	0x1A, 0x25, 0x25, 0x1A, 

	// @80 '9' (4 pixels wide)
	//  ## 
	// #  # 
	//  ###
	//    #
	//   # 
	//  # 
	//      
	//      
	0x02, 0x25, 0x15, 0x0E, 

	// @85 ':' (1 pixels wide)
	//  
	//  
	// #
	//  
	//  
	// #
	//  
	//  
	0x24, 

	// @86 ';' (1 pixels wide)
	//  
	//  
	// #
	//  
	//  
	// #
	// #
	//  
	0x64, 

	// @87 '<' (3 pixels wide)
	//      
	//   #
	//  #  
	// #    
	//  # 
	//   #
	//      
	//      
	0x08, 0x14, 0x22, 

	// @92 '=' (3 pixels wide)
	//      
	//      
	// ###
	//      
	// ###
	//      
	//      
	//      
	0x14, 0x14, 0x14,

	// @97 '>' (3 pixels wide)
	//      
	// #    
	//  #  
	//   #
	//  #  
	// #    
	//      
	//      
	0x22, 0x14, 0x08, 

	// @102 '?' (3 pixels wide)
	// ## 
	//   #
	// ##
	// # 
	//    
	// # 
	//    
	//    
	0x2D, 0x05, 0x02, 

	// @105 '@' (7 pixels wide)
	//   ### 
	//  #   #
	// #  ## #
	// # #  ##
	// # ####
	//  #    
	//   #### 
	//       
	0x1C, 0x22, 0x59, 0x55, 0x55, 0x5E, 0x0C,

	// @111 'A' (5 pixels wide)
	//   #  
	//  # # 
	// #   # 
	// #####
	// #   #
	// #   #
	//      
	//      
	0x3C, 0x0A, 0x09, 0x0A, 0x3C, 

	// @116 'B' (4 pixels wide)
	// ### 
	// #  #
	// ### 
	// #  #
	// #  #
	// ###
	//     
	//     
	0x3F, 0x25, 0x25, 0x1A, 

	// @120 'C' (3 pixels wide)
	//  ##
	// #   
	// #    
	// #    
	// #   
	//  ##
	//      
	//      
	0x1E, 0x21,  0x21,

	// @125 'D' (4 pixels wide)
	// ###  
	// #  # 
	// #  #
	// #  #
	// #  # 
	// ###  
	//      
	//      
	0x3F, 0x21, 0x21, 0x1E, 

	// @130 'E' (3 pixels wide)
	// ###
	// #   
	// ###
	// #   
	// #   
	// ###
	//     
	//     
	0x3F, 0x25, 0x25, 

	// @134 'F' (3 pixels wide)
	// ###
	// #   
	// ###  
	// #   
	// #   
	// #   
	//     
	//     
	0x3F, 0x05, 0x05,

	// @138 'G' (5 pixels wide)
	//  ## 
	// #  #
	// #    
	// # ##
	// #  #
	//  ###
	//       
	//       
	0x1E, 0x21, 0x29, 0x3A, 

	// @144 'H' (3 pixels wide)
	// # #
	// # #
	// ###
	// # #
	// # #
	// # #
	//     
	//     
	0x3F, 0x04, 0x3F, 

	// @148 'I' (3 pixels wide)
	// ###
	//  # 
	//  # 
	//  # 
	//  # 
	// ###
	//   
	//   
	 0x21, 0x3F, 0x21, 

	// @150 'J' (3 pixels wide)
	//  ##
	//   #
	//   #
	//   #
	//   #
	// ###
	//    
	//    
	0x20, 0x21, 0x3F, 

	// @153 'K' (3 pixels wide)
	// # #
	// # # 
	// ##  
	// ##  
	// # # 
	// # #
	//     
	//     
	0x3F, 0x0C, 0x33, 

	// @157 'L' (3 pixels wide)
	// #  
	// #  
	// #  
	// #  
	// #  
	// ###
	//    
	//    
	0x3F, 0x20, 0x20, 

	// @160 'M' (5 pixels wide)
	// #   #
	// ## ##
	// # # #
	// #   #
	// #   #
	// #   #
	//      
	//      
	0x3F, 0x02, 0x04, 0x02, 0x3F, 

	// @165 'N' (4 pixels wide)
	// #  #
	// ## #
	// ## #
	// # ##
	// # ##
	// #  #
	//     
	//     
	0x3F, 0x06, 0x18, 0x3F, 

	// @169 'O' (4 pixels wide)
	//  ## 
	// #  #
	// #  #
	// #  #
	// #  #
	//  ## 
	//       
	//       
	0x1E, 0x21, 0x21, 0x1E, 

	// @175 'P' (4 pixels wide)
	// ### 
	// #  #
	// #  #
	// ### 
	// #   
	// #   
	//     
	//     
	0x3F, 0x09, 0x09, 0x06, 

	// @179 'Q' (4 pixels wide)
	//  ## 
	// #  #
	// #  #
	// #  #
	// #  #
	//  ##
	//   #  
	//    
	0x1E, 0x21, 0x61, 0x1E, 

	// @185 'R' (4 pixels wide)
	// ### 
	// #  #
	// #  #
	// ### 
	// # # 
	// #  #
	//     
	//     
	0x3F, 0x09, 0x19, 0x26, 

	// @189 'S' (4 pixels wide)
	//  ###
	// #   
	//  ## 
	//    #
	//    #
	// ### 
	//     
	//     
	0x22, 0x25, 0x25, 0x19, 

	// @193 'T' (3 pixels wide)
	// ###
	//  #  
	//  #  
	//  #  
	//  #  
	//  #  
	//      
	//      
	0x01, 0x3F, 0x01, 

	// @198 'U' (4 pixels wide)
	// #  #
	// #  #
	// #  #
	// #  #
	// #  #
	//  ##
	//     
	//     
	0x1F, 0x20, 0x20, 0x1F, 

	// @202 'V' (5 pixels wide)
	// #   #
	// #   #
	//  # #
	//  # # 
	//   # 
	//   #  
	//      
	//      
	0x03, 0x0C, 0x30, 0x0C, 0x03, 

	// @207 'W' (5 pixels wide)
	// #   #
	// #   # 
	// # # # 
	// # # # 
	// # # #  
	//  # #  
	//         
	//         
	0x1F, 0x20, 0x1C, 0x20, 0x1F, 

	// @215 'X' (5 pixels wide)
	// #   #
	//  # # 
	//   # 
	//   # 
	//  # # 
	// #   #
	//      
	//      
	0x21, 0x12, 0x0C, 0x12, 0x21, 

	// @220 'Y' (5 pixels wide)
	// #   #
	// #   # 
	//  # # 
	//   #  
	//   #  
	//   #  
	//      
	//      
	0x03, 0x04, 0x38, 0x04, 0x03, 

	// @225 'Z' (4 pixels wide)
	// ####
	//    # 
	//   # 
	//  #  
	// #   
	// ####
	//      
	//      
	0x31, 0x29, 0x25, 0x23, 

	// @230 '[' (2 pixels wide)
	// ##
	// # 
	// # 
	// # 
	// # 
	// # 
	// # 
	// ##
	0xFF, 0x81, 

	// @232 '\' (4 pixels wide)
	// #   
	// #  
	//  #  
	//  #  
	//   # 
	//   # 
	//    #
	//    # 
	0x03, 0x0C, 0x30, 0x80, 

	// @236 ']' (2 pixels wide)
	// ##
	//  #
	//  #
	//  #
	//  #
	//  #
	//  #
	// ##
	0x81, 0xFF, 

	// @238 '^' (3 pixels wide)
	//  # 
	// # #
	// 
	//     
	//     
	//     
	//     
	//     
	 0x02, 0x01, 0x02, 

	// @242 '_' (4 pixels wide)
	//      
	//      
	//      
	//      
	//      
	//      
	// ####
	//      
	0x40, 0x40, 0x40, 0x40, 

	// @247 'a' (4 pixels wide)
	//     
	//  ##  
	//    #
	//  ###
	// #  #
	//  ###
	//     
	//     
	0x10, 0x2A, 0x2A, 0x3C, 

	// @251 'b' (3 pixels wide)
	// #   
	// #  
	// ## 
	// # #
	// # #
	// ## 
	//     
	//     
	0x3F, 0x24, 0x18, 

	// @255 'c' (3 pixels wide)
	//     
	//     
	//  ##
	// #   
	// #   
	//  ##
	//     
	//     
	0x18, 0x24, 0x24, 

	// @259 'd' (3 pixels wide)
	//   # 
	//   #
	//  ##
	// # #
	// # #
	//  ##
	//     
	//     
	0x18, 0x24, 0x3F, 

	// @263 'e' (4 pixels wide)
	//     
	//    
	//  ##
	// ####
	// #   
	//  ##
	//     
	//     
	0x18, 0x2C, 0x2C, 0x08, 

	// @267 'f' (2 pixels wide)
	//  #
	// # 
	// ## 
	// # 
	// # 
	// # 
	//   
	//   
	0x3E, 0x05, 

	// @269 'g' (4 pixels wide)
	//     
	//     
	//  ###
	// #  #
	// #  #
	//  ###
	//    #
	//  ## 
	0x18, 0xA4, 0xA4, 0x7C, 

	// @273 'h' (3 pixels wide)
	// #   
	// # 
	// ##
	// # #
	// # #
	// # # 
	//    
	//    
	0x3F, 0x04, 0x38, 

	// @276 'i' (1 pixels wide)
	// 
	// # 
	//  
	// #
	// #
	// # 
	//  
	//  
	0x3A, 

	// @277 'j' (2 pixels wide)
	//  
	//  # 
	// 
	//  #
	//  #
	//  #
	//  #
	// # 
	0x80, 0x7A, 

	// @279 'k' (3 pixels wide)
	// #  
	// #  
	// # #
	// ## 
	// ## 
	// # #
	//    
	//    
	0x3F, 0x18, 0x24, 

	// @282 'l' (1 pixels wide)
	// #
	// #
	// #
	// #
	// #
	// #
	//  
	//  
	0x3F, 

	// @283 'm' (5 pixels wide)
	//       
	//       
	// ## #
	// # # #
	// # # #
	// #   #
	//       
	//       
	0x3C, 0x04, 0x18, 0x04, 0x38, 

	// @289 'n' (3 pixels wide)
	//    
	//    
	// ##
	// # #
	// # #
	// # # 
	//    
	//    
	0x3C, 0x04, 0x38, 

	// @292 'o' (4 pixels wide)
	//     
	//     
	//  ##
	// #  #
	// #  #
	//  ##
	//     
	//     
	0x18, 0x24, 0x24, 0x18, 

	// @296 'p' (3 pixels wide)
	//     
	//     
	// ## 
	// # #
	// # #
	// ## 
	// #   
	// #   
	0xFC, 0x24, 0x18, 

	// @300 'q' (3 pixels wide)
	//     
	//     
	//  ##
	// # #
	// # #
	//  ##
	//   #
	//   # 
	0x18, 0x24, 0xFC, 

	// @304 'r' (1 pixels wide)
	//  
	//  
	//  #
	// #
	// #
	// #
	//  
	//  
	0x38,0x04, 

	// @305 's' (4 pixels wide)
	//     
	//     
	//  ###
	// ## 
	//   ##
	// ###
	//     
	//     
	0x28, 0x2C, 0x34, 0x14, 

	// @309 't' (2 pixels wide)
	// # 
	// # 
	// ## 
	// # 
	// # 
	//  #
	//   
	//   
	0x1F, 0x24, 

	// @311 'u' (3 pixels wide)
	//    
	//    
	// # # 
	// # #
	// # #
	// ###
	//    
	//    
	0x3C, 0x20, 0x3C, 

	// @314 'v' (3 pixels wide)
	//     
	//     
	// # #
	// # #
	// # # 
	//  #
	//     
	//     
	0x1C, 0x20, 0x1C, 

	// @318 'w' (5 pixels wide)
	//       
	//       
	// #   #
	// # # #
	// # # #
	//  # # 
	//       
	//       
	0x1C, 0x20, 0x18, 0x20, 0x1C, 

	// @324 'x' (3 pixels wide)
	//    
	//    
	// # #
	//  # 
	//  #
	// #  #
	//    
	//    
	0x24, 0x18, 0x24, 

	// @327 'y' (3 pixels wide)
	//     
	//     
	// # #
	// # #
	// # #
	//  ## 
	// 	# 
	// #  
	0x9C, 0x60, 0x3C, 

	// @331 'z' (3 pixels wide)
	//     
	//     
	//  ###
	//   # 
	//  #  
	//  ###
	//     
	//     
	0x34, 0x2C, 0x24, 

	// @335 '{' (3 pixels wide)
	//   #
	//  # 
	//  # 
	// #  
	//  # 
	//  # 
	//  # 
	//   #
	0x08, 0x76, 0x81, 

	// @338 '|' (1 pixels wide)
	// #
	// #
	// #
	// #
	// #
	// #
	// #
	// #
	0xFF, 

	// @339 '}' (3 pixels wide)
	// # 
	//  # 
	//  # 
	//   #
	//  # 
	//  # 
	//  # 
	// #  
	0x81, 0x76, 0x08, 

	// @342 '~' (5 pixels wide)
	//      
	//      
	//  #  
	// # # #
	//    # 
	//      
	//      
	//      
	0x08, 0x04, 0x08, 0x10, 0x08, 
};


// Character descriptors for for Large Prop 26pt
// { [Char width in bits], [Offset into MedProp_26ptCharBitmaps in bytes] }
const PROGMEM FONT_CHAR_INFO Tiny_6pt_Descriptors[] = 
{
	{2, 0},	// Space  
	{1, 2},	// ! 
	{3, 3},	// " 
	{5, 6},	// # 
	{4, 11},	// $ 
	{7, 15},	// % 
	{6, 22},	// & 
	{1, 28},	// ' 
	{2, 29},	// ( 
	{2, 31},	// ) 
	{3, 33},	// * 
	{3, 36},	// + 
	{2, 39},	// 
	{3, 41},	// - 
	{1, 44},	// . 
	{3, 45},	// / 
	{3, 48},	// 0
	{3, 51},	// 1
	{3, 54},	// 2
	{3, 57},	// 3
	{4, 60},	// 4
	{3, 64},	// 5
	{4, 67},	// 6
	{3, 71},	// 7
	{4, 74},	// 8
	{4, 78},	// 9
	{1, 82},	// : 
	{1, 83},	// ; 
	{3, 84},	// < 
	{3, 87},	// = 
	{3, 90},	// > 
	{3, 93},	// ? 
	{7, 96},	// @ 
	{5, 103},	// A 
	{4, 108},	// B 
	{3, 112},	// C 
	{4, 115},	// D 
	{3, 119},	// E 
	{3, 122},	// F 
	{4, 125},	// G 
	{3, 129},	// H 
	{3, 132},	// I 
	{3, 135},	// J 
	{3, 138},	// K 
	{3, 141},	// L 
	{5, 144},	// M 
	{4, 149},	// N 
	{4, 153},	// O 
	{4, 157},	// P 
	{4, 161},	// Q 
	{4, 165},	// R 
	{4, 169},	// S 
	{3, 173},	// T 
	{4, 176},	// U 
	{5, 180},	// V 
	{5, 185},	// W 
	{5, 190},	// X 
	{5, 195},	// Y 
	{4, 200},	// Z 
	{2, 204},	// [ 
	{4, 206},	// backslash
	{2, 210},	// ] 
	{3, 212},	// ^ 
	{4, 215},	// _ 
	{0, 219},	// ` 
	{4, 219},	// a 
	{3, 223},	// b 
	{3, 226},	// c 
	{3, 229},	// d 
	{4, 232},	// e 
	{2, 236},	// f 
	{4, 238},	// g 
	{3, 242},	// h 
	{1, 245},	// i 
	{2, 246},	// j 
	{3, 248},	// k 
	{1, 251},	// l 
	{5, 252},	// m 
	{3, 257},	// n 
	{4, 260},	// o 
	{3, 264},	// p 
	{3, 267},	// q 
	{2, 270},	// r 
	{4, 272},	// s 
	{2, 276},	// t 
	{3, 278},	// u 
	{3, 281},	// v 
	{5, 284},	// w 
	{3, 289},	// x 
	{3, 292},	// y 
	{3, 295},	// z 
	{3, 298},	// { 
	{1, 301},	// | 
	{3, 302},	// } 
	{5, 305},	// ~ 
};


// Font information for for Large Prop 26pt
const FONT_INFO Tiny_6ptFont[] =
{
	1, //  Character height
	' ', //  Start character
	'~', //  End character
	1,	// Spacing between characters
	Tiny_6pt_Descriptors, //  Character descriptor array
	Tiny_6pt_Bitmaps, //  Character bitmap array
};

#endif