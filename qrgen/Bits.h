// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once
#ifndef BITS_H
#define BITS_H
#include <vector>
#include <cassert>

#include "Version.h"
#include "Level.h"
namespace qrgen {
	typedef std::vector<uint8_t> Bytes;

	class Bits {
	private:
		Bytes bits; //array of code bytes
		int size;//size of code bytes

	public:
		/*constructor
		*
		* @param b		array of code bytes
		* @param s		size of code bytes
		*/
		Bits() { bits.resize(0); size = 0;}
		Bits(Bytes b, int s) { bits = b; size = s; }

		~Bits() { bits.swap(Bytes()); size = 0; }

		void reset() { bits.clear(); size = 0;}//reset

		int getSize() { return size; }//return size

		Bytes getBits() {  return bits; }//return bits

		bool get(int i) { return (bits[i / 8] & (1 << (7 - i & 0x07))) != 0; }// return is 1 or not at index i

		void append(bool bit);//add a bit in the end

		void append(Bits other);// add another bits array in the end

	 /**
	 * Appends the least-significant bits, from value, in order from most-significant to
	 * least-significant. For example, appending 6 bits from 0x000001E will append the bits
	 * 0, 1, 1, 1, 1, 0 in that order.
	 *
	 * @param v {@code int} containing bits to append
	 * @param numBits bits from value to append
	 */
		void write(int v, int numBits);

		void pad(int n);//add padding  with n size

		/**
		*Generate Reed Solomon Check bytes and append at the end of date bytes
		*@param ver 	the version of qrcode
		*@param lvl 	the level of qrcode
		**/
		void addCheckBytes(Version *ver, LEVEL lvl); 

	private:

		void ensureCapacity(int size);//capacity check

	};
}

#endif // !BITS_H
