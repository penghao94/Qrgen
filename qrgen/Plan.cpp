
#include "Plan.h"
#include "Version.h"
#include "Level.h"
#include <cassert>
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
typedef std::vector<std::vector<qrgen::Pixel>> MatrixP;
typedef std::vector<qrgen::Pixel>  VectorP;

qrgen::Plan* qrgen::Plan::verPlan(qrgen::Version* v){

	Plan* p=new Plan();
	p->setVersion(v);
	assert(!(v->getVersion() <Version::MIN_VERSION|| v->getVersion() >Version::MAX_VERSION)&&"Wrong qr version!!!");

	int size = 17 + 4 * v->getVersion();
	MatrixP pixels(size);
	for (int i = 0; i < size; i++) pixels[i].resize(size);
	
	/*timing position*/
	int timingpos = 6;
	for (int i = 0; i < size; i++){
		Pixel pixel(Pixel::PixelRole::TIMING);
		if ((i & 0x1) == 0) pixel.orPixel(Pixel::BLACK.getPixel());

		pixels[i][timingpos] = pixel;
		pixels[timingpos][i] = pixel;
	}
	
	//position box
	setPosBox(pixels, 3, 3);
	setPosBox(pixels, size - 4, 3);
	setPosBox(pixels, 3, size - 4);
   
	//Alignment box
	VerInfo verinfo= Version::VERSION_INFOS[v->getVersion()];

	for (int x = 4; x + 5 < size;) {
		for (int y = 4; y + 5 < size;) {
			// don't overwrite timing markers
			if ((x < 7 && y < 7) || (x < 7 && y + 5 >= size - 7) || (x + 5 >= size - 7 && y < 7)) {}
			else 
				setAlignBox(pixels, x+2, y+2);//offset (2,2)

			if (y == 4)
				y = verinfo.apos;
			else 
				y += verinfo.stride;
		} 

		if (x == 4) 
			x = verinfo.apos;
		else 
			x += verinfo.stride;
	}
	
	//version pattern
	int pattern = verinfo.pattern;
	if (pattern != 0) {
		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 3; y++) {
				Pixel pixel(Pixel::PixelRole::VERSION_PATTERN);
				if ((pattern & 1) != 0) {
					pixel.orPixel(Pixel::BLACK.getPixel());
				}

				pixels[size - 11 + y][x] = pixel;
				pixels[x][size - 11 + y] = pixel;
				pattern >>= 1;
			}
		}
	}
	
	/*I do not know what fuck it is*/
	Pixel pixel(Pixel::PixelRole::UNUSED);
	pixel.orPixel(Pixel::BLACK.getPixel());
	pixels[size - 8][8] = pixel;

	//set pixels
	p->setPixels(pixels);
	return p;
}

void qrgen::Plan::formatPlan(Plan* p, LEVEL l, Mask *m){
	//Format pixels
	//L 3 0010 1100 0000 0000
	//    0010 1001 1011 1000
	//    0000 0101 1011 1000
	//    0000 0101 0011 0111
	//	  0000 0000 1000 1111
	//    0010 1100 0000 0000
    //    0010 1100 1000 1111

	int formatBit = (l ^ 0x1) << 13;//level: L=01,M=00,Q=11,H=10
	formatBit |= m->getMask()<<10;//mask

	int formatPoly = 0x537;//0101 0011 0111
	int rem = formatBit;

	for (int i = 14; i >= 10; i--){
		if ((rem&(1 << i)) != 0){
			rem ^= (formatPoly << (i - 10));
		}
	}

	formatBit |= rem;

	int inv = 0x5412;// 0101 0100 0001 0010

	int size = (p->getPixels()).size();

	for (int i = 0; i < 15; i++){
		Pixel pixel(Pixel::PixelRole::FORMAT);
		pixel.setOffset(i);

		if (((formatBit >> i) & 1) == 1) pixel.orPixel(Pixel::BLACK.getPixel());
		
		if (((inv >> i) & 1) == 1){ 
			pixel.setInv(!pixel.shouldInv());
			pixel.xorPixel(Pixel::BLACK.getPixel());
		}

		//top left
		if (i < 6) (p->pixels)[i][8] = pixel;
		else if (i < 8) (p->pixels)[i + 1][8] = pixel;
		else if (i < 9) (p->pixels)[8][7] = pixel;
		else (p->pixels)[8][14 - i] = pixel;

		//bottom right
		if (i < 8) (p->pixels)[8][size - 1 - i] = pixel;
		else (p->pixels)[size - 1 - (14 - i)][8] = pixel;
	}


}


