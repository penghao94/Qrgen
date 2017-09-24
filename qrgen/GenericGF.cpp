#include "GenericGF.h"

qrgen::GenericGF::GenericGF(int primitive, int size, int b)
{
	this->primitive = primitive;
	this->size = size;
	this->geneBase = b;

	expTab.resize(size);
	logTab.resize(size);

	/*calculate all integer in Galois field */

	int x = 1;
	int alpha = 2;//the generator alpha is 2

				  // alpha^2 % size
	for (int i = 0; i < size; i++) {
		expTab[i] = x;
		x *= 2;
		x = x >= size ? (x^primitive)&(size - 1) : x;
	}

	for (int i = 0; i < size - 1; i++) logTab[expTab[i]] = i;	//logTable[0] == 0 but this should never be used
}

std::string qrgen::GenericGF::to_String()
{
	char ch[8];
	sprintf(ch, "%X", primitive);
	std::string str(ch);
	return "GF(0x" + str + "," + std::to_string(size) + ")";
}

