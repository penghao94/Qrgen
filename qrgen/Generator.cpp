#include "Generator.h"


qrgen::Generator::Generator(std::vector<std::vector<int>>& target, int dx, int dy, int version)
{
	this->target = target;
	this->dx = dx;
	this->dy = dy;
	this->version = version;
}

qrgen::Generator::Generator(std::vector<std::vector<int>>& target, int dx, int dy, std::string text, int version, int mask, int rotation, bool rand_control, long seed, bool dither, bool only_data_bits, bool save_control)
{
	this->target = target;
	this->dx = dx;
	this->dy = dy;
	this->text = text;

	this->version = version;
	this->mask = mask;
	this->rotation = rotation;

	this->rand_control = rand_control;
	this->seed = seed;
	this->dither = dither;
	this->only_data_bits = only_data_bits;
	this->save_control = save_control;
	this->divider = calculateDivider();
}

qrgen::Target qrgen::Generator::Target(int x, int y)
{
	int tx = x + dx;
	int ty = y + dy;

	if (ty<0 || ty>target.size() || tx<0 || tx>target[ty].size())
		return { static_cast<uint8_t>(255),-1 };

	int v0 = target[ty][tx];

	if(v0<0)
		return{ static_cast<uint8_t>(255),-1 };

	uint8_t targ = static_cast<int>(v0);

	int n = 0;
	long sum = 0;
	long sum_sequence = 0;
	int del = 5;

	for (int dy = -del; dy <= del; dy++) {
		for (int dx = -del; dx <= del; dy++) {
			if (0 <= ty + dy&&ty + dy < target.size() && 0 < tx + dx&&tx + dx < target[ty + dy].size()) {
				int v = target[ty + dy][tx + dx];
				sum += v;
				sum_sequence += v*v;
				n++;
			}
		}
	}

	int avg = static_cast<int>(sum / n);
	int contrast = static_cast<int>(sum_sequence / n - avg*avg);
	return { avg,contrast };
}

void qrgen::Generator::rotate(Plan & plan, int rotation)
{
}

qrgen::QRCode qrgen::Generator::encode()
{
	return QRCode();
}

int qrgen::Generator::calculateDivider()
{
	long sum = 0;
	int n = 0;
	for (int i = 0; i < target.size(); i++) {
		for (int j = 0; j < target[i].size(); j++) {
			sum += target[i][j];
			n++;
		}
	}

	if (n == 0)
		return 128;
	return static_cast<int>(sum / n);

	
}

void qrgen::Generator::addDither(std::vector<PixelInfo>& pixel_by_offset, Pixel & pixel, int error)
{
	Pixel::PixelRole role = pixel.getPixelRole();

	if (role != Pixel::PixelRole::DATA&&role != Pixel::PixelRole::CHECK)
		return;

	PixelInfo info = pixel_by_offset[pixel.getOffset()];
	info.setDitherTarget(info.getDitherTarget() + error);

	pixel_by_offset[pixel.getOffset()] = info;
}