void qrgen::Plan::lvlPlan(Plan* p, Version *v, LEVEL l){

	VerInfo verinfo= Version::VERSION_INFOS[v->getVersion()];//Version information
	int num_blocks = verinfo.lvlInfos[l].num_of_block;// number of blocks
	int num_cbytes = verinfo.lvlInfos[l].cbytes_pre_block;//number of check bytes per block
	int num_dbytes = (verinfo.bytes - num_blocks*num_cbytes) / num_blocks;//number of data bytes per block
	int num_extra = (verinfo.bytes - num_blocks*num_cbytes) % num_blocks;// number of extra bytes

	int dataBits = (num_dbytes*num_blocks + num_extra) * 8; //data bits
	int checkBits = num_cbytes*num_blocks * 8;// check data bits
	
	p->setBlocksNum(num_blocks);
	p->setdBytesNum(verinfo.bytes - num_cbytes*num_blocks);
	p->setcBytesNum(num_cbytes*num_blocks);

	//make data+checksum pixels
	VectorP data(dataBits);
	for (int i = 0; i < dataBits; i++){
		Pixel pixel(Pixel::PixelRole::DATA);
		pixel.setOffset(i);
		data[i] = pixel;
	}

	VectorP check(checkBits);
	for (int i = 0; i < checkBits; i++){
		Pixel pixel(Pixel::PixelRole::CHECK);
		pixel.setOffset(i+dataBits);
		check[i] = pixel;
	}

	//split into blocks
	MatrixP dataList(num_blocks);
	MatrixP checkList(num_blocks);

	int dataIndex = 0, checkIndex = 0;

	for (int i = 0; i < num_blocks; i++){

		//The last few blocks have an extra data byte(8 pixels).
		int dbytes = num_dbytes;
		if (i >=(num_blocks - num_extra)) dbytes++;

		VectorP dataBlock(dbytes * 8);
		dataList[i].assign(data.begin() + dataIndex, data.begin() + dataIndex + dbytes * 8);
		for (int j = 0; j < dataList[i].size(); j++) dataList[i][j].setBlockIndex(i);

		checkList[i].assign(check.begin() + checkIndex, check.begin() + checkIndex + num_cbytes * 8);
		for (int j = 0; j < checkList[i].size(); j++) checkList[i][j].setBlockIndex(i);
		
		dataIndex += 8 * dbytes;
		checkIndex += 8 * num_cbytes;
		
	}
	
	
	assert(!(dataIndex != dataBits || checkIndex != checkBits) && "Build data/check block error");

	//build up bit sequence,taking first byte of each block, then second byte, and so on. Then checksums.http://www.pclviewer.com/rs2/qrtopology.hmt

	VectorP bits;

	int bitIndex = 0;

	for (int i = 0; i < num_dbytes + 1; i++) {
		for (int j = 0; j < dataList.size(); j++) {
			if (i * 8 < dataList[j].size()) {
				bits.insert(bits.begin() + bitIndex, dataList[j].begin() + i * 8, dataList[j].begin() + (i + 1) * 8);
				bitIndex += 8;
			}
		}
	}

	for (int i = 0; i < num_cbytes; i++) {
		for (int j = 0; j < checkList.size(); j++) {
			if (j * 8 < checkList[j].size()) {
				bits.insert(bits.begin() + bitIndex, checkList[j].begin() + i * 8, checkList[j].begin() + (i + 1) * 8);
				bitIndex += 8;
			}
		}
	}

	assert(!(bitIndex != bits.size())&&"copy to bit error!!!");

	//Sweep up pair of columns, then down,assigning to right then left pixel, Repeat.

	int size = (p->pixels).size();
	VectorP src;
	src.swap(bits);
	src.insert(src.begin()+bitIndex,7,Pixel(Pixel::PixelRole::EXTRA));
	int srcIndex = 0;

	for (int x = size; x > 0;) {
		for (int y = size - 1; y >= 0; y--) {

			if ((p->pixels)[y][x - 1].getPixelRole() == 0)
				(p->pixels)[y][x - 1] = src[srcIndex++];

			if ((p->pixels)[y][x - 2].getPixelRole() == 0)
				(p->pixels)[y][x - 2] = src[srcIndex++];
		}
		x -= 2;

		if (x == 7) x--; //vertical timing strip

		for (int y = 0; y < size; y++) {

			if ((p->pixels)[y][x - 1].getPixelRole() == 0)
				(p->pixels)[y][x - 1] = src[srcIndex++];
			
			if ((p->pixels)[y][x - 2].getPixelRole() == 0) 
				(p->pixels)[y][x - 2] = src[srcIndex++];
		}
		x -= 2;
	}
}

