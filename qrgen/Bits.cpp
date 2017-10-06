#include "Bits.h"
#include <vector>
#include "Reed_Solomon_encoder.h"
#include "RSUtil.h"
typedef std::vector<uint8_t> Bytes;

void qrgen::Bits::append(bool bit)
{
	ensureCapacity(size + 1);
	if (bit)    bits[size / 8] |= 1 << (7 - size & 0x07);
	size++;
}

void qrgen::Bits::append(Bits other)
{
	int s = other.getSize();
	ensureCapacity(size + s);

	for (int i = 0; i < s; i++)
		append(other.get(i));
	
}

void qrgen::Bits::write(int v, int numBits)
{
	assert(!(numBits < 0 || numBits>32)&& "Number bits must be between 0 and 32");
	ensureCapacity(numBits + size);

	for (int i = numBits; i > 0; i--)
		append(((v >> (i - 1)) & 0x01) == 1);
}
/*
if we assume n=15, size=3; then write(0,4), n=11, size=7, shift=1; write(0,1), n=10, size=8; pad=1,
*/
void qrgen::Bits::pad(int n)
{
	assert(!(n < 0) && "qr: invalid pad size");
	if (n <= 4)
		write(0, n);
	else {
		write(0, 4);
		n -= 4;
		int shift = 8 - size & 0x07;
		n -= shift;
		write(0, shift);
		int pad = n / 8;
		for (int i = 0; i < pad; i += 2) {
			write(0xec, 8); //1110 1100
			if (i + 1 >= pad) break;
			write(0x11, 8); //0001 0001
		}
	}
}

void qrgen::Bits::addCheckBytes(Version * ver, LEVEL lvl)
{
	int num_dBytes = ver->dataBytes(lvl);

	VerInfo verinfo = Version::VERSION_INFOS[ver->getVersion()];
	VerLvlInfo lvlinfo = verinfo.lvlInfos[lvl];
	int npb_dBytes = num_dBytes / lvlinfo.num_of_block;
	int num_extras = num_dBytes % lvlinfo.num_of_block;

	RSencoder rs(new qrgen::GenericGF(0x011D, 256, 0));

	int dataIndex = 0;
	for (int i = 0; i < lvlinfo.num_of_block; i++) {
		if (i == lvlinfo.num_of_block - num_extras)
			npb_dBytes++;

		Bytes checkBytes = geneECBytes(rs, bits, dataIndex, npb_dBytes, lvlinfo.cbytes_pre_block);
		dataIndex += npb_dBytes;
		append(Bits(checkBytes, lvlinfo.cbytes_pre_block * 8)); 
	}
	assert(!(size / 8 != verinfo.bytes) && "internal error");
}

void qrgen::Bits::ensureCapacity(int s)
{
	if (s > bits.size() * 8) bits.resize((s + 7) / 8);
}
