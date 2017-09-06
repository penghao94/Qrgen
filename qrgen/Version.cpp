#include<vector>
#include "Version.h"

typedef std::vector<qrgen::Version::VerInfo> VerInfos;
typedef std::vector<qrgen::Version::VerLvlInfo> VerLvlInfos;
typedef qrgen::Version::VerLvlInfo LvlInfo;

qrgen::Version::Version(int v){
	version = v;
}

int qrgen::Version::getVersion(){
	return version;
}

void qrgen::Version::setVersion(int v){
	version = v;
}

int qrgen::Version::getSize(){
	if (version <= 9) return 0;
	if (version <= 26) return 1;
	return 2;
}

int qrgen::Version::dataBytes(LEVEL l){
	VerInfo ver = VERSION_INFOS[version];
	VerLvlInfo lvl = ver.lvlInfos[l];
	return ver.bytes - lvl.num_of_block*lvl.cbytes_pre_block;
}

VerInfos qrgen::Version::VERSION_INFOS = VerInfos{
	VerInfo(0, 0, 0, 0x0, VerLvlInfos{}),																			 // null
	VerInfo(100, 100, 26, 0x0, VerLvlInfos{LvlInfo(1, 7), LvlInfo(1, 10), LvlInfo(1, 13), LvlInfo(1, 17)}),          // 1
	VerInfo(16, 100, 44, 0x0, VerLvlInfos{LvlInfo(1, 10), LvlInfo(1, 16), LvlInfo(1, 22), LvlInfo(1, 28)}),          // 2
	VerInfo(20, 100, 70, 0x0, VerLvlInfos{LvlInfo(1, 15), LvlInfo(1, 26), LvlInfo(2, 18), LvlInfo(2, 22)}),          // 3
	VerInfo(24, 100, 100, 0x0, VerLvlInfos{LvlInfo(1, 20), LvlInfo(2, 18), LvlInfo(2, 26), LvlInfo(4, 16)}),         // 4
	VerInfo(28, 100, 134, 0x0, VerLvlInfos{LvlInfo(1, 26), LvlInfo(2, 24), LvlInfo(4, 18), LvlInfo(4, 22)}),         // 5
	VerInfo(32, 100, 172, 0x0, VerLvlInfos{LvlInfo(2, 18), LvlInfo(4, 16), LvlInfo(4, 24), LvlInfo(4, 28)}),         // 6
	VerInfo(20, 16, 196, 0x7c94, VerLvlInfos{LvlInfo(2, 20), LvlInfo(4, 18), LvlInfo(6, 18), LvlInfo(5, 26)}),       // 7
	VerInfo(22, 18, 242, 0x85bc, VerLvlInfos{LvlInfo(2, 24), LvlInfo(4, 22), LvlInfo(6, 22), LvlInfo(6, 26)}),       // 8
	VerInfo(24, 20, 292, 0x9a99, VerLvlInfos{LvlInfo(2, 30), LvlInfo(5, 22), LvlInfo(8, 20), LvlInfo(8, 24)}),       // 9
	VerInfo(26, 22, 346, 0xa4d3, VerLvlInfos{LvlInfo(4, 18), LvlInfo(5, 26), LvlInfo(8, 24), LvlInfo(8, 28)}),       // 10
	VerInfo(28, 24, 404, 0xbbf6, VerLvlInfos{LvlInfo(4, 20), LvlInfo(5, 30), LvlInfo(8, 28), LvlInfo(11, 24)}),      // 11
	VerInfo(30, 26, 466, 0xc762, VerLvlInfos{LvlInfo(4, 24), LvlInfo(8, 22), LvlInfo(10, 26), LvlInfo(11, 28)}),     // 12
	VerInfo(32, 28, 532, 0xd847, VerLvlInfos{LvlInfo(4, 26), LvlInfo(9, 22), LvlInfo(12, 24), LvlInfo(16, 22)}),     // 13
	VerInfo(24, 20, 581, 0xe60d, VerLvlInfos{LvlInfo(4, 30), LvlInfo(9, 24), LvlInfo(16, 20), LvlInfo(16, 24)}),     // 14
	VerInfo(24, 22, 655, 0xf928, VerLvlInfos{LvlInfo(6, 22), LvlInfo(10, 24), LvlInfo(12, 30), LvlInfo(18, 24)}),    // 15
	VerInfo(24, 24, 733, 0x10b78, VerLvlInfos{LvlInfo(6, 24), LvlInfo(10, 28), LvlInfo(17, 24), LvlInfo(16, 30)}),   // 16
	VerInfo(28, 24, 815, 0x1145d, VerLvlInfos{LvlInfo(6, 28), LvlInfo(11, 28), LvlInfo(16, 28), LvlInfo(19, 28)}),   // 17
	VerInfo(28, 26, 901, 0x12a17, VerLvlInfos{LvlInfo(6, 30), LvlInfo(13, 26), LvlInfo(18, 28), LvlInfo(21, 28)}),   // 18
	VerInfo(28, 28, 991, 0x13532, VerLvlInfos{LvlInfo(7, 28), LvlInfo(14, 26), LvlInfo(21, 26), LvlInfo(25, 26)}),   // 19
	VerInfo(32, 28, 1085, 0x149a6, VerLvlInfos{LvlInfo(8, 28), LvlInfo(16, 26), LvlInfo(20, 30), LvlInfo(25, 28)}),  // 20
	VerInfo(26, 22, 1156, 0x15683, VerLvlInfos{LvlInfo(8, 28), LvlInfo(17, 26), LvlInfo(23, 28), LvlInfo(25, 30)}),  // 21
	VerInfo(24, 24, 1258, 0x168c9, VerLvlInfos{LvlInfo(9, 28), LvlInfo(17, 28), LvlInfo(23, 30), LvlInfo(34, 24)}),  // 22
	VerInfo(28, 24, 1364, 0x177ec, VerLvlInfos{LvlInfo(9, 30), LvlInfo(18, 28), LvlInfo(25, 30), LvlInfo(30, 30)}),  // 23
	VerInfo(26, 26, 1474, 0x18ec4, VerLvlInfos{LvlInfo(10, 30), LvlInfo(20, 28), LvlInfo(27, 30), LvlInfo(32, 30)}), // 24
	VerInfo(30, 26, 1588, 0x191e1, VerLvlInfos{LvlInfo(12, 26), LvlInfo(21, 28), LvlInfo(29, 30), LvlInfo(35, 30)}), // 25
	VerInfo(28, 28, 1706, 0x1afab, VerLvlInfos{LvlInfo(12, 28), LvlInfo(23, 28), LvlInfo(34, 28), LvlInfo(37, 30)}), // 26
	VerInfo(32, 28, 1828, 0x1b08e, VerLvlInfos{LvlInfo(12, 30), LvlInfo(25, 28), LvlInfo(34, 30), LvlInfo(40, 30)}), // 27
	VerInfo(24, 24, 1921, 0x1cc1a, VerLvlInfos{LvlInfo(13, 30), LvlInfo(26, 28), LvlInfo(35, 30), LvlInfo(42, 30)}), // 28
	VerInfo(28, 24, 2051, 0x1d33f, VerLvlInfos{LvlInfo(14, 30), LvlInfo(28, 28), LvlInfo(38, 30), LvlInfo(45, 30)}), // 29
	VerInfo(24, 26, 2185, 0x1ed75, VerLvlInfos{LvlInfo(15, 30), LvlInfo(29, 28), LvlInfo(40, 30), LvlInfo(48, 30)}), // 30
	VerInfo(28, 26, 2323, 0x1f250, VerLvlInfos{LvlInfo(16, 30), LvlInfo(31, 28), LvlInfo(43, 30), LvlInfo(51, 30)}), // 31
	VerInfo(32, 26, 2465, 0x209d5, VerLvlInfos{LvlInfo(17, 30), LvlInfo(33, 28), LvlInfo(45, 30), LvlInfo(54, 30)}), // 32
	VerInfo(28, 28, 2611, 0x216f0, VerLvlInfos{LvlInfo(18, 30), LvlInfo(35, 28), LvlInfo(48, 30), LvlInfo(57, 30)}), // 33
	VerInfo(32, 28, 2761, 0x228ba, VerLvlInfos{LvlInfo(19, 30), LvlInfo(37, 28), LvlInfo(51, 30), LvlInfo(60, 30)}), // 34
	VerInfo(28, 24, 2876, 0x2379f, VerLvlInfos{LvlInfo(19, 30), LvlInfo(38, 28), LvlInfo(53, 30), LvlInfo(63, 30)}), // 35
	VerInfo(22, 26, 3034, 0x24b0b, VerLvlInfos{LvlInfo(20, 30), LvlInfo(40, 28), LvlInfo(56, 30), LvlInfo(66, 30)}), // 36
	VerInfo(26, 26, 3196, 0x2542e, VerLvlInfos{LvlInfo(21, 30), LvlInfo(43, 28), LvlInfo(59, 30), LvlInfo(70, 30)}), // 37
	VerInfo(30, 26, 3362, 0x26a64, VerLvlInfos{LvlInfo(22, 30), LvlInfo(45, 28), LvlInfo(62, 30), LvlInfo(74, 30)}), // 38
	VerInfo(24, 28, 3532, 0x27541, VerLvlInfos{LvlInfo(24, 30), LvlInfo(47, 28), LvlInfo(65, 30), LvlInfo(77, 30)}), // 39
	VerInfo(28, 28, 3706, 0x28c69, VerLvlInfos{LvlInfo(25, 30), LvlInfo(49, 28), LvlInfo(68, 30), LvlInfo(81, 30)}), // 40
};