void qrgen::Plan::maskPlan(Plan * p, Mask* m)
{
	p->setMasK(m);
	int size = (p->pixels).size();

	for (int y = 0; y <size ; y++) {
		for (int x = 0; x < size; x++) {

			Pixel pixel = (p->pixels)[y][x];
			Pixel::PixelRole r = pixel.getPixelRole();

			if ((r == Pixel::PixelRole::DATA || r == Pixel::PixelRole::CHECK || r == Pixel::PixelRole::EXTRA) && p->mask->shouldInv(y, x)) {
				pixel.setInv(!pixel.shouldInv());
				pixel.xorPixel(Pixel::BLACK.getPixel());
				(p->pixels)[y][x] = pixel;//either inner or outer, but inner has less computing.
			}
	
		}

	}
}

qrgen::Plan * qrgen::Plan::newPlan(Version* v, LEVEL l, Mask* m)
{
	Plan *p = verPlan(v);
	formatPlan(p, l, m);
	lvlPlan(p, v, l);
	maskPlan(p, m);
	return p;
}

qrgen::QRCode qrgen::Plan:: encode(Plan * plan, Encoding & encode1, Encoding & encode2)
{
	Bits bits;
	std::string err = encode1.validate();
	assert((err == "") && "encoding check error");
	encode1.encode(bits, plan->getVersion());

    err = encode2.validate();
	assert((err == "") && "encoding check error");
	encode2.encode(bits, plan->getVersion());

	if (bits.getSize() > plan->getdBytesNum() * 8) 
		assert("Can not encode!!");

	bits.addCheckBytes(plan->getVersion(), plan->getLevel());
	Bytes bytes = bits.getBits();
	MatrixP pixels = plan->getPixels();

	return { bytes,pixels };
}

void qrgen::Plan::setPosBox(std::vector<VectorP>&pixels, int x, int y){
	Pixel pw(Pixel::PixelRole::POSITION);
	Pixel pb(Pixel::PixelRole::POSITION);

	pb.setPixel(Pixel::BLACK.getPixel());

	//position box https://github.com/nayuki/QR-Code-generator
	for (int i = -4; i <= 4; i++){
		for (int j = -4; j <=4; j++){
			int dist = std::max(std::abs(i), std::abs(j));// Chebyshev/infinity norm
			int dx = x + j, dy = y + i;
			if (0 <= dx && dx < int(pixels.size()) && 0 <= dy && dy < int(pixels.size()))
				pixels[dy][dx] = (dist != 2 && dist != 4) ? pb : pw;
		}
	}
}

void qrgen::Plan::setAlignBox(std::vector<VectorP>&pixels, int x, int y){
	
	Pixel pw(Pixel::PixelRole::POSITION);
	Pixel pb(Pixel::PixelRole::POSITION);

	pb.setPixel(Pixel::BLACK.getPixel());
	
	//https://github.com/nayuki/QR-Code-generator
	for (int i = -2; i <= 2; i++){
		for (int j = -2; j <= 2; j++){
			pixels[y + i][x + j] = std::max(std::abs(i), std::abs(j)) != 1 ? pb : pw;
		}
	}
}