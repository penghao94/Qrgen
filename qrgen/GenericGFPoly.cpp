#include "GenericGFPoly.h"



qrgen::GenericGFPoly::GenericGFPoly(qrgen::GenericGF *field, std::vector<int>& coef)
{
	assert(!(coef.size() == 0) && "Illegal Argument Exception!!!");

	this->field = field;

	int coeff_len = coef.size();

	// Leading term must be non-zero for anything except the constant polynomial "0"
	if (coeff_len > 1 && coef[0] == 0) {
		int fnz = 1;// first no zero number
		while (fnz < coeff_len&&coef[fnz] == 0) fnz++;

		fnz == coeff_len ? this->coeff.push_back(0) : this->coeff.assign(coef.begin() + fnz, coef.end());
	}
	else
		this->coeff = coef;
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::getZero()
{
	return new GenericGFPoly(this->field, std::vector<int>{0});
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::getOne()
{
	return new GenericGFPoly(this->field, std::vector<int>{1});
}

int qrgen::GenericGFPoly::evaluateAt(const int a)
{
	if (a == 0) return getCoeff(0); // Just return the x^0 coefficient

	int size = coeff.size();
	if (a == 1) {
		int result = 0;//Just the sum  of the coefficients
		for (int c : coeff) result = GenericGF::add_or_sub(result, c);
		return result;
	}

	int result = coeff[0];
	for (int c : coeff) result = GenericGF::add_or_sub(field->multiply(a, result), c);
	return result;
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::add_or_sub(qrgen::GenericGFPoly * other)
{
	assert(!(typeid(field) == typeid(other)) && "GenericGFPolys do not have same GenericGF field !!!");

	if (isZero()) {
		return other;
	}
	if (other->isZero()) {
		return this;
	}

	std::vector<int> sm_coeff = this->coeff;
	std::vector<int> lg_coeff = other->coeff;
	if (sm_coeff.size() > lg_coeff.size()) swap(sm_coeff, lg_coeff);

	std::vector<int> sumDiff;
	int d = lg_coeff.size() - sm_coeff.size();
	sumDiff.assign(lg_coeff.begin(), lg_coeff.begin() + d);
	sumDiff.resize(lg_coeff.size());

	for (int i = d; i < lg_coeff.size(); i++) sumDiff[i] = GenericGF::add_or_sub(sm_coeff[i - d], lg_coeff[i]);
	return new GenericGFPoly(field, sumDiff);
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::multiply(const int scalar)
{
	if (scalar == 0) return getZero();

	if (scalar == 1) return this;

	int size = coeff.size();
	std::vector<int> product(size);
	for (int i = 0; i < size; i++) product[i] = field->multiply(coeff[i], scalar);

	return new GenericGFPoly(field, product);
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::multiply(qrgen::GenericGFPoly * other)
{
	assert(!(typeid(field) == typeid(other)) && "GenericGFPolys do not have same GenericGF field !!!");
	if (isZero() || other->isZero())
		return getZero();

	std::vector<int> acoeff = this->coeff;
	int asize = this->coeff.size();

	std::vector<int> bcoeff = other->coeff;
	int bsize = other->coeff.size();

	std::vector<int> product(asize + bsize - 1);
	for (int i = 0; i < asize; i++)
	{
		int ac = acoeff[i];
		for (int j = 0; j < bsize; j++) {
			product[i + j] = qrgen::GenericGF::add_or_sub(product[i + j], field->multiply(ac, bcoeff[j]));
		}
	}
	return new GenericGFPoly(field, product);
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::multiply_by_monomial(const int d, const int c)
{
	assert(!(d < 0) && "Illegal Argument Exception!!!");

	if (c == 0)	return getZero();

	int size = coeff.size();
	std::vector<int> product(size + d);
	for (int i = 0; i < size; i++) product[i] = field->multiply(coeff[i], c);

	
	return new GenericGFPoly(field, product);
}

std::vector<qrgen::GenericGFPoly*> qrgen::GenericGFPoly::divide(qrgen::GenericGFPoly * other)
{
	assert(!(typeid(field) == typeid(other)) && "GenericGFPolys do not have same GenericGF field !!!");
	assert(!(other->isZero()) && "Divide by 0!!!");

	GenericGFPoly *qut = getZero();
	GenericGFPoly *rem = this;
	int denomin = other->getCoeff(other->getDegree());
	int inverse_denomin = field->inverse(denomin);

	while (rem->getDegree() >= other->getDegree() &&!( rem->isZero())) {
 		int degreeDiff = rem->getDegree() - other->getDegree();
		
		int scale = field->multiply(rem->getCoeff(rem->getDegree()), inverse_denomin);
		
		GenericGFPoly *term = other->multiply_by_monomial(degreeDiff, scale);
		GenericGFPoly *iter_qut = buildMonomial(degreeDiff, scale);

		qut = qut->add_or_sub(iter_qut);
		rem = rem->add_or_sub(term);
	}
	return std::vector<GenericGFPoly*>{qut, rem};
}

qrgen::GenericGFPoly * qrgen::GenericGFPoly::buildMonomial(int d, int c)
{
	assert(!(d < 0) && "Illegal Argument Exception!!!");

	if (c == 0) return getZero();
	std::vector<int> coef(d + 1);
	coef[0] = c;
	return new GenericGFPoly(this->field, coef);
}

std::string qrgen::GenericGFPoly::to_string()
{
	std::string str = "";

	int index = 0;
	for (int d = getDegree(); d >= 0; d--) {
		int c = getCoeff(d);

		if (c != 0) {
			if (c < 0) {
				str += "-";
				c = -c;
			}
			else if (index > 0) str += "+";

			if (d == 0 || c != 1) {
				int ap = field->log(c);

				if (ap == 0) str += "1";
				else if (ap == 1) str += "a";
				else {
					str += "a^";
					str += std::to_string(ap);
				}
			}

			if (d != 0) {
				if (d == 1) str += "x";
				else {
					str += "x^";
					str += std::to_string(d);
				}
			}

		}
	}

	return str;
}