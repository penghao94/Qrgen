#include "Number.h"

std::string qrgen::Number::validate()
{
	if (number == "") return "";
	for (auto i : number) {
		if (i<'0' || i>'9') return "non-numeric string" + number;
	}
	return "";
}

int qrgen::Number::availableBits(Version * ver)
{
	return 4 + (ver->getSize() == 0 ? 10 : ver->getSize() == 1 ? 12 : 14) + (10 * number.length() + 2) / 3;
}

void qrgen::Number::encode(Bits & bits, Version * ver)
{
	bits.write(1, 4);
	bits.write(number.length(), ver->getSize() == 0 ? 10 : ver->getSize() == 1 ? 12 : 14);
	int i = 0;
	for (i = 0; i < number.length(); i += 3) {
		int w = (number[i] - '0') * 100 + (number[i + 1] - '0') * 10 + (number[i + 2] - '0');
		bits.write(w, 10);
	}
	int w=0;

	switch (number.length() - i) {
	case 1:
		w = number[i] - '0';
		bits.write(w, 4);
		break;
	case 2:
		w = (number[i] - '0') * 10 + (number[i+1] - '0');
		bits.write(w, 7);
		break;
	}
}
