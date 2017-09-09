// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#ifndef GENERICGFPOLY_H
#define GENERICGFPOLY_H
#include "GenericGF.h"
#include<vector>
#include<string>
namespace qrgen {
	/**
	* <p>Represents a polynomial whose coefficients are elements of a GF.
	* Instances of this class are immutable.</p>
	*
	* <p>Much credit is due to William Rucklidge since portions of this code are an indirect
	* port of his C++ Reed-Solomon implementation.</p>
	*
	* @author Sean Owen
	*
	* A java version rewrote by C++
	*/
	class  GenericGFPoly final {
	private:
		static GenericGF field;
		static std::vector<int> coeff;
		static GenericGFPoly *zero;
		static GenericGFPoly *one;
	public:

		/**
		*field			the {@link GenericGF} instance representing the field to use to perform computations
		*coefficients	coefficients as ints representing elements of GF(size), arranged from most significant (highest-power term) coefficient to least significant
		*
		*throws IllegalArgumentException if argument is null or empty,
		* or if leading coefficient is 0 and this is not a
		* constant polynomial (that is, it is not the monomial "0")
		*/
		GenericGFPoly(qrgen::GenericGF field, std::vector<int> &coef);
		~GenericGFPoly() { coeff.clear(); coeff.swap(std::vector<int>()); }

		GenericGFPoly * getZero();

		GenericGFPoly * getOne(); 

		/*return the monomial representing coefficient * x^degree*/
		//qrgen::GenericGFPoly *buildMonomial(int d, int c);

		/* return coefficients of this polynomial */
		std::vector<int> getCoeff() { return coeff; }

		/*return degree of of this polynomial */
		int getDegree() { return coeff.size(); }
		
		/*return if this polynomial is the monomial "0"*/
		bool isZero() { return coeff[0] == 0; }

		/* return coefficient of x^degree term in this polynomial*/
		int getCoeff(const int degree) { return coeff[coeff.size() - 1 - degree]; }

		/* return evaluation of this polynomial at given point*/
		int evaluateAt(const int a);

		/*return sum/difference of field and other*/
		GenericGFPoly* add_or_sub(GenericGFPoly* other);

		/*return product of field and scalar*/
		GenericGFPoly* multiply(const int scalar);

		GenericGFPoly* multiply(GenericGFPoly *other);

		/*return product of monomial in degree d and coefficient c*/
		GenericGFPoly* multiply_by_monomial(const int d, const int c);
		
		/*return quotient and reminder divided by polynomial other */
		std::vector<GenericGFPoly*> divide(GenericGFPoly *other);

		GenericGFPoly * buildMonomial(int d, int c);
		/* return string*/
		std::string to_string();
	};

	inline qrgen::GenericGFPoly::GenericGFPoly(qrgen::GenericGF field, std::vector<int> &coef)
	{
		assert((coef.size() == 0)&& "Illegal Argument Exception!!!");

		this->field = field;

		int coeff_len = coef.size();

		// Leading term must be non-zero for anything except the constant polynomial "0"
		if (coeff_len > 1 && coef[0] == 0) {
			int fnz = 1;// first no zero number
			while (fnz < coeff_len&&coef[fnz] == 0) fnz++;
			
			fnz == coeff_len ? this->coeff.push_back(0) : this->coeff.assign(coef.begin()+fnz, coef.end());
		}
		else
			this->coeff = coef;

		zero = new GenericGFPoly(this->field, std::vector<int>{0});
		one = new GenericGFPoly(this->field, std::vector<int>{1});
	}

	inline GenericGFPoly * GenericGFPoly::getZero()
	{
		return zero;
	}

	inline GenericGFPoly * GenericGFPoly::getOne()
	{
		return one;
	}

	inline int GenericGFPoly::evaluateAt(const int a)
	{
		if (a == 0) return getCoeff(0); // Just return the x^0 coefficient

		int size = coeff.size();
		if (a == 1) {
			int result = 0;//Just the sum  of the coefficients
			for (int c : coeff) result = GenericGF::add_or_sub(result, c);
			return result;
		}

		int result = coeff[0];
		for (int c : coeff) result = GenericGF::add_or_sub(field.multiply(a, result), c);
		return result;
	}

