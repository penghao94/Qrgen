#include<vector>
#include "Version.h"
typedef std::vector<qrgen::VerLvlInfo> VerLvlInfos;


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

bool qrgen::Version::init()
{
	VERSION_INFOS.push_back({ 0, 0, 0, 0x0, VerLvlInfos{} });								                   // null
	VERSION_INFOS.push_back({ 100, 100, 26, 0x0, VerLvlInfos{ { 1, 7 },{ 1, 10 },{ 1, 13 },{ 1, 17 } } });          // 1
	VERSION_INFOS.push_back({ 16, 100, 44, 0x0, VerLvlInfos{ { 1, 10 },{ 1, 16 },{ 1, 22 },{ 1, 28 } } });          // 2
	VERSION_INFOS.push_back({ 20, 100, 70, 0x0, VerLvlInfos{ { 1, 15 },{ 1, 26 },{ 2, 18 },{ 2, 22 } } });          // 3
	VERSION_INFOS.push_back({ 24, 100, 100, 0x0, VerLvlInfos{ { 1, 20 },{ 2, 18 },{ 2, 26 },{ 4, 16 } } });         // 4
	VERSION_INFOS.push_back({ 28, 100, 134, 0x0, VerLvlInfos{ { 1, 26 },{ 2, 24 },{ 4, 18 },{ 4, 22 } } });         // 5
	VERSION_INFOS.push_back({ 32, 100, 172, 0x0, VerLvlInfos{ { 2, 18 },{ 4, 16 },{ 4, 24 },{ 4, 28 } } });        // 6
	VERSION_INFOS.push_back({ 20, 16, 196, 0x7c94, VerLvlInfos{ { 2, 20 },{ 4, 18 },{ 6, 18 },{ 5, 26 } } });       // 7
	VERSION_INFOS.push_back({ 22, 18, 242, 0x85bc, VerLvlInfos{ { 2, 24 },{ 4, 22 },{ 6, 22 },{ 6, 26 } } });       // 8
	VERSION_INFOS.push_back({ 24, 20, 292, 0x9a99, VerLvlInfos{ { 2, 30 },{ 5, 22 },{ 8, 20 },{ 8, 24 } } });       // 9
	VERSION_INFOS.push_back({ 26, 22, 346, 0xa4d3, VerLvlInfos{ { 4, 18 },{ 5, 26 },{ 8, 24 },{ 8, 28 } } });       // 10
	VERSION_INFOS.push_back({ 28, 24, 404, 0xbbf6, VerLvlInfos{ { 4, 20 },{ 5, 30 },{ 8, 28 },{ 11, 24 } } });      // 11
	VERSION_INFOS.push_back({ 30, 26, 466, 0xc762, VerLvlInfos{ { 4, 24 },{ 8, 22 },{ 10, 26 },{ 11, 28 } } });     // 12
	VERSION_INFOS.push_back({ 32, 28, 532, 0xd847, VerLvlInfos{ { 4, 26 },{ 9, 22 },{ 12, 24 },{ 16, 22 } } });     // 13
	VERSION_INFOS.push_back({ 24, 20, 581, 0xe60d, VerLvlInfos{ { 4, 30 },{ 9, 24 },{ 16, 20 },{ 16, 24 } } });     // 14
	VERSION_INFOS.push_back({ 24, 22, 655, 0xf928, VerLvlInfos{ { 6, 22 },{ 10, 24 },{ 12, 30 },{ 18, 24 } } });    // 15
	VERSION_INFOS.push_back({ 24, 24, 733, 0x10b78, VerLvlInfos{ { 6, 24 },{ 10, 28 },{ 17, 24 },{ 16, 30 } } });   // 16
	VERSION_INFOS.push_back({ 28, 24, 815, 0x1145d, VerLvlInfos{ { 6, 28 },{ 11, 28 },{ 16, 28 },{ 19, 28 } } });   // 17
	VERSION_INFOS.push_back({ 28, 26, 901, 0x12a17, VerLvlInfos{ { 6, 30 },{ 13, 26 },{ 18, 28 },{ 21, 28 } } });   // 18
	VERSION_INFOS.push_back({ 28, 28, 991, 0x13532, VerLvlInfos{ { 7, 28 },{ 14, 26 },{ 21, 26 },{ 25, 26 } } });   // 19
	VERSION_INFOS.push_back({ 32, 28, 1085, 0x149a6, VerLvlInfos{ { 8, 28 },{ 16, 26 },{ 20, 30 },{ 25, 28 } } });  // 20
	VERSION_INFOS.push_back({ 26, 22, 1156, 0x15683, VerLvlInfos{ { 8, 28 },{ 17, 26 },{ 23, 28 },{ 25, 30 } } });  // 21
	VERSION_INFOS.push_back({ 24, 24, 1258, 0x168c9, VerLvlInfos{ { 9, 28 },{ 17, 28 },{ 23, 30 },{ 34, 24 } } });  // 22
	VERSION_INFOS.push_back({ 28, 24, 1364, 0x177ec, VerLvlInfos{ { 9, 30 },{ 18, 28 },{ 25, 30 },{ 30, 30 } } });  // 23
	VERSION_INFOS.push_back({ 26, 26, 1474, 0x18ec4, VerLvlInfos{ { 10, 30 },{ 20, 28 },{ 27, 30 },{ 32, 30 } } }); // 24
	VERSION_INFOS.push_back({ 30, 26, 1588, 0x191e1, VerLvlInfos{ { 12, 26 },{ 21, 28 },{ 29, 30 },{ 35, 30 } } }); // 25
	VERSION_INFOS.push_back({ 28, 28, 1706, 0x1afab, VerLvlInfos{ { 12, 28 },{ 23, 28 },{ 34, 28 },{ 37, 30 } } }); // 26
	VERSION_INFOS.push_back({ 32, 28, 1828, 0x1b08e, VerLvlInfos{ { 12, 30 },{ 25, 28 },{ 34, 30 },{ 40, 30 } } }); // 27
	VERSION_INFOS.push_back({ 24, 24, 1921, 0x1cc1a, VerLvlInfos{ { 13, 30 },{ 26, 28 },{ 35, 30 },{ 42, 30 } } }); // 28
	VERSION_INFOS.push_back({ 28, 24, 2051, 0x1d33f, VerLvlInfos{ { 14, 30 },{ 28, 28 },{ 38, 30 },{ 45, 30 } } }); // 29
	VERSION_INFOS.push_back({ 24, 26, 2185, 0x1ed75, VerLvlInfos{ { 15, 30 },{ 29, 28 },{ 40, 30 },{ 48, 30 } } }); // 30
	VERSION_INFOS.push_back({ 28, 26, 2323, 0x1f250, VerLvlInfos{ { 16, 30 },{ 31, 28 },{ 43, 30 },{ 51, 30 } } }); // 31
	VERSION_INFOS.push_back({ 32, 26, 2465, 0x209d5, VerLvlInfos{ { 17, 30 },{ 33, 28 },{ 45, 30 },{ 54, 30 } } }); // 32
	VERSION_INFOS.push_back({ 28, 28, 2611, 0x216f0, VerLvlInfos{ { 18, 30 },{ 35, 28 },{ 48, 30 },{ 57, 30 } } }); // 33
	VERSION_INFOS.push_back({ 32, 28, 2761, 0x228ba, VerLvlInfos{ { 19, 30 },{ 37, 28 },{ 51, 30 },{ 60, 30 } } }); // 34
	VERSION_INFOS.push_back({ 28, 24, 2876, 0x2379f, VerLvlInfos{ { 19, 30 },{ 38, 28 },{ 53, 30 },{ 63, 30 } } }); // 35
	VERSION_INFOS.push_back({ 22, 26, 3034, 0x24b0b, VerLvlInfos{ { 20, 30 },{ 40, 28 },{ 56, 30 },{ 66, 30 } } }); // 36
	VERSION_INFOS.push_back({ 26, 26, 3196, 0x2542e, VerLvlInfos{ { 21, 30 },{ 43, 28 },{ 59, 30 },{ 70, 30 } } }); // 37
	VERSION_INFOS.push_back({ 30, 26, 3362, 0x26a64, VerLvlInfos{ { 22, 30 },{ 45, 28 },{ 62, 30 },{ 74, 30 } } }); // 38
	VERSION_INFOS.push_back({ 24, 28, 3532, 0x27541, VerLvlInfos{ { 24, 30 },{ 47, 28 },{ 65, 30 },{ 77, 30 } } }); // 39
	VERSION_INFOS.push_back({ 28, 28, 3706, 0x28c69, VerLvlInfos{ { 25, 30 },{ 49, 28 },{ 68, 30 },{ 81, 30 } } }); // 40
	return true;
}
std::vector<qrgen::VerInfo> qrgen::Version::VERSION_INFOS;
bool _init = qrgen::Version::init();

/*
bool qrgen::Version::init(){
	
}
std::vector<qrgen::VerInfo> qrgen::Version::VERSION_INFOS;
bool _init=qrgen::Version::init();*/