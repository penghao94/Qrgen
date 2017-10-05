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

qrgen::Target qrgen::Generator::targt(int x, int y)
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
	return { static_cast<uint8_t>(avg),contrast };
}

void qrgen::Generator::rotate(Plan *plan, int rotation)
{
	if (rotation == 0)
		return;

	int n = plan->getPixels().size();
	std::vector<std::vector<Pixel>> pixels(n);
	for (int i = 0; i < n; i++) pixels[i].resize(n);

	switch (rotation)
	{
	case 1:
		for (int y = 0; y < n; y++)
			for (int x = 0; x < n; x++)
				pixels[y][x] = (plan->getPixels())[x][n - 1 - y];
		break;

	case 2:
		for (int y = 0; y < n; y++)
			for (int x = 0; x < n; x++)
				pixels[y][x] = (plan->getPixels())[n - 1 - y][n - 1 - x];
		break;

	case 3:
		for (int y = 0; y < n; y++)
			for (int x = 0; x < n; x++)
				pixels[y][x] = (plan->getPixels())[n - 1 - x][y];
		break;

	}

	plan->setPixels(pixels);
}


qrgen::QRCode qrgen::Generator::encode()
{
	/*Initiate plan*/
	Version *ver=new Version(version);
	Mask *m=new Mask(mask);
	Plan *plan = Plan::newPlan(ver, LEVEL::L, m);

	/*rotation*/
	rotate(plan, rotation);

	//TODO: Random random = new Random(seed);

	/*QR parameters*/
	int num_dataBytes_block = plan->getdBytesNum() / plan->getBlocksNum();// number of data bytes per block
	int num_checkBytes_block = plan->getcBytesNum() / plan->getBlocksNum();//number of check bytes per block
	int num_extraBytes = plan->getdBytesNum() - num_dataBytes_block*plan->getBlocksNum();//number of extra bytes
	RSencoder encoder(new qrgen::GenericGF(0x011D, 256, 0)); //Reed Solomon encoder

	/*Build information about pixels,indexed by data/check bit number*/
	std::vector<PixelInfo> pixel_offset((plan->getdBytesNum() + plan->getcBytesNum()) * 8);

	// there is a issue about whether order[i] should be recopy to primary position
	std::vector<std::vector<Pixel>> pixels = plan->getPixels();

	std::vector<std::vector<bool>> expect(pixels.size());
	for (int i = 0; i < pixels.size(); i++)  expect[i].resize(pixels.size());
	
	for (int y = 0; y < pixels.size(); y++) {
		for (int x = 0; x < pixels.size(); x++) {

			Pixel pixel = pixels[y][x];
			expect[y][x] = ((pixel.getPixel()&Pixel::BLACK.getPixel()) != 0);

			Target tar = targt(x, y);
			if (rand_control&&tar.contrast >= 0)
				tar.contrast = static_cast<int>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 128) + 64 * ((x + y) % 2) + 64 * ((x + y) % 3 % 2);

			Pixel::PixelRole role = pixel.getPixelRole();
			if (role == Pixel::PixelRole::DATA || role == Pixel::PixelRole::CHECK)
				pixel_offset[pixel.getOffset()] = PixelInfo(x, y, Pixel(pixel), tar.targt, tar.contrast);
		}
	}

	/*Count fixed initial data bits, prepare template text*/
    //TODO: std::string str=text+"#";
	Bits bits;
	Raw(text).encode(bits, plan->getVersion());
	Number("").encode(bits, plan->getVersion());

	int head_size = bits.getSize();
	int data_bits_remain = plan->getdBytesNum() * 8 - head_size;
	assert((data_bits_remain < 0) && "Can not encode text into available bits");

	std::string number;
	int errorCount;
	do 
	{
		int nd = num_dataBytes_block;
		bits.reset();
		number.assign(data_bits_remain / 10 * 3, '0');

		Raw(text).encode(bits, plan->getVersion());
		Number(number).encode(bits, plan->getVersion());
		bits.addCheckBytes(plan->getVersion(), plan->getLevel());

		Bytes data = bits.getBits();

		int data_offset = 0;
		int check_offset = 0;
		int main_data_bits = head_size + data_bits_remain /10*10;

		/*choose pixel*/
		std::vector<BitBlock> bit_blocks(plan->getBlocksNum());

		for (int block_num = 0; block_num < plan->getBlocksNum(); block_num++) {

			if (block_num == plan->getBlocksNum() - num_extraBytes) nd++;

			BitBlock bit_block(nd, num_checkBytes_block, encoder, data, data_offset / 8, data, data_offset / 8);
			bit_blocks[block_num] = bit_block;

			/*Determine which bits in this block we can try to edit*/
			int low = 0, high = nd * 8;

			if (low < head_size - data_offset) {

				low = head_size - data_offset;
				if (low > high) low = high;
			}

			if (high > main_data_bits - data_offset) {

				high = main_data_bits - data_offset;
				if (high < low) high = low;
			}
			
			/*Preserve [0,low) and [high, nd*8]*/

			for (int i = 0; i < low; i++) {
				bool can_set = bit_block.canSet(i, static_cast<uint8_t>(data[data_offset / 8 + i / 8] >> (7 - i & 0x7) & 0x1));
				assert((!can_set) && "Can not preserve required bits!");
			}

			for (int i = high; i < nd * 8; i++) {
				bool can_set = bit_block.canSet(i, static_cast<uint8_t>(data[data_offset / 8 + i / 8] >> (7 - i & 0x7) & 0x1));
				assert((!can_set) && "Can not preserve required bits!");
			}

			/*Can edit [low,high) and checksum bits to hit target. Determine which ones to try first*/
			std::vector<PixelOrder> order((high - low) + num_checkBytes_block * 8);

			for (int i = 0; i < order.size(); i++)	order[i] = { 0, 0 };

			for (int i = low; i < high; i++)  order[i - low].offset = data_offset + i;
			for (int i = 0; i < num_checkBytes_block * 8; i++)  order[high - low + i].offset = plan->getdBytesNum() * 8 + check_offset + i;

			if (only_data_bits) order.resize(high - low);

			for (int i = 0; i < order.size(); i++) {
				order[i].priority = pixel_offset[order[i].offset].getContrast() << 8 | static_cast<int>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX)*256);
				// there is a issue about whether order[i] should be recopy to primary position
			}

			/*sort*/
			const auto less = [](const PixelOrder o1,const PixelOrder o2)->bool {
				return (o1.priority < o2.priority);
			};
			std::stable_sort(order.begin(), order.end(), less);

			bool mark = false;
			
			for (int i = 0; i < order.size(); i++) {
				PixelOrder po = order[i];
				PixelInfo info = pixel_offset[po.offset];
				int value = static_cast<int>(info.getTarget()) & 0xFF;
				value = value < divider ? 1 : 0;

				Pixel pixel = info.getPixel();
				if (pixel.shouldInv())  value ^= 1;
				if (info.getHardZero())  value = 0;

				int index = 0;

				if (pixel.getPixelRole() == Pixel::PixelRole::DATA)
					index = po.offset - data_offset;
				else
					index = po.offset = plan->getdBytesNum() * 8 - check_offset + nd * 8;

				if (bit_block.canSet(index, static_cast<uint8_t>(value))) {
					info.setBlock(bit_block);
					info.setBitIndex(index);
					if (mark) pixels[info.getY()][info.getX()] = Pixel::BLACK;
				}
				else {
					if (mark) pixels[info.getY()][info.getX()] = Pixel(0);
				}
			}// end of order
			bit_block.copyOut();

			bool cheat = false;

			for (int i = 0; i < nd * 8; i++) {
				PixelInfo info = pixel_offset[plan->getdBytesNum() * 8 + check_offset + i];
				Pixel pixel(pixels[info.getY()][info.getX()]);

				if ((bit_block.getBlockBytes()[i / 8] & (1 << (7 - i & 0x7))) != 0)  pixel.xorPixel(Pixel::BLACK.getPixel());

				expect[info.getY()][info.getX()] = ((pixel.getPixel()&Pixel::BLACK.getPixel()) != 0);

				/*What dose this mean, though cheat is false?*/
				if (cheat) {
					Pixel p = new Pixel(pixel);
					p.setPixel(Pixel::BLACK.getPixel());
					pixels[info.getY()][info.getX()] = p;
				}
			}

			for (int i = 0; i < num_checkBytes_block * 8; i++) {
				PixelInfo info = pixel_offset[plan->getdBytesNum() * 8 + check_offset + i];
				Pixel pixel(pixels[info.getY()][info.getX()]);

				if ((bit_block.getBlockBytes()[nd+i / 8] & (1 << (7 - i & 0x7))) != 0)  pixel.xorPixel(Pixel::BLACK.getPixel());

				expect[info.getY()][info.getX()] = ((pixel.getPixel()&Pixel::BLACK.getPixel()) != 0);

				/*What dose this mean, though cheat is false?*/
				if (cheat) {
					Pixel p = new Pixel(pixel);
					p.setPixel(Pixel::BLACK.getPixel());
					pixels[info.getY()][info.getX()] = p;
				}
			}

			data_offset += nd * 8;
			check_offset += num_checkBytes_block * 8;

		}

		/*Pass over all pixels again,dithering*/
		if (dither) {
			for (int i = 0; i < pixel_offset.size(); i++) {
				// there is a issue about whether pixel_offset[i] should be recopy to primary position
				pixel_offset[i].setDitherTarget(pixel_offset[i].getTarget());
			}

			for (int y = 0; y < pixels.size(); y++) {
				std::vector<Pixel> row = pixels[y];

				for (int x = 0; x < pixels.size(); x++) {

					Pixel pixel = row[x];
					Pixel::PixelRole role = pixel.getPixelRole();
					if (role != Pixel::PixelRole::DATA&&role != Pixel::PixelRole::CHECK) continue;

					PixelInfo info = pixel_offset[pixel.getOffset()];
					if (info.getBlock().isEmpty())  continue;//does not choose this pixel

					pixel = info.getPixel();

					uint8_t pixelValue = 1;
					int grayValue = 0;
					int targ = info.getDitherTarget();

					if (targ >= divider) {
						//want white 
						pixelValue = 0;
						grayValue = 255;
					}

					uint8_t bitValue = pixelValue;
					if (pixel.shouldInv())  bitValue ^= 1;

					if (info.getHardZero() && bitValue != 0) {
						bitValue ^= 1;
						pixelValue ^= 1;
						grayValue ^= 0xFF;
					}

					//Set Pixel value as we want it 
					info.getBlock().reset(info.getBitIndex(), bitValue);

					int error = targ - grayValue;

					if (x + 1 < row.size()) addDither(pixel_offset, row[x + 1], error * 7 / 16);
				}
			}

			for (int i = 0; i < bit_blocks.size(); i++)  bit_blocks[i].copyOut();

		}

		errorCount = 0;
		/*Copy numbers back out*/
		for (int i = 0; i < data_bits_remain / 10; i++) {
			//pull out 10 bits
			int v = 0;
			for (int j = 0; j < 10; j++) {
				int index = head_size + 10 * i + j;
				v <<= 1;
				v |= ((data[index / 8] >> (7 - index & 7)) & 0x1);

			}

			//Turn into 3 digits
			if (v >= 1000) {
				// Oops - too many 1 bits.
				// We know the 512, 256, 128, 64, 32 bits are all set.
				// Pick one at random to clear.  This will break some
				// checksum bits, but so be it.
				PixelInfo info = pixel_offset[head_size + 10 * i + 3];
				info.setContrast(INT_MAX >> 8);
				info.setHardZero(true);
				errorCount++;
			}
			number[i * 3 + 0] = static_cast<uint8_t>(v / 100 + '0');
			number[i * 3 + 1] = static_cast<uint8_t>(v / 10 % 10 + '0');
			number[i * 3 + 2] = static_cast<uint8_t>(v % 10 + '0');
		}
	} while (errorCount>0);

	Bits final_bits;
    Raw(text).encode(final_bits, plan->getVersion());
	Number(number).encode(final_bits, plan->getVersion());
	final_bits.addCheckBytes(plan->getVersion(), plan->getLevel());

	QRCode qrCode = Plan::encode(plan, Raw(text), Number(number));
	return qrCode;




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


