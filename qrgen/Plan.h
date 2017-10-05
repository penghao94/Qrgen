// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#ifndef PLAN_H
#define PLAN_H
#include "Version.h"
#include "Level.h"
#include "Mask.h"
#include "Pixel.h"
#include "QRCode.h"
#include "EncodeInterface.h"
namespace qrgen{

	class Plan{
	private:
		Version* version;
		LEVEL level;
		Mask *mask;

		int num_of_dbytes;//number of data bytes
		int num_of_cbytes;//number of check bytes
		int num_of_blocks;//number of blicks

		std::vector < std::vector<Pixel>>pixels; //Matrics of pixel information

	public:

		/*constructor*/
		Plan(){}
		~Plan(){ pixels.clear(); pixels.swap(std::vector < std::vector<Pixel>>()); }
		/*getters and setters*/

		Version* getVersion(){ return version; }
		void setVersion(Version *v){ version = v; }

		LEVEL getLevel(){ return level; }
		void setLevel(LEVEL l){ level = l; }

		Mask* getMask(){ return mask; }
		void setMasK(Mask* m){ mask = m; }

		int getdBytesNum(){ return num_of_dbytes; }
		void setdBytesNum(int n){ num_of_dbytes = n; }

		int getcBytesNum(){ return num_of_cbytes; }
		void setcBytesNum(int n){ num_of_cbytes = n; }

		int getBlocksNum(){ return num_of_blocks; }
		void setBlocksNum(int n){ num_of_blocks = n; }

		std::vector<std::vector<Pixel>> getPixels(){ return pixels; }
		void setPixels(std::vector<std::vector<Pixel>>& p){ pixels = p; }

		/*Add version information*/
		static Plan* verPlan(Version *v);

		

		/*Add format information*/
		static void formatPlan(Plan * p, LEVEL l, Mask * m);

		/*Add error correction level information*/
		static void lvlPlan(Plan * p, Version * v, LEVEL l);

		/*Add mask information*/
		static void maskPlan(Plan *p, Mask *m);

		/*combine information*/
		static Plan* newPlan(Version* v, LEVEL l, Mask* m);

		static QRCode encode(Plan *plan, Encoding &encode1,Encoding &encode2);
	private:
		/*Set position box*/
		static void setPosBox(std::vector<std::vector<Pixel>>& pixels, int x, int y);

		/*Set align box*/
		static void setAlignBox(std::vector<std::vector<Pixel>>&pixels, int x, int y);
	};
}
#endif //PLAN_H