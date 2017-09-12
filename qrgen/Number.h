// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef NUMBER_H_
#define NUMBER_H_
#include "EncodeInterface.h"
#include<string>
namespace qrgen{
	class Number :public Encoding {
	public:
		/*Constructor*/
		Number() { number = ""; }
		Number(std::string num) { number = num; }

		std::string validate();
		int availableBits(Version *ver);
		void encode(Bits& bits, Version *ver);

	private:
		std::string number;
	};

}
#endif // !NUMBER_H_

