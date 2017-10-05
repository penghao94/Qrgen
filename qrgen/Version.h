// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#pragma once

#ifndef VERSION_H
#define VERSION_H
#include "Level.h"
#include <vector>
namespace qrgen{

	/*templete of Version Level Information*/
	struct VerLvlInfo
	{
		int num_of_block;		//number of blocks
		int cbytes_pre_block;	// number of check bytes pre block
	};

	/*templete of Version informations*/
	struct VerInfo
	{
		int apos;
		int stride;
		int bytes;
		int pattern;
		std::vector<VerLvlInfo> lvlInfos;
	};
	class Version {
	public:
		const static int MIN_VERSION = 1;
		const static int MAX_VERSION = 40;
		static bool _init;
		static std::vector<VerInfo> VERSION_INFOS;
	private:
		int version;

	public:
		Version(int v); //Version constructor

		int getVersion();	

		void setVersion(int v);

		int getSize();	//get Extra size

		/* returns the number of data bytes that can be stored in a QR code with the given version and level*/
		int dataBytes(LEVEL l);

		static bool init();
	};

}
#endif //VERSION_H