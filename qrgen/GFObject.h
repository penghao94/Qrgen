#pragma once
// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef GFOBJECT_H_
#define GFOBJECT_H_
#include "GenericGF.h"
namespace gfobj {
	qrgen::GenericGF AZTEC_DATA_12 = qrgen::GenericGF(0x1069, 4096, 1);// x^12 + x^6 + x^5 + x^3 + 1
	qrgen::GenericGF AZTEC_DATA_10 = qrgen::GenericGF(0x409, 1024, 1);// x^10 + x^3 + 1
	qrgen::GenericGF AZTEC_DATA_6 = qrgen::GenericGF(0x43, 64, 1);// x^6 + x + 1
	qrgen::GenericGF AZTEC_PARAM = qrgen::GenericGF(0x13, 16, 1);// x^4 + x + 1
	qrgen::GenericGF QR_CODE_FIELD_256 = qrgen::GenericGF(0x011D, 256, 0);// x^8 + x^4 + x^3 + x^2 + 1
}
#endif // !GFOBJECT_H_

