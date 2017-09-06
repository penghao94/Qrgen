// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef MASK_H
#define MASK_H

namespace qrgen{
	class Mask{
	private:
		int mask;
	public:
		Mask(int m){ mask = m; }//constructor
		/*getter and setter*/
		int getMask(){ return mask; }
		void setMask(int m){ mask = m; }

		/*adjust whether invert or not*/
		bool shouldInv(int y, int x){
			switch (mask) {
			case 0:
				return (y + x) % 2 == 0;
			case 1:
				return y % 2 == 0;
			case 2:
				return x % 3 == 0;
			case 3:
				return (y + x) % 3 == 0;
			case 4:
				return (y / 2 + x / 3) % 2 == 0;
			case 5:
				return y*x % 2 + y*x % 3 == 0;
			case 6:
				return (y*x % 2 + y*x % 3) % 2 == 0;
			case 7:
				return (y*x % 3 + (y + x) % 2) % 2 == 0;
			}
			return false;
		}
	};
}
#endif  //MASK_H