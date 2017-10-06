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

		int size = b.getSize();
		if (size < data_bytes * 8)
			b.pad(data_bytes * 8 - size);
		assert(!(b.getSize() != data_bytes * 8) && "qrcode has too mush data");

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

std::string qrgen::toSvgString(MatrixP &pixels, int border)
{
	if (border < 0)
		throw "Border must be non-negative";
	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (pixels.size() + border * 2) << " " << (pixels.size() + border * 2) << "\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\" stroke-width=\"0\"/>\n";
	sb << "\t<path d=\"";
	bool head = true;
	for (int y = 0; y < pixels.size(); y++) {
		for (int x =0; x < pixels.size(); x++) {
			if (pixels[y][x].getPixel() == 1) {
				if (head)
					head = false;
				else
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\" stroke-width=\"0\"/>\n";
	sb << "</svg>\n";
	std::ofstream out("qr.svg");
	out << sb.str();
	out.close();
	return sb.str();
}
