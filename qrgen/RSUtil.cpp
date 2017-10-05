#include "RSUtil.h"
typedef std::vector<uint8_t> Bytes;

Bytes qrgen::geneECBytes(RSencoder encoder, const Bytes &dBytes, int pos, int len, int num_Ecb)
{
	int num_dBytes = len;
	std::vector<int> toEncode(num_dBytes + num_Ecb);
	for (int i = 0; i < num_dBytes; i++) toEncode[i] = dBytes[pos + i] & 0xFF;

	encoder.encode(toEncode, num_Ecb);

	Bytes ecBytes(num_Ecb);

	for (int i = 0; i < num_Ecb; i++)
		ecBytes[i] = static_cast<uint8_t>(toEncode[num_dBytes + i] & 0xFF);
	return ecBytes;
}
