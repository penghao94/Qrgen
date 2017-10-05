// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once 
#include <vector>
#include <algorithm>
namespace qrgen {
	/*
	This struct is amid to sort and determine which pixel should be inverted
	@param (x,y)    coordinate of qrcode
	@param priority    the priority of bit block  pixel belongs to
	@param area     the 2D visible area of pixel
	@param adaptable   whether pixel can be adapted or not
	*/
	struct PixelProperty {
		int x;
		int y;
		int priority;
		double area;
		double adaptable;
	};

	/*Sort pixel property according to adaptable*/
	void pixel_property_sort(std::vector<PixelProperty>& property) {
		const auto greater = [](PixelProperty p1, PixelProperty p2)->bool {return (p1.adaptable > p2.adaptable); };
		std::stable_sort(property.begin(), property.end());
	}

}
