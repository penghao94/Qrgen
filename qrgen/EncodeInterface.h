// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#ifndef ENCODE_INTERFACE_H
#define ENCODE_INTERFACE_H

#include<string>
#include "Version.h"
#include "Bits.h"
namespace qrgen {
	class Encoding {
	 public:
		 virtual std::string validate() = 0;
		 virtual int availableBits(Version *ver) = 0;
		 virtual void encode(Bits& bits, Version *ver) = 0;

	};
}
#endif // !ENCODE_INTERFACE_H

