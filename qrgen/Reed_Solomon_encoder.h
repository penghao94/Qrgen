// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma  once
#ifndef RSENCODER_H
#define RSENCODER_H
#include "GenericGF.h"
#include "GenericGFPoly.h"
#include <vector>
#include <algorithm>
namespace qrgen {
	/**
	* <p>Implements Reed-Solomon enbcoding, as the name implies.</p>
	*
	* @author Sean Owen
	* @author William Rucklidge
	*
	*A java version rewrote by C++
	*/
	class RSencoder final {
	private:
		GenericGF *field;
		std::vector<GenericGFPoly*> cacheGen;

		/* return the last Generator*/
		GenericGFPoly * buildGenerator(int d);

	public:
		/*Construct a Reed Solomon encoder */
		qrgen::RSencoder::RSencoder() {};
		RSencoder(GenericGF *field);

		/*encode*/
		void encode(std::vector<int> &E, int ecBytes);
	};
}
#endif // !RSENCODER_H
