// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef LEVEL_H
#define LEVEL_H

namespace qrgen{
	/*Error Correct Level 
	* L LOW
	* M MEDIUM
	* Q QUARTILE
	* H HIGH
	*/
	enum LEVEL
	{
		L, M, Q, H
	};
}
#endif  //LEVEL_H