// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef PIXEL_H
#define PIXEL_H

#include<string>
#include<bitset>

namespace qrgen{

	/**
	*
	* ---------------------------------------------------------------------------------------------------------
	* | 18bits for offset|8 bit for block number | 4bits for Role | 1bit for invert | 1bit for black or white |
	* ---------------------------------------------------------------------------------------------------------
	*
	*/

	class Pixel{
	public:
		enum PixelRole
		{
			UNKNOWN, //not used
			POSITION, //position pattern
			ALIGNMENT,
			TIMING,
			FORMAT,
			VERSION_PATTERN,
			UNUSED,
			DATA,
			CHECK,
			EXTRA
		};
		 static Pixel BLACK;
		static Pixel INVERT;

	private:
		int data;

	public:
		/*constructor*/
		Pixel(){ data = 0; }
		Pixel(int v){ data = v; }

		Pixel(PixelRole pr){ data = pr << 2; }

		Pixel(Pixel *p){ data = p->data; }

		/*pixle getter and setter*/

		int getPixel(){ return data & 0x01; }

		void setPixel(int v){data= (data >> 1 << 1) | (v & 0x01); }

		/*bit block getter and setter*/
		int getBlockIndex() { return (data >> 6) & 0xff; }
		void setBlockIndex(int num) { data = data >> 14 << 14 | (num << 6) | (data & 0x3f); }

		/*offset getter and setter*/
		int getOffset(){ return data >> 14; }

		void setOffset(int v){ data = (v << 14) | (data & 0x3f); }

		/*pixel role getter and setter*/
		PixelRole getPixelRole(){ return (PixelRole)((data >> 2) & 0x0f); }
		
		/*set INVERT and adjust INVERT*/

		bool shouldInv(){ return ((data >> 1) & 0x1) == 1; }

		void setInv(bool inv){ int v = inv ? 1 : 0; data=data >> 2 << 2 | (v << 1) | (data & 0x1); }

		/*OR and XOR operator*/

		void orPixel(int v){ data |= v & 0x01; }

		void xorPixel(int v){ data ^= v & 0x01; }

		int getData(){ return data; }
	};
}


#endif //PIXEL_H