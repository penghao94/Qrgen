#include "Reed_Solomon_encoder.h"
#include "GenericGFPoly.h"
qrgen::GenericGFPoly * qrgen::RSencoder::buildGenerator(int d)
{
	using namespace std;
	if (d > cacheGen.size()) {
		qrgen::GenericGFPoly* lastGen = cacheGen[cacheGen.size() - 1];
		for (int i = cacheGen.size(); i <= d; i++) {
			qrgen::GenericGFPoly* nextGen = lastGen->multiply(new qrgen::GenericGFPoly(field, std::vector<int>{1,field->exp(i-1+field->getGeneBase())}));
			cacheGen.push_back(nextGen);
			lastGen = nextGen;
		}	
	}
	return cacheGen[d];
}

qrgen::RSencoder::RSencoder(GenericGF *field)
{
	this->field = field;
	this->cacheGen.push_back(new GenericGFPoly(field, std::vector<int>{1}));
}

void qrgen::RSencoder::encode(std::vector<int>& E, int ecBytes)
{
	assert(!(ecBytes==0)&& "No error correction bytes!!!");

	int dataBytes = E.size() - ecBytes;
	assert(!(dataBytes <= 0)&& "No data bytes provided!!!");

	GenericGFPoly *gen = buildGenerator(ecBytes);
	
	
	std::vector<int> infoCoeff;
	infoCoeff .assign(E.begin(), E.begin() + dataBytes);
	
	GenericGFPoly *info = new GenericGFPoly(field, infoCoeff);
	
	info=info->multiply_by_monomial(ecBytes, 1);
	
	GenericGFPoly *rem = (info->divide(gen))[1];
	std::vector<int> coeff = rem->getCoeff();

	int num_zero_coeff = ecBytes - coeff.size();

	for (int i = 0; i < num_zero_coeff; i++) 
		E[dataBytes + i] = 0;

	std::copy(coeff.begin(), coeff.end(), E.begin() + dataBytes + num_zero_coeff);

}
