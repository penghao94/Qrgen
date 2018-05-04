#include <vector>
#include <string>
#include <iostream>
#include "Qrcoder.h"
using namespace std;

int main(int argc, char *argv[])
{
	qrgen::RSencoder rs(new qrgen::GenericGF(0x011D, 256, 0));
	
	
	string text = "QR code is a cool work!!";
	qrgen::Bits bits;
	qrgen::LEVEL level = qrgen::LEVEL::H;
	qrgen::Mask * mask = new qrgen::Mask(3);
	qrgen::Version *version=qrgen::getMinVersion(text, level, bits);
	std::vector<std::vector<qrgen::Pixel>> pixels = qrgen::encode(bits, version, level, mask);
	//for (auto i : bits.getBits()) std::cout << static_cast<int>(i) <<endl;
	/*vector<uint8_t> data = qrgen::geneECBytes(rs, bits.getBits(), 0, 60, 112);
	for (auto i : data) std::cout << static_cast<int>(i) << endl;*/
	//cout << endl;
	int num_block = qrgen::Version::VERSION_INFOS[version->getVersion()].lvlInfos[level].num_of_block;
	std::cout << version->getVersion() << std::endl;
	qrgen::toSvgString("qr.svg",pixels, 4);
	qrgen::toSvgStringColor(pixels, 4,num_block);
	int size = 17 + 4 * version->getVersion();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < num_block; j++) {
			bool right = false;
			while (!right)
			{
				int x = static_cast<double>(rand()) / static_cast<double>(RAND_MAX)*size;
				int y = static_cast<double>(rand()) / static_cast<double>(RAND_MAX)*size;
				if (pixels[y][x].getPixel()==1&&pixels[y][x].getBlockIndex() == j&&(pixels[y][x].getPixelRole() == qrgen::Pixel::PixelRole::CHECK || pixels[y][x].getPixelRole() == qrgen::Pixel::PixelRole::DATA)) {
					pixels[y][x].setPixel(0);
					right = true;
				}
			}
		}
		std::string temp = "qr" + std::to_string(i) + ".svg";
		qrgen::toSvgString(temp,pixels, 4);

	}
	system("pause");
}
