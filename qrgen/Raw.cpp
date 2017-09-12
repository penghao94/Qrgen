#include "Raw.h"
#include <vector>
typedef std::vector<uint8_t> Bytes;

qrgen::Raw::Raw(std::string str)
{
	raw = str;
	
}

std::string qrgen::Raw::validate()
{
	return nullptr;
}

int qrgen::Raw::availableBits(Version * ver)
{
	return 4 + (ver->getSize() == 0 ? 8 : 16) + raw.length() * 8;
}

void qrgen::Raw::encode(Bits& bits, Version * ver)
{
	bits.write(4, 4);
	Bytes data;
	for (auto c : raw) data.push_back(static_cast<uint8_t>(c));

	bits.write(data.size(), ver->getSize() == 0 ? 8 : 16);

	for (auto i : data) bits.write(i, 8);

}


