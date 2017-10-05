// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once
#ifndef CODEWORD_H_
#define CODEWORD_H_

namespace qrgen {
	/*
	This object is used to create a table of code word to maintain if  a pixel replaced in this code word
	if true, all pixels in this code word will obtain the greatest priority
	*/
	class CodeWord {
	private:
		bool is_invert = false;
	public:
		void setTrue() { is_invert = true; }
		bool getStatus() { return is_invert; }
	};
}

#endif // !PIXELINVERT_H_


