// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include<limits.h>

#include<vector>
#include<string>
#include<algorithm>
#include "Property.h"
#include "Pixel.h"
#include "PixelInfo.h"
#include "PixelOrder.h"
#include "Plan.h"
#include "QRCode.h"
#include "Bits.h"
#include "Reed_Solomon_encoder.h"
#include "Raw.h"
#include "Number.h"
typedef std::vector<uint8_t> Bytes;

namespace qrgen {
	struct Target
	{
		uint8_t targt;
		int contrast;
	};
	class Generator {
	 private:
		 std::vector<std::vector<int>> target;
		 int divider;
		 int dx, dy;
		 std::string text;
		 int version;
		 int mask;
		 int rotation;
		 bool rand_control;
		 long seed;
		 bool dither;
		 bool only_data_bits;
		 bool save_control;
		 Bytes control;

	public:
		/*constructor*/

		Generator(std::vector<std::vector<int>>& target, int dx, int dy, int version);
		
		Generator(std::vector<std::vector<int>>&target, int dx, int dy, std::string text, 
			int version, int mask, int rotation, bool rand_control, long seed, bool dither,
			bool only_data_bits, bool save_control);

		/*Property getter and setter*/
		PROPERTY(std::vector<std::vector<int>>&, Target, target);

		PROPERTY(int, Divider, divider);

		PROPERTY(int, Dx, dx);

		PROPERTY(int, Dy, dy);

		PROPERTY(std::string, Text, text);

		PROPERTY(int, Version, version);

		PROPERTY(int, Mask, mask);

		PROPERTY(int, Rotation, rotation);

		PROPERTY(bool, RandControl, rand_control);

		PROPERTY(long, Seed, seed);

		PROPERTY(bool, Dither, dither);

		PROPERTY(bool, OnlyDataBits, only_data_bits);

		PROPERTY(bool, SaveControl, save_control);

		PROPERTY(Bytes&, Control, control);
		
		/* return average and variance at (x,y) with range */
		Target targt(int x, int y);

		void rotate(Plan *plan, int rotation);

		

		QRCode encode();

	private:
		int calculateDivider();

		void addDither(std::vector<PixelInfo>&pixel_by_offset,Pixel &pixel,int error);
	};
}

#endif // !GENERATOR_H_