	inline GenericGFPoly* GenericGFPoly::add_or_sub(GenericGFPoly* other)
	{
		assert((typeid(field) == typeid(other))&& "GenericGFPolys do not have same GenericGF field !!!");
		
		if (isZero()) {
			return other;
		}
		if (other->isZero()) {
			return this;
		}

		std::vector<int> sm_coeff = this->coeff;
		std::vector<int> lg_coeff = other->coeff;

		if (sm_coeff.size() > lg_coeff.size()) swap(sm_coeff, lg_coeff);

		std::vector<int> sumDiff(lg_coeff.size());
		int d = lg_coeff.size() - sm_coeff.size();
		sumDiff.assign(lg_coeff.begin(), lg_coeff.begin() + d);
		for (int i = d; i < lg_coeff.size(); i++) sumDiff[i] = GenericGF::add_or_sub(sm_coeff[i - d], lg_coeff[i]);
		return new GenericGFPoly(field, sumDiff);
	}

	inline GenericGFPoly * GenericGFPoly::multiply(int scalar)
	{
		if (scalar == 0) return getZero();

		if (scalar == 1) return this;
		
		int size = coeff.size();
		std::vector<int> product(size);
		for (int i = 0; i < size; i++) product[i] = field.multiply(coeff[i], scalar);

		return new GenericGFPoly(field, product);
	}

	inline GenericGFPoly * GenericGFPoly::multiply(GenericGFPoly * other)
	{
		assert((typeid(field) == typeid(other))&& "GenericGFPolys do not have same GenericGF field !!!");
		if (isZero() || other->isZero()) return getZero();

		std::vector<int> acoeff = this->coeff;
		int asize = this->coeff.size();

		std::vector<int> bcoeff = other->coeff;
		int bsize = other->coeff.size();

		std::vector<int> product(asize + bsize - 1);
		for (int i=0;i<asize;i++)
		{
			int ac = acoeff[i];
			for (int j = 0; j < bsize; j++) {
				product[i + j] = qrgen::GenericGF::add_or_sub(product[i + j], field.multiply(ac, bcoeff[j]));
			}
		}
	}

	inline GenericGFPoly * GenericGFPoly::multiply_by_monomial(const int d, const int c)
	{
		assert((d < 0)&& "Illegal Argument Exception!!!");

		if (c == 0)	return getZero();

		int size = coeff.size();
		std::vector<int> product (size + d);
		for (int i = 0; i < size; i++) product[i] = field.multiply(coeff[i], c);

		return new GenericGFPoly(field, product);
	}

	inline std::vector<GenericGFPoly*> GenericGFPoly::divide(GenericGFPoly * other)
	{
		assert((typeid(field) == typeid(other))&& "GenericGFPolys do not have same GenericGF field !!!");
		assert((other->isZero())&& "Divide by 0!!!");

		GenericGFPoly *qut = getZero();
		GenericGFPoly *rem = this;

		int denomin = other->getCoeff(other->getDegree());
		int inverse_denomin = field.inverse(denomin);

		while (rem->getDegree()>=other->getDegree()&&rem->isZero()){
			int degreeDiff = rem->getDegree() - other->getDegree();
			int scale = field.multiply(rem->getDegree(), inverse_denomin);

			GenericGFPoly *term = other->multiply_by_monomial(degreeDiff, scale);
			GenericGFPoly *iter_qut = buildMonomial(degreeDiff, scale);
			qut = qut->add_or_sub(iter_qut);
			rem = rem->add_or_sub(term);
		}
		return std::vector<GenericGFPoly*>{qut, rem};
	}

	inline GenericGFPoly * GenericGFPoly::buildMonomial(int d, int c)
	{
		assert((d < 0)&& "Illegal Argument Exception!!!");

		if (c == 0) return zero;

		std::vector<int> coeff(d + 1);
		coeff[0] = c;
		return new GenericGFPoly(this->field, coeff);
	}

	inline std::string GenericGFPoly:: to_string()
	{
		std::string str = "";

		int index = 0;
		for (int d = getDegree(); d >= 0; d--) {
			int c = getCoeff(d);

			if (c != 0) {
				if (c < 0) {
					str+="-";
					c = -c;
				}
				else if (index > 0) str += "+";

				if (d == 0 || c != 1) {
					int ap = field.log(c);

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


}
#endif // !GENERICGFPOLY_H

