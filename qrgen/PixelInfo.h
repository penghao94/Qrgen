// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#ifndef PIXELINFO_H
#define PIXELINFO_H
#include "BitBlock.h"
#include "Pixel.h"
#include "Property.h"
namespace qrgen {
	class PixelInfo {
	private:
		/*Qrcode position information,however x is column and y is row*/
		int x, y;

		/*pixel preparing information
		* ----------------------------------------------------------------------------------
		| 26bits for offset | 4bits for Role | 1bit for invert | 1bit for black or white |
		* ----------------------------------------------------------------------------------
		*/
		Pixel pixel;

		/*What's the fuck ?*/
		uint8_t target;

		/*I don't know what's the fuck, may be I will know in the future*/
		int ditherTarget;

		/*why need contrast?*/
		int contrast;

		/*what's the fuck?*/
		bool hardZero;

		/*This shit has not be written, I will write it later*/
		BitBlock block;

		/*Indices? */
		int bitIndex;
	public:
		/*
		Fuck constructor
		@param x column
		@param y row
		@param p pixel
		@param t target
		@param c contrast
		*/
		PixelInfo() { x = 0, y = 1, ditherTarget = 0, contrast = 0, hardZero = false, bitIndex = 0; }
		
		PixelInfo(int x, int y, Pixel &p, uint8_t t, int c) { this->x = x; this->y = y; pixel = p; target = t; contrast = c; }

		/*fucking getter and setter*/
		PROPERTY(int, X, x);

		PROPERTY(int, Y, y);

		PROPERTY(Pixel, Pixel, pixel);
		
		PROPERTY(uint8_t, Target, target);

		PROPERTY(BitBlock, Block, block);

		PROPERTY(int, DitherTarget, ditherTarget);

		PROPERTY(bool, HardZero, hardZero);

		PROPERTY(int, Contrast, contrast);

		PROPERTY(int, BitIndex, bitIndex);
	};
}
#endif // !PIXELINFO_H

