// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once
#ifndef QRCODER_H_
#define QRCODER_H_

#include <string>
#include<sstream>
#include<fstream>
#include "Raw.h"
#include "RSUtil.h"
#include "Bits.h"
#include "Plan.h"

namespace qrgen {
	typedef std::vector<std::vector<qrgen::Pixel>> MatrixP;

	/*Get minimal version size for data */
	Version * getMinVersion(std::string &text, LEVEL level,Bits &bits);
	MatrixP encode(Bits&bits,Version *version, LEVEL level, Mask *mask);
	std::string toSvgString(std::string file,MatrixP &pixels, int border);
	std::string toSvgStringColor(MatrixP & pixels, int border, int num_block);
}
#endif // !QRCODER_H_


