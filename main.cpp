#include <vector>
#include <string>
#include <iostream>
#include "Qrcoder.h"
using namespace std;

int main(int argc, char *argv[])
{
	
	string text = "3D Qrcode";
	qrgen::Bits bits;
	qrgen::LEVEL level = qrgen::LEVEL::H;
	qrgen::Mask * mask = new qrgen::Mask(3);
	qrgen::Version *version=qrgen::getMinVersion(text, level, bits);
	cout << version->getVersion() << endl;
	std::vector<std::vector<qrgen::Pixel>> pixels = qrgen::encode(bits, version, level, mask);

	for (int y = 0; y < pixels.size(); y++) {
		for (int x = 0; x < pixels.size(); x++) {
			qrgen::Pixel p = pixels[y][x];
			string s = p.getPixel() == 0 ? "  " : "##";
			std::cout << s;;
			
		}
		std::cout << std::endl;
	}
	system("pause");
}
