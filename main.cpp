#include <vector>
#include <string>
#include <iostream>
#include "Qrcoder.h"
using namespace std;

int main(int argc, char *argv[])
{
	qrgen::RSencoder rs(new qrgen::GenericGF(0x011D, 256, 0));
	
	
	string text = "https://github.com/swannyPeng/Qrgen";
	qrgen::Bits bits;
	qrgen::LEVEL level = qrgen::LEVEL::H;
	qrgen::Mask * mask = new qrgen::Mask(3);
	qrgen::Version *version=qrgen::getMinVersion(text, level, bits);
	std::vector<std::vector<qrgen::Pixel>> pixels = qrgen::encode(bits, version, level, mask);
	//for (auto i : bits.getBits()) std::cout << static_cast<int>(i) <<endl;
	/*vector<uint8_t> data = qrgen::geneECBytes(rs, bits.getBits(), 0, 60, 112);
	for (auto i : data) std::cout << static_cast<int>(i) << endl;*/
	//cout << endl;

	for (int y = 0; y < pixels.size(); y++) {
		for (int x = 0; x < pixels.size(); x++) {
			qrgen::Pixel p = pixels[y][x];
			string s = p.getPixel() == 0 ? "  " : "##";
			std::cout << s;;
			
		}
		std::cout << std::endl;
	}
	qrgen::toSvgString(pixels, 4);
	system("pause");
}
