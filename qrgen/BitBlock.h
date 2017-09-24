// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef BITBLOCK_H
#define BITBLOCK_H

//#include <cstdint>
#include <vector>

#include "Reed_Solomon_encoder.h"
namespace qrgen {

	typedef std::vector<uint8_t> Bytes;

	class BitBlock {
	private:
		int num_dBytes;
		int num_cBytes;
		Bytes blockBytes;
		std::vector<Bytes> maskMatrix;
		int maskIndex;
		RSencoder encoder;
		Bytes pridBytes;
		int pridIndex;
		Bytes pricBytes;
		int pricIndex;

	public:
		/*constructor*/
		BitBlock() {}
		BitBlock(int num_dBytes, int num_cBytes, RSencoder &encoder, Bytes &pridBytes, int pridIndex, Bytes &pricBytes, int pricIndex);
		~BitBlock() { pridBytes.swap(Bytes()); pricBytes.swap(Bytes()); maskMatrix.swap(std::vector<Bytes>()); }
		Bytes getBlockBytes() { return blockBytes; }

		void check();

		void reset(int index, uint8_t value);

		bool canSet(int index, uint8_t value);
		bool isEmpty();

		void copyOut();

		void exchangeRow(std::vector<Bytes> &matrix, int i, int j);
	};
}
#endif // !BITBLOCK_H
