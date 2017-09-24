
// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef RSUTIL_H
#define RSUTIL_H

#include "Reed_Solomon_encoder.h"
#include <vector>
namespace qrgen {
	typedef std::vector<unsigned char> Bytes;
	/*Generate Reed Solomon check bytes
	* encoder	Reed Solomon encoder
	* dBytes	Data bytes
	* pos		position
	* len		length
	* num_Ecb	number of error correction bytes
	*/
	Bytes geneECBytes(RSencoder encoder, const Bytes &dBytes, int pos, int len, int num_Ecb);
}
#endif // !RSUTIL_H
