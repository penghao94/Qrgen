#include "Qrcoder.h"
typedef std::vector<std::vector<qrgen::Pixel>> MatrixP;

qrgen::Version * qrgen::getMinVersion(std::string&text, LEVEL level,Bits &bits)
{
	
	for (int i = Version::MIN_VERSION; i < Version::MAX_VERSION; i++) {
		Bits b;
		Raw(text).encode(b,new Version(i));
		VerInfo v = Version::VERSION_INFOS[i];
		int block_num =v.lvlInfos[level].num_of_block;//number of blocks
		int check_bytes_num = v.lvlInfos[level].cbytes_pre_block;//number of check bytes per block
		int data_bytes = v.bytes - check_bytes_num*block_num;//number of data bytes

		if (b.getBits().size() > data_bytes || i<6) continue;
		b.addCheckBytes(new Version(i), level);
		bits = b;
		return new Version(i);
	}
}

MatrixP qrgen::encode(Bits& bits,Version *version, LEVEL level, Mask * mask)
{
	
	Plan *plan = Plan::newPlan(version, level, mask);
	MatrixP pixels = plan->getPixels();

	for (int y = 0; y < pixels.size(); y++) {
		for (int x = 0; x < pixels.size(); x++) {
			Pixel::PixelRole role = pixels[y][x].getPixelRole();
			if (role == Pixel::DATA || role == Pixel::CHECK) {
				int index = pixels[y][x].getOffset();
				int value = static_cast<int>((bits.getBits()[index / 8] >> (7 - index & 0x7)) & 0x1);
				pixels[y][x].xorPixel(value);
			}
		}
	}

	return pixels;
}
