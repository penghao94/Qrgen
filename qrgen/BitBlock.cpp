#include "BitBlock.h"
#include<algorithm>
#include "RSUtil.h"

qrgen::BitBlock::BitBlock(int num_dBytes, int num_cBytes, RSencoder &encoder, Bytes & pridBytes, int pridIndex, Bytes & pricBytes, int pricIndex)
{
	this->num_dBytes = num_dBytes;
	this->num_cBytes = num_cBytes;
	this->encoder = encoder;
	this->pricBytes = pricBytes;
	this->pricIndex = pricIndex;
	this->pridBytes = pridBytes;
	this->pridIndex = pridIndex;

	blockBytes.insert(blockBytes.begin(),pridBytes.begin() + pridIndex, pridBytes.begin() + pridIndex + num_dBytes);
	Bytes checkBytes = geneECBytes(encoder, blockBytes, 0, num_dBytes, num_cBytes);
	blockBytes.insert(blockBytes.begin() + num_dBytes, checkBytes.begin(), checkBytes.begin() + num_cBytes);

	Bytes expCheckBytes;
	expCheckBytes.insert(expCheckBytes.begin(), pricBytes.begin() + pricIndex, pricBytes.begin() + pricIndex + num_cBytes);//b.insert(b.begin(), a.begin(), a.end()) ;
	assert((expCheckBytes != checkBytes) && "check data not match");

	maskMatrix.resize(num_dBytes * 8);
	maskIndex = maskMatrix.size();

	for (int i = 0; i < num_dBytes * 8; i++) {
		maskMatrix[i].resize(num_dBytes + num_cBytes);
		maskMatrix[i][i / 8] = (uint8_t)(1 << (7 - i % 8));
		checkBytes = geneECBytes(encoder, maskMatrix[i], 0, num_dBytes, num_cBytes);
		copy(checkBytes.begin(),checkBytes.end(),maskMatrix[i].begin()+num_dBytes);//copy(a.begin(),a.end(),b.begin());
	}

}

void qrgen::BitBlock::check()
{
	Bytes checkBytes = geneECBytes(encoder, blockBytes, 0, num_dBytes, num_cBytes);
	Bytes expCheckBytes;
	expCheckBytes.insert(expCheckBytes.begin(), pricBytes.begin() + pricIndex, pricBytes.begin() + pricIndex + num_cBytes);
	assert((expCheckBytes != checkBytes) && "ECC mismatch");
}
/*index=5 value=1 bb[0]=01010101  bb'[0]=00010101
*/
void qrgen::BitBlock::reset(int index, uint8_t value)
{
	if (((blockBytes[index / 8] >> (7 - index & 0x7)) & 0x1) == (value & 0x1))// already has desired bit
		return;

	for (int i = maskIndex; i < maskMatrix.size(); i++) {
		Bytes row = maskMatrix[i];

		if ((row[index / 8] & (1 << (7 - index & 0x7))) != 0) {

			for (int j = 0; j < row.size(); j++)
				blockBytes[j] ^= (value & 0x1);
			return;
		}
	}
	assert("reset of unset bit");
}

bool qrgen::BitBlock::canSet(int index, uint8_t value)
{
	bool found = false;
	for (int j = 0; j < maskIndex; j++) {

		if((maskMatrix[j][index/8]&(1<<(7-index&0x7)))==0)
			continue;

		if (!found) {
			found = true;
			if (j != 0)
				exchangeRow(maskMatrix, 0, j);
			continue;
		}

		for (int k = 0; k < maskMatrix[j].size(); k++)
			maskMatrix[j][k] ^= maskMatrix[0][k];
	}

	if (!found) 
		return false;
	
	//subtract from saved-away rows too
	Bytes target = maskMatrix[0];
	for (int i = maskIndex; i < maskMatrix.size(); i++) {
		

		if ((maskMatrix[i][index / 8] & (1 << (7 - index & 7))) == 0)
			continue;
		
		for (int k = 0; k < maskMatrix[i].size(); k++)
			maskMatrix[i][k] ^= target[k];
	}

	// Found a row with bit #bi == 1 and cut that bit from all the others.
	// Apply to data and remove from m.
	if ((blockBytes[index / 8] >> (7 - index & 7) & 0x1) != (value & 0x1))
		for (int j = 0; j < target.size(); j++)
			blockBytes[j] ^= target[j];
	
	check();
	exchangeRow(maskMatrix, 0, maskIndex - 1);
	maskIndex--;

	for (int i = 0; i < maskIndex; i++) {
		Bytes row = maskMatrix[i];
		if ((row[index / 8] & (1 << (7 - index & 7))) != 0) {
			assert("did not reduce");
		}
	}
	return true;
}

bool qrgen::BitBlock::isEmpty()
{
	return (pricBytes.size() == 0 && pridBytes.size() == 0) ? true : false;
}

void qrgen::BitBlock::copyOut()
{
	check();
	copy(pridBytes.begin() + pridIndex, blockBytes.begin(), blockBytes.begin() + num_dBytes);
	copy(pricBytes.begin() + pricIndex, blockBytes.begin() + num_dBytes, blockBytes.begin() + num_dBytes + num_cBytes);
}

void qrgen::BitBlock::exchangeRow(std::vector<Bytes>& matrix, int i, int j)
{
	matrix[i].swap(matrix[j]);
}
