#include "Alpha.h"
#include <algorithm>
qrgen::Alpha::Alpha()
{
	alpha = "";
	const std::string ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";
	const std::vector<int> alpha_len = { 9,11,13 };
}

qrgen::Alpha::Alpha(std::string a)
{
	alpha = a;
	const std::string ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";
	const std::vector<int> alpha_len = { 9,11,13 };
}

std::string qrgen::Alpha::validate()
{
	if (alpha == "") return "";

	for (auto i : alpha)
		if (std::find(ALPHABET.begin(), ALPHABET.end(), alpha[i]) == ALPHABET.end())
			return "non-alphanumeric string " + alpha;

}

int qrgen::Alpha::availableBits(Version * ver)
{
	return 4 + alpha_len[ver->getSize()] + (11 * alpha.length() + 1) / 2;
}

void qrgen::Alpha::encode(Bits & bits, Version * ver)
{
	bits.write(2, 4);
	bits.write(alpha.length(), alpha_len[ver->getSize()]);
	int i = 0;

	for (i = 0; i + 2 <= alpha.length(); i += 2) {
		int w = static_cast<int>(std::find(ALPHABET.begin(), ALPHABET.end(), alpha[i]) - ALPHABET.begin()) * 45 +
			static_cast<int>(std::find(ALPHABET.begin(), ALPHABET.end(), alpha[i + 1]) - ALPHABET.begin());

		bits.write(w, 11);
	}

	if (i < alpha.length()) {
		int w = static_cast<int>(std::find(ALPHABET.begin(), ALPHABET.end(), alpha[i])-ALPHABET.begin());
		bits.write(w, 6);
	}
}
